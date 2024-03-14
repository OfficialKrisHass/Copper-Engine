#pragma once

#include "Engine/Core/Core.h"

#include "Engine/AssetStorage/AssetMap.h"
#include "Engine/AssetStorage/AssetStorage.h"

namespace Copper {

	template<typename AssetType> class AssetPtr {

	public:
		AssetPtr() = default;
		AssetPtr(const UUID& uuid) : m_key(uuid) {}
		AssetPtr(const AssetPtr& other) : m_key(other.m_key) {}

		const UUID& AssetUUID() const { return m_key; }
		
		AssetType* operator->() const { return AssetStorage::GetAssetMap<AssetType>().GetRaw(m_key); }

		bool operator==(const UUID& other) const { return m_key == other; }
		bool operator==(const AssetPtr& other) const { return m_key == other.m_key; }
		bool operator!=(const UUID& other) const { return !(*this == other); }
		bool operator!=(const AssetPtr& other) const { return !(*this == other); }

		AssetPtr& operator=(const AssetPtr& other) {

			m_key = other.m_key;
			return *this;

		}
		AssetPtr& operator=(const UUID& other) {

			m_key = other;
			return *this;

		}

		operator UUID() const { return m_key; }

		operator AssetType*() const { return AssetStorage::GetAssetMap<AssetType>().GetRaw(m_key); }
		operator bool() const { return AssetStorage::GetAssetMap<AssetType>().GetRaw(m_key); }

	private:
		UUID m_key = UUID("");

	};

	typedef AssetPtr<class Material> MaterialAsset;
	typedef AssetPtr<class Texture> TextureAsset;
	typedef AssetPtr<class Mesh> MeshAsset;

}