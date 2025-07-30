#pragma once

#include "Engine/Core/Core.h"

#include "Engine/AssetStorage/AssetMap.h"

// Registers a new asset type, this has to be in the in the global scope and inside no namespace.
// If type is inside a namespace, you have to 'using namespace <namespace Type is in>' before this.
#define REGISTER_ASSET_TYPE(Namespace, Type) namespace Copper::AssetStorage {\
                                             using namespace Namespace;\
                                             inline AssetMap<Type> Type ## Map;\
                                             template<> inline AssetMap<Type>& GetAssetMap<Type>() { return Type ## Map; } }\
                                             namespace Namespace {\
                                             typedef AssetPtr<Type> Type ## Asset; }

namespace Copper::AssetStorage {

    // Retrieves the asset map. This function is deleted by default and has to be template specialized manually.
    // To do so, use REGISTER_ASSET_TYPE.
    template<typename AssetType> AssetMap<AssetType>& GetAssetMap() = delete;

    namespace traits {

        template<typename T> struct IsAssetTypeRegistered {

        private:
            template<typename U> static auto Impl(int) -> decltype(GetAssetMap<U>(), std::true_type{});
            template<typename U> static auto Impl(...) -> std::false_type;

        public:
            static constexpr bool value = decltype(Impl<T>(0))::value;

        };

    }

    template<typename AssetType, typename... Args> inline AssetPtr<AssetType> CreateAsset(Args&&... args) {
        
        CUP_FUNCTION();

        static_assert(traits::IsAssetTypeRegistered<AssetType>::value, "Asset type is not registered!");
        return GetAssetMap<AssetType>().Create(std::forward<Args>(args)...);

    }
    template<typename AssetType, typename... Args> inline AssetPtr<AssetType> InsertAsset(const UUID& uuid, Args&&... args) {

        CUP_FUNCTION();

        static_assert(traits::IsAssetTypeRegistered<AssetType>::value, "Asset type is not registered!");
        return GetAssetMap<AssetType>().Insert(uuid, std::forward<Args>(args)...);

    }

    template<typename AssetType> inline AssetPtr<AssetType> GetAsset(const UUID& uuid) {
        
        CUP_FUNCTION();

        static_assert(traits::IsAssetTypeRegistered<AssetType>::value, "Asset type is not registered!");
        return GetAssetMap<AssetType>().Get(uuid);
    
    }

    template<typename AssetType> inline void DeleteAsset(const UUID& uuid) {
        
        CUP_FUNCTION();

        static_assert(traits::IsAssetTypeRegistered<AssetType>::value, "Asset type is not registered!");
        GetAssetMap<AssetType>().Delete(uuid);
    
    }

    template<typename AssetType> inline bool ContainsAsset(const UUID& uuid) {

        CUP_FUNCTION();

        static_assert(traits::IsAssetTypeRegistered<AssetType>::value, "Asset type is not registered!");
        return GetAssetMap<AssetType>().Contains(uuid);

    }

}

#include "Engine/AssetStorage/AssetPtr.h"
