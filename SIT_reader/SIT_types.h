#ifndef SIT_TYPES_H
#define SIT_TYPES_H

#include <string>
#include <vector>
#include <memory>
#include <stdint.h>

namespace mast
{
  class SystemModelNode;
} // End of namespace mast


 typedef struct node_name_s
  {
    std::uint8_t is_transparent;
    std::string name;
  } name_type;

enum ctrl_position { BEFORE, AFTER};
enum logic_polarity { HIGH, LOW};

typedef struct node_list_type_s
  {
    std::uint32_t n_nodes;
    std::string name;
    std::vector<std::shared_ptr<mast::SystemModelNode>> nodes;
  } node_list_type;

  struct linker_information
  {
   std::shared_ptr<mast::SystemModelNode> linker_node;
   uint32_t column;
   uint32_t line;
   uint32_t derivations;
   uint32_t selector_kind_index;
   std::string selector_name;
  };

#endif
