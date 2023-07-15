#pragma once

#include <yaml-cpp/yaml.h>

namespace Copper {

    template<typename T> inline YAML::Emitter& operator<<(YAML::Emitter& out, const CMath::quat<T>& q) {

        out << YAML::Flow << YAML::BeginSeq << q.w << q.x << q.y << q.z << YAML::EndSeq;
        return out;

    }

}

namespace YAML {

    template<typename T> struct convert<Copper::CMath::quat<T>> {

        static Node encode(const Copper::CMath::quat<T>& q) {

			Node node;
			node.push_back(q.w);
			node.push_back(q.x);
			node.push_back(q.y);
			node.push_back(q.z);
			node.SetStyle(EmitterStyle::Flow);

			return node;

		}
		static bool decode(const Node& node, Copper::CMath::quat<T>& q) {

			if (!node.IsSequence()) return false;
			if (node.size() == 3) {

				LogWarn("Node {} has a size of 3 but is trying to be read as a Quaternion, reading it as Euler Angles.", node.Tag());

				Copper::CMath::vec<3, T> eulerAngles;
				eulerAngles.x = node[0].as<T>();
				eulerAngles.y = node[1].as<T>();
				eulerAngles.z = node[2].as<T>();

				q = eulerAngles;
				return true;

			}

			q.w = node[0].as<T>();
			q.x = node[1].as<T>();
			q.y = node[2].as<T>();
			q.z = node[3].as<T>();

			return true;

		}

    };

}