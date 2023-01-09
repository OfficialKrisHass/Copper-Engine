#pragma once

#include "Engine/Events/Event.h"

#include "CopperECS/Object.h"
#include "CopperECS/Component.h"

namespace Copper {

	class ObjectEvent : public Event {

	public:
		Object* obj;

		virtual std::string ToString() const override {
			
			std::stringstream ss;
			ss << "Object Event: " << obj->GetID() << " (" << obj->tag->name << ")";
			return ss.str();
		
		}

	};

	class ComponentEvent : public Event {

	public:
		Component* component;

		virtual std::string ToString() const override {

			std::stringstream ss;
			ss << "Component Event: " << component->object->GetID() << " (" << component->object->tag->name << ")";
			return ss.str();

		}

	};

}