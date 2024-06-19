#include "Base.h"
#include "Init.h"

#include <mono/jit/jit.h>

#include <mono/metadata/object.h>
#include <mono/metadata/reflection.h>
#include <mono/metadata/attrdefs.h>

#include <iostream>

namespace APIBinder {

    MonoDomain* domain = nullptr;

    MonoImage* image = nullptr;
    MonoAssembly* assembly = nullptr;

    MonoClass* nativeClassAttr = nullptr;
    MonoClass* nativeFunctionAttr = nullptr;

    void BindClasses();
    void BindClass(MonoClass* klass);
    void BindMethod(MonoClass* klass, MonoMethod* method);

    int Entry() {

        std::cout << "\nRunning Copper Scripting API Binder\n\n";

        // Setup

        domain = InitMono();
        assembly = LoadAssembly(&image);

        nativeClassAttr = mono_class_from_name_case(image, "Copper", "NativeClassAttribute");
        nativeFunctionAttr = mono_class_from_name_case(image, "Copper", "NativeFunctionAttribute");

        std::cout << "\n";

        // Run

        BindClasses();

        return 0;

    }

    void BindClasses() {

        const MonoTableInfo* typeTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
        uint32 num = mono_table_info_get_rows(typeTable);
        
        for (uint32 i = 0; i < num; i++) {

            uint32 cols[MONO_TYPEDEF_SIZE];
            mono_metadata_decode_row(typeTable, i, cols, MONO_TYPEDEF_SIZE);

            const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
            const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);
            if (std::string(name) == "<Module>") continue;

            MonoClass* klass = mono_class_from_name_case(image, nameSpace, name);
            if (klass != nullptr)
                BindClass(klass);

        }

    }
    void BindClass(MonoClass* klass) {

        MonoCustomAttrInfo* attrInfo = mono_custom_attrs_from_class(klass);
        if (attrInfo == nullptr) return;
        if (!mono_custom_attrs_has_attr(attrInfo, nativeClassAttr)) return;

        std::cout << "Binding Class: " << mono_class_get_name(klass) << "\n";

        void* iter = nullptr;
        while (MonoMethod* method = mono_class_get_methods(klass, &iter)) {

            attrInfo = mono_custom_attrs_from_method(method);
            if (attrInfo == nullptr) continue;
            if (!mono_custom_attrs_has_attr(attrInfo, nativeFunctionAttr)) continue;

            BindMethod(klass, method);

        }

        std::cout << "\n";

    }
    void BindMethod(MonoClass* klass, MonoMethod* method) {

        std::cout << "Binding Method: " << mono_method_get_name(method) << "\n";

    }

}

int main() { return APIBinder::Entry(); }
