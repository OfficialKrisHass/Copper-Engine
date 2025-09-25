#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Renderer/Mesh.h"
#include "Engine/Renderer/Material.h"

#include "Engine/AssetStorage/AssetPtr.h"

#include "Engine/Components/Component.h"

COMPONENT_FORWARD_DECL()

namespace Copper {

    class MeshRenderer : public Component {

        COMPONENT_FRIEND_CLASSES();

    public:
        MeshRenderer() = default;

        MeshAsset mesh;
        MaterialAsset material;

    };

}
