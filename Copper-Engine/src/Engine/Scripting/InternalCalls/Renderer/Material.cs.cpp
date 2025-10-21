#include "cupch.h"
#include "Material.cs.h"
#include "Engine/Renderer/Material.h"

#include "Engine/Scripting/ScriptingEngine.h"
#include "Engine/Scripting/Classes.h"

#include <mono/jit/jit.h>
#include <mono/metadata/object.h>

#define GET_ASSET(assetType, managedAsset, assetName) CU_ASSERT(managedAsset != nullptr, "Can't get an asset from nullptr managed asset.");\
                                                      MonoArray* uuidBytes = nullptr; mono_field_get_value(managedAsset, AssetUUIDField(), &uuidBytes);\
                                                      UUID uuid = mono_array_addr(uuidBytes, uint8, 0); assetType assetName = uuid;\
                                                      CU_ASSERT(assetName.IsValid(), "{0} retrieved from C# {0} asset is invalid.", #assetType)

namespace Copper::Scripting::Material {

    typedef ::Copper::Material Material;

    Color GetAlbedo(MonoObject* material) {

        CUP_FUNCTION();

        GET_ASSET(MaterialAsset, material, mat);
        return mat->albedo;

    }
    float GetTiling(MonoObject* material) {

        CUP_FUNCTION();

        GET_ASSET(MaterialAsset, material, mat);
        return mat->tiling;

    }

    void SetAlbedo(MonoObject* material, Color value) {

        CUP_FUNCTION();

        GET_ASSET(MaterialAsset, material, mat);
        mat->albedo = value;

    }
    void SetTiling(MonoObject* material, float value) {

        CUP_FUNCTION();

        GET_ASSET(MaterialAsset, material, mat);
        mat->tiling = value;

    }

}
