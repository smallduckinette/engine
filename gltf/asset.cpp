#include "asset.h"

#include <fstream>
#include <iterator>
#include <boost/algorithm/string/predicate.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#undef GLM_ENABLE_EXPERIMENTAL

#include "base64.h"
#include "engine/core/json_utils.h"

namespace glm
{
  std::ostream& operator<<(std::ostream & str, const glm::mat4 & m)
  {
    return str << glm::to_string(m);
  }

  std::ostream& operator<<(std::ostream & str, const glm::vec3 & v)
  {
    return str << glm::to_string(v);
  }

  std::ostream& operator<<(std::ostream & str, const glm::vec4 & v)
  {
    return str << glm::to_string(v);
  }

  std::ostream& operator<<(std::ostream & str, const glm::quat & q)
  {
    return str << glm::to_string(q);
  }
}

namespace std
{  
  template<typename T>
  ostream & operator<<(ostream & str, const optional<T> & opt)
  {
    str << "{";
    if(opt)
      str << *opt;
    str << "}";
    return str;
  }

  template<typename T, typename U>
  ostream & operator<<(ostream & str, const map<T, U> & values)
  {
    str << "[";
    bool first = true;
    for(auto && item : values)
    {
      if(!first)
        str << ", ";
      str << item.first <<  " -> " << item.second;
      first = false;      
    }
    str << "]";
    return str;
  }
  
  template<typename T>
  ostream & operator<<(ostream & str, const vector<T> & values)
  {
    str << "[";
    bool first = true;
    for(auto && item : values)
    {
      if(!first)
        str << ", ";
      str << item;
      first = false;
    }
    str << "]";
    return str;
  }
}

namespace
{
  template<typename T>
  bool close(const T & left, const T & right)
  {
    auto d = (-right) + left;
    return glm::dot(d, d) < 0.01;
  }
  
  template<typename T>
  bool close(const std::optional<T> & left, const std::optional<T> & right)
  {
    if(left && right)
      return close(*left, *right);
    else
      return((left && right) || (!left && !right));
  }
}

////////////////////
// Scene
////////////////////

engine::gltf::Scene::Scene(const Json::Value & sceneDocument)
{
  get(sceneDocument, "name", _name);
  get(sceneDocument, "nodes", _nodes);
}

engine::gltf::Scene::Scene(const std::optional<std::string> & name,
                   const std::vector<size_t> & nodes):
  _name(name),
  _nodes(nodes)
{
}

bool engine::gltf::Scene::operator==(const Scene & other) const
{
  return
    _name == other._name &&
    _nodes == other._nodes;
}

std::ostream & engine::gltf::operator<<(std::ostream & str, const Scene & scene)
{
  str << "<"
      << scene._name << ", "
      << scene._nodes
      << ">";
  return str;
}

////////////////////
// Node
////////////////////

engine::gltf::Node::Node(const Json::Value & nodeDocument)
{
  get(nodeDocument, "camera", _camera);
  get(nodeDocument, "children", _children);
  get(nodeDocument, "skin", _skin);
  get(nodeDocument, "matrix", _matrix);
  get(nodeDocument, "mesh", _mesh);
  get(nodeDocument, "rotation", _rotation);
  get(nodeDocument, "scale", _scale);
  get(nodeDocument, "translation", _translation);
  get(nodeDocument, "weights", _weights);
  get(nodeDocument, "name", _name);
}

engine::gltf::Node::Node(const std::optional<size_t> & camera,
                 const std::vector<size_t> & children,
                 const std::optional<size_t> & skin,
                 const std::optional<glm::mat4> & matrix,
                 const std::optional<size_t> & mesh,
                 const std::optional<glm::quat> & rotation,
                 const std::optional<glm::vec3> & scale,
                 const std::optional<glm::vec3> & translation,
                 const std::vector<size_t> & weights,
                 const std::optional<std::string> & name):
  _camera(camera),
  _children(children),
  _skin(skin),
  _matrix(matrix),
  _mesh(mesh),
  _rotation(rotation),
  _scale(scale),
  _translation(translation),
  _weights(weights),
  _name(name)
{
}

