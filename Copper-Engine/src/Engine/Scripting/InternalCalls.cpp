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
            void EditorLog(MonoString*);
            void EditorLogWarn(MonoString*);
            void EditorLogError(MonoString*); );

    INTERNAL_CALL_GROUP_END();
    


    INTERNAL_CALL_GROUP(Math);
    
    INTERNAL_CALL_CLASS(Quaternion, 
            Vector3 ToEuler(CMath::Quaternion*);
            void FromEuler(Vector3, CMath::Quaternion*); );

    INTERNAL_CALL_GROUP_END();


    
    INTERNAL_CALL_GROUP(ECS);

    INTERNAL_CALL_CLASS(Entity, 
            MonoString* get_name(MonoObject*);
            void set_name(MonoObject*, MonoString*);

            MonoObject* get_transform(MonoObject*);

            MonoObject* GetComponent(MonoObject*, MonoReflectionType*);
            bool HasComponent(MonoObject*, MonoReflectionType*); );

    INTERNAL_CALL_GROUP_END();



    INTERNAL_CALL_GROUP(Components);

    INTERNAL_CALL_CLASS(Component,
            MonoObject* get_entity(MonoObject*);
            MonoObject* get_transform(MonoObject*); );

    INTERNAL_CALL_CLASS(Transform, 
            Vector3 get_position(MonoObject*);
            void set_position(MonoObject*, Vector3);

            void GetRotation(MonoObject*, Quaternion*);
            void SetRotation(MonoObject*, Quaternion*);

            Vector3 get_scale(MonoObject*);
            void set_scale(MonoObject*, Vector3);

            Vector3 get_globalPosition(MonoObject*);
            void set_globalPosition(MonoObject*, Vector3);

            void GetGlobalRotation(MonoObject*, Quaternion*);
            void SetGlobalRotation(MonoObject*, Quaternion*);

            Vector3 get_globalScale(MonoObject*);
            void set_globalScale(MonoObject*, Vector3);

            Vector3 get_forward(MonoObject*);
            Vector3 get_right(MonoObject*);
            Vector3 get_up(MonoObject*); );

    INTERNAL_CALL_CLASS(Camera, 
            float get_fov(MonoObject*);
            void set_fov(MonoObject*, float);

            float get_nearPlane(MonoObject*);
            void set_nearPlane(MonoObject*, float);
            float get_farPlane(MonoObject*);
            void set_farPlane(MonoObject*, float); );

    INTERNAL_CALL_CLASS(Light,
            Color get_color(MonoObject*);
            void set_color(MonoObject*, Color);
            uint32 get_type(MonoObject*);
            void set_type(MonoObject*, uint32);

            float get_intensity(MonoObject*);
            void set_intensity(MonoObject*, float); );

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

        ADD_INTERNAL_CALL(Entity, HasComponent);
        ADD_INTERNAL_CALL(Entity, GetComponent);

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

        // Camera

        ADD_INTERNAL_CALL_RAW(Camera, get_fov);
        ADD_INTERNAL_CALL_RAW(Camera, set_fov);

        ADD_INTERNAL_CALL_RAW(Camera, get_nearPlane);
        ADD_INTERNAL_CALL_RAW(Camera, set_nearPlane);
        ADD_INTERNAL_CALL_RAW(Camera, get_farPlane);
        ADD_INTERNAL_CALL_RAW(Camera, set_farPlane);
        
        // Light

        ADD_INTERNAL_CALL_RAW(Light, get_color);
        ADD_INTERNAL_CALL_RAW(Light, set_color);
        ADD_INTERNAL_CALL_RAW(Light, get_type);
        ADD_INTERNAL_CALL_RAW(Light, set_type);

        ADD_INTERNAL_CALL_RAW(Light, get_intensity);
        ADD_INTERNAL_CALL_RAW(Light, set_intensity);

    }

}
