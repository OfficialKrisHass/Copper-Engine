# Blog 0 - Asset Storage
In this blog I'm gonna talk about [how I want the Asset Storage  to work](https://github.com/OfficialKrisHass/Copper-Engine/blob/dev/Blog/AssetStorage.md#how), [how it should be used](https://github.com/OfficialKrisHass/Copper-Engine/blob/dev/Blog/AssetStorage.md#how-will-this-be-used) and stuff like that

## Why
The Asset storage is meant to help with the fact that every mesh stored it's material in the class, and if 10 meshes had the same material, there would be unneccessary 10 copies of the same material in Memory, and also that need to be sent to the Shader which is a big performance nono.
To fight that, we can instead store all of the materials in one single place, and the 10 meshes can just have a pointer pointing to that material, which can save up on space massivelly.

## How will this be used
Only a few assets will be loaded by the engine automatically, this includes stuff like the White material and texture, Primitive meshes, etc. The editor will then load all of the assets in the Project asset directory into the asset storage, and this will be done behind the scenes, without the dev noticing anything. And then once the game is gonna be built, the Editor is going to compile all of the assets it loaded into the Asset Storage, into a single file called the Asset Pack, that the Engine loads when the game is launched as a standalone app.

Developers will also be able to store their own data in the Asset Storage. HOWEVER, that can only be done during the game runtime (standalone App or Editor Play mode), and this data will NOT be serialized and deserialized into the Asset Pack. The users of Copper will have to do that on their own.
It is also possible to store custom data types in the Asset Storage, for that, see [Custom Data storing](https://github.com/OfficialKrisHass/Copper-Engine/blob/dev/Blog/AssetStorage.md#custom-data-storing)

## How
### General overview
Things such as a Texture, Material, Mesh, etc can be classified as Assets, And all of the assets will be stored in custom AssetMap classes that will be a wrapper around an unordered map of UUID-Asset key-value type.
```cpp
template<typename AssetType> class AssetMap {

public:
    AssetType* Create() {

        ...

    }
    AssetType* Get(UUID id) {

        ...

    }

private:
    std::unordered_map<UUID, AssetType> m_map;

}
```

And whenever you want to use this AssetMap, you can get it by calling a template function.
```cpp
template<typename AssetType> AssetMap<AssetType> GetAssetMap();

AssetMap<Texture> textures;
AssetMap<Material> materials;

template<> AssetMap<Texture> GetAssetMap<Texture>() { return textures; }
template<> AssetMap<Material> GetAssetMap<Material>() { return materials; }
```
Or you can use global template functions.
```cpp
template<typename AssetType> AssetType* CreateAsset() { return GetAssetMap<AssetType>().Create(); }
template<typename AssetType> AssetType* GetAsset() { return GetAssetMap<AssetType>().Get(); }
```
### AssetPtr class
It is possible for a pointer to an Asset located in an AssetMap could become invalid due to the AssetMap rehashing/realocating. Which could cause a lot of issues that a pointer to an Asset can, at any point become invalid. To fix that, a part of the Asset Storage system, will be our own smart pointer. It will store the key to the asset it is pointing to, and overload the -> operator, to return the pointer to the Asset located at that key.
```cpp
template<typename AssetType> struct AssetPtr {

    AssetType* operator->() const { return GetAssetMap<AssetType>().Get(m_key); }

private:
    UUID m_key;

}
```
And to rid the devs (and myself) of having to type `AssetPtr<Material>`, we can create typedefs for commonly used AssetPtrs.
```cpp
typedef AssetPtr<MaterialData> Material;
typedef AssetPtr<TextureData> Material;
```
You can also see that we rename the old names (`Material` -> `MaterialData`) since we don't want the user to use `MaterialData` basically at any point since it is used only for storing. We could rename the AssetPtr typedef to `MaterialPtr`, but that would cause confusion for someone who doesn't know about the Asset Storage that there is a `Material` class and `MaterialPtr`.

## Custom Data storing
You can store custom classes as Assets by literally including 2 lines of code. An AssetMap of your desired AssetType variable, and then a specification of the GetAssetMap function.
```cpp
AssetMap<CustomAssetType> myAssets;

template<> AssetMap<CustomAssetType> GetAssetMap<CustomAssetType>() { return myAssets; }
```
Now you can use functions like `CreateAsset`, `GetAsset` with the template argument of `CustomAssetType`, with the added benefit that you get low level access to the AssetMap myAssets.

> [!IMPORTANT]
> The AssetMap of `myAssets` WILL NOT be serialized nor deserialized, meaning you need to Load the assets into it yourself. Either by serializing it into its own file and then populating it by deserializing that file, or by any other means, depending on how you want to use the AssetMap

> [!NOTE]
> Maybe there could be a way to allow the devs to serialize custom file types as Assets into the AssetPack ?

## Editor
Most of what I have just talked about is included in the Engine. The editor is going to contain a bunch of stuff that mostly either populates or works with the Asset Storage.

### Asset files
Most of the Assets will be stuff like Materials and Models, which exist as a file in the Project directory. And since the Engine will not be ever working with these files as they will be loaded into the Asset Pack during build, the serialization and deserialization of these asset files will be done in the Editor.

#### Meta files
All of the Asset files will have a .cum (Copper Meta, what the fuck were you thinking of) file that stores the UUID of the corresponding file. The UUID is then used as the key to the AssetMap where the Asset is stored.

### Asset Loader
First at launch, after the Project has been loaded, but before we open the scene, we scan the Project directory for every asset (image, material, model, ... files) and load it into the Asset Storage. And then whenver a file is created or deleted in the Project, we check if its an asset file, and if it is, we add or remove it from the Asset Storage.
We get the UUID of the Asset from its Meta file so that we always store it with the same key in the AssetMap
```cpp
void LoadAssetFiles() {

    for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(<Project directory>)) {

        if (!IsAssetFile(entry)) continue;

        UUID uuid = GetUUID(entry);
        switch (AssetFileType(entry)) {

            case Material: CreateAsset<Material>(uuid);
            case Texture: CreateAsset<Texture>(uuid);
            .
            .
            .

        }

    }

}
```
It is possible that when we are Loading the Asset Files, we load an Asset that is referencing an Asset that wasn't loaded yet (e.g. A material being loaded before it's texture is). But since `AssetPtr` only stores the UUID and not the Asset itself, we can set the `AssetPtr` of the material without the Texture the `AssetPtr` is pointing to being loaded first.