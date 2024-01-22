#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scene/Scene.h"

namespace Copper {

	template<typename T> class ComponentView {

	private:
		struct Iterator;

	public:
		ComponentView(Scene* scene) : m_scene(scene) {

			while (m_beginIndex != MAX_ENTITY_COMPONENTS && !ValidEntity(m_beginIndex))
				m_beginIndex++;
			while (m_endIndex > m_beginIndex && !ValidEntity(m_endIndex - 1))
				m_endIndex--;

		}

		const struct Iterator begin() const { return Iterator(m_beginIndex, m_endIndex, m_scene); }
		const struct Iterator end() const { return Iterator(m_endIndex, m_endIndex, m_scene); }

	private:
		Scene* m_scene = nullptr;

		uint32 m_beginIndex = 0;
		uint32 m_endIndex = MAX_ENTITY_COMPONENTS;

		bool ValidEntity(uint32 index) {

			InternalEntity* entity = m_scene->GetEntityFromID(index);
			return entity && entity->GetComponent<T>();

		}

		struct Iterator {

			Iterator(uint32 index, uint32 endIndex, Scene* scene) : m_index(index), m_endIndex(endIndex), m_scene(scene) {}

			T* operator*() { return m_scene->GetEntityFromID(m_index)->GetComponent<T>(); }

			bool operator!=(const Iterator& other) {

				// other.index should be the endIndex, and if it nos, something is terribly wrong :)
				return other.m_index > m_index;

			}

			Iterator& operator++() {

				do m_index++;
				while (m_index != m_endIndex && !ValidEntity(m_index));

				return *this;

			}

		private:
			uint32 m_index = 0;
			uint32 m_endIndex = MAX_ENTITY_COMPONENTS;

			Scene* m_scene = nullptr;

			bool ValidEntity(uint32 index) {

				InternalEntity* entity = m_scene->GetEntityFromID(index);
				return entity && entity->GetComponent<T>();

			}

		};

	};
	template<typename ... Components> class ComponentViewOR {

	private:
		struct Iterator;

	public:
		ComponentViewOR(Scene* scene) : m_scene(scene) {

			// Instead of checking all of existing entities we can narrow the range down
			// Getting the index of the very first entity that has at least one of the
			// Components and then the index + 1 of the last one
			while (m_beginIndex != MAX_ENTITY_COMPONENTS) {

				if (ValidEntity(m_beginIndex)) break;
				else m_beginIndex++;

			}
			while (m_endIndex > m_beginIndex) {

				if (ValidEntity(m_endIndex - 1)) break;
				else m_endIndex--;

			}

		}

		const struct Iterator begin() const { return Iterator(m_beginIndex, m_endIndex, m_scene, m_ids); }
		const struct Iterator end() const { return Iterator(m_endIndex, m_endIndex, m_scene, m_ids); }

	private:
		Scene* m_scene = nullptr;

		uint32 m_beginIndex = 0;
		uint32 m_endIndex = MAX_ENTITY_COMPONENTS;

		uint32 m_ids[sizeof...(Components)] = { Registry::GetCID<Components>()... };

		bool ValidEntity(uint32 index) {

			InternalEntity* entity = m_scene->GetEntityFromID(index);
			if (!entity)
				return false;

			// This is faster then making a component mask and ANDing it with the
			// entity component mask... I hope

			for (int i = 0; i < sizeof...(Components); i++)
				if (entity->HasComponent(m_ids[i])) return true;

			return false;

		}

		struct Iterator {
		
		public:
			Iterator(uint32 index, uint32 endIndex, Scene* scene, const uint32* ids) : m_index(index), m_endIndex(endIndex), m_scene(scene), m_ids(ids) {}

			InternalEntity* operator*() { return m_scene->GetEntityFromID(m_index); }

			bool operator!=(const Iterator& other) { return m_index < other.m_index; }

			Iterator& operator++() {
				
				do {

					m_index++;

					// If we are at the last index, then the endIndex should be equal to the index (since we just made it one bigger)
					// o endIndex - index == 0 (and 0 < 1) in which case we know there is no reason to do any checks, so we can just end

					if (m_endIndex - m_index < 1)
						break;

					InternalEntity* entity = m_scene->GetEntityFromID(m_index);
					if (!entity)
						continue;

					bool cont = true;
					for (uint32 i = 0; i < sizeof...(Components); i++) {

						if (entity->HasComponent(m_ids[i])) {

							cont = false;
							break;

						}

					}

					if (!cont)
						break;

				} while(m_index < m_endIndex);

				return *this;

			}

		private:
			uint32 m_index = 0;
			uint32 m_endIndex = MAX_ENTITY_COMPONENTS;

			Scene* m_scene = nullptr;
			const uint32* m_ids = nullptr;

		};

	};

