//===========================================================================
//                           YamlNodesCache.cpp
//===========================================================================
//
//! @file YamlNodesCache.cpp
//!
//! Implements class YamlNodesCache
//!
//===========================================================================

#include "YamlNodesCache.hpp"

using std::experimental::string_view;
using mast::YamlNodesCache;


//! Caches a YAML::Node
//!
//! @note When level is higher than max managed level, nothing is done
//!
//! @param level    Depth level of node (zero based)
//! @param nodeId   Associated node identifier (usually a hierarchy path)
//! @param node     Node to cache
//!
//! @return true when the node has effectively put in cache, false otherwise
bool YamlNodesCache::CacheNode(size_t level, string_view nodeId, YAML::Node node)
{
  if (level < m_nodeCache.size())
  {
    auto& buffer = m_nodeCache[level];
    if (buffer.size() == MAX_CACHED_PER_DEPTH)
    {
      buffer.pop_back();
    }

    buffer.push_front(NodeCacheItem(nodeId, node));
    return true;
  }
  return false;
}
//
//  End of: YamlNodesCache::CacheNode
//---------------------------------------------------------------------------



//! Clears all cache content
//!
void YamlNodesCache::Clear ()
{
  for (auto& buffer : m_nodeCache)
  {
    buffer.clear();
  }
}
//
//  End of: YamlNodesCache::Clear
//---------------------------------------------------------------------------

//===========================================================================
// End of YamlNodesCache.cpp
//===========================================================================
