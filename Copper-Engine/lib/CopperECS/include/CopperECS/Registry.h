#pragma once

#include <vector>
#include <bitset>
#include <unordered_map>

#include "Object.h"
#include "ComponentList.h"

#include "Engine/Events/RegistryEvent.h"

namespace Copper {

	extern int cCounter;

	extern ObjectEvent oce;
	extern ObjectEvent ode;
	extern ComponentEvent cae;
	extern ComponentEvent cre;

	void AddObjectCreatedEventFunc(std::function<bool(const Event&)> func);
	void AddObjectDestroyedEventFunc(std::function<bool(const Event&)> func);
	void AddComponentAddedEventFunc(std::function<bool(const Event&)> func);
	void AddComponentRemovedEventFunc(std::function<bool(const Event&)> func);

	template<typename T> int GetCID() {

		static int cID = cCounter++;
		return cID;

	}

	struct ComponentPool {

	public:
		ComponentPool() = default;
		ComponentPool(size_t size) : size(size), data(new char[maxComponents * size]), count(0) {}
		~ComponentPool() { delete data; }

		void* Add(int32_t objID) {

			componentIndexes[objID].push_back(count);
			count++;

			return data + (count - 1) * size;

		}
		void* Get(uint32_t index) { return data + index * size; }
		void* Get(int32_t objID, uint32_t index = 0) { return data + componentIndexes[objID][index] * size; }

		uint32_t GetCount() const { return count; }
		uint32_t GetCount(int32_t objID) { return (uint32_t) componentIndexes[objID].size(); }

	private:
		char* data = nullptr;
		std::unordered_map<int32_t, std::vector<uint32_t>> componentIndexes;

		size_t size;
		uint32_t count;

	};

	class Registry {

		friend class Scene;
		friend class Object;

	public:
		Object CreateObject(Scene* scene, Vector3 position, Vector3 rotation, Vector3 scale, const std::string& name) {

			if (!gaps.empty()) {

				int32_t id = gaps.front();

				objects[id].id = id;
				objects[id].scene = scene;
				
				callCAE = false;
				objects[id].tag = AddComponent<Tag>(objects[id]);
				objects[id].tag->name = name;

				objects[id].transform = AddComponent<Transform>(objects[id]);
				objects[id].transform->position = position;
				objects[id].transform->rotation = rotation;
				objects[id].transform->scale = scale;
				objects[id].transform->object = CreateShared<Object>(objects[id]);
				callCAE = true;

				gaps.erase(gaps.begin());

				oce.obj = &objects[id];
				oce.Call();
				oce.Clear();

				return objects[id];

			}

			int32_t id = (int32_t) objects.size();

			objects.push_back(Object());

			objects[id].id = id;
			objects[id].scene = scene;
			
			callCAE = false;
			objects[id].tag = AddComponent<Tag>(objects[id]);
			objects[id].tag->name = name;

			objects[id].transform = AddComponent<Transform>(objects[id]);
			objects[id].transform->position = position;
			objects[id].transform->rotation = rotation;
			objects[id].transform->scale = scale;
			objects[id].transform->object = CreateShared<Object>(objects[id]);
			callCAE = true;

			oce.obj = &objects[id];
			oce.Call();
			oce.Clear();

			return objects[id];

		}
		Object CreateObjectFromID(int32_t id, Scene* scene, Vector3 position, Vector3 rotation, Vector3 scale, const std::string& name) {

			if (id > (int32_t) objects.size() - 1) objects.resize(id + 1, Object());
			else {

				objects[id].tag->name = name;

				objects[id].transform->position = position;
				objects[id].transform->rotation = rotation;
				objects[id].transform->scale = scale;
				objects[id].transform->object = CreateShared<Object>(objects[id]);

				oce.obj = &objects[id];
				oce.Call();
				oce.Clear();

				return objects[id];

			}

			objects[id].id = id;
			objects[id].scene = scene;

			callCAE = false;
			objects[id].tag = AddComponent<Tag>(objects[id]);
			objects[id].tag->name = name;

			objects[id].transform = AddComponent<Transform>(objects[id]);
			objects[id].transform->position = position;
			objects[id].transform->rotation = rotation;
			objects[id].transform->scale = scale;
			objects[id].transform->object = CreateShared<Object>(objects[id]);
			callCAE = true;

			oce.obj = &objects[id];
			oce.Call();
			oce.Clear();

			return objects[id];

		}
		void DestroyObject(Object& obj) {

			ode.obj = &objects[obj.id];
			ode.Call();
			ode.Clear();

			gaps.push_back(obj.id);
			objects[obj.id] = Object();

			obj.transform = nullptr;
			obj.tag = nullptr;

			obj.scene = nullptr;
			obj.id = -1;
			obj.componentMask.clear();

		}

