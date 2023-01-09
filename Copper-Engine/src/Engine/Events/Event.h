#pragma once

#include "cupch.h"

#include "Engine/Core/Core.h"

#define BindEventFunc(func) std::bind(&func, this, std::placeholders::_1)

namespace Copper {

	class Event {

	public:
		Event() = default;

		void Call() {

			for (int i = 0; i < callbacks.size() && !handled; i++) {

				handled = callbacks[i](*this);

			}

		}

		void Clear() { handled = false; }

		inline Event& operator+=(std::function<bool(const Event&)> func) { callbacks.push_back(func); return *this; }
		virtual std::string ToString() const { return "Event"; };

	private:
		std::vector<std::function<bool(const Event&)>> callbacks;
		bool handled = false;

	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e) { return os << e.ToString(); }

}