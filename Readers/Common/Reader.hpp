//===========================================================================
//                           Reader.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Reader.hpp
//!
//! Declares  class for managing parsing of ICL files
//!
//===========================================================================

#ifndef READER_H__85E09160_16F2_4CF2_B5B2_F3F37C59F0FD__INCLUDED_
  #define READER_H__85E09160_16F2_4CF2_B5B2_F3F37C59F0FD__INCLUDED_

#include "AppFunctionNameAndNode.hpp"
#include "PathSelector.hpp"
#include "SubModelPlaceHolder.hpp"
#include "Parser_PublicData.hpp"

#include <string>
#include <vector>
#include <experimental/string_view>
#include <cstddef>
#include <istream>
#include <memory>
#include <iostream>

namespace mast
{
  class SystemModel;
  class SystemModelNode;
}

namespace Parsers
{
class Reader
{
  public:
  virtual ~Reader() = default;
  Reader() = delete;
  Reader(std::shared_ptr<mast::SystemModel> sm);


  //! Parses a file to construct a SystemModel
  //!
  //! @param filename  File path
  //!
  void Parse(std::experimental::string_view filename);

  //! Parses an excerpt to construct a SystemModel
  //!
  void ParseExcerpt(std::istream& stream);

  //! Parses a formatted excerpt to construct a SystemModel
  //!
  void ParseExcerpt(const std::string& excerpt);

  //! Returns SystemModelNode build from parsed file or excerpt
  //!
  std::shared_ptr<mast::SystemModelNode> ParsedSystemModel() { return m_publicData.parsedTopNode; }

  //! Associations of PDL algorithm names to nodes
  //!
  const std::vector<mast::AppFunctionNameAndNode>& PDLAlgorithmNameToNodeAssociation() { return m_publicData.namesAndNodes; };

  //! Returns error message in case of failure, empty string otherwise
  //!
  std::string ErrorMessage() const { return m_errorMessage; }

  //! Identifies parts of the model that are not instanciated yet and where to insert (splice) them
  //!
  const std::vector<mast::SubModelPlaceHolder>& PlaceHolders() const { return m_publicData.placeHolders; }

  protected:

  Parser_PublicData& PublicData() { return m_publicData; }

  virtual void       Parse_Impl(std::istream& stream) = 0;

  private:
  std::string       m_errorMessage; //!< Error message build while parsing file or excerpt (empty when successful)
  Parser_PublicData m_publicData;   //!< Data useful for caller
};

} // End of: namespace Parsers


#endif  // not defined READER_H__85E09160_16F2_4CF2_B5B2_F3F37C59F0FD__INCLUDED_
