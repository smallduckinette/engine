#include "engine/core/serializejson.h"

const Json::Value & engine::detail::getNode(const Json::Value & doc,
                                            const std::string & label,
                                            Json::ValueType valueType)
{
  const Json::Value * node = doc.find(label.c_str(), label.c_str() + label.size());

  if(!node)
    throw std::runtime_error("Cannot find node " + label);

  if(!node->isConvertibleTo(valueType))
    throw std::runtime_error("Node " + label + " cannot be converted to type");

  return *node;
}

void engine::detail::deserializeJsonValue(std::string & value,
                                          const Json::Value & node)
{
  value = node.asString();
}

void engine::detail::deserializeJsonValue(std::filesystem::path & value,
                                          const Json::Value & node)
{
  value = node.asString();
}

void engine::detail::deserializeJsonValue(int & value,
                                          const Json::Value & node)
{
  value = node.asInt();
}

void engine::detail::deserializeJsonValue(float & value,
                                          const Json::Value & node)
{
  value = node.asFloat();
}

void engine::deserializeJson(std::string & value,
                             const Json::Value & doc,
                             const std::string & label)
{
  detail::deserializeJsonValue(value, detail::getNode(doc, label, Json::ValueType::stringValue));
}

void engine::deserializeJson(std::filesystem::path & value,
                             const Json::Value & doc,
                             const std::string & label)
{
  detail::deserializeJsonValue(value, detail::getNode(doc, label, Json::ValueType::stringValue));
}

void engine::deserializeJson(int & value,
                             const Json::Value & doc,
                             const std::string & label)
{
  detail::deserializeJsonValue(value, detail::getNode(doc, label, Json::ValueType::intValue));
}

void engine::deserializeJson(float & value,
                             const Json::Value & doc,
                             const std::string & label)
{
  detail::deserializeJsonValue(value, detail::getNode(doc, label, Json::ValueType::realValue));
}
