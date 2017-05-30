//===========================================================================
//                           YamlFile.cpp
//===========================================================================
//
//! @file YamlFile.cpp
//!
//! Implements class YamlFile
//!
//===========================================================================

#include "YamlFile.hpp"
#include "Utility.hpp"
#include "g3log/g3log.hpp"

#include <tuple>
#include <regex>
#include <fstream>

using std::regex;
using std::regex_match;
using std::smatch;
using std::ofstream;
using std::ifstream;
using std::string;
using std::experimental::string_view;
using std::tuple;
using std::make_tuple;
using std::tie;
using std::make_unique;

using namespace mast;


//! Creates root node as a map node
//!
YamlFile::YamlFile ()
  : m_rootNode (YAML::NodeType::Map)
  , m_isDirty  (false)
{
}
//
//  End of: YamlFile::YamlFile
//---------------------------------------------------------------------------




//! Constructs with associated files
//!
YamlFile::YamlFile (string filePath)
  : m_filePath (std::move(filePath))
  , m_isDirty  (false)
{
  if (Utility::FileExists(m_filePath))
  {
    m_rootNode = YAML::LoadFile(m_filePath);
  }
}
//
//  End of: YamlFile::YamlFile
//---------------------------------------------------------------------------



//! Changes/Sets associated file
//!
//! @param associatedFilePath   New associated file path
//!
void YamlFile::AssociatedFile (std::string associatedFilePath)
{
  if (!m_filePath.empty() && m_isDirty)
  {
    Save();
  }

  if (m_filePath != associatedFilePath)
  {
    LoadFile(associatedFilePath);
    m_filePath = associatedFilePath;
  }
}
//
//  End of: YamlFile::AssociatedFile
//---------------------------------------------------------------------------



//! Returns current YAML tree as a string
//!
string YamlFile::AsString () const
{
  YAML::Emitter emitter;
  emitter.SetOutputCharset(YAML::EmitNonAscii);

  string asString;
  {
    std::lock_guard<std::mutex> lock(m_yamlMutex);
//+    m_nodeCache.Clear();  // To remove aliases
    emitter << m_rootNode;
    asString = emitter.str();
  }

  return asString;
}
//
//  End of: YamlFile::AsString
//---------------------------------------------------------------------------



//! Checks if some string is well formatted YAML
//!
//! @param yaml Text representing YAML stuff
//!
//! @return True and empty string when YAML text has been parsed successfuly
//!         and false and error message string when failed to parse it
tuple<bool, string> YamlFile::Check (string_view yaml)
{
  try
  {
    YAML::Load(yaml);
  }
  catch(std::exception& exc)
  {
    return make_tuple(false, exc.what());
  }
  catch (...)
  {
    LOG(DEBUG) << "got unknown type exception";
    return make_tuple(false, "Failed to check YAML:\n" + yaml);
  }

  return make_tuple(true, string());
}
//
//  End of: YamlFile::Check
//---------------------------------------------------------------------------


//! CheckFiles if some string is well formatted YAML
//!
//! @param filePath YAML file path to check
//!
//! @return True and empty string when YAML file has been parsed successfuly
//!         and false and error message string when failed to parse (or open) it
tuple<bool, string> YamlFile::CheckFile (const string& filePath)
{
  try
  {
    ifstream ifs(filePath);
    if (!ifs.is_open())
    {
      return make_tuple(false, "Cannot open YAML file: " + filePath);
    }
    YAML::Load(ifs);
  }
  catch(std::exception& exc)
  {
    return make_tuple(false, exc.what());
  }
  catch (...)
  {
    LOG(DEBUG) << "got unknown type exception";
    return make_tuple(false, "Failed to check YAML file: " + filePath);
  }

  return make_tuple(true, string());
}
//
//  End of: YamlFile::CheckFile
//---------------------------------------------------------------------------


//! Extracts line and column from YAML parser error
//!
//! @param errorMessage YAML parser error message
//! @return Line and column numbers (or zeros when error message does not contain YAML parser error message)
//!
tuple<uint32_t, uint32_t> YamlFile::ExtractLineColumnInformation (const string& errorMessage)
{
  uint32_t line   = 0;
  uint32_t column = 0;

  auto   expr = R"(yaml-cpp: error at line (\d+), column (\d+):.*)";
  regex  searchRegex(expr);
  smatch matchResult;

  if (regex_match(errorMessage, matchResult, searchRegex))
  {
    auto lineField = matchResult[1];
    auto colField  = matchResult[2];

    line   = std::stoi(lineField);
    column = std::stoi(colField);
  }

  return make_tuple(line, column);
}
//
//  End of: YamlFile::ExtractLineColumnInformation
//---------------------------------------------------------------------------



