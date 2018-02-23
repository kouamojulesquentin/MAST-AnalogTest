//===========================================================================
//                           AST_AttributesConverter.cpp
//===========================================================================
// Copyright (C) 2018 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_AttributesConverter.cpp
//!
//! Implements class AST_AttributesConverter
//!
//===========================================================================

#include "AST_AttributesConverter.hpp"
#include "AST_Attribute.hpp"
#include "AST_Instance.hpp"
#include "AST_Module.hpp"
#include "Chain.hpp"
#include "g3log/g3log.hpp"
#include "Utility.hpp"

using std::vector;
using std::shared_ptr;

using namespace mast;
using namespace Parsers;


//! Creates associations between algorithm names and SystemModelNode
//!
//! @note Association are created from attributes with name 'PDL' that provide algorithm names
//!
//! @param instance     ICL instance for which a Chain has been created (optional)
//! @param module       Module for which attributes with name PDL provide name of algorithms
//! @param parentNode   SystemModelNode associated with processed module
//!
vector<AppFunctionNameAndNode> AST_AttributesConverter::CollectAlgorithms (AST_Instance* instance, AST_Module* module, shared_ptr<ParentNode> parentNode)
{
  vector<AppFunctionNameAndNode> mergedAssociations;
  vector<AppFunctionNameAndNode> localAssociations;

  // Lamba: Merges "local" association with those collected from current module or instance
  //
  auto mergeAssociations = [&mergedAssociations, &localAssociations]()
  {
    for (auto&& algorithmAssociation : localAssociations)
    {
      mergedAssociations.emplace_back(std::move(algorithmAssociation));
    }
  };

  // ---------------- Local "PDL attributes"
  //
  if (parentNode != nullptr)
  {
    localAssociations = CollectLocalAlgorithms(instance, module, parentNode);
    mergeAssociations();
  }

  // ---------------- Instances "PDL attributes"
  //
  const auto& instances = module->Instances();
  for (auto instance : instances)
  {
    auto associatedNode = instance->AssociatedChain();
    auto instanceModule = instance->UniquifiedModule();

    CHECK_VALUE_NOT_NULL(instanceModule, "Houps: Instance is not associated with an uniquified module");

    if (associatedNode != nullptr)  // Standard case is that for each instance there is an associated chain in SystemModel but for some (very simple) there are none
    {
      localAssociations = CollectAlgorithms(instance, instanceModule, associatedNode);
      mergeAssociations();
    }
    else if (instanceModule->HasAttributes())
    {
      LOG(INFO) << "Trying to pull up \"PDL\" attribute(s) defined in instance \"" << instance->Name() << "\" into its parent module \"" << module->Name() << "\"" ;
      localAssociations = CollectAlgorithms(instance, instanceModule, parentNode);
      mergeAssociations();
    }
  }

  return mergedAssociations;
}
//
//  End of: AST_AttributesConverter::CollectAlgorithms
//---------------------------------------------------------------------------


//! For each attribute with name "PDL", associates the algorithm name with a ParentNode
//!
//! @note PDL attributes can be defined at Module and/or Instance levels
//!
//! @param instance     ICL instance for which a Chain has been created (optional)
//! @param module       Processed module
//! @param parentNode   Associated node in SystemModel
//!
vector<AppFunctionNameAndNode> AST_AttributesConverter::CollectLocalAlgorithms (AST_Instance* instance, AST_Module* module, shared_ptr<ParentNode> parentNode)
{
  vector<AppFunctionNameAndNode> algorithmAssociations; // Use local associations for fast and simple search for override management

  // Lamba: Check if there is already an association with the same algorithm name
  //
  auto hasAlreadyAssociationFor = [&algorithmAssociations](const auto& algorithmId)
  {
    auto sameName = [&algorithmId](const auto& item){return item.appName == algorithmId;};

    return std::find_if(algorithmAssociations.begin(), algorithmAssociations.end(), sameName) != algorithmAssociations.cend();
  };

  // Lamba: "Convert" a collection of "PDL" attributes
  //
  auto processAttributes = [&](const auto& attributes)
  {
    for (const auto attribute : attributes)
    {
      if (attribute->Name() == "PDL")
      {
        auto algorithmId = attribute->ValueAsText();
        if (!hasAlreadyAssociationFor(algorithmId))
        {
          constexpr uint32_t line = 0;                                      // File line in which the attribute is defined is not yet supported
          algorithmAssociations.emplace_back(algorithmId, parentNode, line);
        }
      }
    }
  };

  // ---------------- At Module level
  //
  if (module != nullptr)
  {
    processAttributes(module->Attributes());
  }

  // ---------------- At instance level
  //
  if (instance != nullptr)
  {
    processAttributes(instance->Attributes());
  }

  return algorithmAssociations;
}
//
//  End of: AST_AttributesConverter::CollectLocalAlgorithms
//---------------------------------------------------------------------------


//===========================================================================
// End of AST_AttributesConverter.cpp
//===========================================================================
