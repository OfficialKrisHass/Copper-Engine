#pragma once

#include <Copper.h>

namespace Editor {

    using namespace Copper;

    struct Change {

        enum class Type : uint8 {

            None = 0,

            EntityCreated,
            EntityTransformed,

        };

        Type type = Type::None;

        Change() = default;
        Change(Type type) : type(type) {}

        inline Change& operator<<(int32 data) {

            CUP_FUNCTION();

            m_data.push_back(std::to_string(data));
            return *this;

        }
        inline Change& operator<<(uint32 data) {

            CUP_FUNCTION();

            m_data.push_back(std::to_string(data));
            return *this;

        }
        inline Change& operator<<(float data) {

            CUP_FUNCTION();

            m_data.push_back(std::to_string(data));
            return *this;

        }
        inline Change& operator<<(double data) {

            CUP_FUNCTION();

            m_data.push_back(std::to_string(data));
            return *this;

        }
        inline Change& operator<<(const std::string& data) {

            CUP_FUNCTION();

            m_data.push_back(data);
            return *this;

        }

        inline Change& operator<<(const Vector3& data) {

            CUP_FUNCTION();

            std::stringstream ss;
            ss << data.x << ' ' << data.y << ' ' << data.z;
            m_data.push_back(ss.str());

            return * this;

        }
        inline Change& operator<<(const Quaternion& data) {

            CUP_FUNCTION();

            std::stringstream ss;
            ss << data.w << ' ' << data.x << ' ' << data.y << ' ' << data.z;
            m_data.push_back(ss.str());

            return * this;

        }
        
        std::vector<std::string> m_data;

    };

}