//! Gets value at path a bool
//!
//! @param path Paths from root node to requested value ('/' separated keys)
//!
bool YamlFile::GetAsBool (string_view path)
{
  CHECK_PARAMETER_NOT_EMPTY(path, "Path must not be empty");
  return GetAs_impl<bool>(path);
}
//
//  End of: YamlFile::GetAsBool
//---------------------------------------------------------------------------


//! Gets value at path a bool
//!
//! @param path           Paths from root node to requested value ('/' separated keys)
//! @param defaultValue   Default value if there is no value yet for specified key path
//! @param autoCreate     When true and there is not value yet for specified key path,
//!                       the value is created (along a necessary path)
//!
//! @return Found value or default value
bool YamlFile::GetAsBool (string_view path, bool defaultValue, bool autoCreate)
{
  CHECK_PARAMETER_NOT_EMPTY(path, "Path must not be empty");
  return GetAs_impl<bool>(path, defaultValue, autoCreate);
}
//
//  End of: YamlFile::GetAsBool
//---------------------------------------------------------------------------


//! Gets value at path and index as a string
//!
//! @note Support "out of order" access (converting sequence to map)
//!
//! @param path           Paths from root node to requested value ('/' separated keys)
//! @param index          Index of node in sequence (zero based)
//!
//! @return Found value or throw
bool YamlFile::GetAsBool (string_view path, int index)
{
  CHECK_PARAMETER_NOT_EMPTY(path, "Path must not be empty");
  return GetAs_impl<bool>(path, index);
}
//
//  End of: YamlFile::GetAsBool
//---------------------------------------------------------------------------


//! Gets value at path and index as a string
//!
//! @note Support "out of order" access (converting sequence to map)
//!
//! @param path           Paths from root node to requested value ('/' separated keys)
//! @param index          Index of node in sequence (zero based)
//! @param defaultValue   Default value if there is no value yet for specified key path
//! @param autoCreate     When true and there is not value yet for specified key path,
//!                       the value is created (along a necessary path)
//!
//! @return Found value or default value
bool YamlFile::GetAsBool (string_view path, int index, bool defaultValue, bool autoCreate)
{
  CHECK_PARAMETER_NOT_EMPTY(path, "Path must not be empty");
  return GetAs_impl<bool>(path, index, defaultValue, autoCreate);
}
//
//  End of: YamlFile::GetAsBool
//---------------------------------------------------------------------------




//! Gets value at path as an int
//!
//! @param path Paths from root node to requested value ('/' separated keys)
//!
int YamlFile::GetAsInt (string_view path)
{
  CHECK_PARAMETER_NOT_EMPTY(path, "Path must not be empty");
  return GetAs_impl<int>(path);
}
//
//  End of: YamlFile::GetAsInt
//---------------------------------------------------------------------------

//! Gets value at path as an int
//!
//! @param path           Paths from root node to requested value ('/' separated keys)
//! @param defaultValue   Default value if there is no value yet for specified key path
//! @param autoCreate     When true and there is not value yet for specified key path,
//!                       the value is created (along a necessary path)
//!
//! @return Found value or default value
int YamlFile::GetAsInt (string_view path, int defaultValue, bool autoCreate)
{
  CHECK_PARAMETER_NOT_EMPTY(path, "Path must not be empty");
  return GetAs_impl<int>(path, defaultValue, autoCreate);
}
//
//  End of: YamlFile::GetAsInt
//---------------------------------------------------------------------------


//! Gets value at path and index as a string
//!
//! @note Support "out of order" access (converting sequence to map)
//!
//! @param path           Paths from root node to requested value ('/' separated keys)
//! @param index          Index of node in sequence (zero based)
//!
//! @return Found value or throw
int YamlFile::GetAsInt (string_view path, int index)
{
  CHECK_PARAMETER_NOT_EMPTY(path, "Path must not be empty");
  return GetAs_impl<int>(path, index);
}
//
//  End of: YamlFile::GetAsInt
//---------------------------------------------------------------------------


