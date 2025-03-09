#pragma once

namespace Editor::NewModal::Data {

    inline const char* options[] = {

        "Folder",

        "C# Script",

        "Material",

    };
    inline const char* optionsDetails[] = {

        "Create a new folder in the current directory.",

        "Create a new C# script based on the Component class.\n\n"
        "Scripts are used to give objects functionality by writing code. A script can be put on any entity as a component (that is if it derives "
        "the Component class) and run the code inside the script\n\n"
        "The new script will be autofilled with a basic sample component class, with the name provided below.",

        "Create a new material.\n\n"
        "Materials are used to give a mesh texture, color, normals, and other details. A material can be put on any mesh, changing how said mesh "
        "looks based on the parameters of the material (the texture, color, normal map, etc)\n\n"
        "The new material will have no texture and a white colour rendering any mesh pure white",

    };

    inline constexpr int optionsCount = static_cast<int>(sizeof(options) / sizeof(*options));

}
