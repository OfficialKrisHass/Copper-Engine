#pragma once

#include "Engine/Core/Core.h"

#ifdef CU_WINDOWS
    #define OTHER_SEPARATOR '/'
#else
    #define OTHER_SEPARATOR '\\'
#endif

namespace std::filesystem { class path; }
namespace Copper::Filesystem { class Path; }
namespace YAML { template<> struct convert<Copper::Filesystem::Path>; }

namespace Copper::Filesystem {

    class Path {

        friend YAML::convert<Path>;

    public:
        Path() = default;

        Path(const char* string, bool replaceSeparator = true);
        Path(const std::string& string, bool replaceSeparator = true);

#ifdef CU_WINDOWS
        static constexpr char separator = '\\';
#else
        static constexpr char separator = '/';
#endif

        //Makes sure that the path has the correct Separator and updates the Path this function has been called on
        void ReplaceSeparator();
        //Generates a new Path with the correct Separator. This function does not update the Path the function has been called on
        Path CreateAndReplaceSeparator() const;

        void Delete();

        void Add(const std::string& string, bool replaceSeparator = true);

        std::string Extension() const;
        Path File(bool withExtension = false) const;
        Path RelativeTo(const Path& parent) const;
        Path ParentPath() const;

        bool Directory() const;
        bool Empty() const { return str.empty(); }

        Path operator+(const std::string& other) const;
        Path operator+(const Path& other) const;
        Path operator+(const char* other) const;
        Path operator+(const char  other) const;

        Path operator/(const std::string& other) const;
        Path operator/(const Path& other) const;
        Path operator/(const char* other) const;

        Path& operator+=(const std::string& other);
        Path& operator+=(const Path& other);
        Path& operator+=(const char* other);
        Path& operator+=(const char  other);

        Path& operator/=(const std::string& other);
        Path& operator/=(const Path& other);
        Path& operator/=(const char* other);

        char& operator[](uint32_t index);

        bool operator==(const Path& other) const;
        bool operator==(const char* other) const;
        bool operator==(const std::string& other) const;

        bool operator!=(const Path& other) const;
        bool operator!=(const char* other) const;
        bool operator!=(const std::string& other) const;

        operator std::string() const;

        const std::string& String() const;

    private:
        std::string str = "";

    };

}

#include "Path.inl"