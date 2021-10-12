//--------------------------------------------------
// Atta
// atta.h
// Date: 2021-08-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_ATTA_H
#define ATTA_ATTA_H
#include <atta/memorySystem/allocators/stackAllocator.h>
#include <atta/eventSystem/eventManager.h>

namespace atta
{
    class Atta
    {
    public:
        struct CreateInfo
        {
            std::filesystem::path projectFile = "";
        };

        Atta(const CreateInfo& info);
        ~Atta();

        void run();

    private:
        enum class SimulationState
        {
            NOT_RUNNING = 0,
            RUNNING,
            PAUSED,
        };

        // Handle events
        void onWindowClose(Event& event);
        void onSimulationStateChange(Event& event);

        // Memory
        StackAllocator* _mainAllocator;

        // State
        bool _shouldFinish;
        SimulationState _simulationState;

        // Fast execution (precompute to execute clone scripts faster)
        //struct FactoryPrecompute
        //{
        //	
        //}
    };
}

#endif// ATTA_ATTA_H
