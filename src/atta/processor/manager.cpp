//--------------------------------------------------
// Atta Processor Module
// manager.cpp
// Date: 2023-02-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/event/interface.h>
#include <atta/processor/manager.h>
#include <atta/utils/config.h>

namespace atta::processor {

Manager& Manager::getInstance() {
    static Manager instance;
    return instance;
}

void Manager::startUpImpl() {
    _cpuSerial = std::make_shared<CpuSerial>();
    _cpuParallel = std::make_shared<CpuParallel>();
    _gpu = std::make_shared<Gpu>();
    _processor = _cpuSerial;
    _dt = 0.015;
}

void Manager::shutDownImpl() {
    if (_state != State::IDLE)
        stop();

    _processor.reset();
    _cpuSerial.reset();
    _cpuParallel.reset();
    _gpu.reset();
}

void Manager::start() {
    if (_state == State::IDLE) {
        LOG_DEBUG("processor::Manager", "Start");
        _processor->start();
        _processor->resume();
        _state = State::RUNNING;
    } else
        LOG_WARN("processor::Manager", "Can't [w]start[] when the simulation is already running");
}

void Manager::pause() {
    if (_state == State::RUNNING) {
        LOG_DEBUG("processor::Manager", "Pause");
        _processor->pause();
        _state = State::PAUSED;
    } else
        LOG_WARN("processor::Manager", "Can't [w]pause[] when the simulation is not running");
}

void Manager::resume() {
    if (_state == State::PAUSED) {
        LOG_DEBUG("processor::Manager", "Continue");
        _processor->resume();
        _state = State::RUNNING;
    } else
        LOG_WARN("processor::Manager", "Can't [w]resume[] when the simulation is not paused");
}

void Manager::stop() {
    if (_state != State::IDLE) {
        LOG_DEBUG("processor::Manager", "Stop");
        _processor->stop();
        _state = State::IDLE;
    } else
        LOG_WARN("processor::Manager", "Can't [w]stop[] when the simulation is not running");
}

void Manager::setTypeImpl(Type type) {
    if (_state != State::IDLE)
        _processor->stop();

    switch (type) {
        case Type::CPU_SERIAL:
            _processor = _cpuSerial;
            LOG_SUCCESS("processor::Manager", "Processor [w]CPU_SERIAL[] selected");
            break;
        case Type::CPU_PARALLEL:
            _processor = _cpuParallel;
            LOG_SUCCESS("processor::Manager", "Processor [w]CPU_PARALLEL[] selected");
            break;
        case Type::GPU:
            _processor = _gpu;
            LOG_SUCCESS("processor::Manager", "Processor [w]GPU[] selected");
            break;
        default:
            LOG_WARN("processor::Manager", "This processor is not supported yet");
    }

    if (_state != State::IDLE)
        _processor->start();
    if (_state == State::RUNNING)
        _processor->resume();
}

Type Manager::getTypeImpl() { return _processor->getType(); }

State Manager::getStateImpl() { return _state; }

std::shared_ptr<Processor> Manager::getProcessorImpl() { return _processor; }
std::shared_ptr<CpuSerial> Manager::getCpuSerialProcessorImpl() { return _cpuSerial; }
std::shared_ptr<CpuParallel> Manager::getCpuParallelProcessorImpl() { return _cpuParallel; }
std::shared_ptr<Gpu> Manager::getGpuProcessorImpl() { return _gpu; }

} // namespace atta::processor
