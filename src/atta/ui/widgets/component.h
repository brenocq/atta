//--------------------------------------------------
// Atta UI Module
// component.h
// Date: 2025-02-26
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_WIDGETS_COMPONENT_H
#define ATTA_UI_WIDGETS_COMPONENT_H

#include <atta/component/base.h>
#include <atta/component/components/component.h>
#include <atta/component/entity.h>

namespace atta::ui {

void componentWidget(cmp::Entity entity, cmp::ComponentId cid, cmp::Component* comp);

void renderAttributes(const std::vector<cmp::AttributeDescription>& aDescs, cmp::Component* comp, unsigned compSizeOf);
void renderAttribute(cmp::AttributeDescription aDesc, void* d, unsigned size);

} // namespace atta::ui

#endif // ATTA_UI_WIDGETS_COMPONENT_H