bool engine::gltf::Node::operator==(const Node & other) const
{
  return 
    _camera == other._camera &&
    _children == other._children &&
    _skin == other._skin &&
    _matrix == other._matrix &&
    _mesh == other._mesh &&
    close(_rotation, other._rotation) &&
    close(_scale, other._scale) &&
    close(_translation, other._translation) &&
    _weights == other._weights &&
    _name == other._name;    
}

std::ostream & engine::gltf::operator<<(std::ostream & str, const Node & node)
{
  str << "<"
      << node._camera << ", "
      << node._children << ", "
      << node._skin << ", "
      << node._matrix << ", "
      << node._mesh << ", "
      << node._rotation << ", "
      << node._scale << ", "
      << node._translation << ", "
      << node._weights << ", "
      << node._name
      << ">";
  return str;
}

////////////////////
// Primitive
////////////////////

engine::gltf::Primitive::Primitive(const Json::Value & primitiveDocument)
{
  get(primitiveDocument, "attributes", _attributes);
  get(primitiveDocument, "indices", _indices);
  get(primitiveDocument, "material", _material);
  get(primitiveDocument, "mode", _mode, GLenum(GL_TRIANGLES));
  get(primitiveDocument, "targets", _targets);
}

engine::gltf::Primitive::Primitive(const std::map<std::string, size_t> & attributes,
                           const std::optional<size_t> & indices,
                           const std::optional<size_t> & material,
                           GLenum mode,
                           const std::vector<std::map<std::string, size_t> > & targets):
  _attributes(attributes),
  _indices(indices),
  _material(material),
  _mode(mode),
  _targets(targets)
{
}

bool engine::gltf::Primitive::operator==(const Primitive & other) const
{
  return
    _attributes == other._attributes &&
    _indices == other._indices &&
    _material == other._material &&
    _mode == other._mode &&
    _targets == other._targets;
}

std::ostream & engine::gltf::operator<<(std::ostream & str, const Primitive & primitive)
{
  str << "<"
      << primitive._attributes << ", "
      << primitive._indices << ", "
      << primitive._material << ", "
      << primitive._mode << ", "
      << primitive._targets
      << ">";
  return str;
}

////////////////////
// Mesh
////////////////////

engine::gltf::Mesh::Mesh(const Json::Value & meshDocument)
{
  const Json::Value * primitivesDoc = getNode(meshDocument, "primitives");
  if(primitivesDoc)
  {
    for(auto && primitiveDoc : *primitivesDoc)
    {
      _primitives.push_back(Primitive(primitiveDoc));
    }
  }  
  get(meshDocument, "weights", _weights);
  get(meshDocument, "name", _name);
}

engine::gltf::Mesh::Mesh(const std::vector<Primitive> & primitives,
                 const std::vector<size_t> & weights,
                 const std::optional<std::string> & name):
  _primitives(primitives),
  _weights(weights),
  _name(name)
{
}

bool engine::gltf::Mesh::operator==(const Mesh & other) const
{
  return
    _primitives == other._primitives &&
    _weights == other._weights &&
    _name == other._name;
}

std::ostream & engine::gltf::operator<<(std::ostream & str, const Mesh & mesh)
{
  str << "<"
      << mesh._primitives << ", "
      << mesh._weights << ", "
      << mesh._name
      << ">";
  return str;
}

////////////////////
// Buffer
////////////////////
engine::gltf::Buffer::Buffer(const Json::Value & bufferDocument,
                     const std::filesystem::path & modelPath)
{
  get(bufferDocument, "name", _name);
  
  size_t byteLength;
  std::optional<std::string> uri;
  
  get(bufferDocument, "byteLength", byteLength);
  get(bufferDocument, "uri", uri);
  
  if(!uri)
    _data.resize(byteLength);
  else
  {
    std::string inlineHeader("data:application/octet-stream;base64,");
    if(boost::algorithm::starts_with(*uri, inlineHeader))
    {
      _data = base64_decode(uri->begin() + inlineHeader.size(), uri->end());
    }
    else
    {
      std::ifstream file(modelPath / *uri);
      std::copy(std::istreambuf_iterator<char>(file),
                std::istreambuf_iterator<char>(),
                std::back_inserter(_data));
    }
    
    if(_data.size() != byteLength)
      throw std::runtime_error("Inconsistent buffer size");
  }
}

