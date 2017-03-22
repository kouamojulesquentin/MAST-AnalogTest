#ifndef SIT_TYPES_H
#define SIT_TYPES_H

#include <string>
#include <vector>
#include <memory>
#include <stdint.h>

namespace mast
{
  class SystemModelNode;
  class Linker;
  enum class SelectorProperty;
} // End of namespace mast


struct name_type
{
  std::uint8_t is_transparent;
  std::string  name;
};

struct node_list_type
{
  std::uint32_t n_nodes;
  std::string   name;
  std::vector<std::shared_ptr<mast::SystemModelNode>> nodes;
};

//! Informations to create PathSelector associated with linker
//!
//! @note Register driving the selector may be yet unknown when the linker is created
//!
struct linker_information
{
  std::shared_ptr<mast::Linker> linker_node;
  uint32_t                      line              = 0;
  uint32_t                      beginColumn       = 0;
  uint32_t                      endColumn         = 0;
  uint32_t                      max_derivations   = 0;
  mast::SelectorProperty        selector_property = static_cast<mast::SelectorProperty>(0);
  std::string                   selector_kind_name;
  std::string                   selector_name;
};

#endif
