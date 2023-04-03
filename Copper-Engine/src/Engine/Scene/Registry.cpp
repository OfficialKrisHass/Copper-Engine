#include "cupch.h"
#include "Registry.h"

namespace Copper {

	EntityEvent entityCreatedEvent;
	EntityEvent entityRemovedEvent;
	ComponentEvent componentAddedEvent;
	ComponentEvent componentRemovedEvent;
	
	void AddEntityCreatedEventFunc(std::function<bool(const Event&)> func) { entityCreatedEvent += func; }
	void AddEntityRemovedEventFunc(std::function<bool(const Event&)> func) { entityRemovedEvent += func; }
	void AddComponentAddedEventFunc(std::function<bool(const Event&)> func) { componentAddedEvent += func; }
	void AddComponentRemovedEventFunc(std::function<bool(const Event&)> func) { componentRemovedEvent += func; }

}