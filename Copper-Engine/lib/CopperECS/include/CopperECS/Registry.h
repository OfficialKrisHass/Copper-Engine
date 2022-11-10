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

	public:
		Object CreateObject(Scene* scene, Vector3 position, Vector3 rotation, Vector3 scale, std::string name) {

			if (!gaps.empty()) {

				Object obj;
				obj.id = gaps.front();
				obj.scene = scene;
				obj.name = name;

				obj.transform = AddComponent<Transform>(obj);
				obj.transform->position = position;
				obj.transform->rotation = rotation;
				obj.transform->scale = scale;

				objects[obj.id] = obj;
				gaps.erase(gaps.begin());

				return obj;

			}

			Object obj;
			obj.id = (int32_t) objects.size();
			obj.scene = scene;
			obj.name = name;

			objects.push_back(obj);

			obj.transform = AddComponent<Transform>(obj);
			obj.transform->position = position;
			obj.transform->rotation = rotation;
			obj.transform->scale = scale;
			objects[obj.id].transform = obj.transform;
			objects[obj.id].transform->object = CreateShared<Object>(objects[obj.id]);

			return obj;

		}
		Object CreateObjectFromID(int32_t id, Scene* scene, Vector3 position, Vector3 rotation, Vector3 scale, std::string name) {

			if (id > (int32_t) objects.size() - 1) {

				objects.resize(id + 1, Object());

				Object obj;
				obj.id = id;
				obj.scene = scene;
				obj.name = name;

				objects[id] = obj;

				obj.transform = AddComponent<Transform>(obj);
				obj.transform->position = position;
				obj.transform->rotation = rotation;
				obj.transform->scale = scale;
				objects[obj.id].transform = obj.transform;
				objects[obj.id].transform->object = CreateShared<Object>(objects[obj.id]);

				return obj;

			}

			Object obj;
			obj.id = id;
			obj.scene = scene;
			obj.name = name;

			objects[id] = obj;

			obj.transform = AddComponent<Transform>(obj);
			obj.transform->position = position;
			obj.transform->rotation = rotation;
			obj.transform->scale = scale;
			objects[obj.id].transform = obj.transform;
			objects[obj.id].transform->object = CreateShared<Object>(objects[obj.id]);

			return obj;

		}
		void DestroyObject(Object& obj) {

			gaps.push_back(obj.id);

			objects[obj.id] = Object();

			obj.name = "";
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
		int GetNumOfObjects() const { return objects.size(); }

		void SetObjectName(Object obj, std::string name) { obj.name = name, objects[obj.id].name = name; }

	private:
		std::vector<Object> objects;
		std::vector<uint32_t> gaps;
		std::vector<ComponentPool*> pools;

	};

}