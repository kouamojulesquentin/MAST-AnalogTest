//===========================================================================
//                           ICL_Reader.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file ICL_Reader.hpp
//!
//! Declares  class for managing parsing of ICL files
//!
//===========================================================================

#ifndef ICL_READER_H__9F5136A9_6376_4208_9286_B2DFD0D4D57A__INCLUDED_
  #define ICL_READER_H__9F5136A9_6376_4208_9286_B2DFD0D4D57A__INCLUDED_

#include "Reader.hpp"
#include <istream>
#include <memory>

namespace mast
{
  class SystemModel;
  class SystemModelNode;
}

namespace Parsers
{
  class AST;
}

namespace ICL
{
class ICL_Reader : public Parsers::Reader
{
  // ---------------- Public Methods
  //
  public:
  virtual ~ICL_Reader();
  ICL_Reader() = delete;
  ICL_Reader(std::shared_ptr<mast::SystemModel> sm);


  //! Gets whether parsing only ICL without any attempt to convert it to SystemModel
  //!
  bool  ParseOnlyCheckGrammar() const { return m_parseOnlyCheckGrammar; }

  //! Enables/Disables parsing only ICL without any attempt to convert it to SystemModel
  //!
  void  ParseOnlyCheckGrammar (bool onlyGrammarCheck) { m_parseOnlyCheckGrammar = onlyGrammarCheck; }


  // ---------------- Protected Methods
  //
  protected:
  virtual void Parse_Impl       (std::istream& stream) override;
  virtual void UpdateAstFromIcl (std::istream& stream);

  Parsers::AST* AST() const { return m_ast.get(); } //!< Returns current AST (for debug purpose)

  virtual std::shared_ptr<mast::SystemModelNode> GenerateSystemModelNodes (Parsers::AST* ast);

  void UniquifyAST ();

  // ---------------- Private Fields
  //
  private:
  std::unique_ptr<Parsers::AST> m_ast;                           //!< Instance member to be visible by unit tests (for debug purpose)
  bool                          m_parseOnlyCheckGrammar = false; //!< When true, parsing stops after conversion to AST (no more check nor conversion to SystemModel is done)
};
} // End of: namespace ICL

#endif  // not defined ICL_READER_H__9F5136A9_6376_4208_9286_B2DFD0D4D57A__INCLUDED_
