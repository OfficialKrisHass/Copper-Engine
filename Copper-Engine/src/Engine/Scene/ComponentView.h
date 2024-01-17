#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scene/Scene.h"

namespace Copper {

	/*
	Test Examples

	Component 1 - RigidBody
	Component 2 - MeshRenderer
	Component 3 - Box Collider
	Component 4 - Transform - Every Entity has this one

	Case0: ComponentView<> 
		Crashes, use EntityView instead
	Case1: ComponentView<RigidBody>
		Should give a pointer to a valid RigidBody component in a loop
		RigidBody Pool cases:
			Case-1: 0000000000000000000000000000 - beginIndex = 32, endIndex = 32
			Case0:  0000000000000000000000000001 - beginIndex = 31, endIndex = 32
			Case1:  1000000000000000000000000000 - beginIndex =  0, endIndex =  1
			Case2:  0100000000000000000000000000 - beginIndex =  1, endIndex =  2
			Case3:  0010000000000000000000000000 - beginIndex =  2, endIndex =  3
			Case4:  1000000000000100000000000000 - beginIndex =  0, endIndex = 14
			Case5:  1000000000000000000000000001 - beginIndex =  0, endIndex = 32
			Case6:  1000000000000000000000000010 - beginIndex =  0, endIndex = 31
			Case7:  1011011101001100111001010001 - beginIndex =  0, endIndex = 32
			Case8:  0101101110100110011100101010 - beginIndex =  1, endIndex = 31
			Case9:  0010110111010011001110010100 - beginIndex =  2, endIndex = 30
			Case10: 0101101110100110011100101011 - beginIndex =  1, endIndex = 32


	Case2: 
	*/

	template<typename T> class ComponentView {

	private:
		struct Iterator;

	public:
		ComponentView(Scene* scene) : scene(scene) {

			while (beginIndex != MAX_ENTITY_COMPONENTS && !ValidEntity(beginIndex)) {

				beginIndex++;

			}
			while (endIndex > beginIndex && !ValidEntity(endIndex - 1)) {

				endIndex--;

			}

		}

		const Iterator begin() const { return Iterator(beginIndex, endIndex, scene); }
		const Iterator end() const { return Iterator(endIndex, endIndex, scene); }

	private:
		Scene* scene;

		uint32_t beginIndex = 0;
		uint32_t endIndex = MAX_ENTITY_COMPONENTS;

		bool ValidEntity(uint32_t index) {

			InternalEntity* entity = scene->GetEntityFromID(index);
			return entity && entity->GetComponent<T>();

		}

		struct Iterator {

			Iterator(uint32_t index, uint32_t endIndex, Scene* scene) : index(index), endIndex(endIndex), scene(scene) {}

			T* operator*() {

				return scene->GetEntityFromID(index)->GetComponent<T>();

			}
			bool operator!=(const Iterator& other) {

				// other.index == endIndex - PS: If this is not the case, then something is terribly wrong :)
				return other.index > index;

			}
			Iterator& operator++() {

				do {

					index++;

				} while (index != endIndex && !ValidEntity(index));

				return *this;

			}

		private:
			uint32_t index;
			uint32_t endIndex;

			Scene* scene;

			bool ValidEntity(uint32_t index) {

				InternalEntity* entity = scene->GetEntityFromID(index);
				return entity && entity->GetComponent<T>();

			}

		};

	};

	template<typename ... Components> class ComponentViewOR {

	private:
		struct Iterator;

	public:
		ComponentViewOR(Scene* scene) : scene(scene) {

			// Instead of checking all of existing entities we can narrow the range down
			// Getting the index of the very first entity that has at least one of the
			// Components and then the index + 1 of the last one
			while (beginIndex != MAX_ENTITY_COMPONENTS) {

				if (ValidEntity(beginIndex)) break;
				else beginIndex++;

			}
			while (endIndex > beginIndex) {

				if (ValidEntity(endIndex - 1)) break;
				else endIndex--;

			}

		}

		const Iterator begin() const { return Iterator(beginIndex, endIndex, scene, ids); }
		const Iterator end() const { return Iterator(endIndex, endIndex, scene, ids); }

	private:
		Scene* scene = nullptr;

		uint32_t beginIndex = 0;
		uint32_t endIndex = MAX_ENTITY_COMPONENTS;

		int ids[sizeof...(Components)] = { Registry::GetCID<Components>()... };

		bool ValidEntity(uint32_t index) {

			InternalEntity* entity = scene->GetEntityFromID(index);
			if (!entity)
				return false;

			// This is faster then making a component mask and ANDing it with the
			// entity component mask... I hope

			for (int i = 0; i < sizeof...(Components); i++)
				if (entity->HasComponent(ids[i])) return true;

			return false;

		}



		struct Iterator {
		
		public:
			Iterator(uint32_t index, uint32_t endIndex, Scene* scene, const int* ids) : index(index), endIndex(endIndex), scene(scene), ids(ids) {}

			InternalEntity* operator*() {

				return scene->GetEntityFromID(index);

			}
			bool operator!=(const Iterator& other) {
				
				return index < other.index;

			}
			Iterator& operator++() {
				
				do {

					index++;

					// If we are at the last index, then the endIndex should be equal to the index (since we just made it one bigger)
					// o endIndex - index == 0 (and 0 < 1) in which case we know there is no reason to do any checks, so we can just end

					if (endIndex - index < 1)
						break;

					InternalEntity* entity = scene->GetEntityFromID(index);
					if (!entity)
						continue;

					bool cont = true;
					for (int i = 0; i < sizeof...(Components); i++) {

						if (entity->HasComponent(ids[i])) {

							cont = false;
							break;

						}

					}

					if (!cont)
						break;

				} while(index < endIndex);

				return *this;

			}

		private:
			uint32_t index = 0;
			uint32_t endIndex = MAX_ENTITY_COMPONENTS;

			Scene* scene = nullptr;
			const int* ids = nullptr;

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