////////////////////
// BufferView
////////////////////

engine::gltf::BufferView::BufferView(const Json::Value & bufferViewDocument)
{
  get(bufferViewDocument, "buffer", _buffer);
  get(bufferViewDocument, "byteOffset", _byteOffset, size_t(0));
  get(bufferViewDocument, "byteLength", _byteLength);
  get(bufferViewDocument, "byteStride", _byteStride, size_t(0));
  get(bufferViewDocument, "target", _target);
  get(bufferViewDocument, "name", _name);
}

engine::gltf::BufferView::BufferView(size_t buffer,
                             size_t byteOffset,
                             size_t byteLength,
                             size_t byteStride,
                             const std::optional<GLenum> & target,
                             const std::optional<std::string> & name):
  _buffer(buffer),
  _byteOffset(byteOffset),
  _byteLength(byteLength),
  _byteStride(byteStride),
  _target(target),
  _name(name)
{
}

bool engine::gltf::BufferView::operator==(const BufferView & other) const
{
  return
    _buffer == other._buffer &&
    _byteOffset == other._byteOffset &&
    _byteLength == other._byteLength &&
    _byteStride == other._byteStride &&
    _target == other._target &&
    _name == other._name;
}

std::ostream & engine::gltf::operator<<(std::ostream & str, const BufferView & bufferView)
{
  str << "<"
      << bufferView._buffer << ", "
      << bufferView._byteOffset << ", "
      << bufferView._byteLength << ", "
      << bufferView._byteStride << ", "
      << bufferView._target << ", "
      << bufferView._name
      << ">";
  return str;
}

////////////////////
// Sparse
////////////////////
engine::gltf::Sparse::Sparse(const Json::Value & sparseDocument)
{
  get(sparseDocument, "count", _count);
  get(sparseDocument, "indices", _indices);
  get(sparseDocument, "values", _values);
}

engine::gltf::Sparse::Sparse(size_t count,
                     const std::map<std::string, size_t> & indices,
                     const std::map<std::string, size_t> & values):
  _count(count),
  _indices(indices),
  _values(values)
{
}

bool engine::gltf::Sparse::operator==(const Sparse & other) const
{
  return
    _count == other._count && 
    _indices == other._indices && 
    _values == other._values;
}

std::ostream & engine::gltf::operator<<(std::ostream & str, const Sparse & sparse)
{
  str << "<"
      << sparse._count << ", "
      << sparse._indices << ", "
      << sparse._values
      << ">";
  return str;
}

////////////////////
// Accessor
////////////////////
engine::gltf::Accessor::Accessor(const Json::Value & accessorDocument)
{
  get(accessorDocument, "bufferView", _bufferView);
  get(accessorDocument, "byteOffset", _byteOffset, size_t(0));
  get(accessorDocument, "componentType", _componentType);
  get(accessorDocument, "normalized", _normalized, false);
  get(accessorDocument, "count", _count);
  get(accessorDocument, "type", _type);
  get(accessorDocument, "max", _max);
  get(accessorDocument, "min", _min);
  const Json::Value * sparse = getNode(accessorDocument, "sparse");
  if(sparse)
    _sparse = Sparse(sparse);
  get(accessorDocument, "name", _name);
}

engine::gltf::Accessor::Accessor(const std::optional<size_t> & bufferView,
                         size_t byteOffset,
                         GLenum componentType,
                         bool normalized,
                         size_t count,
                         const std::string & type,
                         const std::vector<float> & max,
                         const std::vector<float> & min,
                         const std::optional<Sparse> & sparse,
                         const std::optional<std::string> & name):
  _bufferView(bufferView),
  _byteOffset(byteOffset),
  _componentType(componentType),
  _normalized(normalized),
  _count(count),
  _type(type),
  _max(max),
  _min(min),
  _sparse(sparse),
  _name(name)
{
}

