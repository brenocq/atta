//--------------------------------------------------
// Atta Utils Module
// profiler.h
// Date: 2022-10-22
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UTILS_PROFILER_H
#define ATTA_UTILS_PROFILER_H
#include <chrono>

namespace atta {

//---------- Profiler ----------//
class Profiler {
  public:
    struct Record {
        const char* name;///< Function name
        uint32_t threadId;///< Thread Id
        uint64_t begin;///< Start time in ns
        uint64_t end;///< End time in ns
    };

    static Profiler& getInstance();

    static void startRecording();
    static void stopRecording();
    static void clearRecords();

    static void addRecord(Record record);
    static const std::vector<Record>& getRecords();
    static bool isRecording();

  private:
    Profiler() = default;

    std::vector<Record> _records;
    bool _recording;
};

//---------- ProfilerRecord ----------//
class ProfilerRecord {
public:
    ProfilerRecord(const char* name);
    ~ProfilerRecord();

private:
    const char* _name;
    std::chrono::time_point<std::chrono::high_resolution_clock> _startTime;
};

} // namespace atta

//---------- Macros ----------//
#ifdef ATTA_PROFILE
#define PROFILE_NAME(name) ProfilerRecord profilerRecord##__LINE__(name)
#else
#define PROFILE_NAME(name)
#endif

#define PROFILE() PROFILE_NAME(__PRETTY_FUNCTION__)

#endif // ATTA_UTILS_PROFILER_H
