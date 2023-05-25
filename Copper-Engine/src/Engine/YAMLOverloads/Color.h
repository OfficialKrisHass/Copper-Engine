#pragma once

#include "Engine/Renderer/Color.h"

#include <yaml-cpp/yaml.h>

namespace Copper {

    inline YAML::Emitter& operator<<(YAML::Emitter& out, const Color& col) {

		out << YAML::Flow << YAML::BeginSeq << col.r << col.g << col.b << col.a << YAML::EndSeq;
		return out;

	}

}

namespace YAML {

    template<> struct convert<Copper::Color> {

        static Node encode(const Copper::Color& col) {

	    	Node node;
	    	node.push_back(col.r);
	    	node.push_back(col.g);
	    	node.push_back(col.b);
	    	node.push_back(col.a);
	    	node.SetStyle(EmitterStyle::Flow);

	    	return node;

	    }
	    static bool decode(const Node& node, Copper::Color& col) {

	    	if (!node.IsSequence() || node.size() != 4) return false;

	    	col.r = node[0].as<float>();
	    	col.g = node[1].as<float>();
	    	col.b = node[2].as<float>();
	    	col.a = node[3].as<float>();

	    	return true;

	    }

    };

}

