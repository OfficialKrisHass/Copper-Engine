#pragma once

#include "Engine/Core/Core.h"

#define BindEventFunc(func) std::bind(&func, this, std::placeholders::_1)

namespace Copper {

	class Event {

	public:
		Event() = default;

		inline void Call() { (*this)(); }
		inline void Clear() { m_success = true; }

		inline Event& operator+=(std::function<bool(const Event&)> func) { m_callbacks.push_back(func); return *this; }
		inline bool operator()() {

			for (int i = 0; i < m_callbacks.size() && m_success; i++) {

				m_success = m_callbacks[i](*this);

			}

			bool tmp = m_success;
			m_success = true;

			return tmp;

		}

	private:
		std::vector<std::function<bool(const Event&)>> m_callbacks;
		bool m_success = true;

	};

	class SimpleEvent {

	public:
		inline SimpleEvent& operator+=(std::function<void()> func) { m_callbacks.push_back(func); return *this; }
		inline void operator()() { for (size_t i = 0; i < m_callbacks.size(); i++) { m_callbacks[i](); } }

	private:
		std::vector<std::function<void()>> m_callbacks;

	};

}