bool engine::gltf::Accessor::operator==(const Accessor & other) const
{
  return
    _bufferView == other._bufferView &&
    _byteOffset == other._byteOffset &&
    _componentType == other._componentType &&
    _normalized == other._normalized &&
    _count == other._count &&
    _type == other._type &&
    _max == other._max &&
    _min == other._min &&
    _sparse == other._sparse &&
    _name == other._name;
}

GLint engine::gltf::Accessor::getComponentCount() const
{
  if(_type == "SCALAR")
    return 1;
  else if(_type == "VEC2")
    return 2;
  else if(_type == "VEC3")
    return 3;
  else if(_type == "VEC4")
    return 4;
  else if(_type == "MAT2")
    return 4;
  else if(_type == "MAT3")
    return 9;
  else if(_type == "MAT4")
    return 16;
  else
    throw std::runtime_error("Unknown component type " + _type);
}

std::ostream & engine::gltf::operator<<(std::ostream & str, const Accessor & accessor)
{
  str << "<"
      << accessor._bufferView << ", "
      << accessor._byteOffset << ", "
      << accessor._componentType << ", "
      << accessor._normalized << ", "
      << accessor._count << ", "
      << accessor._type << ", "
      << accessor._max << ", "
      << accessor._min << ", "
      << accessor._sparse << ", "
      << accessor._name
      << ">";
  return str;
}

////////////////////
// TextureInfo
////////////////////

engine::gltf::TextureInfo::TextureInfo(const Json::Value & materialDocument)
{
  get(materialDocument, "index", _index);
  get(materialDocument, "texCoord", _texCoord, size_t(0));
}

engine::gltf::TextureInfo::TextureInfo(size_t index,
                               size_t texCoord):
  _index(index),
  _texCoord(texCoord)
{
}

bool engine::gltf::TextureInfo::operator==(const TextureInfo & other) const
{
  return
    _index == other._index &&
    _texCoord == other._texCoord;
}

std::ostream & engine::gltf::operator<<(std::ostream & str, const TextureInfo & textureInfo)
{
  str << "<"
      << textureInfo._index << ", "
      << textureInfo._texCoord << ", "
      << ">";
  return str;
}

////////////////////
// NormalTextureInfo
////////////////////

engine::gltf::NormalTextureInfo::NormalTextureInfo(const Json::Value & materialDocument)
{
  get(materialDocument, "index", _index);
  get(materialDocument, "texCoord", _texCoord, size_t(0));
  get(materialDocument, "scale", _scale, 1.0f);
}

engine::gltf::NormalTextureInfo::NormalTextureInfo(size_t index,
                                           size_t texCoord,
                                           float scale):
  _index(index),
  _texCoord(texCoord),
  _scale(scale)
{
}

bool engine::gltf::NormalTextureInfo::operator==(const NormalTextureInfo & other) const
{
  return
    _index == other._index &&
    _texCoord == other._texCoord &&
    close(_scale, other._scale);
}

std::ostream & engine::gltf::operator<<(std::ostream & str, const NormalTextureInfo & textureInfo)
{
  str << "<"
      << textureInfo._index << ", "
      << textureInfo._texCoord << ", "
      << ">";
  return str;
}

////////////////////
// OcclusionTextureInfo
////////////////////

engine::gltf::OcclusionTextureInfo::OcclusionTextureInfo(const Json::Value & materialDocument)
{
  get(materialDocument, "index", _index);
  get(materialDocument, "texCoord", _texCoord, size_t(0));
  get(materialDocument, "strength", _strength, 1.0f);
}

engine::gltf::OcclusionTextureInfo::OcclusionTextureInfo(size_t index,
                                                 size_t texCoord,
                                                 float strength):
  _index(index),
  _texCoord(texCoord),
  _strength(strength)
{
}

bool engine::gltf::OcclusionTextureInfo::operator==(const OcclusionTextureInfo & other) const
{
  return
    _index == other._index &&
    _texCoord == other._texCoord &&
    close(_strength, other._strength);
}

