#include "cupch.h"
#include "Classes.h"

#include "Engine/Scripting/ScriptingEngine.h"

#include <mono/metadata/class.h>

#define GET_CLASS(class) classes[static_cast<uint8>(Class::class)] = mono_class_from_name(ScriptingAPIAssembly().GetImage(), "Copper", #class);\
                         CU_ASSERT(classes[static_cast<uint8>(Class::class)] != nullptr, "Failed to get class Copper.{}", #class);

namespace Copper::Scripting {

    MonoClass* classes[static_cast<uint8>(Class::COUNT)];

    void InitializeClasses() {
        
        CUP_FUNCTION();

        CU_ASSERT(ScriptingAPIAssembly().IsValid(), "Called InitializeClasses() while ScriptingAPI assembly is invalid!");
        CU_ASSERT(ScriptingAPIAssembly().GetImage() != nullptr, "Called InitializeClasses() while ScriptingAPI image is invalid!");

        GET_CLASS(Base);
        
        GET_CLASS(Entity);
        GET_CLASS(Component);
        GET_CLASS(Transform);

        GET_CLASS(MeshRenderer);
        GET_CLASS(Camera);
        GET_CLASS(Light);

        GET_CLASS(RigidBody);
        GET_CLASS(Collider);
        GET_CLASS(BoxCollider);
        GET_CLASS(SphereCollider);
        GET_CLASS(CapsuleCollider);

        GET_CLASS(Asset);
        //GET_CLASS(Texture);
        GET_CLASS(Material);

        GET_CLASS(ShowInEditorAttribute);
        GET_CLASS(HideInEditorAttribute);


    }

    MonoClass* GetClass(Class klass) {

        CUP_FUNCTION();

        CU_ASSERT(klass != Class::None, "Can't get invalid Class (Class::None)!");
        CU_ASSERT(classes[static_cast<uint8>(klass)] != nullptr, "Class '{}' is invalid!", static_cast<uint8>(klass));

        return classes[static_cast<uint8>(klass)];

    }
    Class MonoClassToClass(MonoClass *klass) {

        CUP_FUNCTION();

        for (uint32 i = 0; i < static_cast<uint8>(Class::COUNT); i++)
            if (classes[i] == klass) return static_cast<Class>(i);

        return Class::None;

    }

}
