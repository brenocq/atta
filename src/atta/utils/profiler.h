//--------------------------------------------------
// Atta Utils Module
// profiler.h
// Date: 2022-10-22
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UTILS_PROFILER_H
#define ATTA_UTILS_PROFILER_H
#include <chrono>
#include <atta/utils/stringId.h>

namespace atta {

//---------- Profiler ----------//
class Profiler {
  public:
    using Time = uint64_t;///< Time point in ns
    using ThreadId = uint32_t;///< Thread id
    static constexpr Time ticksPerSecond = 1000*1000*1000;
    struct Record {
        StringId name;///< Function name
        ThreadId threadId;///< Thread Id
        Time begin;///< Start time in ns
        Time end;///< End time in ns
    };

    static Profiler& getInstance();

    static void startRecording();
    static void stopRecording();
    static void clearRecords();

    static void addRecord(Record record);
    static const std::vector<Record>& getRecords();
    static bool isRecording();

    /// Calculate unique names from _records
    static std::vector<StringId> calcNames();
    /// Calculate unique threadIds from _records
    static std::vector<ThreadId> calcThreadIds();
    /// Calculate records separated by name
    static std::vector<std::pair<StringId, std::vector<Record>>> calcRecordsByName(size_t startIdx = 0);
    /// Calculate records separated by threadId
    static std::vector<std::pair<ThreadId, std::vector<Record>>> calcRecordsByThreadId(size_t startIdx = 0);

    /// Get recording start time
    static Time getStart();
    /// Get recording stop time
    static Time getStop();
    /// Get total recording time (stop - start)
    static Time getTotalTime();
    /// Get time string
    static std::string getTimeString(Time time);
    /// Get function color
    static void getFuncColor(StringId name, uint8_t& r, uint8_t& g, uint8_t& b);
    /// Get function name without return type and parameters
    static std::string cropFuncName(std::string name);


  private:
    Profiler() = default;

    std::vector<Record> _records;
    bool _recording;
    Time _start;
    Time _stop;
};

//---------- ProfilerRecord ----------//
class ProfilerRecord {
public:
    ProfilerRecord(StringId name);
    ~ProfilerRecord();

private:
    StringId _name;
    static bool _record;
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
