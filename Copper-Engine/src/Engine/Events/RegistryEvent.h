#pragma once

#include "Engine/Events/Event.h"

#include "Engine/Scene/Entity.h"

namespace Copper {

	class EntityEvent : public Event {

	public:
		Entity entity;

	};

	class ComponentEvent : public Event {

	public:
		class Component* component = nullptr;
        int32 componentID = 0;

	};

}
