//===========================================================================
//                           Yaml_Traits.h
//===========================================================================
//! @file Yaml_Traits.h
//!
//! Declares cxxtest traits for classes and enums in Yaml library
//!
//===========================================================================


#ifndef YAML_TRAITS_H__D0FFA8D8_7F7_4097_F493_486497D50E8__INCLUDED_
  #define YAML_TRAITS_H__D0FFA8D8_7F7_4097_F493_486497D50E8__INCLUDED_

#include "yaml-cpp/node/type.h"
#include <cxxtest/ValueTraits.h>

//! Enables CxxTest to display enums using their names (instead of their integer values)
//!
CXXTEST_ENUM_TRAITS(YAML::NodeType,
                    CXXTEST_ENUM_MEMBER(YAML::NodeType::Undefined)
                    CXXTEST_ENUM_MEMBER(YAML::NodeType::Null)
                    CXXTEST_ENUM_MEMBER(YAML::NodeType::Scalar)
                    CXXTEST_ENUM_MEMBER(YAML::NodeType::Sequence)
                    CXXTEST_ENUM_MEMBER(YAML::NodeType::Map)
                   );


#endif  // not defined YAML_TRAITS_H__D0FFA8D8_7F7_4097_F493_486497D50E8__INCLUDED_
//===========================================================================
// End of Yaml_Traits.h
//===========================================================================



