#pragma once

//-------------------------
//------Copper Engine------
//-------------------------

//Core
#include "Engine/Core/Core.h"
#include "Engine/Core/Engine.h"
#include "Engine/Core/Log.h"

//Events
#include "Engine/Events/Event.h"
#include "Engine/Events/WindowEvent.h"
#include "Engine/Events/KeyEvent.h"
#include "Engine/Events/RegistryEvent.h"

//Input
#include "Engine/Input/Input.h"
#include "Engine/Input/AxisManager.h"
#include "Engine/Input/Popup.h"
#include "Engine/Input/KeyCodes.h"

//Utilities
#include "Engine/Utilities/FileUtils.h"

//Rendering
#include "Engine/Renderer/Texture.h"
#include "Engine/Renderer/Mesh.h"

//-----------------------------------
//------Entity Component System------
//-----------------------------------
#include "Engine/Scene/CopperECS.h"

//Components
#include "Engine/Components/Transform.h"
#include "Engine/Components/Camera.h"
#include "Engine/Components/Light.h"
#include "Engine/Components/MeshRenderer.h"
#include "Engine/Components/ScriptComponent.h"
#include "Engine/Components/RigidBody.h"
#include "Engine/Components/Collider.h"
#include "Engine/Components/BoxCollider.h"
#include "Engine/Components/SphereCollider.h"
#include "Engine/Components/CapsuleCollider.h"

//-------------------------
//------Miscellaneous------
//-------------------------

//YAML Overloads
#include "Engine/YAMLOverloads/Everything.h"