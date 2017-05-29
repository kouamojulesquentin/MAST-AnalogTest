#ifndef COLLECTIONSTACK_H_62B23520_7C8E_11DE_8A39_0800200C9A66
#define COLLECTIONSTACK_H_62B23520_7C8E_11DE_8A39_0800200C9A66

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

#include <stack>
#include <cassert>
#include <string>
#include <stdexcept>

namespace YAML {
struct CollectionType {
  enum value { NoCollection, BlockMap, BlockSeq, FlowMap, FlowSeq, CompactMap };
};

class CollectionStack {
 public:
  CollectionType::value GetCurCollectionType() const {
    if (collectionStack.empty())
      return CollectionType::NoCollection;
    return collectionStack.top();
  }

  void PushCollectionType(CollectionType::value type) {
    collectionStack.push(type);
  }

  void PopCollectionType(CollectionType::value type)
  {
    if (type != GetCurCollectionType())
    {
      std::string msg;
      msg.append("std::invalid_argument: CollectionType to pop is different from expected.\nIn file:     ").append(__FILE__);
      msg.append("\nIn file:     ").append(__FILE__);
      msg.append("\nIn function: ").append(__func__);
      msg.append("\nAt line:     ").append(std::to_string(__LINE__));

      throw std::invalid_argument(msg);
    }
    collectionStack.pop();
  }

 private:
  std::stack<CollectionType::value> collectionStack;
};
}

#endif  // COLLECTIONSTACK_H_62B23520_7C8E_11DE_8A39_0800200C9A66
