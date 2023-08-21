#pragma once

#include "Engine/Core/Core.h"

#define BindEventFunc(func) std::bind(&func, this, std::placeholders::_1)

namespace Copper {

	class Event {

	public:
		Event() = default;

		void Call() {

			(*this)();

		}

		void Clear() { success = true; }

		inline Event& operator+=(std::function<bool(const Event&)> func) { callbacks.push_back(func); return *this; }
		inline bool operator()() {

			for (int i = 0; i < callbacks.size() && success; i++) {

				success = callbacks[i](*this);

			}

			bool tmp = success;
			success = true;

			return tmp;

		}

	private:
		std::vector<std::function<bool(const Event&)>> callbacks;
		bool success = true;

	};

	class SimpleEvent {

	public:
		inline SimpleEvent& operator+=(std::function<void()> func) { callbacks.push_back(func); return *this; }
		inline void operator()() { for (size_t i = 0; i < callbacks.size(); i++) { callbacks[i](); } }

	private:
		std::vector<std::function<void()>> callbacks;

	};

}