//--------------------------------------------------
// Atta
// atta.h
// Date: 2021-08-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_ATTA_H
#define ATTA_ATTA_H
#include <atta/event/event.h>
#include <atta/memory/allocators/stackAllocator.h>
#include <chrono>

namespace atta {

class Atta {
  public:
    struct CreateInfo {
        std::filesystem::path projectFile = "";
    };

    Atta(const CreateInfo& info);
    ~Atta();

    void run();
    void loop();
    void step();

  private:
    // Handle events
    void onWindowClose(event::Event& event);
    void onSimulationStateChange(event::Event& event);

    // Memory
    memory::StackAllocator* _mainAllocator;

    // State
    bool _shouldFinish;
    bool _shouldStep;

    // Track real time
    std::chrono::time_point<std::chrono::high_resolution_clock> _lastStep;
    std::chrono::time_point<std::chrono::high_resolution_clock> _currStep;
};

} // namespace atta

#endif // ATTA_ATTA_H
