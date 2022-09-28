#include "cupch.h"
#include "Material.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

namespace Copper {

    void Material::Serialize(std::filesystem::path path) {

        YAML::Emitter out;

        out << YAML::BeginSeq;
        out << "Material:";
        
        out << YAML::BeginMap;
        
        out << YAML::Key << "color";
        out << YAML::Value << YAML::BeginSeq;
        out << color.r;
        out << color.g;
        out << color.b;
        out << color.a;
        out << YAML::EndSeq;
        
        out << YAML::EndMap;
        out << YAML::EndSeq;

        std::ofstream file(path);
        file << out.c_str();
        
    }

    Material Deserialize(std::filesystem::path path) {

        YAML::Node data;

        try {

            data = YAML::LoadFile(path.string());
            
        } catch(YAML::ParserException e) { LogError("Could Not Deserialize Material file. Path: {0}\n\n", path.string(), e.what()); return Material::Null(); }

        Material mat;

        YAML::Node color = data[1]["color"];
        
        float r = color[0].as<float>();
        float g = color[1].as<float>();
        float b = color[2].as<float>();
        float a = color[3].as<float>();

        mat.color = Color(r, g, b, a);

        //Log("Material Color: R: {0}, G: {1}, B: {2}, A: {3}", r, g, b, a);
        Log("Material Color: {0}", mat.color);

        return mat;
        
    }
    
}