	// Why the fuck did I think this was a good idea
	// why couldnt I just stick with the tutorial I copied
	/*template<typename ... Components> class ComponentView {

	private:
		struct Iterator;

		ComponentView(Scene* scene, Operation operation = Operation::AND) : scene(scene) {

			CU_ASSERT(sizeof...(Components) != 0, "ComponentView has to have at least 1 template parameter");

			int ids[] = { Registry::GetCID<Components>()... };

			pool = scene->GetComponentPool(ids[0]);
			if (!pool) {

				beginIndex = MAX_ENTITY_COMPONENTS;
				endIndex = MAX_ENTITY_COMPONENTS;
				return;

			}

			while (beginIndex < MAX_ENTITY_COMPONENTS) {

				// Just continue if current index is an invalid entity

				if (InvalidEntity(beginIndex)) {

					beginIndex++;
					continue;

				}

				if (HasAllComponents(beginIndex, operation)) beginIndex++;
				else break;

			}
			while (endIndex > beginIndex) {

				// Just continue if current index is an invalid entity
				
				if (InvalidEntity(endIndex - 1)) {

					endIndex--;
					continue;

				}
				if (HasAllComponents(endIndex - 1, operation)) endIndex--;
				else break;

			}

		}

		const Iterator begin() const { return Iterator(beginIndex, endIndex); }
		const Iterator end() const { return Iterator(endIndex, endIndex); }

	private:
		Scene* scene = nullptr;
		Registry::ComponentPool* pool = nullptr;

		uint32_t beginIndex = 0;
		uint32_t endIndex = MAX_ENTITY_COMPONENTS;

		struct Iterator {

			Iterator(uint32_t index, uint32_t endIndex) : index(index), endIndex(endIndex) {}

			InternalEntity* operator*() {

				return scene->registry.GetEntityFromID(index);

			}
			bool operator!=(const Iterator& other) {

				return index != other.index;

			}
			Iterator& operator++() {

				do {

					index++;

				} while(index < endIndex && (!pool->Valid(index) || !(*static_cast<Component*>(pool->Get(index))->GetEntity())))
				return *this;

			}

		private:
			uint32_t index = 0;
			uint32_t endIndex = MAX_ENTITY_COMPONENTS;

		};

		bool ValidEntity(uint32_t index) { return *scene->GetEntityFromID(index); }
		bool HasAllComponents(uint32_t& index, Operation operations) {

			InternalEntity* entity = scene->GetEntityFromID(index);
			bool continueLoop = false;
			for (uint32_t i = 0; i < sizeof...(Components); i++) {

				// I wrote this on the 2th of October 2023 by being blessed by the god, I do not know if it will or how it works
				// but I pray to the god with all my life that it does work and I never ever have to look at this code again, please

				bool hasComponent = scene->registry.HasComponent(ids[i], entity->ID());
				if (operation == Operation::AND && hasComponent) continue;
				else if (operation == Operation::OR) {

					if (hasComponent) break;
					else if (i != sizeof...(Components) - 1) continue; // Since this runs only if hasComponent = false, we do not have to check if it is false

				}
				
				continueLoop = true;
				break;

			}
			if (!continueLoop) return false;
			return true;

		}

	};*/

}