//! Gets value at path and index as a string
//!
//! @note Support "out of order" access (converting sequence to map)
//!
//! @param path           Paths from root node to requested value ('/' separated keys)
//! @param index          Index of node in sequence (zero based)
//! @param defaultValue   Default value if there is no value yet for specified key path
//! @param autoCreate     When true and there is not value yet for specified key path,
//!                       the value is created (along a necessary path)
//!
//! @return Found value or default value
int YamlFile::GetAsInt (string_view path, int index, int defaultValue, bool autoCreate)
{
  CHECK_PARAMETER_NOT_EMPTY(path, "Path must not be empty");
  return GetAs_impl<int>(path, index, defaultValue, autoCreate);
}
//
//  End of: YamlFile::GetAsInt
//---------------------------------------------------------------------------



//! Gets value at path as a string
//!
//! @param path Paths from root node to requested value ('/' separated keys)
//!
string YamlFile::GetAsString (string_view path)
{
  CHECK_PARAMETER_NOT_EMPTY(path, "Path must not be empty");
  return GetAs_impl<string>(path);
}
//
//  End of: YamlFile::GetAsString
//---------------------------------------------------------------------------



//! Gets value at path as a string
//!
//! @param path           Paths from root node to requested value ('/' separated keys)
//! @param defaultValue   Default value if there is no value yet for specified key path
//! @param autoCreate     When true and there is not value yet for specified key path,
//!                       the value is created (along a necessary path)
//!
//! @return Found value or default value
string YamlFile::GetAsString (string_view path, const string& defaultValue, bool autoCreate)
{
  CHECK_PARAMETER_NOT_EMPTY(path, "Path must not be empty");
  return GetAs_impl<string>(path, defaultValue, autoCreate);
}
//
//  End of: YamlFile::GetAsString
//---------------------------------------------------------------------------


//! Gets value at path and index as a string
//!
//! @note Support "out of order" access (converting sequence to map)
//!
//! @param path           Paths from root node to requested value ('/' separated keys)
//! @param index          Index of node in sequence (zero based)
//!
//! @return Found value or throw
string YamlFile::GetAsString (string_view path, int index)
{
  CHECK_PARAMETER_NOT_EMPTY(path, "Path must not be empty");
  return GetAs_impl<string>(path, index);
}
//
//  End of: YamlFile::GetAsString
//---------------------------------------------------------------------------


//! Gets value at path and index as a string
//!
//! @note Support "out of order" access (converting sequence to map)
//!
//! @param path           Paths from root node to requested value ('/' separated keys)
//! @param index          Index of node in sequence (zero based)
//! @param defaultValue   Default value if there is no value yet for specified key path
//! @param autoCreate     When true and there is not value yet for specified key path,
//!                       the value is created (along a necessary path)
//!
//! @return Found value or default value
string YamlFile::GetAsString (string_view path, int index, const string& defaultValue, bool autoCreate)
{
  CHECK_PARAMETER_NOT_EMPTY(path, "Path must not be empty");
  return GetAs_impl<string>(path, index, defaultValue, autoCreate);
}
//
//  End of: YamlFile::GetAsString
//---------------------------------------------------------------------------



//! Returns node with key matching path from a node
//!
//! @note Caller is responsible to have a lock for underlying yaml node structure
//! @note This bottom-up version has been mesured 25% faster than previous top-down version
//!       With caching feature, this 30% even more efficient (that may be up to twice as fast as initial version)
//!       For C++ tagging, cache hit/miss is almost 1000 when tagging the project (more than 500 files)
//!
//! @param path         Key path from start node ('/' separated keys)
//! @param autoCreate   When true and no node exists at specified path, nodes are created down to specified path
//!
YAML::Node YamlFile::GetNodeForPath (string_view keyPath, bool autoCreate, YAML::NodeType autoCreateType)
{
  CHECK_PARAMETER_NOT_EMPTY(keyPath, "Path from node must not be empty");

  auto tokensCount = Utility::TokensCount (keyPath, PATH_SEPARATOR_S);
  auto level       = tokensCount - 1u;

  auto gotNode = m_nodeCache.GetCachedNode(level, keyPath);
  if (!gotNode.IsNull())
  {
    LOG(INFO) << "Got node from cache[" << level << "] for: " << keyPath;
    return gotNode;
  }

  auto canCacheNode = true;
  auto lastKey      = Utility::BackToken (keyPath, PATH_SEPARATOR_S);

  // Node retrieval/creation
  auto getNode = [&](YAML::Node& refNode)
  {
    auto node = refNode[lastKey];
    if (!node.IsDefined())
    {
      if (autoCreate)
      {
        refNode[lastKey] = YAML::Node(autoCreateType);
        node = refNode[lastKey];
      }
      else
      {
        canCacheNode = false;  // This is to avoid having bad conversion exceptions when using a node defined node
      }
    }
    return node;
  };

  if (tokensCount > 1)
  {
    auto frontPath = Utility::FrontTokens (keyPath, PATH_SEPARATOR_S);
    auto prevNode  = GetNodeForPath(frontPath, autoCreate, YAML::NodeType::Map);  // Create parent node as map node (with no value)

    gotNode = getNode(prevNode);
  }
  else
  {
    gotNode = getNode(m_rootNode);
  }

  if (canCacheNode && m_nodeCache.CacheNode(level, keyPath, gotNode))
  {
    LOG(INFO) << "Saved node in cache[" << level << "] for: " << keyPath;
  }
  return gotNode;
}
//
//  End of YamlFile::GetNodeForPath
//---------------------------------------------------------------------------




