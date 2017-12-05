//===========================================================================
//                           AST_SystemModelGenerator.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_SystemModelGenerator.hpp
//!
//! Declares AST_SystemModelGenerator class
//!
//===========================================================================

#ifndef AST_SYSTEMMODELGENERATOR_H__4AD33E64_B5F0_44D9_839C_E286323534C__INCLUDED_
  #define AST_SYSTEMMODELGENERATOR_H__4AD33E64_B5F0_44D9_839C_E286323534C__INCLUDED_

#include "BinaryVector.hpp"

#include <memory>
#include <vector>
#include <stack>
#include <tuple>
#include <functional>

namespace mast
{
class Chain;
class Linker;
class ParentNode;
class PathSelector;
class UnresolvedPathSelector;
class SystemModel;
class SystemModelNode;
class SystemModelBuilder;
} // End of namespace mast

namespace Parsers
{
class AST_Instance;
class AST_Module;
class AST_Network;
class AST_Port;
class AST_Signal;
class AST_ScanMux;
class AST_ScanMuxSelection;
class AST_ScanRegister;
class AST_Source;

//! Generates Mast SystemModel from AST
//!
class AST_SystemModelGenerator final
{
  // ---------------- Public Methods
  //
  public:
  ~AST_SystemModelGenerator();
  AST_SystemModelGenerator() = delete;
  AST_SystemModelGenerator(std::shared_ptr<mast::SystemModel> systemModel);

  //! Generates a SystemModel (sub-)tree from AST_Network
  //!
  std::shared_ptr<mast::ParentNode> Generate(AST_Network* network);

  // ---------------- Private Methods
  //
  private:
  using SourceSignals_t     = std::vector<AST_Signal*>;
  using SourceSignalsRef_t  = std::reference_wrapper<const SourceSignals_t>;
  using ProcessingContext_t = std::tuple<AST_Module*, SourceSignalsRef_t>;

  std::shared_ptr<mast::PathSelector> Create_PathSelector   (AST_ScanMux* scanMux, AST_Module* module, bool firstSelectionIsEmpty);
  std::vector<AST_ScanRegister*>      FindSelectorRegisters (const SourceSignals_t&,  AST_Module* module) const;
  std::shared_ptr<mast::ParentNode>   Generate_Network      (AST_Network* network);
  void                                Generate_TopModule    (mast::Chain* chain, AST_Module* topModule);


  void FollowTopModulePath (AST_Module* module, const AST_Port* scanOutPort);

  SourceSignalsRef_t  Process_ScanRegister   (AST_ScanRegister* scanRegister);
  ProcessingContext_t Process_Instance_Entry (AST_Instance*     instance, AST_Module* instanceModule, const AST_Port* scanOutPort);
  ProcessingContext_t Process_Instance_Exit  (AST_Port*         scanInPort);
  ProcessingContext_t Process_ScanMux_Entry  (AST_ScanMux*      scanMux,  AST_Module* module);
  ProcessingContext_t Process_ScanMux_EndOfSelectionPath (std::shared_ptr<mast::SystemModelNode> commonLinkerNode);

  void AppendCreatedNodesToParent (mast::ParentNode* parent, size_t levelThreshold);

  std::tuple<bool, AST_Port*> IsSourcedByModuleInput (const AST_Module* module, const SourceSignals_t& signals) const;

  using SelectionTables_t = std::tuple<std::vector<mast::BinaryVector>, std::vector<mast::BinaryVector>>;
  SelectionTables_t MakeSelectionTable (const std::vector<AST_ScanMuxSelection*>&, size_t expectedBitsCount, bool firstSelectionIsEmpty) const;

  std::shared_ptr<mast::Chain> Create_ChainForLinker (const mast::Linker* linker, size_t selectionId);

  void AssignNewNode                     (std::shared_ptr<mast::SystemModelNode> node);
  bool AssignNodesToLinkerFirstSelection (std::shared_ptr<mast::SystemModelNode> commonLinkerNode);

  // ---------------- Private Fields
  //
  private:
  struct InstanceContext final
  {
    AST_Instance*     instance          = nullptr;  //!< Processed instance
    AST_Module*       parentModule      = nullptr;  //!< Module in which Instance is found
    mast::ParentNode* parentNode        = nullptr;  //!< Parent node in which children should be appended
    mast::ParentNode* linkerNode        = nullptr;  //!< Not nullptr when processing element that can potentially be children of a Linker
    size_t            createdNodesLevel = 0;        //!< To know how many to consider as children
  };

  //! Saves processing context in which a ScanMux is reached an converted to a linker
  //!
  struct LinkerContext final
  {
    std::stack<InstanceContext> instancesContext;                 //!< Processing contexts downto instance in which the ScanMux is found
    AST_ScanMux*                processedScanMux       = nullptr; //!< Scan mux being processed
    size_t                      processedSelectionId   = 0;       //!< Offset in Selection vector (to detect how many children must be associated to the Linker)
    size_t                      linkerNodesLevel_first = 0;       //!< To know how many children to assign to Linker first selection
    size_t                      linkerNodesLevel       = 0;       //!< To know how many children to assign to Linker selections
    mast::Linker*               linker                 = nullptr; //!< Created Linker
    mast::ParentNode*           linkerParentNode       = nullptr; //!< Parent node of linker
  };

  using CreatedNodes_t = std::tuple<std::shared_ptr<mast::SystemModelNode>, mast::ParentNode*>; // Created node and its parent node if not linker a linker child

  std::shared_ptr<mast::SystemModel>                         m_systemModel;             //!< SystemModel currently being built
  std::unique_ptr<mast::SystemModelBuilder>                  m_builder;                 //!< Helper to build SystemModel nodes
  std::shared_ptr<mast::ParentNode>                          m_parsedTopNode;           //!< SystemModel tree build from ICL file
  std::stack<CreatedNodes_t>                                 m_createdNodes;            //!< Created children and their default parent not yet attached to its parent (in Linker processing context)
  std::vector<std::shared_ptr<mast::UnresolvedPathSelector>> m_unresolvedPathSelectors; //!< Unresolved Linkers (those for which selector Register(s) where not yet created when Linkers were)
  std::stack<InstanceContext>                                m_instancesContext;        //!< Current module/instance contexts (represents current instanciation path)
  std::stack<LinkerContext>                                  m_linkersContext;          //!< To recover processing context for linkers (need only access to last one)
  AST_Network*                                               m_network = nullptr;       //!< Test network AST used to generate SystemModel tree
};
//
//  End of AST_SystemModelGenerator class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers




#endif  // not defined AST_SYSTEMMODELGENERATOR_H__4AD33E64_B5F0_44D9_839C_E286323534C__INCLUDED_

//===========================================================================
// End of AST_SystemModelGenerator.hpp
//===========================================================================
