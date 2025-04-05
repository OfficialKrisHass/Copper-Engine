#pragma once

#include <Copper.h>

namespace Editor {

    using namespace Copper;

    struct Change {

        enum class Type : uint8 {

            None = 0,

            EntityCreated,

            EntityMoved,
            EntityRotated,
            EntityScaled,

        };

        Type type = Type::None;

        Change() = default;
        Change(Type type) : type(type) {}

        template<typename T> Change& operator<<(T data) {

            CUP_FUNCTION();

            m_data.push_back(std::to_string(data));
            return *this;

        }
        Change& operator<<(const std::string& data);

        Change& operator<<(const Vector3& data);
        Change& operator<<(const Quaternion& data);

        template<typename T> Change& operator>>(T& out) {

            CUP_FUNCTION();

            CU_ASSERT(m_index < m_data.size(), "Can't read from change as the read index ({}) is out of range ({})", m_index, m_data.size());

            Read(out);

            m_index++;
            return *this;

        }

        inline void ResetIndex() { m_index = 0; }

        inline const std::vector<std::string>& GetData() const { return m_data; }

    private:
        std::vector<std::string> m_data;
        uint32 m_index = 0;

        template<typename T> void Read(T& out) {

            CUP_FUNCTION();

            std::stringstream ss = std::stringstream(m_data[m_index]);
            ss >> out;

        }
        void Read(std::string& out);

        void Read(Vector3& out);
        void Read(Quaternion& out);

    };

}
