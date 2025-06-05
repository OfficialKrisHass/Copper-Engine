#pragma once

#include "Engine/Core/Core.h"

#include <bitset>

#define INVALID_ENTITY_ID 4'294'967'295
#define MAX_ENTITY_COMPONENTS 32

namespace Copper {

    class Transform;
    class Scene;

    class InternalEntity {

        friend class Registry;
        friend class Scene;
        friend class Entity;

    public:
        InternalEntity() = default;

        std::string name = "";

        template<typename T> T* AddComponent();
        template<typename T> T* GetComponent();
        template<typename T> bool HasComponent();
        template<typename T> void RemoveComponent();

        void* GetComponent(int componentID);
        bool HasComponent(int componentID);
        void RemoveComponent(int componentID);

        Transform* GetTransform() const { return m_transform; }
        uint32_t GetID() const { return m_id; }

        bool operator==(const InternalEntity& other) const { return m_id == other.m_id; }

    private:
        uint32_t m_id = INVALID_ENTITY_ID;
        Transform* m_transform = nullptr;

        std::bitset<MAX_ENTITY_COMPONENTS> m_cMask;

        void Invalidate() {

            m_id = INVALID_ENTITY_ID;
            name = "";
            m_transform = nullptr;
            m_cMask.reset();

        }

    };

}

template<> struct fmt::formatter<Copper::InternalEntity> : fmt::formatter<std::string> {

    auto format(const Copper::InternalEntity& entity, format_context& ctx) const -> decltype(ctx.out()) {

        return fmt::format_to(ctx.out(), "{} ({})", entity.name, entity.GetID());

    }

};
