//--------------------------------------------------
// Atta Processor Module
// processor.h
// Date: 2023-02-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PROCESSOR_PROCESSORS_PROCESSOR_H
#define ATTA_PROCESSOR_PROCESSORS_PROCESSOR_H
#include <atomic>
#include <atta/processor/base.h>
#include <condition_variable>
#include <mutex>
#include <thread>

namespace atta::processor {

class Processor {
  public:
    Processor(Type type);

    Type getType() const;
    size_t getStepCount() const;

    virtual void start();
    virtual void pause();
    virtual void resume();
    virtual void stop();

    virtual void readData();
    virtual void writeData();

  protected:
    virtual void startThread() = 0;
    bool shouldRun();

    const Type _type;
    size_t _stepCount; ///< Current step count when executing

    std::thread _thread;

  private:
    // Handle pause and resume
    std::mutex _mtx;
    std::condition_variable _cvPause;     ///< Condition variable to notify that thread was paused
    std::condition_variable _cvWasPaused; ///< Condition variable to notify that thread was resumes
    bool _pause;                          ///< Signal to pause thread

    // Handle stop
    std::atomic<bool> _stop; ///< Flag to ask thread to stop
};

} // namespace atta::processor

#endif // ATTA_PROCESSOR_PROCESSORS_PROCESSOR_H
