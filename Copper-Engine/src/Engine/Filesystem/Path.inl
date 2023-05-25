#include "Path.h"

namespace Copper::Filesystem {

    inline Path Path::operator+(const std::string& other) const {

        Path ret = this->str;
        ret.Add(other);
        return ret;

    }
    inline Path Path::operator+(const Path& other) const { return *this + other.str; }
    inline Path Path::operator+(const char* other) const { return *this + std::string(other); }
    inline Path Path::operator+(const char  other) const {
        
        Path ret = this->str;

        if(other == separator || other == OTHER_SEPARATOR) ret.str += separator;
        else ret.str += other;

        return ret;

    }
    
    inline Path Path::operator/(const std::string& other) const {
        
        Path ret = this->str;
        size_t end = str.length() - 1;

        if ((ret.str[end] == separator || ret.str[end] == OTHER_SEPARATOR) && (other[0] == separator || other[0] == OTHER_SEPARATOR))
            ret.str.pop_back();
        else if((ret.str[end] != separator && ret.str[end] != OTHER_SEPARATOR) && (other[0] != separator && other[0] != OTHER_SEPARATOR))
            ret.str += separator;

        ret.str += other;
        ret.ReplaceSeparator();

        return ret;

    }
    inline Path Path::operator/(const Path& other) const { return *this / other.str; }
    inline Path Path::operator/(const char* other) const { return *this / std::string(other); }

    inline Path& Path::operator+=(const std::string& other) {

        Add(other);
        return *this;

    }
    inline Path& Path::operator+=(const Path& other) { return *this += other.str; }
    inline Path& Path::operator+=(const char* other) { return *this += std::string(other); }
    inline Path& Path::operator+=(const char  other) {

        if(other == separator || other == OTHER_SEPARATOR) str += separator;
        else str += other;

        return *this;

    }

    inline Path& Path::operator/=(const std::string& other) {

        size_t end = str.length() - 1;

        if ((str[end] == separator || str[end] == OTHER_SEPARATOR) && (other[0] == separator || other[0] == OTHER_SEPARATOR))
            str.pop_back();
        else if((str[end] != separator && str[end] != OTHER_SEPARATOR) && (other[0] != separator && other[0] != OTHER_SEPARATOR))
            str += separator;

        str += other;
        ReplaceSeparator();

        return *this;

    }
    inline Path& Path::operator/=(const Path& other) { return *this /= other.str; }
    inline Path& Path::operator/=(const char* other) { return *this /= std::string(other); }

    inline char& Path::operator[](uint32_t index) { return str[index]; }  

    inline bool Path::operator==(const Path& other) const { return str == other.str; }
    inline bool Path::operator==(const char* other) const { return str == other; }
    inline bool Path::operator==(const std::string& other) const { return str == other; } 

    inline bool Path::operator!=(const Path& other) const {return !(*this == other); }
    inline bool Path::operator!=(const char* other) const {return !(*this == other); }
    inline bool Path::operator!=(const std::string& other) const {return !(*this == other); }

    inline Path::operator std::string() const { return str; } 

    inline const std::string& Path::String() const { return str; }

    inline std::ostream& operator<<(std::ostream& os, const Path& path) {

        os << path.String();
        return os;

    }

}