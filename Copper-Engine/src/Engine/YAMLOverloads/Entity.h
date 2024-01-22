#pragma once

#include "Engine/Core/Engine.h"

#include "Engine/Scene/InternalEntity.h"

#include <yaml-cpp/yaml.h>

namespace Copper {

    inline YAML::Emitter& operator<<(YAML::Emitter& out, const InternalEntity* entity) {

        out << (entity ? entity->ID() : INVALID_ENTITY_ID);
		return out;

    }

}

namespace YAML {

    using namespace Copper;

    template<> struct convert<Copper::InternalEntity*> {

        static Node encode(const Copper::InternalEntity*& entity) {

            Node node;
            node.push_back((entity ? entity->ID() : INVALID_ENTITY_ID));

            return node;

        }
        static bool decode(const Node& node, Copper::InternalEntity*& entity) {

            uint32 id = node.as<uint32>();

            if(id == INVALID_ENTITY_ID) entity = nullptr;
            else if(id < GetNumOfEntities()) entity = GetEntityFromID(id);
            else entity = CreateEntityFromID(id);

            return true;

        }

    };

}

