#ifndef __CORE_SERIALIZEJSON_H__
#define __CORE_SERIALIZEJSON_H__

#include <filesystem>
#include <json/json.h>

namespace engine
{
  /// Deserialize a string
  void deserializeJson(std::string & value,
                       const Json::Value & doc,
                       const std::string & label);

  /// Deserialize a path
  void deserializeJson(std::filesystem::path & value,
                       const Json::Value & doc,
                       const std::string & label);
}

#endif
