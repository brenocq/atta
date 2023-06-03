//--------------------------------------------------
// Atta Script Module
// world.h
// Date: 2023-05-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_SCRIPTS_WORLD_H
#define ATTA_SCRIPT_SCRIPTS_WORLD_H

namespace atta::script {

class World {
  public:
    ATTA_CPU_GPU virtual void onStart() = 0;
    ATTA_CPU_GPU virtual void onStop() = 0;

    ATTA_CPU_GPU virtual void onUpdateBefore() = 0;
    ATTA_CPU_GPU virtual void onUpdateAfter() = 0;
};

} // namespace atta::script

#endif // ATTA_SCRIPT_SCRIPTS_WORLD_H
