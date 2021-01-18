#include "entityfactory.h"

#include <fstream>

#include "engine/core/json_utils.h"
#include "engine/entity/system.h"

engine::EntityFactory::EntityFactory(const std::string & entityFile)
{
  std::filesystem::path file(entityFile);
  if(!std::filesystem::exists(file))
    throw std::runtime_error("Cannot find entity file " + entityFile);
  
  _rootDirectory = file.parent_path();
  
  std::ifstream str(entityFile);
  str >> _doc;
}

void engine::EntityFactory::registerSystem(const std::string & name, System * system)
{
  system->init(_rootDirectory);
  _systems.insert({name, system});
}

engine::EntityId engine::EntityFactory::addEntity(const std::string & entity)
{
  auto entityDoc = getNodeOrThrow(_doc, entity);
  
  EntityId entityId = _entityIdGenerator.generate();
  for(auto && member : entityDoc.getMemberNames())
  {
    auto it = _systems.find(member);
    if(it != _systems.end())
    {
      it->second->add(entityId, engine::getNodeOrThrow(entityDoc, member));
    }
  }
  return entityId;
}
