#pragma once

#include "Engine/Core/Core.h"

#define BindEventFunc(func) std::bind(&func, this, std::placeholders::_1)

namespace Copper {

	class Event {

	public:
		Event() = default;

		inline void Call() { (*this)(); }

		inline Event& operator+=(std::function<bool(const Event&)> func) { m_callbacks.push_back(func); return *this; }
		inline bool operator()() {

            for (int i = 0; i < m_callbacks.size(); i++) {

                if (m_callbacks[i](*this)) continue;
                return false;

            }

            return true;

		}

	private:
		std::vector<std::function<bool(const Event&)>> m_callbacks;

	};

	class SimpleEvent {

	public:
		inline SimpleEvent& operator+=(std::function<void()> func) { m_callbacks.push_back(func); return *this; }
		inline void operator()() {
			
			for (size_t i = 0; i < m_callbacks.size(); i++) {
				
				m_callbacks[i]();
			
			}
		
		}

	private:
		std::vector<std::function<void()>> m_callbacks;

	};

}
