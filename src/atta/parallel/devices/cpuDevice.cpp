//--------------------------------------------------
// Atta Parallel Module
// cpuDevice.cpp
// Date: 2023-02-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/parallel/devices/cpuDevice.h>

namespace atta::parallel {

CpuDevice::CpuDevice() : Device(Type::CPU), _stopWorkers(false), _nextIdx(1), _endIdx(0), _batchSize(16), _busyWorkers(0) {
    // Create workers
    for (int i = 0; i < getMaxNumWorkers(); i++)
        _threads.push_back(std::thread(&CpuDevice::worker, this));
}

CpuDevice::~CpuDevice() { stopWorkers(); }

void CpuDevice::compute(uint32_t start, uint32_t end, std::function<void(uint32_t idx)> func) {
    // If no work to be done, return
    if (start == end)
        return;

    {
        std::lock_guard<std::mutex> lock(_workMutex);
        _nextIdx = start;
        _endIdx = end - 1;
        _func = std::move(func);
    }
    _wakeUpWorkers.notify_all();

    // Wait for job to finish
    while (!jobFinished()) {
    }
}

void CpuDevice::setNumWorkers(uint32_t numWorkers) {
    if (!jobFinished()) {
        LOG_WARN("parallel::CpuDevice", "Can't change number of workers while they are working");
        return;
    }
    if (numWorkers > getMaxNumWorkers()) {
        LOG_WARN("parallel::CpuDevice", "Can't change number of workers to [w]$0[], maximum is [w]$1[]", numWorkers, getMaxNumWorkers());
        return;
    }

    // Stop workers
    stopWorkers();

    // Launch new workers
    for (uint32_t i = 0; i < numWorkers; i++)
        _threads.push_back(std::thread(&CpuDevice::worker, this));
}

uint32_t CpuDevice::getNumWorkers() const { return _threads.size(); }

uint32_t CpuDevice::getMaxNumWorkers() const { return std::thread::hardware_concurrency() - 1; }

void CpuDevice::worker() {
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

bool CpuDevice::hasWork() { return _nextIdx <= _endIdx; }

bool CpuDevice::jobFinished() { return !hasWork() && _busyWorkers == 0; }

void CpuDevice::stopWorkers() {
    _stopWorkers = true;
    _wakeUpWorkers.notify_all();
    for (auto& t : _threads)
        t.join();
    _threads.clear();
    _stopWorkers = false;
}

} // namespace atta::parallel
