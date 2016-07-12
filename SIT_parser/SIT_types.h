#ifndef SIT_TYPES_H
#define SIT_TYPES_H
 
#include <string>
#include <stdint.h>
 

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
  } node_list_type;

typedef struct IR_coding_s
  {
    std::uint32_t n_words;
    std::string codeword; 
  } IR_coding_type;

#endif
