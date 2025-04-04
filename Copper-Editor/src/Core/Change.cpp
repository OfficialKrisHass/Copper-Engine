#include "Change.h"

namespace Editor {

    // Write 

    Change& Change::operator<<(const std::string& data) {

        CUP_FUNCTION();

        m_data.push_back(data);
        return *this;

    }
    
    Change& Change::operator<<(const Vector3& data) {

        CUP_FUNCTION();

        std::stringstream ss;
        ss << data.x << ' ' << data.y << ' ' << data.z;
        m_data.push_back(ss.str());

        return *this;

    }
    Change& Change::operator<<(const Quaternion& data) {

        CUP_FUNCTION();

        std::stringstream ss;
        ss << data.w << ' ' << data.x << ' ' << data.y << ' ' << data.z;
        m_data.push_back(ss.str());

        return *this;

    }

    // Read

    void Change::Read(Vector3& out) {

        CUP_FUNCTION();

        std::stringstream ss = std::stringstream(m_data[m_index]);
        ss >> out.x >> out.y >> out.z;

    }
    void Change::Read(Quaternion& out) {

        CUP_FUNCTION();

        std::stringstream ss = std::stringstream(m_data[m_index]);
        ss >> out.w >> out.x >> out.y >> out.z;

    }

}
