//--------------------------------------------------
// Atta Script Module
// script.h
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_SCRIPT_H
#define ATTA_SCRIPT_SCRIPT_H
#include <atta/component/interface.h>

namespace atta::script {

class Script {
  public:
    Script() = default;
    virtual ~Script() {};
    virtual void update(component::Entity entity, float dt) = 0;
};

} // namespace atta::script

#endif // ATTA_SCRIPT_SCRIPT_H
