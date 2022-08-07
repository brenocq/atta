//--------------------------------------------------
// Atta Event Module
// createComponent.h
// Date: 2022-03-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_CREATE_COMPONENT_H
#define ATTA_EVENT_EVENTS_CREATE_COMPONENT_H
#include <atta/component/manager.h>
#include <atta/event/event.h>

namespace atta::event {

class CreateComponent : public EventTyped<SID("CreateComponent")> {
  public:
    component::ComponentId componentId;
    component::EntityId entityId;
    component::Component* component;
};

} // namespace atta::event
#endif // ATTA_EVENT_EVENTS_CREATE_COMPONENT_H
