#include <boost/test/unit_test.hpp>

#include <iostream>
#include "gltf/asset.h"


BOOST_AUTO_TEST_SUITE(Gltf)

BOOST_AUTO_TEST_CASE(testLoadSimpleAsset)
{
  engine::gltf::Asset asset("plain.gltf");

  // Scene
  {
    BOOST_TEST(0 == asset._scene);
  }
  
  // Scenes
  {
    std::vector<engine::gltf::Scene> expected{
                                              {
                                               engine::gltf::Scene("Scene", {0, 1, 2})
                                              }};
    
    BOOST_TEST(expected == asset._scenes, boost::test_tools::per_element());
  }
  
  // Nodes
  {
    std::vector<engine::gltf::Node> expected{
                                             {
                                              engine::gltf::Node({}, {}, {}, {}, {},
                                                                 glm::quat(0.570, 0.169, 0.755, -0.272),
                                                                 {},
                                                                 glm::vec3(4.076, 5.903, -1.005),
                                                                 {},
                                                                 "Light"),
                                              engine::gltf::Node({}, {}, {}, {}, {},
                                                                 glm::quat(0.780, 0.483, 0.336, -0.208),
                                                                 {},
                                                                 glm::vec3(7.358, 4.95, 6.925),
                                                                 {},
                                                                 "Camera"),
                                              engine::gltf::Node({}, {}, {}, {}, {0},
                                                                 {},
                                                                 {},
                                                                 {},
                                                                 {},
                                                                 "Torus")
                                             }};
    
    BOOST_TEST(expected == asset._nodes, boost::test_tools::per_element());
  }

  // Meshes
  {
    std::vector<engine::gltf::Mesh> expected{
                                             {
                                              engine::gltf::Mesh({{engine::gltf::Primitive({{"NORMAL", 1}, {"POSITION", 0}, {"TEXCOORD_0", 2}}, {3}, {0}, GL_TRIANGLES, {})}},
                                                                 {},
                                                                 "Torus")
                                             }};
    
    BOOST_TEST(expected == asset._meshes, boost::test_tools::per_element());
  }

  // Buffers
  {
    BOOST_TEST(1 == asset._buffers.size());
  }
  
  // Buffer views
  {
    std::vector<engine::gltf::BufferView> expected{
                                                   {
                                                    engine::gltf::BufferView(0, 0, 7644, 0, {}, {}),
                                                    engine::gltf::BufferView(0, 7644, 7644, 0, {}, {}),
                                                    engine::gltf::BufferView(0, 15288, 5096, 0, {}, {}),
                                                    engine::gltf::BufferView(0, 20384, 6912, 0, {}, {})
                                                   }};
    
    BOOST_TEST(expected == asset._bufferViews, boost::test_tools::per_element());
  }

  // Accessors
  {
    std::vector<engine::gltf::Accessor> expected{
                                                 engine::gltf::Accessor(0, 0, GL_FLOAT, false, 637, "VEC3", {1.25, 0.25, 1.25}, {-1.25, -0.25, -1.25}, {}, {}),
                                                 engine::gltf::Accessor(1, 0, GL_FLOAT, false, 637, "VEC3", {}, {}, {}, {}),
                                                 engine::gltf::Accessor(2, 0, GL_FLOAT, false, 637, "VEC2", {}, {}, {}, {}),
                                                 engine::gltf::Accessor(3, 0, GL_UNSIGNED_SHORT, false, 3456, "SCALAR", {}, {}, {}, {})
    };
    
    BOOST_TEST(expected == asset._accessors, boost::test_tools::per_element());
  }

  // Materials
  {
    std::vector<engine::gltf::Material> expected{
                                                 engine::gltf::Material("Material",
                                                                        engine::gltf::PbrMetallicRoughness(glm::vec4(0.8, 0, 0.04, 1), {}, 0, 0.1, {}),
                                                                        {},
                                                                        {},
                                                                        {},
                                                                        glm::vec3(0, 0, 0),
                                                                        "OPAQUE",
                                                                        0.5,
                                                                        0)
    };
    
    BOOST_TEST(expected == asset._materials, boost::test_tools::per_element());
  }
}

BOOST_AUTO_TEST_CASE(testLoadAnimation)
{
  engine::gltf::Asset asset("translate.gltf");

  // Animations
  {
    std::vector<engine::gltf::Animation> expected{
                                                  engine::gltf::Animation({engine::gltf::Channel(0, engine::gltf::Target(3, "translation"))},
                                                                          {engine::gltf::AnimationSampler(8, "CUBICSPLINE", 9)},
                                                                          "Cylinder.001Action")
    };
    
    BOOST_TEST(expected == asset._animations, boost::test_tools::per_element());
  }
}

BOOST_AUTO_TEST_SUITE_END()
