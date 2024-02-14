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
				m_endIndex = 0;
				return;

			}

			m_endIndex = scene->GetNumOfEntities() - 1;

			while (m_beginIndex != m_endIndex + 1 && !(scene->GetEntityFromID(m_beginIndex))) { m_beginIndex++; }
			while (m_endIndex >= m_beginIndex && !(scene->GetEntityFromID(m_endIndex))) { m_endIndex--; }

		}

		const struct Iterator begin() const { return Iterator(m_beginIndex, m_scene, m_endIndex); }
		const struct Iterator end() const { return Iterator(m_endIndex, m_scene); }

	private:
		Scene* m_scene = nullptr;
		uint32_t m_beginIndex = 0;
		uint32_t m_endIndex = 0;

		struct Iterator {

		public:
			Iterator(uint32_t index, Scene* scene, uint32_t endIndex = 0) : m_index(index), m_scene(scene), m_endIndex(endIndex) {}

			InternalEntity* operator*() { CUP_FUNCTION(); return m_scene->GetEntityFromID(m_index); }

			bool operator!=(const Iterator& other) { return m_index != other.m_index + 1; }

			Iterator& operator++() {

				CUP_FUNCTION();

				if (m_index == m_endIndex) {

					m_index++;
					return *this;

				}

				do m_index++;
				while (!(m_scene->GetEntityFromID(m_index)));

				return *this;

			}

		private:
			uint32_t m_index;
			Scene* m_scene;
			uint32_t m_endIndex;

		};

	};

}