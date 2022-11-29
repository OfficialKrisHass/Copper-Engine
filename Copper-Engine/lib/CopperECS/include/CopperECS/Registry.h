#pragma once

#include <vector>
#include <bitset>

#include "Object.h"

#include "Engine/Components/ScriptComponent.h"

#include "Engine/Scripting/ScriptEngine.h"

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

		ScriptComponent* AddScriptComponent(Object& obj, std::string nameSpace, std::string scriptName) {

			if (scripts.find(obj.id) == scripts.end()) scripts[obj.id] = std::vector<ScriptComponent>();

			int index = (int) scripts[obj.id].size();

			scripts[obj.id].push_back(ScriptComponent(nameSpace, scriptName, obj));

			if (!scripts[obj.id][index].valid) return nullptr;

			return &scripts[obj.id][index];

		}
		ScriptComponent* GetScriptComponent(int32_t id, std::string nameSpace, std::string scriptName) {

			for (int i = 0; i < scripts[id].size(); i++) {

				if (scripts[id][i].nameSpace == nameSpace && scripts[id][i].scriptName == scriptName) return &scripts[id][i];

			}

			return nullptr;

		}
		bool HasScriptComponent(int32_t id, std::string nameSpace, std::string scriptName) {

			for (ScriptComponent script : scripts[id]) {

				if (script.nameSpace == nameSpace && script.scriptName == scriptName) return true;

			}

			return false;

		}

		Object GetObjectFromID(int32_t id) const { return objects[id];  }
		int GetNumOfObjects() const { return (int) objects.size(); }

	private:
		std::vector<Object> objects;
		std::vector<uint32_t> gaps;
		std::vector<ComponentPool*> pools;

		std::unordered_map<int32_t, std::vector<ScriptComponent>> scripts;

	};

}