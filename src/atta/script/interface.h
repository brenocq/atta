//--------------------------------------------------
// Atta Script Module
// interface.h
// Date: 2022-08-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_INTERFACE_H
#define ATTA_SCRIPT_INTERFACE_H

#include <atta/script/registry/controllerRegistry.h>
#include <atta/script/registry/worldRegistry.h>
#include <atta/script/scripts/controller.h>
#include <atta/script/scripts/world.h>

namespace atta::script {

void startUp();
void shutDown();

void update(float dt);

std::vector<StringId> getScriptSids();

} // namespace atta::script

#endif // ATTA_SCRIPT_INTERFACE_H
