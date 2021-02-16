#include "engine/core/serializejson.h"

namespace
{
  const Json::Value * getNode(const Json::Value & doc,
                              const std::string & label,
                              Json::ValueType valueType)
  {
    const Json::Value * node = doc.find(label.c_str(), label.c_str() + label.size());

    if(!node)
      throw std::runtime_error("Cannot find node " + label);

    if(!node->isConvertibleTo(valueType))
      throw std::runtime_error("Node " + label + " cannot be converted to type");

    return node;
  }
}

void engine::deserializeJson(std::string & value,
                             const Json::Value & doc,
                             const std::string & label)
{
  auto node = getNode(doc, label, Json::ValueType::stringValue);

  value = node->asString();
}

void engine::deserializeJson(std::filesystem::path & value,
                             const Json::Value & doc,
                             const std::string & label)
{
  auto node = getNode(doc, label, Json::ValueType::stringValue);

  value = node->asString();
}

void engine::deserializeJson(int & value,
                             const Json::Value & doc,
                             const std::string & label)
{
  auto node = getNode(doc, label, Json::ValueType::intValue);

  value = node->asInt();
}