		template<typename T> T* AddComponent(Object& obj) {

			if (!obj) return nullptr;
			if (!objects[obj.id]) return nullptr;

			int cID = GetCID<T>();
			
			if (obj.componentMask.size() <= cID) { obj.componentMask.resize(cID + 1, 0); objects[obj.id].componentMask.resize(cID + 1, 0); }
			if (obj.componentMask[cID] == 1 && !T::multipleOnOneObject) {
				LogError("Can't Add Component to Object {} because there already is one", obj.tag->name); return nullptr; }

			if (pools.size() <= cID) pools.resize(cID + 1, nullptr);
			if (!pools[cID]) pools[cID] = new ComponentPool(sizeof(T));

			T* component = new (pools[cID]->Add(obj.id)) T();
			component->valid = true;
			component->index = pools[cID]->GetCount(obj.id) - 1;

			objects[obj.id].componentMask[cID]++;
			if (obj.componentMask[cID] != objects[obj.id].componentMask[cID]) obj.componentMask[cID] = objects[obj.id].componentMask[cID];

			if (!callCAE) return component;

			cae.component = (Component*) component;
			cae.Call();
			cae.Clear();

			return component;

		}
		template<typename T> T* GetComponent(int32_t id, uint32_t index = 0) {

			if (!objects[id]) return nullptr;

			int cID = GetCID<T>();

			if (objects[id].componentMask.size() <= cID || objects[id].componentMask[cID] <= 0) return nullptr;

			T* component = static_cast<T*>(pools[cID]->Get(id, index));
			return component;

		}
		template<typename T> bool HasComponent(int32_t id) {

			if (!objects[id]) return false;

			int cID = GetCID<T>();

			if (objects[id].componentMask.size() <= cID) return false;
			return objects[id].componentMask[cID] >= 1;

		}
		template<typename T> void RemoveComponent(Object& obj, uint32_t index = 0) {

			if (!obj) return;
			if (!objects[obj.id]) return;

			int cID = GetCID<T>();
			
			if (objects[obj.id].componentMask.size() <= cID || objects[obj.id].componentMask[cID] == 0) return;

			T* component = (T*) pools[cID]->Get(obj.id, index);
			if (!component->valid) return;

			objects[obj.id].componentMask[cID]--;
			obj.componentMask[cID]--;

			component->valid = false;

			cre.component = (Component*) component;
			cre.Call();
			cre.Clear();

		}

		template<typename T> ComponentList<T> GetComponents(Scene* scene, int32_t id) {

			if (!objects[id]) return ComponentList<T>(); //Empty constructor = invalid ComponentList

			int cID = GetCID<T>();
			if (objects[id].componentMask.size() <= cID || objects[id].componentMask[cID] <= 0) return ComponentList<T>(); //Empty constructor = invalid ComponentList

			return ComponentList<T>(scene, id, cID);

		}

		int GetNumOfObjects() const { return (int) objects.size(); }

		Object& GetObjectFromID(int32_t id) {
			
			return objects[id];
		
		}

		ComponentPool* GetComponentPool(uint32_t cID) { return pools[cID]; }

	private:
		std::vector<Object> objects;
		std::vector<uint32_t> gaps;
		std::vector<ComponentPool*> pools;

		bool callCAE;

	};

}