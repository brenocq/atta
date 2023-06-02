//--------------------------------------------------
// Atta Processor Module
// cpu.h
// Date: 2023-02-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PROCESSOR_PROCESSORS_CPU_H
#define ATTA_PROCESSOR_PROCESSORS_CPU_H
#include <atomic>
#include <atta/processor/processors/processor.h>
#include <condition_variable>
#include <mutex>
#include <thread>

namespace atta::processor {

class Cpu : public Processor {
  public:
    Cpu();
    ~Cpu();

    void setNumWorkers(uint32_t numWorkers);
    uint32_t getNumWorkers() const;
    uint32_t getMaxNumWorkers() const;

  private:
    void startThread() override;

    void loop();

    void worker();

    bool hasWork();
    bool jobFinished();
    void stopWorkers();

    // Thread pool
    std::vector<std::thread> _threads;
    bool _stopWorkers; ///< Notify workers to stop

    // Synchronize workers
    std::mutex _jobMutex;                   ///< Mutex to modify job specification
    std::condition_variable _wakeUpWorkers; ///< Wake up all workers

    // Job specification
    std::mutex _workMutex; ///< Mutex to modify job specification
    uint32_t _nextIdx;     ///< Next index to execute
    uint32_t _endIdx;      ///< Final index to execute
    uint32_t _batchSize;   ///< Worker batch size
    std::function<void(uint32_t idx)> _func;
    std::atomic<uint32_t> _busyWorkers;
};

} // namespace atta::processor

#endif // ATTA_PROCESSOR_PROCESSORS_CPU_H