//! Loads yaml textual representation from string
//!
//! @param yaml         YAML representation
//! @param saveIfDirty  When true and current content has been modified since
//!                     last load/save, then it is saved to associated file
//!
void YamlFile::Load (string_view yaml, bool saveIfDirty)
{
  if (m_isDirty && saveIfDirty)
  {
    Save();
  }

  {
    std::unique_lock<std::mutex> lock(m_yamlMutex);
    m_rootNode = YAML::Load(yaml);
    m_isDirty  = false;
    m_nodeCache.Clear();
  }
}
//
//  End of: YamlFile::Load
//---------------------------------------------------------------------------


//! Loads yaml textual representation from file
//!
//! @param filePath     YAML file path to load
//! @param saveIfDirty  When true and current content has been modified since
//!                     last load/save, then it is saved to associated file
//!
void YamlFile::LoadFile (const string& filePath, bool saveIfDirty)
{
  if (m_isDirty && saveIfDirty)
  {
    Save();
  }

  {
    std::unique_lock<std::mutex> fileLock(m_fileMutex);
    ifstream ifs(filePath);
    if (!ifs.is_open())
    {
      CHECK_FAILED("Cannot open YAML file: " + filePath);
    }

    {
      std::unique_lock<std::mutex> yamlLock(m_yamlMutex);
      m_rootNode = YAML::Load(ifs);
      m_isDirty  = false;
      m_nodeCache.Clear();
    }
  }
}
//
//  End of: YamlFile::Load
//---------------------------------------------------------------------------


//! Removes key at path
//!
//! @note Does nothing if the key does not exist
//! @note Does not support index style key
//!
void YamlFile::RemoveKey (string_view keyPath)
{
  CHECK_PARAMETER_NOT_EMPTY(keyPath, "Cannot remove key of no path");

  std::unique_lock<std::mutex> lock(m_yamlMutex);

  // ---------------- Check if target node is defined
  //
  YAML::Node node = GetNodeForPath(keyPath, false);

  if (node.IsDefined())
  {
    // ---------------- Search for parent node
    //
    YAML::Node* refNode = &m_rootNode;
    YAML::Node  parentNode;
    auto separatorPos = keyPath.rfind(PATH_SEPARATOR);
    if (separatorPos != string_view::npos)
    {
      auto parentPath = keyPath.substr(0, separatorPos);
      parentNode      = GetNodeForPath(parentPath, false);
      refNode         = &parentNode;

      keyPath.remove_prefix(separatorPos + 1);
    }

    // ---------------- Do remove node from its parent
    //
    if (refNode->IsMap())
    {
      refNode->remove(keyPath);
      m_isDirty = true;
    }

    m_nodeCache.Clear();
  }
}
//
//  End of: YamlFile::RemoveKey
//---------------------------------------------------------------------------


//! Saves current YAML tree into file
//!
void YamlFile::Save (const string& filePath)
{
  CHECK_PARAMETER_NOT_EMPTY(filePath, "Cannot save YAML content to 'empty' path");

  {
    std::unique_lock<std::mutex> fileLock(m_fileMutex);
    ofstream file(filePath, std::ios::trunc);

    CHECK_TRUE(file.is_open(), "Failed to open file is write mode: " + filePath);

    if (file.is_open())
    {
      auto asText = AsString(); // Use lock on yaml mutex!

      file << asText;
      file.flush();
      m_isDirty = false;
    }
  }
}
//
//  End of: YamlFile::Save
//---------------------------------------------------------------------------


