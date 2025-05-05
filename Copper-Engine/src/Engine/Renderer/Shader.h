#pragma once
// Source is in Platform/<API>/<API>Shader.cpp

#include "Engine/Core/Core.h"

namespace Copper {

    // Higher level, platform indenpendant wrapper for a Shader 
    class Shader {

    public:
        Shader() = default;
        Shader(const fs::path& vertexPath, const fs::path& fragmentPath) { Create(vertexPath, fragmentPath); }

        void Create(const fs::path& vertexPath, const fs::path& fragmentPath);

        // Using

        void Bind() const;
        void Unbind() const;

        void Delete();

        // Uniforms

        uint32 UniformLocation(const std::string& name) const;

        void LoadInt(uint32 location, int32 value) const;
        void LoadUInt(uint32 location, uint32 value) const;
        void LoadFloat(uint32 location, float value) const;

        void LoadVec2(uint32 location, const Vector2& vec) const;
        void LoadVec3(uint32 location, const Vector3& vec) const;
        void LoadVec4(uint32 location, const Vector4& vec) const;
        void LoadColor(uint32 location, const Color& col) const;

        void LoadMat3(uint32 location, const Matrix3& mat) const;
        void LoadMat4(uint32 location, const Matrix4& mat) const;

        inline void LoadInt(const std::string& name, int32 value) const { LoadInt(UniformLocation(name), value); }
        inline void LoadUInt(const std::string& name, uint32 value) const { LoadUInt(UniformLocation(name), value); }
        inline void LoadFloat(const std::string& name, float value) const { LoadFloat(UniformLocation(name), value); }

        inline void LoadVec2(const std::string& name, const Vector2& vec) const { LoadVec2(UniformLocation(name), vec); }
        inline void LoadVec3(const std::string& name, const Vector3& vec) const { LoadVec3(UniformLocation(name), vec); }
        inline void LoadVec4(const std::string& name, const Vector4& vec) const { LoadVec4(UniformLocation(name), vec); }
        inline void LoadColor(const std::string& name, const Color& col) const { LoadColor(UniformLocation(name), col); }

        inline void LoadMat3(const std::string& name, const Matrix3& mat) const { LoadMat3(UniformLocation(name), mat); }
        inline void LoadMat4(const std::string& name, const Matrix4& mat) const { LoadMat4(UniformLocation(name), mat); }

        // Misc 

        inline operator bool() const { return m_id != 0; }
        inline bool IsValid() const { return (bool) *this; }

    private:
        uint32 m_id = 0;
        uint32 m_vertex = 0;
        uint32 m_fragment = 0;

    };

}
