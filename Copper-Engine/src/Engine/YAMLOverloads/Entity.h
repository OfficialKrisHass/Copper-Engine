#pragma once

#include "Engine/Core/Engine.h"

#include "Engine/Scene/InternalEntity.h"

#include <yaml-cpp/yaml.h>

namespace Copper {

    inline YAML::Emitter& operator<<(YAML::Emitter& out, const InternalEntity* entity) {

        out << (entity ? entity->ID() : invalidID);
		return out;

    }

}

namespace YAML {

    using namespace Copper;

    template<> struct convert<Copper::InternalEntity*> {

        static Node encode(const Copper::InternalEntity*& entity) {

            Node node;
            node.push_back((entity ? entity->ID() : invalidID));

            return node;

        }
        static bool decode(const Node& node, Copper::InternalEntity*& entity) {

            uint32_t id = node.as<uint32_t>();

            if(id == invalidID) entity = nullptr;
            else if(id < GetNumOfEntities()) entity = GetEntityFromID(id);
            else entity = CreateEntityFromID(id);

            return true;

        }

    };

}

