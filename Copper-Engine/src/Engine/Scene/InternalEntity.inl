#pragma once

#include "Scene.h"

namespace Copper {

    template<typename T> T* InternalEntity::AddComponent() { CUP_FUNCTION(); return m_scene->m_registry.template AddComponent<T>(m_id); }
    template<typename T> T* InternalEntity::GetComponent() { CUP_FUNCTION(); return m_scene->m_registry.template GetComponent<T>(m_id); }
    template<typename T> bool InternalEntity::HasComponent() { CUP_FUNCTION(); return m_scene->m_registry.template HasComponent<T>(m_id); }
    template<typename T> void InternalEntity::RemoveComponent() { m_scene->m_registry.template RemoveComponent<T>(m_id); }

    inline void* InternalEntity::GetComponent(int componentID) { CUP_FUNCTION(); return m_scene->m_registry.GetComponent(componentID, m_id); }
	inline bool InternalEntity::HasComponent(int componentID) { CUP_FUNCTION(); return m_scene->m_registry.HasComponent(componentID, m_id); }
	inline void InternalEntity::RemoveComponent(int componentID) { CUP_FUNCTION(); m_scene->m_registry.RemoveComponent(componentID, m_id); }

}