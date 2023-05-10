#pragma once

#include "Engine/Filesystem/Path.h"

#include <yaml-cpp/yaml.h>

namespace Copper {

    inline YAML::Emitter& operator<<(YAML::Emitter& out, const Filesystem::Path& path) {

        out << path.String();
        return out;

    }

}

namespace YAML {

    template<> struct convert<Copper::Filesystem::Path> {

        static Node encode(const Copper::Filesystem::Path& path) {

            Node node;
            node.push_back(path.String());

            return node;

        }
        static bool decode(const Node& node, Copper::Filesystem::Path& path) {

            path.str = node.as<std::string>();

            return true;

        }


    };

}

