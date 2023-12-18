#pragma once

#include <yaml-cpp/yaml.h>

namespace Copper {

    inline YAML::Emitter& operator<<(YAML::Emitter& out, const fs::path& path) {

        out << path.string();
        return out;

    }

}

namespace YAML {

    template<> struct convert<fs::path> {

        static Node encode(const fs::path& path) {

            Node node;
            node.push_back(path.string());

            return node;

        }
        static bool decode(const Node& node, fs::path& path) {

            path = node.as<std::string>();

            return true;

        }


    };

}

