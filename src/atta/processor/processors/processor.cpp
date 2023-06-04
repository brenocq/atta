//--------------------------------------------------
// Atta Processor Module
// processor.cpp
// Date: 2023-02-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/processor/processors/processor.h>

namespace atta::processor {

Processor::Processor(Type type) : _type(type), _pause(true), _stepCount(0) {}

Type Processor::getType() const { return _type; }

void Processor::start() {
    writeData();
    _stop = false;
    _pause = true;
    startThread();
}

void Processor::pause() {
    std::unique_lock<std::mutex> lock(_mtx);
    // Ask to pause
    _pause = true;
    // Wait until paused
    _cvWasPaused.wait(lock, [] { return true; });
    readData();
}

void Processor::resume() {
    writeData();
    // Ask to resume
    std::lock_guard<std::mutex> lock(_mtx);
    _pause = false;
    _cvPause.notify_one();
}

void Processor::stop() {
    _stop = true;
    if (_pause)
        resume();
    _thread.join();
    readData();
}

void Processor::readData() {}

void Processor::writeData() {}

bool Processor::shouldRun() {
    std::unique_lock<std::mutex> lock(_mtx);
    if (_pause) {
        _cvWasPaused.notify_one();                       // Notify it was paused
        _cvPause.wait(lock, [this] { return !_pause; }); // Wait until not paused
    }
    return !_stop;
}

} // namespace atta::processor
