#pragma once

#include "Engine/Core/Core.h"

#define BindEventFunc(func) std::bind(&func, this, std::placeholders::_1)

namespace Copper {

    class Event {

    public:
        Event() = default;

        inline void Call() const { (*this)(); }
        inline void Clear() { m_callbacks.clear(); }

        inline Event& operator+=(std::function<bool(const Event&)> func) {

            CUP_FUNCTION();

            m_callbacks.push_back(func);
            return *this;

        }
        inline bool operator()() const {

            CUP_FUNCTION();

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
        inline void Call() const { (*this)(); }
        inline void Clear() { m_callbacks.clear(); }

        inline SimpleEvent& operator+=(std::function<void()> func) {

            CUP_FUNCTION();

            m_callbacks.push_back(func);
            return *this;

        }
        inline void operator()() const {

            CUP_FUNCTION();

            for (size_t i = 0; i < m_callbacks.size(); i++)
                m_callbacks[i]();

        }

    private:
        std::vector<std::function<void()>> m_callbacks;

    };

}
