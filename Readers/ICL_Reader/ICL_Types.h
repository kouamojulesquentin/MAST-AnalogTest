

#ifndef ICL_TYPES_H__13976FFB_1B98_4AF7_78B3_F79FFC44E47C__INCLUDED_
  #define ICL_TYPES_H__13976FFB_1B98_4AF7_78B3_F79FFC44E47C__INCLUDED_

#include <string>
#include <vector>
#include <memory>
#include <stdint.h>

//! @todo [JFC]-[October/02/2017]: Refactor AND comment that (this is a copy from SIT reader)
//!
namespace mast
{
  class SystemModelNode;
  class Linker;
  enum class SelectorProperty;
}


struct name_type
{
  std::uint8_t is_transparent;
  std::string  name;
};

struct node_list_type
{
  std::uint32_t n_nodes = 0;
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
  std::string                   selector_kind_name;
  std::string                   selector_reg_name;   //!< Name of register associated with the selector (driving the mux)
  std::string                   selector_parameters; //!< Optional parameters (for custom PathSelector)
  mast::SelectorProperty        selector_property = static_cast<mast::SelectorProperty>(0);
};

#endif  // not defined ICL_TYPES_H__13976FFB_1B98_4AF7_78B3_F79FFC44E47C__INCLUDED_
