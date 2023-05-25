//--------------------------------------------------
// Atta Script Module
// manager.cpp
// Date: 2022-08-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/script/interface.h>
#include <atta/script/manager.h>

namespace atta::script {

void startUp() { Manager::getInstance().startUpImpl(); }

void shutDown() { Manager::getInstance().shutDownImpl(); }

void update(float dt) {
    PROFILE();
    Manager::getInstance().updateImpl(dt);
}

std::vector<StringId> getScriptSids() { return Manager::getInstance().getScriptSidsImpl(); }

} // namespace atta::script