std::ostream & engine::gltf::operator<<(std::ostream & str, const OcclusionTextureInfo & textureInfo)
{
  str << "<"
      << textureInfo._index << ", "
      << textureInfo._texCoord << ", "
      << ">";
  return str;
}

////////////////////
// PbrMetallicRoughness
////////////////////

engine::gltf::PbrMetallicRoughness::PbrMetallicRoughness(const Json::Value & pbrDocument)
{
  get(pbrDocument, "baseColorFactor", _baseColorFactor, glm::vec4(1, 1, 1, 1));
  const Json::Value * baseColorTexture = getNode(pbrDocument, "baseColorTexture");
  if(baseColorTexture)
  {
    _baseColorTexture = TextureInfo(*baseColorTexture);
  }
  get(pbrDocument, "metallicFactor", _metallicFactor, 1.0f);
  get(pbrDocument, "roughnessFactor", _roughnessFactor, 1.0f);
  const Json::Value * metallicRoughnessTexture = getNode(pbrDocument, "metallicRoughnessTexture");
  if(metallicRoughnessTexture)
  {
    _metallicRoughnessTexture = TextureInfo(*metallicRoughnessTexture);
  }
}

engine::gltf::PbrMetallicRoughness::PbrMetallicRoughness(const glm::vec4 & baseColorFactor,
                                                 const std::optional<TextureInfo> & baseColorTexture,
                                                 float metallicFactor,
                                                 float roughnessFactor,
                                                 const std::optional<TextureInfo> & metallicRoughnessTexture):
  _baseColorFactor(baseColorFactor),
  _baseColorTexture(baseColorTexture),
  _metallicFactor(metallicFactor),
  _roughnessFactor(roughnessFactor),
  _metallicRoughnessTexture(metallicRoughnessTexture)
{
}

bool engine::gltf::PbrMetallicRoughness::operator==(const PbrMetallicRoughness & other) const
{
  return
    close(_baseColorFactor, other._baseColorFactor) &&
    _baseColorTexture == other._baseColorTexture &&
    close(_metallicFactor, other._metallicFactor) &&
    close(_roughnessFactor, other._roughnessFactor) &&
    _metallicRoughnessTexture == other._metallicRoughnessTexture;
}

std::ostream & engine::gltf::operator<<(std::ostream & str, const PbrMetallicRoughness & pbr)
{
  str << "<"
      << pbr._baseColorFactor << ", "
      << pbr._baseColorTexture << ", "
      << pbr._metallicFactor << ", "
      << pbr._roughnessFactor << ", "
      << pbr._metallicRoughnessTexture
      << ">";
  return str;
}

////////////////////
// Material
////////////////////

engine::gltf::Material::Material(const Json::Value & materialDocument)
{
  get(materialDocument, "name", _name);
  const Json::Value * pbr = getNode(materialDocument, "pbrMetallicRoughness");
  if(pbr)
  {
    _pbrMetallicRoughness = PbrMetallicRoughness(*pbr);
  }
  const Json::Value * normal = getNode(materialDocument, "normalTexture");
  if(normal)
  {
    _normalTexture = NormalTextureInfo(*normal);
  }
  const Json::Value * occlusion = getNode(materialDocument, "occlusionTexture");
  if(occlusion)
  {
    _occlusionTexture = OcclusionTextureInfo(*occlusion);
  }  
  const Json::Value * emissive = getNode(materialDocument, "emissiveTexture");
  if(emissive)
  {
    _emissiveTexture = TextureInfo(*emissive);
  }
  get(materialDocument, "emissiveFactor", _emissiveFactor, glm::vec3(0, 0, 0));
  get(materialDocument, "alphaMode", _alphaMode, std::string("OPAQUE"));
  get(materialDocument, "alphaCutoff", _alphaCutoff, 0.5f);
  get(materialDocument, "doubleSided", _doubleSided, false);
}

