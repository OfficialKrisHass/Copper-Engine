#include "Base.h"
#include "Init.h"

#include <mono/jit/jit.h>

#include <iostream>

namespace APIBinder {

    MonoDomain* domain = nullptr;

    MonoImage* image = nullptr;
    MonoAssembly* assembly = nullptr;

    void BindClass(MonoClass* klass);

    int Entry() {

        std::cout << "\nRunning Copper Scripting API Binder\n\n";

        domain = InitMono();
        assembly = LoadAssembly(&image);

        std::cout << "\n";
        
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

        return 0;

    }

    void BindClass(MonoClass* klass) {

        std::cout << mono_class_get_name(klass) << "\n";

    }

}

int main() { return APIBinder::Entry(); }
