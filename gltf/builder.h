#ifndef __GLTF_BUILDER_H__
#define __GLTF_BUILDER_H__

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <filesystem>
#include <glm/glm.hpp>

namespace engine { class Clock; }
namespace engine::adh { class Node; }
namespace engine::adh { class Transform; }
namespace engine::adh { class Primitive; }
namespace engine::adh { class Shader; }
namespace engine::adh { class Texture; }
namespace engine::adh { class Animation; }
namespace engine::adh { template<typename T> class Interpolator; }

namespace engine::gltf
{
  class Asset;
  class Accessor;
  class AnimationSampler;

  class Builder
  {
  public:
    Builder(Clock * clock,
            const std::filesystem::path & shaderPath,
            const std::filesystem::path & gltfFile);

    std::unique_ptr<adh::Node> build(std::vector<std::unique_ptr<adh::Animation> > & animations) const;

  private:
    std::shared_ptr<adh::Node> buildNode(size_t nodeIndex, std::map<size_t, std::shared_ptr<adh::Transform> > & animationNodes) const;
    std::unique_ptr<adh::Node> buildMesh(size_t meshIndex) const;
    std::unique_ptr<adh::Texture> buildTexture(const std::string & name,
                                               size_t textureIndex) const;
    template<typename T>
    std::unique_ptr<adh::Interpolator<T> > buildInterpolator(const gltf::AnimationSampler & sampler) const;

    void setIndicesBuffer(const std::shared_ptr<adh::Primitive> & primitiveNode,
                          const gltf::Accessor & accessor) const;
    size_t setDataBuffer(const std::shared_ptr<adh::Primitive> & primitiveNode,
                         const gltf::Accessor & accessor) const;

    std::shared_ptr<adh::Shader> getShader(const std::vector<std::string> & defines) const;

    Clock * _clock;
    std::filesystem::path _shaderPath;
    std::shared_ptr<Asset> _asset;
    std::filesystem::path _modelPath;
  };
}

#endif
