#ifndef __ENTITY_ENTITYID_H__
#define __ENTITY_ENTITYID_H__

#include "entity/strongid.h"

namespace engine
{
  class EntityIdDiscriminant;
  typedef StrongId<EntityIdDiscriminant, size_t> EntityId;
  typedef StrongIdGenerator<EntityId> EntityIdGenerator;
}

#endif
