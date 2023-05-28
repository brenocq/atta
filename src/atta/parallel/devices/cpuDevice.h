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
#include <condition_variable>

namespace atta::parallel {

class CpuDevice : public Device {
  public:
    CpuDevice();
    ~CpuDevice();

    void run(uint32_t start, uint32_t end, std::function<void(uint32_t idx)> func) override;

    /**
     * @brief Run scripts in multiple threads
     *
     * @tparam Script Derived script class
     * @param entity First clone
     * @param dt Simulation dt
     * @param num Number of entities with sequential id to run
     */
    template <typename Script>
    void run(cmp::Entity entity, float dt, uint32_t num);

    void setNumWorkers(uint32_t numWorkers);
    uint32_t getNumWorkers() const;
    uint32_t getMaxNumWorkers() const;

  private:
    void worker();

    bool hasWork();
    bool jobFinished();
    void stopWorkers();

    // Thread pool
    std::vector<std::thread> _threads;
    bool _stopWorkers;///< Notify workers to stop

    // Synchronize workers
    std::mutex _jobMutex;///< Mutex to modify job specification
    std::condition_variable _wakeUpWorkers;///< Wake up all workers

    // Job specification
    std::mutex _workMutex;///< Mutex to modify job specification
    uint32_t _nextIdx;///< Next index to execute
    uint32_t _endIdx;///< Final index to execute
    uint32_t _batchSize;///< Worker batch size
    std::function<void(uint32_t idx)> _func;
    std::atomic<uint32_t> _busyWorkers;
};

} // namespace atta::parallel

#include <atta/parallel/devices/cpuDevice.inl>

#endif // ATTA_PARALLEL_DEVICES_CPU_DEVICE_H
