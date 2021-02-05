#include "engine/core/serializejson.h"


void engine::deserializeJson(std::string & value,
                             const Json::Value & doc,
                             const std::string & label)
{
  const Json::Value * node = doc.find(label.c_str(), label.c_str() + label.size());

  if(!node)
    throw std::runtime_error("Cannot find node " + label);

  if(!node->isConvertibleTo(Json::ValueType::stringValue))
    throw std::runtime_error("Node " + label + " cannot be converted to string");

  value = node->asString();
}

void engine::deserializeJson(std::filesystem::path & value,
                             const Json::Value & doc,
                             const std::string & label)
{
  const Json::Value * node = doc.find(label.c_str(), label.c_str() + label.size());

  if(!node)
    throw std::runtime_error("Cannot find node " + label);

  if(!node->isConvertibleTo(Json::ValueType::stringValue))
    throw std::runtime_error("Node " + label + " cannot be converted to string");

  value = node->asString();
}
