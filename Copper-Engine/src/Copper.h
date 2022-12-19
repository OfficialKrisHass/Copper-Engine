#pragma once

//-------------------------
//------Copper Engine------
//-------------------------

//Core
#include "Engine/Core/Engine.h"
#include "Engine/Core/Core.h"
#include "Engine/Core/Log.h"
#include "Engine/Core/Window.h"

//Events
#include "Engine/Events/Event.h"
#include "Engine/Events/WindowEvent.h"
#include "Engine/Events/KeyEvent.h"

//Input
#include "Engine/Input/Input.h"
#include "Engine/Input/Error.h"

//Utilities
#include "Engine/Utilities/Wrappers.h"
#include "Engine/Utilities/FileUtils.h"

//Rendering
#include "Engine/Renderer/Texture.h"
#include "Engine/Renderer/Mesh.h"

//-----------------------------------
//------Entity Component System------
//-----------------------------------
#include "CopperECS/CopperECS.h"

//Components
#include "Engine/Components/Transform.h"
#include "Engine/Components/Camera.h"
#include "Engine/Components/Light.h"
#include "Engine/Components/MeshRenderer.h"
#include "Engine/Components/ScriptComponent.h"