engine::gltf::Material::Material(const std::optional<std::string> & name,
                         const std::optional<PbrMetallicRoughness> & pbrMetallicRoughness,
                         const std::optional<NormalTextureInfo> & normalTexture,
                         const std::optional<OcclusionTextureInfo> & occlusionTexture,
                         const std::optional<TextureInfo> & emissiveTexture,
                         const glm::vec3 & emissiveFactor,
                         const std::string & alphaMode,
                         float alphaCutoff,
                         bool doubleSided):
  _name(name),
  _pbrMetallicRoughness(pbrMetallicRoughness),
  _normalTexture(normalTexture),
  _occlusionTexture(occlusionTexture),
  _emissiveTexture(emissiveTexture),
  _emissiveFactor(emissiveFactor),
  _alphaMode(alphaMode),
  _alphaCutoff(alphaCutoff),
  _doubleSided(doubleSided)
{
}

bool engine::gltf::Material::operator==(const Material & other) const
{
  return
    _name == other._name &&
    _pbrMetallicRoughness == other._pbrMetallicRoughness &&
    _normalTexture == other._normalTexture &&
    _occlusionTexture == other._occlusionTexture &&
    _emissiveTexture == other._emissiveTexture &&
    close(_emissiveFactor, other._emissiveFactor) &&
    _alphaMode == other._alphaMode &&
    close(_alphaCutoff, other._alphaCutoff) &&
    _doubleSided == other._doubleSided;
}

std::ostream & engine::gltf::operator<<(std::ostream & str, const Material & material)
{
  str << "<"
      << material._name << ", "
      << material._pbrMetallicRoughness << ", "
      << material._normalTexture << ", "
      << material._occlusionTexture << ", "
      << material._emissiveTexture << ", "
      << material._emissiveFactor << ", "
      << material._alphaMode  << ", "
      << material._alphaCutoff << ", "
      << material._doubleSided
      << ">";
  return str;
}

////////////////////
// Texture
////////////////////

engine::gltf::Texture::Texture(const Json::Value & textureDocument)
{
  get(textureDocument, "sampler", _sampler);
  get(textureDocument, "source", _source);
  get(textureDocument, "name", _name);
}

engine::gltf::Texture::Texture(const std::optional<size_t> & sampler,
                       const std::optional<size_t> & source,
                       const std::optional<std::string> & name):
  _sampler(sampler),
  _source(source),
  _name(name)
{
}

bool engine::gltf::Texture::operator==(const Texture & other) const
{
  return
    _sampler == other._sampler &&
    _source == other._source &&
    _name == other._name;
}

std::ostream & engine::gltf::operator<<(std::ostream & str, const Texture & texture)
{
  str << "<"
      << texture._sampler << ", "
      << texture._source << ", "
      << texture._name
      << ">";
  return str;
}

////////////////////
// Sampler
////////////////////

engine::gltf::Sampler::Sampler(const Json::Value & samplerDocument)
{
  get(samplerDocument, "magFilter", _magFilter);
  get(samplerDocument, "minFilter", _minFilter);
  get(samplerDocument, "wrapS", _wrapS, GLenum(GL_REPEAT));
  get(samplerDocument, "wrapT", _wrapT, GLenum(GL_REPEAT));
  get(samplerDocument, "name", _name);
}

engine::gltf::Sampler::Sampler(const std::optional<GLenum> & magFilter,
                       const std::optional<GLenum> & minFilter,
                       GLenum wrapS,
                       GLenum wrapT,
                       const std::optional<std::string> & name):
  _magFilter(magFilter),
  _minFilter(minFilter),
  _wrapS(wrapS),
  _wrapT(wrapT),
  _name(name)
{
}

bool engine::gltf::Sampler::operator==(const Sampler & other) const
{
  return
    _magFilter == other._magFilter &&
    _minFilter == other._minFilter &&
    _wrapS == other._wrapS &&
    _wrapT == other._wrapT &&
    _name == other._name;
}

std::ostream & engine::gltf::operator<<(std::ostream & str, const Sampler & sampler)
{
  str << "<"
      << sampler._magFilter << ", "
      << sampler._minFilter << ", "
      << sampler._wrapS << ", "
      << sampler._wrapT << ", "
      << sampler._name
      << ">";
  return str;
}

