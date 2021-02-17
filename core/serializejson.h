#ifndef __CORE_SERIALIZEJSON_H__
#define __CORE_SERIALIZEJSON_H__

#include <filesystem>
#include <json/json.h>

namespace engine
{
  namespace detail
  {
    const Json::Value & getNode(const Json::Value & doc,
                                const std::string & label,
                                Json::ValueType valueType);

    /// Deserialize a string value
    void deserializeJsonValue(std::string & value,
                              const Json::Value & node);

    /// Deserialize a path value
    void deserializeJsonValue(std::filesystem::path & value,
                              const Json::Value & node);

    /// Deserialize an int value
    void deserializeJsonValue(int & value,
                              const Json::Value & node);

    /// Deserialize an float value
    void deserializeJsonValue(float & value,
                              const Json::Value & node);

    /// Deserialize a generic type value
    template<typename T>
    void deserializeJsonValue(T & value,
                              const Json::Value & node)
    {
      // Look for a deserialize method on the object
      value.deserialize(node);
    }
  }

  /// Deserialize a string
  void deserializeJson(std::string & value,
                       const Json::Value & doc,
                       const std::string & label);

  /// Deserialize a path
  void deserializeJson(std::filesystem::path & value,
                       const Json::Value & doc,
                       const std::string & label);

  /// Deserialize an int
  void deserializeJson(int & value,
                       const Json::Value & doc,
                       const std::string & label);

  /// Deserialize an float
  void deserializeJson(float & value,
                       const Json::Value & doc,
                       const std::string & label);

  /// Deserialize a generic type
  template<typename T>
  void deserializeJson(T & value,
                       const Json::Value & doc,
                       const std::string & label)
  {
    // Look for a deserialize method on the object
    detail::deserializeJsonValue(value,
                                 detail::getNode(doc, label, Json::ValueType::objectValue));
  }

  /// Deserialize an optional
  template<typename T>
  void deserializeJson(std::optional<T> & value,
                       const Json::Value & doc,
                       const std::string & label)
  {
    if(doc.isMember(label))
    {
      value = T();
      deserializeJson(*value, doc, label);
    }
    else
    {
      value.reset();
    }
  }

  /// Deserialize a vector
  template<typename T>
  void deserializeJson(std::vector<T> & value,
                       const Json::Value & doc,
                       const std::string & label)
  {
    for(auto && node : detail::getNode(doc, label, Json::ValueType::arrayValue))
    {
      T item;
      detail::deserializeJsonValue(item, node);
      value.push_back(item);
    }
  }
}

#endif
