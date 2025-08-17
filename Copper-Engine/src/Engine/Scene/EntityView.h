#pragma once

#include "Scene.h"

namespace Copper {

    class EntityView {

    private:
        struct Iterator;

    public:
        EntityView(Scene* scene) : m_scene(scene) {

            CUP_FUNCTION();

            if (scene->GetNumOfEntities() == 0) {

                m_beginIndex = 1;
                m_endIndex = 1;
                return;

            }

            m_endIndex = scene->GetNumOfEntities();

            while (scene->GetEntityFromID(m_beginIndex) == nullptr)
                m_beginIndex++;

            while (m_endIndex > m_beginIndex + 1 && scene->GetEntityFromID(m_endIndex - 1) == nullptr)
                m_endIndex--;

        }

        const struct Iterator begin() const { return Iterator(m_beginIndex, m_scene, m_endIndex); }
        const struct Iterator end() const { return Iterator(m_endIndex, m_scene); }

    private:
        Scene* m_scene = nullptr;
        uint32_t m_beginIndex = 0;
        uint32_t m_endIndex = 0;

        struct Iterator {

        public:
            Iterator(uint32_t index, Scene* scene) : m_index(index), m_scene(scene), m_endIndex(0) {}
            Iterator(uint32_t index, Scene* scene, uint32_t endIndex) : m_index(index), m_scene(scene), m_endIndex(endIndex) {}

            InternalEntity* operator*() {

                CUP_FUNCTION();

                CU_ASSERT(m_scene->GetEntityFromID(m_index) != nullptr, "Entity View iterator is at nullptr! Index: {}", m_index);
                return m_scene->GetEntityFromID(m_index);

            }

            bool operator!=(const Iterator& other) { return m_index != other.m_index; }

            Iterator& operator++() {

                CUP_FUNCTION();

                do m_index++;
                while (m_index < m_endIndex && m_scene->GetEntityFromID(m_index) == nullptr);

                return *this;

            }

        private:
            uint32_t m_index;
            uint32_t m_endIndex;
            Scene* m_scene;

        };

    };

}
