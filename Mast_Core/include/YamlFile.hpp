//===========================================================================
//                           YamlFile.hpp
//===========================================================================
//
//! @file YamlFile.hpp
//!
//! Declares YamlFile class
//!
//===========================================================================

#ifndef YAMLFILE_H__5A03FF85_94A0_4F8D_8382_323E4EA3AB69__INCLUDED_
  #define YAMLFILE_H__5A03FF85_94A0_4F8D_8382_323E4EA3AB69__INCLUDED_

#include "yaml-cpp/yaml.h"
#include "Utility.hpp"
#include "YamlNodesCache.hpp"

#include <string>
#include <experimental/string_view>
#include <memory>
#include <tuple>
#include <atomic>
#include <mutex>


namespace YAML
{
  class Node;
}

namespace mast
{
//! Represents a YAML formatted file, with helper methods to get/set values, track unsaved state...
//!
//! @note It is intended to be thread safe in the sens of each action can be done by a different thread.
//!       Use only exclusive access mutex because even getter can modify the underlying structure.
//!
class MAST_CORE_EXPORT YamlFile
{
  // ---------------- Public members
  //
  public:
  static constexpr char PATH_SEPARATOR   = '|'; //!< Yaml hierarchy path separator
  static constexpr auto PATH_SEPARATOR_S = "|"; //!< Yaml hierarchy path separator

  // ---------------- Public  Methods
  //
  public:
  using string_view = std::experimental::string_view;

  virtual ~YamlFile() = default;
  YamlFile();
  YamlFile(std::string filePath);

  virtual const std::string& AssociatedFile() const { return m_filePath; }   //!< Returns currently associated file
  virtual void               AssociatedFile(std::string associatedFilePath); //!< Changes/Sets associated file

  std::string AsString () const;

  bool IsDirty() const { return m_isDirty; }   //!< Returns dirty flag

  static std::tuple<bool, std::string> Check       (string_view        yaml);                               //!< Checks if some string is well formatted YAML
  static std::tuple<bool, std::string> CheckFile   (const std::string& filePath);                           //!< Checks if some file contains well formatted YAML
  std::tuple<bool,        std::string> TryLoadFile (const std::string& filePath, bool saveIfDirty = false); //!< Tries loading from a yaml file

  static std::tuple<uint32_t, uint32_t> ExtractLineColumnInformation(const std::string& errorMessage);      //!< Extracts line and column from YAML parser error

  void Load     (string_view        yaml,     bool saveIfDirty = false); //!< Loads from a string
  void LoadFile (const std::string& filePath, bool saveIfDirty = false); //!< Loads from a yaml file

  virtual void Save(const std::string& filePath);
  virtual void Save() { Save(m_filePath); }

  std::string GetAsString (string_view path); //!< Gets value at path as string
  int         GetAsInt    (string_view path); //!< Gets value at path as int
  bool        GetAsBool   (string_view path); //!< Gets value at path as bool

  std::string GetAsString (string_view path, const std::string& defaultValue, bool autoCreate); //!< Gets value at path as string
  int         GetAsInt    (string_view path, int                defaultValue, bool autoCreate); //!< Gets value at path as int
  bool        GetAsBool   (string_view path, bool               defaultValue, bool autoCreate); //!< Gets value at path as bool

  std::string GetAsString (string_view path, int index);                                                           //!< Gets sequence value at path as string
  int         GetAsInt    (string_view path, int index);                                                           //!< Gets sequence value at path as int
  bool        GetAsBool   (string_view path, int index);                                                           //!< Gets sequence value at path as bool

  std::string GetAsString (string_view path, int index, const std::string& defaultValue, bool autoCreate); //!< Gets sequence value at path as string
  int         GetAsInt    (string_view path, int index, int                defaultValue, bool autoCreate); //!< Gets sequence value at path as int
  bool        GetAsBool   (string_view path, int index, bool               defaultValue, bool autoCreate); //!< Gets sequence value at path as bool

  std::tuple<bool, std::string> TryGetAsString (string_view path); //!< Tries getting value at path as string
  std::tuple<bool, int>         TryGetAsInt    (string_view path); //!< Tries getting value at path as int
  std::tuple<bool, bool>        TryGetAsBool   (string_view path); //!< Tries getting value at path as bool

  std::tuple<bool, std::string> TryGet (string_view path, const std::string&) { return TryGetAsString (path); } //!< Tries getting value at path as string @note this overload is useful in template context
  std::tuple<bool, int>         TryGet (string_view path, int)                { return TryGetAsInt    (path); } //!< Tries getting value at path as int    @note this overload is useful in template context
  std::tuple<bool, bool>        TryGet (string_view path, bool)               { return TryGetAsBool   (path); } //!< Tries getting value at path as bool   @note this overload is useful in template context

  std::string Get(string_view path, const std::string& defaultValue, bool autoCreate) { return GetAsString (path, defaultValue, autoCreate); }
  int         Get(string_view path, int                defaultValue, bool autoCreate) { return GetAsInt    (path, defaultValue, autoCreate); }
  bool        Get(string_view path, bool               defaultValue, bool autoCreate) { return GetAsBool   (path, defaultValue, autoCreate); }

