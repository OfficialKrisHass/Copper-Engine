#pragma once

#include <vector>
#include <bitset>

#include "Object.h"

namespace Copper {

	extern int cCounter;

	template<typename T> int GetCID() {

		static int cID = cCounter++;
		return cID;

	}

	struct ComponentPool {

	public:
		ComponentPool(size_t size) : size(size), data(new char[maxComponents * size]) {}
		~ComponentPool() { delete data; }

		void* Get(uint32_t index) { return data + index * size; }

	private:
		size_t size;
		char* data = nullptr;

	};

	class Registry {

		friend class Scene;
		friend class Object;

	public:
		Object CreateObject(Scene* scene, Vector3 position, Vector3 rotation, Vector3 scale, std::string name) {

			if (!gaps.empty()) {

				int32_t id = gaps.front();

				objects[id].id = id;
				objects[id].scene = scene;
				
				objects[id].tag = AddComponent<Tag>(objects[id]);
				objects[id].tag->name = name;

				objects[id].transform = AddComponent<Transform>(objects[id]);
				objects[id].transform->position = position;
				objects[id].transform->rotation = rotation;
				objects[id].transform->scale = scale;
				objects[id].transform->object = CreateShared<Object>(objects[id]);

				gaps.erase(gaps.begin());

				return objects[id];

			}

			int32_t id = (int32_t) objects.size();

			objects.push_back(Object());

			objects[id].id = id;
			objects[id].scene = scene;
			
			objects[id].tag = AddComponent<Tag>(objects[id]);
			objects[id].tag->name = name;

			objects[id].transform = AddComponent<Transform>(objects[id]);
			objects[id].transform->position = position;
			objects[id].transform->rotation = rotation;
			objects[id].transform->scale = scale;
			objects[id].transform->object = CreateShared<Object>(objects[id]);

			return objects[id];

		}
		Object CreateObjectFromID(int32_t id, Scene* scene, Vector3 position, Vector3 rotation, Vector3 scale, std::string name) {

			if (id > (int32_t) objects.size() - 1) objects.resize(id + 1, Object());

			objects[id].id = id;
			objects[id].scene = scene;

			objects[id].tag = AddComponent<Tag>(objects[id]);
			objects[id].tag->name = name;

			objects[id].transform = AddComponent<Transform>(objects[id]);
			objects[id].transform->position = position;
			objects[id].transform->rotation = rotation;
			objects[id].transform->scale = scale;
			objects[id].transform->object = CreateShared<Object>(objects[id]);

			return objects[id];

		}
		void DestroyObject(Object& obj) {

			gaps.push_back(obj.id);
			objects[obj.id] = Object();

			obj.transform = nullptr;
			obj.tag = nullptr;

			obj.scene = nullptr;
			obj.id = -1;
			obj.componentMask = std::bitset<maxComponents>();

		}

		template<typename T> T* AddComponent(Object& obj) {

			if (!obj) return nullptr;
			if (!objects[obj.id]) return nullptr;

			int cID = GetCID<T>();

			if (pools.size() <= cID) pools.resize(cID + 1, nullptr);
			if (pools[cID] == nullptr) pools[cID] = new ComponentPool(sizeof(T));

			T* component = new (pools[cID]->Get(obj.id)) T();
			objects[obj.id].componentMask.set(cID);
			obj.componentMask.set(cID);

			return component;

		}
		template<typename T> T* GetComponent(int32_t id) {

			if (!objects[id]) return nullptr;

			int cID = GetCID<T>();
			if (!objects[id].componentMask.test(cID)) return nullptr;

			T* component = static_cast<T*>(pools[cID]->Get(id));
			return component;

		}
		template<typename T> bool HasComponent(int32_t id) {

			if (!objects[id]) return false;

			int cID = GetCID<T>();

			return objects[id].componentMask.test(cID);

		}
		template<typename T> void RemoveComponent(Object& obj) {

			if (!obj) return;
			if (!objects[obj.id]) return;

			int cID = GetCID<T>();
			if (!objects[obj.id].componentMask.test(cID)) return;

			objects[obj.id].componentMask.reset(cID);
			obj.componentMask.reset(cID);

		}

		Object GetObjectFromID(int32_t id) const { return objects[id];  }
		int GetNumOfObjects() const { return (int) objects.size(); }

	private:
		std::vector<Object> objects;
		std::vector<uint32_t> gaps;
		std::vector<ComponentPool*> pools;

	};

}