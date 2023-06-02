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
    _serial = std::make_shared<Serial>();
    _cpu = std::make_shared<Cpu>();
    _gpu = std::make_shared<Gpu>();
    _processor = _serial;
}

void Manager::shutDownImpl() {
    if (_state != State::IDLE)
        stop();

    _processor.reset();
    _serial.reset();
    _cpu.reset();
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
    if (_state != State::IDLE) {
        _processor->stop();
        _processor->readData();
    }

    switch (type) {
        case Type::SERIAL:
            _processor = _serial;
            LOG_SUCCESS("processor::Manager", "Processor [w]SERIAL[] selected");
            break;
        case Type::CPU:
            _processor = _cpu;
            LOG_SUCCESS("processor::Manager", "Processor [w]CPU[] selected");
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
std::shared_ptr<Serial> Manager::getSerialProcessorImpl() { return _serial; }
std::shared_ptr<Cpu> Manager::getCpuProcessorImpl() { return _cpu; }
std::shared_ptr<Gpu> Manager::getGpuProcessorImpl() { return _gpu; }

} // namespace atta::processor
