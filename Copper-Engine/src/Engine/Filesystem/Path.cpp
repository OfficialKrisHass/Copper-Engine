#include "cupch.h"
#include "Path.h"

#ifdef CU_WINDOWS
    #include <filesystem>
#else
    #include <experimental/filesystem>
#endif

namespace Copper::Filesystem {

    Path::Path(const char* string, bool replaceSeparator) {

        str = string;
        
        if (!replaceSeparator) return;
        ReplaceSeparator();

    }
    Path::Path(const std::string& string, bool replaceSeparator) {

        str = string;

        if (!replaceSeparator) return;
        ReplaceSeparator();

    }

    void Path::ReplaceSeparator() {

        size_t len = str.length();
        for(size_t i = 0; i < len; i++) {

            if (str[i] != OTHER_SEPARATOR) continue;
            str[i] = separator;

        }

    }
    Path Path::CreateAndReplaceSeparator() const {

        return Path(str);

    }

    void Path::Delete() const {

    #ifdef CU_WINDOWS
        std::filesystem::remove_all(str);
    #else
        std::experimental::filesystem::remove_all(str);
    #endif

    }

    void Path::Add(const std::string& string, bool replaceSeparator) {

        size_t index = str.length();
        str += string;

        if(!replaceSeparator) return;
        ReplaceSeparator();

    }

    std::string Path::Extension() const {

        return str.substr(str.find_last_of('.') + 1);

    }
    Path Path::File(bool withExtension) const {
        
        size_t fileBegin = str.find_last_of(separator);
        if (fileBegin == std::string::npos) fileBegin = str.find_last_of(OTHER_SEPARATOR);
        if (fileBegin == std::string::npos) fileBegin = 0;
        if (str[fileBegin] == separator || str[fileBegin] == OTHER_SEPARATOR) fileBegin++;

        size_t len = std::string::npos;
        size_t dotIndex = str.find_last_of('.');
        if(dotIndex != std::string::npos && !withExtension) {

            len = dotIndex - fileBegin;

        }

        return Path(str.substr(fileBegin, len));

    }
    Path Path::RelativeTo(const Path& parent) const {

        if(str.length() < parent.str.length() + 1)
            return "";

        size_t index = 0;
        for(size_t i = 0; i < parent.str.length(); i++, index++) {

            if(str[i] != parent.str[i])
                return "";

        }

        if(str[index] == separator || str[index] == OTHER_SEPARATOR)
            index++;
        
        std::string ret = str.substr(index);
        return Path(ret);

    }
    Path Path::ParentPath() const {

        size_t end = std::string::npos;
        if(str.back() == separator || str.back() == OTHER_SEPARATOR) end = str.size() - 2;

        size_t len = str.find_last_of(separator, end);
        if (len == std::string::npos) len = str.find_last_of(OTHER_SEPARATOR);

        return Path(str.substr(0, len));

    }

    bool Path::Directory() const {
    #ifdef CU_WINDOWS
        return std::filesystem::is_directory(str);
    #else
        return std::experimental::filesystem::is_directory(str);
    #endif

    }

}