////////////////////
// Image
////////////////////


engine::gltf::Image::Image(const Json::Value & imageDocument)
{
  get(imageDocument, "uri", _uri);
  get(imageDocument, "mimeType", _mimeType);
  get(imageDocument, "bufferView", _bufferView);
  get(imageDocument, "name", _name);
}

engine::gltf::Image::Image(const std::optional<std::string> & uri,
                   const std::optional<std::string> & mimeType,
                   const std::optional<size_t> & bufferView,
                   const std::optional<std::string> & name):
  _uri(uri),
  _mimeType(mimeType),
  _bufferView(bufferView),
  _name(name)
{
}

bool engine::gltf::Image::operator==(const Image & other) const
{
  return 
    _uri == other._uri &&
    _mimeType == other._mimeType &&
    _bufferView == other._bufferView &&
    _name == other._name;
}

std::ostream & engine::gltf::operator<<(std::ostream & str, const Image & image)
{
  str << "<"
      << image._uri << ", "
      << image._mimeType << ", "
      << image._bufferView << ", "
      << image._name
      << ">";
  return str;
}

////////////////////
// Target
////////////////////

engine::gltf::Target::Target(const Json::Value & targetDocument)
{
  get(targetDocument, "node", _node);
  get(targetDocument, "path", _path);
}

engine::gltf::Target::Target(const std::optional<size_t> & node,
                     const std::string & path):
  _node(node),
  _path(path)
{
}

bool engine::gltf::Target::operator==(const Target & other) const
{
  return
    _node == other._node &&
    _path == other._path;
}

std::ostream & engine::gltf::operator<<(std::ostream & str, const Target & target)
{
  str << "<"
      << target._node << ", "
      << target._path
      << ">";
  return str;
}

////////////////////
// Channel
////////////////////

engine::gltf::Channel::Channel(const Json::Value & channelDocument):
  _target(getNodeOrThrow(channelDocument, "target"))
{
  get(channelDocument, "sampler", _sampler);
}

engine::gltf::Channel::Channel(size_t sampler,
                       const Target & target):
  _sampler(sampler),
  _target(target)
{
}

bool engine::gltf::Channel::operator==(const Channel & other) const
{
  return
    _sampler == other._sampler &&
    _target == other._target;
}

std::ostream & engine::gltf::operator<<(std::ostream & str, const Channel & channel)
{
  str << "<"
      << channel._sampler << ", "
      << channel._target << ", "
      << ">";
  return str;
}

////////////////////
// AnimationSampler
////////////////////

engine::gltf::AnimationSampler::AnimationSampler(const Json::Value & samplerDocument)
{
  get(samplerDocument, "input", _input);
  get(samplerDocument, "interpolation", _interpolation);
  get(samplerDocument, "output", _output);
}

engine::gltf::AnimationSampler::AnimationSampler(size_t input,
                                         const std::string & interpolation,
                                         size_t output):
  _input(input),
  _interpolation(interpolation),
  _output(output)
{
}

bool engine::gltf::AnimationSampler::operator==(const AnimationSampler & other) const
{
  return
    _input == other._input &&
    _interpolation == other._interpolation &&
    _output == other._output;
}

std::ostream & engine::gltf::operator<<(std::ostream & str, const AnimationSampler & sampler)
{
  str << "<"
      << sampler._input << ", "
      << sampler._interpolation << ", "
      << sampler._output
      << ">";
  return str;
}

////////////////////
// Animation
////////////////////

engine::gltf::Animation::Animation(const Json::Value & animationDocument)
{
  const Json::Value * channelsDoc = getNode(animationDocument, "channels");
  if(channelsDoc)
  {
    for(auto && channelDoc : *channelsDoc)
    {
      _channels.push_back(Channel(channelDoc));
    }
  }
  const Json::Value * samplersDoc = getNode(animationDocument, "samplers");
  if(samplersDoc)
  {
    for(auto && samplerDoc : *samplersDoc)
    {
      _samplers.push_back(AnimationSampler(samplerDoc));
    }
  }    
  get(animationDocument, "name", _name);
}

