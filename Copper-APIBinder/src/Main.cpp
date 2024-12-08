#include "Base.h"
#include "Init.h"
#include "OutFile.h"

#include <mono/jit/jit.h>

#include <mono/metadata/object.h>
#include <mono/metadata/reflection.h>
#include <mono/metadata/attrdefs.h>

#include <iostream>

#include <string>
#include <vector>
#include <fstream>

namespace APIBinder {

    MonoDomain* domain = nullptr;

    MonoImage* image = nullptr;
    MonoAssembly* assembly = nullptr;

    MonoClass* nativeClassAttr = nullptr;
    MonoClass* nativeFunctionAttr = nullptr;

    std::ofstream outFile;
    std::vector<MonoClass*> classes;

    void PrepareClasses();
    void BindClass(MonoClass* klass);
    void BindMethod(MonoClass* klass, MonoMethod* method, MonoCustomAttrInfo* attrInfo);

    int Entry() {

        std::cout << "\nRunning Copper Scripting API Binder\n\n";

        // Setup

        domain = InitMono();
        assembly = LoadAssembly(&image);

        nativeClassAttr = mono_class_from_name_case(image, "Copper", "NativeClassAttribute");
        nativeFunctionAttr = mono_class_from_name_case(image, "Copper", "NativeFunctionAttribute");

        BeginOutFile(outFile);

        std::cout << "\n";

        // Run

        PrepareClasses();
        MiddleOutFile(outFile);

        for (MonoClass* klass : classes)
            BindClass(klass);

        // Finish

        FinishOutFile(outFile);

        return 0;

    }

    void PrepareClasses() {

        const MonoTableInfo* typeTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
        uint32 num = mono_table_info_get_rows(typeTable);

        for (uint32 i = 0; i < num; i++) {

            uint32 cols[MONO_TYPEDEF_SIZE];
            mono_metadata_decode_row(typeTable, i, cols, MONO_TYPEDEF_SIZE);

            const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
            const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);
            if (std::string(name) == "<Module>") continue;

            MonoClass* klass = mono_class_from_name_case(image, nameSpace, name);
            if (klass == nullptr) continue;

            MonoCustomAttrInfo* attrInfo = mono_custom_attrs_from_class(klass);
            if (attrInfo == nullptr) continue;
            if (!mono_custom_attrs_has_attr(attrInfo, nativeClassAttr)) continue;

            classes.push_back(klass);

            // Add include line

            MonoObject* attr = mono_custom_attrs_get_attr(attrInfo, nativeClassAttr);
            MonoString* tmp = nullptr;
            mono_field_get_value(attr, mono_class_get_field_from_name(nativeClassAttr, "headerPath"), (void*) &tmp);

            if (tmp == nullptr || mono_string_length(tmp) == 0) return;
            std::string path = mono_string_to_utf8(tmp);

            outFile << "#include \"" << path << "\"\n";

        }

    }
    void BindClass(MonoClass* klass) {

        std::cout << "Binding Class: " << mono_class_get_name(klass) << "\n";

        void* iter = nullptr;
        while (MonoMethod* method = mono_class_get_methods(klass, &iter)) {

            MonoCustomAttrInfo* attrInfo = mono_custom_attrs_from_method(method);
            if (attrInfo == nullptr) continue;
            if (!mono_custom_attrs_has_attr(attrInfo, nativeFunctionAttr)) continue;

            BindMethod(klass, method, attrInfo);

        }

        std::cout << "\n";

    }
    void BindMethod(MonoClass* klass, MonoMethod* method, MonoCustomAttrInfo* attrInfo) {

        std::cout << "Binding Method " << mono_method_get_name(method) << " to ";

        // Get the function name

        MonoObject* attr = mono_custom_attrs_get_attr(attrInfo, nativeFunctionAttr);
        MonoString* tmp = nullptr;
        mono_field_get_value(attr, mono_class_get_field_from_name(nativeFunctionAttr, "name"), &tmp);

        if (tmp == nullptr || mono_string_length(tmp) == 0) return;
        std::string name = mono_string_to_utf8(tmp);

        // Construct full function name

        name.insert(0, "::");
        name.insert(0, mono_class_get_name(klass));

        std::cout << name << "\n";

        // Write to the file

        outFile << Tab(2);
        outFile << "mono_add_internal_call(\"";

        if (const char* nameSpace = mono_class_get_namespace(klass))
            outFile << nameSpace << ".";
        outFile << mono_class_get_name(klass) << "::";
        outFile << mono_method_get_name(method) << "\", ";

        outFile << "(void*) " << name;
        outFile << ");\n";

    }

}

int main() { return APIBinder::Entry(); }
