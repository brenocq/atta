//--------------------------------------------------
// Atta Processor Module
// manager.cpp
// Date: 2023-02-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/event/interface.h>
#include <atta/event/events/simulationContinue.h>
#include <atta/event/events/simulationPause.h>
#include <atta/event/events/simulationStart.h>
#include <atta/event/events/simulationStep.h>
#include <atta/event/events/simulationStop.h>
#include <atta/processor/manager.h>
#include <atta/utils/config.h>

namespace atta::processor {

Manager& Manager::getInstance() {
    static Manager instance;
    return instance;
}

void Manager::startUpImpl() {
    _serial = std::make_shared<Serial>();
    _cpu = std::make_shared<Cpu>();
    _gpu = std::make_shared<Gpu>();
    _processor = _serial;

    // Subscribe to simulation state change events
    event::subscribe<event::SimulationStart>(BIND_EVENT_FUNC(Manager::onSimulationStateChange));
    event::subscribe<event::SimulationStep>(BIND_EVENT_FUNC(Manager::onSimulationStateChange));
    event::subscribe<event::SimulationContinue>(BIND_EVENT_FUNC(Manager::onSimulationStateChange));
    event::subscribe<event::SimulationPause>(BIND_EVENT_FUNC(Manager::onSimulationStateChange));
    event::subscribe<event::SimulationStop>(BIND_EVENT_FUNC(Manager::onSimulationStateChange));
}

void Manager::shutDownImpl() {
    _processor.reset();
    _serial.reset();
    _cpu.reset();
    _gpu.reset();
}

void Manager::setTypeImpl(Type type) {
    switch (type) {
        case Type::SERIAL:
            _processor = _serial;
            LOG_SUCCESS("proc::Manager", "Processor [w]SERIAL[] selected");
            break;
        case Type::CPU:
            _processor = _cpu;
            LOG_SUCCESS("proc::Manager", "Processor [w]CPU[] selected");
            break;
        case Type::GPU:
            _processor = _gpu;
            LOG_SUCCESS("proc::Manager", "Processor [w]GPU[] selected");
            break;
        default:
            LOG_WARN("proc::Manager", "This processor is not supported yet");
    }
}

Type Manager::getTypeImpl() { return _processor->getType(); }
std::shared_ptr<Processor> Manager::getProcessorImpl() { return _processor; }
std::shared_ptr<Serial> Manager::getSerialProcessorImpl() { return _serial; }
std::shared_ptr<Cpu> Manager::getCpuProcessorImpl() { return _cpu; }
std::shared_ptr<Gpu> Manager::getGpuProcessorImpl() { return _gpu; }

void Manager::onSimulationStateChange(event::Event& event) {
    switch (event.getType()) {
        case event::SimulationStart::type: {
            _processor->start();
            LOG_DEBUG("processor::Manager", "Start");
            break;
        }
        case event::SimulationContinue::type: {
            _processor->resume();
            LOG_DEBUG("processor::Manager", "Resume");
            break;
        }
        case event::SimulationPause::type: {
            _processor->pause();
            LOG_DEBUG("processor::Manager", "Pause");
            break;
        }
        case event::SimulationStop::type: {
            // Config::getInstance()._time = 0.0f;
            // Config::getInstance()._realStepSpeed = 0.0f;
            LOG_DEBUG("processor::Manager", "Stop");
            _processor->stop();
            break;
        }
        case event::SimulationStep::type: {
            // if (Config::getState() == Config::State::IDLE) {
            //     event::SimulationStart e;
            //     event::publish(e);
            // }
            // if (Config::getState() == Config::State::RUNNING) {
            //     event::SimulationPause e;
            //     event::publish(e);
            // }
            LOG_WARN("processor::Manager", "Stepping not implemented yet");
            break;
        }
        default: {
            LOG_WARN("processor::Manager", "Unknown simulation event");
        }
    }
}

} // namespace atta::processor