engine::gltf::Animation::Animation(const std::vector<Channel> & channels,
                           const std::vector<AnimationSampler> & samplers,
                           const std::optional<std::string> & name):
  _channels(channels),
  _samplers(samplers),
  _name(name)
{
}

bool engine::gltf::Animation::operator==(const Animation & other) const
{
  return
    _channels == other._channels &&
    _samplers == other._samplers &&
    _name == other._name;
}

std::ostream & engine::gltf::operator<<(std::ostream & str, const Animation & animation)
{
  str << "<"
      << animation._channels << ", "
      << animation._samplers << ", "
      << animation._name
      << ">";
  return str;
}

////////////////////
// Asset
////////////////////

engine::gltf::Asset::Asset(const std::filesystem::path & gltfFile)
{
  if(!std::filesystem::exists(gltfFile))
    throw std::runtime_error("Cannot find file " + std::string(gltfFile));
  
  std::filesystem::path modelPath = gltfFile.parent_path();
  std::ifstream str(gltfFile);
  Json::Value document;
  str >> document;
  
  // Scene
  get(document, "scene", _scene);
  
  // Scenes
  const Json::Value * scenesDoc = getNode(document, "scenes");
  if(scenesDoc)
  {
    for(auto && sceneDoc : *scenesDoc)
    {
      _scenes.push_back(Scene(sceneDoc));
    }
  }

  // Nodes
  const Json::Value * nodesDoc = getNode(document, "nodes");
  if(nodesDoc)
  {
    for(auto && nodeDoc : *nodesDoc)
    {
      _nodes.push_back(Node(nodeDoc));
    }
  }

  // Meshes
  const Json::Value * meshesDoc = getNode(document, "meshes");
  if(meshesDoc)
  {
    for(auto && meshDoc : *meshesDoc)
    {
      _meshes.push_back(Mesh(meshDoc));
    }
  }

  // Buffers
  const Json::Value * buffersDoc = getNode(document, "buffers");
  if(buffersDoc)
  {
    for(auto && bufferDoc : *buffersDoc)
    {
      _buffers.push_back(Buffer(bufferDoc, modelPath));
    }
  }
  
  // BufferViews
  const Json::Value * bufferViewsDoc = getNode(document, "bufferViews");
  if(bufferViewsDoc)
  {
    for(auto && bufferViewDoc : *bufferViewsDoc)
    {
      _bufferViews.push_back(BufferView(bufferViewDoc));
    }
  }

  // Accessors
  const Json::Value * accessorsDoc = getNode(document, "accessors");
  if(accessorsDoc)
  {
    for(auto && accessorDoc : *accessorsDoc)
    {
      _accessors.push_back(Accessor(accessorDoc));
    }
  }
  
  // Materials
  const Json::Value * materialsDoc = getNode(document, "materials");
  if(materialsDoc)
  {
    for(auto && materialDoc : *materialsDoc)
    {
      _materials.push_back(Material(materialDoc));
    }
  }  
  
  // Textures
  const Json::Value * texturesDoc = getNode(document, "textures");
  if(texturesDoc)
  {
    for(auto && textureDoc : *texturesDoc)
    {
      _textures.push_back(Texture(textureDoc));
    }
  }  

  // Samplers
  const Json::Value * samplersDoc = getNode(document, "samplers");
  if(samplersDoc)
  {
    for(auto && samplerDoc : *samplersDoc)
    {
      _samplers.push_back(Sampler(samplerDoc));
    }
  }  

  // Images
  const Json::Value * imagesDoc = getNode(document, "images");
  if(imagesDoc)
  {
    for(auto && imageDoc : *imagesDoc)
    {
      _images.push_back(Image(imageDoc));
    }
  }

  // Animations
  const Json::Value * animationsDoc = getNode(document, "animations");
  if(animationsDoc)
  {
    for(auto && animationDoc : *animationsDoc)
    {
      _animations.push_back(Animation(animationDoc));
    }
  }
}
