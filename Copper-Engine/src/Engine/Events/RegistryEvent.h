#pragma once

#include "Engine/Events/Event.h"

#include "Engine/Scene/Component.h"
#include "Engine/Scene/Entity.h"

namespace Copper {

	class EntityEvent : public Event {

	public:
		Entity entity;

	};

	class ComponentEvent : public Event {

	public:
		Component* component;

	};

}