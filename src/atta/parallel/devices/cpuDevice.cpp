//--------------------------------------------------
// Atta Parallel Module
// cpuDevice.cpp
// Date: 2023-02-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/parallel/devices/cpuDevice.h>

namespace atta::parallel {

CpuDevice::CpuDevice() : Device(Type::CPU), _stopThreads(false), _nextIdx(1), _endIdx(0), _batchSize(1), _busyWorkers(0) {
    const uint32_t numProcessors = std::thread::hardware_concurrency();

    // Create one thread for each processor
    for (int i = 0; i < numProcessors; i++)
        _threads.push_back(std::thread(&CpuDevice::worker, this));
}
CpuDevice::~CpuDevice() {
    _stopThreads = true;
    for (auto& t : _threads)
        t.join();
    _threads.clear();
}

void CpuDevice::parallelFor(uint32_t start, uint32_t end, std::function<void(uint32_t idx)> func) {
    // If no work to be done, return
    if (start == end)
        return;

    {
        std::lock_guard<std::mutex> lock(_mutex);
        _nextIdx = start;
        _endIdx = end - 1;
        _func = std::move(func);
    }

    // Wait for job to finish
    while (hasWork() || _busyWorkers > 0) {
    }
}

void CpuDevice::worker() {
    while (!_stopThreads) {
        if (hasWork()) {
            _busyWorkers++;
            while (hasWork()) {
                uint32_t start;
                uint32_t end;

                // Get work
                {
                    std::lock_guard<std::mutex> lock(_mutex);
                    if(!hasWork())
                        break;
                    start = _nextIdx;
                    end = std::min(_endIdx, _nextIdx + _batchSize);
                    _nextIdx = end + 1;
                }

                // Run work
                for (uint32_t i = start; i <= end; i++)
                    _func(i);
            }
            _busyWorkers--;
        }
    }
}

bool CpuDevice::hasWork() { return _nextIdx <= _endIdx; }

} // namespace atta::parallel
