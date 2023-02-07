//--------------------------------------------------
// Atta Parallel Module
// cpuDevice.h
// Date: 2023-02-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PARALLEL_DEVICES_CPU_DEVICE_H
#define ATTA_PARALLEL_DEVICES_CPU_DEVICE_H

#include <atta/parallel/devices/device.h>
#include <thread>
#include <atomic>
#include <mutex>

namespace atta::parallel {

class CpuDevice : public Device {
  public:
    CpuDevice();
    ~CpuDevice();

    void parallelFor(uint32_t start, uint32_t end, std::function<void(uint32_t idx)> func) override;

  private:
    void worker();
    bool hasWork();

    // Thread pool
    std::vector<std::thread> _threads;
    std::mutex _mutex;///< Mutex to modify job specification
    bool _stopThreads;///< Notify threads to stop

    // Job specification
    uint32_t _nextIdx;///< Next index to execute
    uint32_t _endIdx;///< Final index to execute
    uint32_t _batchSize;///< Worker batch size
    std::function<void(uint32_t idx)> _func;
    std::atomic<uint32_t> _busyWorkers;
};

} // namespace atta::parallel

#endif // ATTA_PARALLEL_DEVICES_CPU_DEVICE_H
