#pragma once

#include "cupch.h"

#include "Engine/Core/Core.h"

namespace Copper {

	class COPPER_API Event {

	public:
		Event() = default;

		void Call() {

			for (int i = 0; i < callbacks.size() && !handled; i++) {

				handled = callbacks[i](*this);

			}

		}

		void Clear() { handled = false; }

		inline Event& operator+=(bool (*func)(Event&)) { callbacks.push_back(func); return *this; }
		virtual std::string ToString() const = 0;

	private:
		std::vector<bool (*) (Event&)> callbacks;
		bool handled = false;

	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e) { return os << e.ToString(); }

}