//! Sets specified typed value for specified path.
//!
//! @param path       Paths from root node to requested value ('/' separated keys)
//! @param newValue   Value to set for specified path (key)
//!
//! @return true when the key/value pair as been created, false when the node already existed
bool YamlFile::Set (string_view path, const string& newValue)
{
  CHECK_PARAMETER_NOT_EMPTY(path, "Path must not be empty");
  return Set_impl<string>(path, newValue);
}
//
//  End of: YamlFile::Set
//---------------------------------------------------------------------------


//! Sets specified typed value for specified path.
//!
//! @param path       Paths from root node to requested value ('/' separated keys)
//! @param newValue   Value to set for specified path (key)
//!
//! @return true when the key/value pair as been created, false when the node already existed
bool YamlFile::Set (string_view path, int newValue)
{
  CHECK_PARAMETER_NOT_EMPTY(path, "Path must not be empty");
  return Set_impl<int>(path, newValue);
}
//
//  End of: YamlFile::Set
//---------------------------------------------------------------------------


//! Sets specified typed value for specified path.
//!
//! @param path       Paths from root node to requested value ('/' separated keys)
//! @param newValue   Value to set for specified path (key)
//!
//! @return true when the key/value pair as been created, false when the node already existed
bool YamlFile::Set (string_view path, bool newValue)
{
  CHECK_PARAMETER_NOT_EMPTY(path, "Path must not be empty");
  return Set_impl<bool>(path, newValue);
}
//
//  End of: YamlFile::Set
//---------------------------------------------------------------------------


//! Tries to gets value at path as a bool
//!
//! @param path Paths from root node to requested value ('/' separated keys)
//!
//! @return Found value and success (true) bool or default bool and failure (false) bool
tuple<bool, bool> YamlFile::TryGetAsBool (string_view path)
{
  CHECK_PARAMETER_NOT_EMPTY(path, "Path must not be empty");
  return TryGetAs_impl<bool>(path);
}
//
//  End of: YamlFile::TryGetAsBool
//---------------------------------------------------------------------------


//! Tries to gets value at path as a int
//!
//! @param path Paths from root node to requested value ('/' separated keys)
//!
//! @return Found value and success (true) bool or default int and failure (false) bool
tuple<bool, int> YamlFile::TryGetAsInt (string_view path)
{
  CHECK_PARAMETER_NOT_EMPTY(path, "Path must not be empty");
  return TryGetAs_impl<int>(path);
}
//
//  End of: YamlFile::TryGetAsInt
//---------------------------------------------------------------------------


//! Tries to gets value at path as a string
//!
//! @param path Paths from root node to requested value ('/' separated keys)
//!
//! @return Found value and success (true) bool or empty string and failure (false) bool
tuple<bool, string> YamlFile::TryGetAsString (string_view path)
{
  CHECK_PARAMETER_NOT_EMPTY(path, "Path must not be empty");
  return TryGetAs_impl<string>(path);
}
//
//  End of: YamlFile::TryGetAsString
//---------------------------------------------------------------------------


//! Tries to load yaml textual representation from file
//!
//! @param filePath     YAML file path to load
//! @param saveIfDirty  When true and current content has been modified since
//!                     last load/save, then it is saved to associated file
//!
//! @return True and empty string when YAML file has been load (and parsed) successfuly
//!         and false and an error message string when failed to parse (or open) it
//!
tuple<bool, string> YamlFile::TryLoadFile (const string& filePath, bool saveIfDirty)
{
  try
  {
    if (m_isDirty && saveIfDirty)
    {
      Save();
      m_isDirty = false;
    }

    ifstream ifs(filePath);
    if (!ifs.is_open())
    {
      return make_tuple(false, "Cannot open YAML file: " + filePath);
    }

    {
      std::unique_lock<std::mutex> lock(m_yamlMutex);
      m_nodeCache.Clear();
      m_rootNode = YAML::Load(ifs);
      m_isDirty  = false;
    }
  }
  catch(std::exception& exc)
  {
    return make_tuple(false, exc.what());
  }
  catch (...)
  {
    LOG(DEBUG) << "got unknown type exception";
    return make_tuple(false, "Failed to Load YAML file: " + filePath);
  }

  return make_tuple(true, string());
}
//
//  End of: YamlFile::TryLoadFile
//---------------------------------------------------------------------------

//===========================================================================
// End of YamlFile.cpp
//===========================================================================