  bool Set (string_view path, const std::string& newValue); //!< Sets string value at path
  bool Set (string_view path, int                newValue); //!< Sets int value at path
  bool Set (string_view path, bool               newValue); //!< Sets bool value at path

  //!< Sets string value at path
  //!< @note  this overload is mainly to protect againgst conversion from char* to bool
  bool Set (string_view path, const char*        newValue) { return Set(path, std::string(newValue)); }

  void RemoveKey (string_view path); //!< Removes key at path

  // ---------------- Private  Methods
  //
  private:
  YAML::Node GetNodeForPath (string_view path, bool autoCreate, YAML::NodeType autoCreateType = YAML::NodeType::Null);

  //! Tries to get specified typed value for specified path.
  //! @note Is private to avoid too many expansions of code template
  template<typename T>
  std::tuple<bool, T> TryGetAs_impl (string_view path)
  {
    std::lock_guard<std::mutex> lock(m_yamlMutex);

    auto node = GetNodeForPath(path, false);

    if (node.IsDefined() && node.IsScalar())
    {
      return std::make_tuple(true, node.as<T>());
    }

    return std::make_tuple(false, T());
  }

  //! Returns specified typed value for specified path.
  //! @note Is private to avoid too many expansions of code template
  template<typename T>
  T GetAs_impl (string_view path)
  {
    bool found;
    T    value;
    std::tie(found, value) = TryGetAs_impl<T>(path);

    if (found)
    {
      return value;
    }
    CHECK_FAILED("Cannot find specified YAML path or it does not refer to a scalar node");
  }

  //! Returns specified typed value for specified path.
  //! @note Is private to avoid too many expansions of code template
  template<typename T>
  T GetAs_impl (string_view path, T defaultValue, bool autoCreate)
  {
    std::lock_guard<std::mutex> lock(m_yamlMutex);

    auto node = GetNodeForPath(path, autoCreate);

    if (node.IsDefined() && node.IsScalar())
    {
      return node.as<T>();
    }

    if (node.IsNull() && autoCreate)
    {
      node = defaultValue;
      m_isDirty = true;
    }

    return defaultValue;
  }


  //! Returns specified typed value for specified path.
  //! @note Is private to avoid too many expansions of code template
  template<typename T>
  T GetAs_impl (string_view path, int index)
  {
    std::lock_guard<std::mutex> lock(m_yamlMutex);

    YAML::Node baseNode = GetNodeForPath(path, false);

    if (       baseNode.IsDefined()
        && (   baseNode.IsSequence()
            || baseNode.IsMap()         // to support out of order (auto)creation
            || baseNode.IsNull()
           )
       )
    {
      auto node = baseNode[index];

      if (node.IsDefined() && node.IsScalar())
      {
        return node.as<T>();
      }
    }

    CHECK_FAILED("Cannot find specified YAML path or it does not refer to a scalar node");
  }


  //! Returns specified typed value for specified path and index.
  //!
  //! @note Support "out of order" access (converting sequence to map)
  //! @note Is private to avoid too many expansions of code template
  template<typename T>
  T GetAs_impl (string_view path, int index, T defaultValue, bool autoCreate)
  {
    std::lock_guard<std::mutex> lock(m_yamlMutex);

    YAML::Node baseNode = GetNodeForPath(path, autoCreate);
    YAML::Node node;

    if (    baseNode.IsDefined()
        && (   baseNode.IsSequence()
            || baseNode.IsMap()         // to support out of order (auto)creation
            || baseNode.IsNull()
           )
       )
    {
      node = baseNode[index];
    }

    if (node.IsDefined() && node.IsScalar())
    {
      return node.as<T>();
    }

    if (autoCreate && (!node.IsDefined() || node.IsNull()))
    {
      node = defaultValue;
      m_isDirty = true;
    }

    return defaultValue;
  }


  //! Sets specified typed value for specified path.
  //!
  //! @note Is private to avoid too many expansions of code template
  //!
  //! @return true when the key/value pair as been created, false when the node already existed
  template<typename T>
  bool Set_impl(string_view path, const T& newValue)
  {
    std::lock_guard<std::mutex> lock(m_yamlMutex);

    auto node    = GetNodeForPath(path, true);
    auto created = node.IsNull();

    if (created || (node.as<T>() != newValue))
    {
      node      = newValue;
      m_isDirty = true;
    }

    return created;
  }

  // ---------------- Private  Fields
  //
  private:

  std::string            m_filePath;  //!< Associated file (may be empty when not needed or managed outside this class)
  mutable std::mutex     m_fileMutex; //!< Required to arbitrate simultaneous calls to save/load - intended to be locked before m_yamlMutex (to avoid dead lock)
  YAML::Node             m_rootNode;  //!< Root YAML node
  mutable std::mutex     m_yamlMutex; //!< Required to serialize access to underlying representation
  std::atomic_bool       m_isDirty;   //!< Becomes true when YAML structure has been modified by not yet saved
  mutable YamlNodesCache m_nodeCache; //!< Mutable because it is logically not a state element of the instance
};
//
//  End of YamlFile class declaration
//---------------------------------------------------------------------------
} // End of namespace mast


#endif  // not defined YAMLFILE_H__5A03FF85_94A0_4F8D_8382_323E4EA3AB69__INCLUDED_

//===========================================================================
// End of YamlFile.hpp
//===========================================================================
