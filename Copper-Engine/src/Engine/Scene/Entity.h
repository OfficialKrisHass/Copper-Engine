#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scene/InternalEntity.h"

namespace Copper {

    class Scene;

    class Entity {

        friend Scene;

    public:
        Entity() = default;
        Entity(class InternalEntity* entity);

        inline uint32_t ID() const { return m_id; }
        bool IsValid() const;

        bool operator==(const Entity& other) const;
        bool operator==(InternalEntity* other) const;
        bool operator!=(const Entity& other) const { return !(*this == other); }
        bool operator!=(InternalEntity* other) const { return !(*this == other); }

        InternalEntity* operator->();

        operator InternalEntity*() const;

    private:
        uint32_t m_id = 4'294'967'295;

    };

}

template<> struct fmt::formatter<Copper::Entity> : fmt::formatter<std::string> {

    auto format(const Copper::Entity& entity, format_context& ctx) const -> decltype(ctx.out()) {

        return fmt::format_to(ctx.out(), "{}", *static_cast<Copper::InternalEntity*>(entity));

    }

};
