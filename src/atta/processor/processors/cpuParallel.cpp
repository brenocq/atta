//--------------------------------------------------
// Atta Processor Module
// cpuParallel.cpp
// Date: 2023-02-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/processor/processors/cpuParallel.h>

namespace atta::processor {

CpuParallel::CpuParallel() : Processor(Type::CPU_PARALLEL), _stopWorkers(false), _nextIdx(1), _endIdx(0), _batchSize(16), _busyWorkers(0) {
    // Create workers
    for (int i = 0; i < getMaxNumWorkers(); i++)
        _threads.push_back(std::thread(&CpuParallel::worker, this));
}

CpuParallel::~CpuParallel() { stopWorkers(); }

void CpuParallel::startThread() { _thread = std::thread(&CpuParallel::loop, this); }

void CpuParallel::loop() {
    while (shouldRun()) {
        _stepCount++;
    }
    _stepCount = 0;
}

// void CpuParallel::run(uint32_t start, uint32_t end, std::function<void(uint32_t idx)> func) {
//     // If no work to be done, return
//     if (start == end)
//         return;
//
//     {
//         std::lock_guard<std::mutex> lock(_workMutex);
//         _nextIdx = start;
//         _endIdx = end - 1;
//         _func = std::move(func);
//     }
//     _wakeUpWorkers.notify_all();
//
//     // Wait for job to finish
//     while (!jobFinished()) {
//     }
// }

void CpuParallel::setNumWorkers(uint32_t numWorkers) {
    if (!jobFinished()) {
        LOG_WARN("processor::CpuParallel", "Can't change number of workers while they are working");
        return;
    }
    if (numWorkers > getMaxNumWorkers()) {
        LOG_WARN("processor::CpuParallel", "Can't change number of workers to [w]$0[], maximum is [w]$1[]", numWorkers, getMaxNumWorkers());
        return;
    }

    // Stop workers
    stopWorkers();

    // Launch new workers
    for (uint32_t i = 0; i < numWorkers; i++)
        _threads.push_back(std::thread(&CpuParallel::worker, this));
}

uint32_t CpuParallel::getNumWorkers() const { return _threads.size(); }

uint32_t CpuParallel::getMaxNumWorkers() const { return std::thread::hardware_concurrency() - 1; }

void CpuParallel::worker() {
    while (!_stopWorkers) {
        // Work
        if (hasWork()) {
            _busyWorkers++;
            while (hasWork()) {
                uint32_t start;
                uint32_t end;

                // Get work
                {
                    std::lock_guard<std::mutex> lock(_workMutex);
                    if (!hasWork())
                        break;
                    start = _nextIdx;
                    end = std::min(_endIdx, _nextIdx + _batchSize);
                    _nextIdx = end + 1;
                }

                // Do work
                for (uint32_t i = start; i <= end; i++)
                    _func(i);
            }
            _busyWorkers--;
        }

        // Sleep
        {
            std::unique_lock lock(_jobMutex);
            _wakeUpWorkers.wait(lock);
        }
    }
}

bool CpuParallel::hasWork() { return _nextIdx <= _endIdx; }

bool CpuParallel::jobFinished() { return !hasWork() && _busyWorkers == 0; }

void CpuParallel::stopWorkers() {
    _stopWorkers = true;
    _wakeUpWorkers.notify_all();
    for (auto& t : _threads)
        t.join();
    _threads.clear();
    _stopWorkers = false;
}

} // namespace atta::processor
