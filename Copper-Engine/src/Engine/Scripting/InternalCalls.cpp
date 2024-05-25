#include "cupch.h"

#include "Engine/Components/Component.h"

#include "Engine/Scene/CopperECS.h"

#include "Engine/Scripting/MonoUtils.h"
#include "Engine/Scripting/ScriptingEngine.h"

#include <mono/jit/jit.h>
#include <mono/metadata/class.h>

#define INTERNAL_CALL_GROUP(name) namespace InternalCalls::name {
#define INTERNAL_CALL_GROUP_END() }
#define INTERNAL_CALL_CLASS(name, calls) namespace name { calls }

#define ADD_INTERNAL_CALL_GROUP(name) using namespace InternalCalls::name
#define ADD_INTERNAL_CALL(class, name) mono_add_internal_call("Copper."#class"::Internal_"#name, (void*) class::name)
#define ADD_INTERNAL_CALL_RAW(class, name) mono_add_internal_call("Copper."#class"::"#name, (void*) class::name)

namespace Copper::Scripting {

    INTERNAL_CALL_GROUP(Core);

    INTERNAL_CALL_CLASS(Editor,
            void EditorLog(MonoString* msg);
            void EditorLogWarn(MonoString* msg);
            void EditorLogError(MonoString* msg););

    INTERNAL_CALL_GROUP_END();
    


    INTERNAL_CALL_GROUP(Math);
    
    INTERNAL_CALL_CLASS(Quaternion, 
            Vector3 ToEuler(CMath::Quaternion* quat);
            void FromEuler(Vector3 euler, CMath::Quaternion* out););

    INTERNAL_CALL_GROUP_END();


    
    INTERNAL_CALL_GROUP(ECS);

    INTERNAL_CALL_CLASS(Entity, 
            MonoString* get_name(MonoObject* entity);
            void set_name(MonoObject* entity, MonoString* value);

            MonoObject* get_transform(MonoObject* entity); );

    INTERNAL_CALL_GROUP_END();



    INTERNAL_CALL_GROUP(Components);

    INTERNAL_CALL_CLASS(Component,
            MonoObject* get_entity(MonoObject* component);
            MonoObject* get_transform(MonoObject* component););

    INTERNAL_CALL_CLASS(Transform, 
            Vector3 get_position(MonoObject* transform);
            void set_position(MonoObject* transform, Vector3 value);

            void GetRotation(MonoObject* transform, Quaternion* ret);
            void SetRotation(MonoObject* transform, Quaternion* value);

            Vector3 get_scale(MonoObject* transform);
            void set_scale(MonoObject* transform, Vector3 value);

            Vector3 get_globalPosition(MonoObject* transform);
            void set_globalPosition(MonoObject* transform, Vector3 value);

            void GetGlobalRotation(MonoObject* transform, Quaternion* ret);
            void SetGlobalRotation(MonoObject* transform, Quaternion* value);

            Vector3 get_globalScale(MonoObject* transform);
            void set_globalScale(MonoObject* transform, Vector3 value);

            Vector3 get_forward(MonoObject* transform);
            Vector3 get_right(MonoObject* transform);
            Vector3 get_up(MonoObject* transform););

    INTERNAL_CALL_GROUP_END();

    void SetupInternalCalls() {

        CUP_FUNCTION();

        ADD_INTERNAL_CALL_GROUP(Core);
        ADD_INTERNAL_CALL_GROUP(Math);
        ADD_INTERNAL_CALL_GROUP(ECS);
        ADD_INTERNAL_CALL_GROUP(Components);

        // Editor

        ADD_INTERNAL_CALL(Editor, EditorLog);
        ADD_INTERNAL_CALL(Editor, EditorLogWarn);
        ADD_INTERNAL_CALL(Editor, EditorLogError);

        // Quaternion
        
        ADD_INTERNAL_CALL(Quaternion, ToEuler);
        ADD_INTERNAL_CALL(Quaternion, FromEuler);

        // Entity

        ADD_INTERNAL_CALL_RAW(Entity, get_name);
        ADD_INTERNAL_CALL_RAW(Entity, set_name);
        
        ADD_INTERNAL_CALL_RAW(Entity, get_transform);

        // Component

        ADD_INTERNAL_CALL_RAW(Component, get_entity);
        ADD_INTERNAL_CALL_RAW(Component, get_transform);

        // Transform

        ADD_INTERNAL_CALL_RAW(Transform, get_position);
        ADD_INTERNAL_CALL_RAW(Transform, set_position);
        
        ADD_INTERNAL_CALL(Transform, GetRotation);
        ADD_INTERNAL_CALL(Transform, SetRotation);

        ADD_INTERNAL_CALL_RAW(Transform, get_scale);
        ADD_INTERNAL_CALL_RAW(Transform, set_scale);

        ADD_INTERNAL_CALL_RAW(Transform, get_globalPosition);
        ADD_INTERNAL_CALL_RAW(Transform, set_globalPosition);

        ADD_INTERNAL_CALL(Transform, GetGlobalRotation);
        ADD_INTERNAL_CALL(Transform, SetGlobalRotation);
        
        ADD_INTERNAL_CALL_RAW(Transform, get_globalScale);
        ADD_INTERNAL_CALL_RAW(Transform, set_globalScale);

        ADD_INTERNAL_CALL_RAW(Transform, get_forward);
        ADD_INTERNAL_CALL_RAW(Transform, get_right);
        ADD_INTERNAL_CALL_RAW(Transform, get_up);

    }

}
