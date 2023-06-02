//--------------------------------------------------
// Atta Processor Module
// serial.h
// Date: 2023-02-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PARALLEL_PROCESSORS_SERIAL_H
#define ATTA_PARALLEL_PROCESSORS_SERIAL_H
#include <atomic>
#include <atta/processor/processors/processor.h>
#include <condition_variable>
#include <mutex>
#include <thread>

namespace atta::processor {

class Serial : public Processor {
  public:
    Serial();
    ~Serial();

    void start() override;
    void pause() override;
    void resume() override;
    void stop() override;

  private:
    void loop();

    std::thread _thread;

    // Handle pause and resume
    std::mutex _mtx;
    std::condition_variable _cvPause;     ///< Condition variable to notify that thread was paused
    std::condition_variable _cvWasPaused; ///< Condition variable to notify that thread was resumes
    bool _pause;                          ///< Signal to pause thread

    // Handle stop
    std::atomic<bool> _stop; ///< Flag to ask thread to stop
};

} // namespace atta::processor

#endif // ATTA_PARALLEL_PROCESSORS_SERIAL_H
