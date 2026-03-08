// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

#include <atta/component/base.h>
#include <atta/component/components/component.h>
#include <atta/component/entity.h>

namespace atta::ui {

void componentWidget(cmp::Entity entity, cmp::ComponentId cid, cmp::Component* comp);

void renderAttributes(const std::vector<cmp::AttributeDescription>& aDescs, cmp::Component* comp, unsigned compSizeOf);
void renderAttribute(cmp::AttributeDescription aDesc, void* d, unsigned size);

} // namespace atta::ui
