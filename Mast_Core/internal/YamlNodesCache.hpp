//===========================================================================
//                           YamlNodesCache.hpp
//===========================================================================
//
//! @file YamlNodesCache.hpp
//!
//! Declares YamlNodesCache class
//!
//===========================================================================


#ifndef YAMLNODESCACHE_H__29A2BCD4_8A81_4829_B89_6CDE36A5297__INCLUDED_
  #define YAMLNODESCACHE_H__29A2BCD4_8A81_4829_B89_6CDE36A5297__INCLUDED_

#include "yaml-cpp/yaml.h"

#include <array>
#include <deque>
#include <algorithm>
#include <experimental/string_view>

namespace mast
{
//! Items stored by cache buffers
//!
//! @note This is necessary to used node reset instead of default assignment done by containers
//!       used to store data
//!
struct NodeCacheItem
{
  NodeCacheItem(std::experimental::string_view nodeId, YAML::Node node)
    : m_nodeId (nodeId)
    , m_node   (node)
  {
  }

  NodeCacheItem(const NodeCacheItem& rhs)
    : m_nodeId (rhs.m_nodeId)
    , m_node   (rhs.m_node)
  {
  }

  NodeCacheItem(NodeCacheItem&& rhs)
    : m_nodeId (std::move(rhs.m_nodeId))
    , m_node   (std::move(rhs.m_node))
  {
  }

  NodeCacheItem& operator=(NodeCacheItem&& rhs)
  {
    if (this != &rhs)
    {
      m_nodeId = std::move(rhs.m_nodeId);
      m_node.reset(rhs.m_node);
    }
    return *this;
  }

  std::string m_nodeId; //!< Identifies the stored (cached) node
  YAML::Node  m_node;   //!< Stored (cached) node
};


//! Manages a 2D cache of YAML nodes.
//!
//! @note The cache uses 2 dimensons. One relative to some hierarchy depth level and
//!       the second one cache nodes at same level (they may not share the same parents)
//! @note The cache takes care of not creating references between related nodes
//!
class YamlNodesCache final
{
  // ---------------- Public  Methods
  //
  public:
  ~YamlNodesCache() = default;
  YamlNodesCache()  = default;

  //! Removes all references to nodes in cache
  //!
  void Clear();

  using string_view = std::experimental::string_view;

  //! Tries to get an already cached node
  //!
  //! @note Inline because it will be called for each search node
  inline YAML::Node GetCachedNode(size_t level, string_view nodeId);

  // ---------------- Cache a node
  //
  bool CacheNode (size_t level, string_view keyPath, YAML::Node);

  // ---------------- Private  Fields
  //
  private:
  static constexpr auto MAX_CACHED_DEPTH     = 6u;   // To include all tagging parameters
  static constexpr auto MAX_CACHED_PER_DEPTH = 20u;  // To be effective for C++ tagging (18 filters); the value is essential to get a high hit/miss ratio

  using CachedNode_t = std::deque<NodeCacheItem>;

  std::array<CachedNode_t, MAX_CACHED_DEPTH> m_nodeCache;
};
//
//  End of YamlNodesCache class declaration
//---------------------------------------------------------------------------

//! Tries to get an already cached node
//!
//! @param level   Depth level of node (zero based)
//! @param nodeId  Associated node identifier (usually a hierarchy path)
//!
//! @return Found cached node or Null node when there is no cached node with nodeId for level
//!
YAML::Node YamlNodesCache::GetCachedNode(size_t level, string_view nodeId)
{
  if (level < m_nodeCache.size())
  {
    const auto& buffer   = m_nodeCache[level];
    const auto  foundPos = std::find_if(buffer.begin(), buffer.end(), [=](const auto& cacheItem) { return cacheItem.m_nodeId == nodeId; });

    if (foundPos != buffer.end())
    {
      return foundPos->m_node;
    }
  }
  return YAML::Node();
}

} // End of namespace mast


#endif  // not defined YAMLNODESCACHE_H__29A2BCD4_8A81_4829_B89_6CDE36A5297__INCLUDED_
//===========================================================================
// End of YamlNodesCache.hpp
//===========================================================================
