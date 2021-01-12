#include "json_utils.h"

#include <glm/gtc/type_ptr.hpp>
#include <cassert>

void engine::detail::parse(const std::string & label,
                           const Json::Value * node,
                           size_t & value)
{
  assert(node);
  
  if(!node->isConvertibleTo(Json::ValueType::uintValue))
    throw std::runtime_error("Node " + label + " cannot be converted to unsigned integer");
  
  value = node->asUInt64();
}

void engine::detail::parse(const std::string & label,
                           const Json::Value * node,
                           unsigned int & value)
{
  assert(node);
  
  if(!node->isConvertibleTo(Json::ValueType::uintValue))
    throw std::runtime_error("Node " + label + " cannot be converted to unsigned integer");
  
  value = node->asUInt();
}

void engine::detail::parse(const std::string & label,
                           const Json::Value * node,
                           std::string & value)
{
  assert(node);
  
  if(!node->isConvertibleTo(Json::ValueType::stringValue))
    throw std::runtime_error("Node " + label + " cannot be converted to string");
  
  value = node->asString();
}

void engine::detail::parse(const std::string & label,
                           const Json::Value * node,
                           float & value)
{
  assert(node);

  if(!node->isConvertibleTo(Json::ValueType::realValue))
    throw std::runtime_error("Node " + label + " cannot be converted to float");
  
  value = node->asDouble(); 
}

void engine::detail::parse(const std::string & label,
                           const Json::Value * node,
                           bool & value)
{
  assert(node);

  if(!node->isConvertibleTo(Json::ValueType::booleanValue))
    throw std::runtime_error("Node " + label + " cannot be converted to boolean");
  
  value = node->asBool();   
}

const Json::Value * engine::getNode(const Json::Value & document,
                                    const std::string & name)
{
  return document.find(name.c_str(), name.c_str() + name.size());
}

const Json::Value & engine::getNodeOrThrow(const Json::Value & document,
                                           const std::string & name)
{
  const Json::Value * node = getNode(document, name);
  if(node)
    return *node;
  else
    throw std::runtime_error("Cannot find expected node " + name);
}

void engine::get(const Json::Value & document,
                 const std::string & name,
                 glm::mat4 & value)
{
  std::vector<float> elts;
  get(document, name, elts);
  if(elts.size() != 16)
    throw std::runtime_error("Failed to parse matrix " + name + " : bad number of elements");
  
  value = glm::make_mat4(&elts[0]);
}

void engine::get(const Json::Value & document,
                 const std::string & name,
                 glm::quat & value)
{
  std::vector<float> elts;
  get(document, name, elts);
  if(elts.size() != 4)
    throw std::runtime_error("Failed to parse quaternion " + name + " : bad number of elements");
  
  value = glm::make_quat(&elts[0]);
}

void engine::get(const Json::Value & document,
                 const std::string & name,
                 glm::vec3 & value)
{
  std::vector<float> elts;
  get(document, name, elts);
  if(elts.size() != 3)
    throw std::runtime_error("Failed to parse vec3 " + name + " : bad number of elements");
  
  value = glm::make_vec3(&elts[0]);
}

void engine::get(const Json::Value & document,
                 const std::string & name,
                 glm::vec4 & value)
{
  std::vector<float> elts;
  get(document, name, elts);
  if(elts.size() != 4)
    throw std::runtime_error("Failed to parse vec4 " + name + " : bad number of elements");
  
  value = glm::make_vec4(&elts[0]);
}
