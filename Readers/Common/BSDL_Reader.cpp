//===========================================================================
//                           BSDL_Reader.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file BSDL_Reader.cpp
//!
//! Implements class BSDL_Reader
//!
//===========================================================================

#include "BSDL_Reader.hpp"
#include "Utility.hpp"

#include <regex>

using std::regex;
using std::match_results;
using std::vector;
using std::experimental::string_view;

using namespace Parsers;
using namespace std::string_literals;
using namespace std::experimental::literals::string_view_literals;



//! Parses BSDL content with special care of specified instructions
//!
//! @note Will capture:
//!         Attribute INSTRUCTION_LENGTH of Top: entity is 5;
//!         Attribute INSTRUCTION_OPCODE of Top: entity is "EXTEST(00000),"& ...
//!
//! @note This code does not support comments with things that look like what we are looking for before they actually occur
//!
//! @param bsdlContent        BSDL excerpt to parse (must be kept valid until results of parsing is no more useful)
//! @param instructionsNames  BSDL instruction name to build selection values from
//!
void BSDL_Reader::Parse (string_view bsdlContent, const vector<string_view>& instructionsNames)
{
  CHECK_PARAMETER_NOT_EMPTY(bsdlContent,       "BSDL excerpt must not be empty");
  CHECK_PARAMETER_NOT_EMPTY(instructionsNames, "Should specify at 1 instruction to get the IR opcode for");

  using MatchResult_t = match_results<string_view::const_iterator>;

  // ---------------- IR length
  //
  regex irLengthRegex(R"(\battribute\s+INSTRUCTION_LENGTH\s+of\s+\w+\s*:\s*entity\s+is\s+(\d+))", regex::ECMAScript);

  MatchResult_t irLengthMatchResult;

  bool found = regex_search(bsdlContent.cbegin(), bsdlContent.cend(), irLengthMatchResult, irLengthRegex);

  CHECK_TRUE(found,                             "Cannot find attribute INSTRUCTION_LENGTH in BSDL");
  CHECK_VALUE_NOT_EMPTY(irLengthMatchResult,    "Failed to get IR length match");
  CHECK_VALUE_EQ(irLengthMatchResult.size(), 2, "Failed to capture IR length");

  auto irLengthSpan = irLengthMatchResult[1u];
  m_irBitsCount = std::stoi(irLengthSpan);

  // ---------------- Selection values
  //
  regex opCodeAttributeRegex (R"(\battribute\s+INSTRUCTION_OPCODE\s+of\s+\w+\s*:\s*entity\s+is\b)", regex::ECMAScript);

  MatchResult_t opCodesAttributeMatchResult;

  found = regex_search(bsdlContent.cbegin(), bsdlContent.cend(), opCodesAttributeMatchResult, opCodeAttributeRegex);
  CHECK_TRUE(found,                                     "Cannot find attribute INSTRUCTION_OPCODE in BSDL");
  CHECK_VALUE_NOT_EMPTY(opCodesAttributeMatchResult,    "Failed to get INSTRUCTION_OPCODE match");

  auto remainingSpan = opCodesAttributeMatchResult.suffix();

  // Lamba: Do search value for a single instruction
  auto findOpCodeForInstruction = [
                                   from = remainingSpan.first,
                                   to   = remainingSpan.second
                                  ]
                                  (auto instructionName)
  {
    regex opCodeValueRegex (R"(\b)"s.append(instructionName.cbegin(), instructionName.cend()).append(R"(\s*\(([01]+)\))"), regex::ECMAScript);

    MatchResult_t opCodeValueMatchResult;

    bool found = regex_search(from, to, opCodeValueMatchResult, opCodeValueRegex);
    CHECK_TRUE(found,                             "Cannot find opcode for instruction \""s
                                                  .append(instructionName.cbegin(), instructionName.cend())
                                                  .append("\" in BSDL"));
    CHECK_VALUE_NOT_EMPTY(opCodeValueMatchResult, "Failed to get opcode value");

    auto subMatch = opCodeValueMatchResult[1];
    auto value    = string_view(subMatch.first, subMatch.length());
    return value;
  };

  // Make sure BYPASS is the first instruction value
  if (instructionsNames.front() != "BYPASS")
  {
    m_selectTable.push_back(findOpCodeForInstruction("BYPASS"sv));
  }

  // Get value for all the requested instruction name
  for (const auto instructionName : instructionsNames)
  {
    m_selectTable.push_back(findOpCodeForInstruction(instructionName));
  }
}
//
//  End of: BSDL_Reader::Parse
//---------------------------------------------------------------------------



//===========================================================================
// End of BSDL_Reader.cpp
//===========================================================================
