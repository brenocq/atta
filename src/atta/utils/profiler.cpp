//--------------------------------------------------
// Atta Utils Module
// profiler.cpp
// Date: 2022-10-22
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/utils/profiler.h>
#include <thread>

namespace atta {

//---------- Profiler ----------//
Profiler& Profiler::getInstance() {
    static Profiler instance{};
    return instance;
}

void Profiler::startRecording() {
    clearRecords();
    getInstance()._recording = true;
}
void Profiler::stopRecording() { getInstance()._recording = false; }

void Profiler::clearRecords() { getInstance()._records.clear(); }

void Profiler::addRecord(Record record) {
    if (!isRecording())
        return;
    getInstance()._records.push_back(record);
    //LOG_DEBUG("Profiler", "Record [w]$2[] from [*y]$0[] to [*y]$1[]", record.begin, record.end, record.name);
}

const std::vector<Profiler::Record>& Profiler::getRecords() { return getInstance()._records; }
bool Profiler::isRecording() { return getInstance()._recording; }

//---------- ProfilerRecord ----------//
ProfilerRecord::ProfilerRecord(const char* name) : _name(name) { _startTime = std::chrono::high_resolution_clock::now(); }
ProfilerRecord::~ProfilerRecord() {
    if (!Profiler::isRecording())
        return;

    std::chrono::time_point<std::chrono::high_resolution_clock> endTime = std::chrono::high_resolution_clock::now();

    uint32_t threadId = std::hash<std::thread::id>{}(std::this_thread::get_id());
    uint64_t begin = std::chrono::time_point_cast<std::chrono::nanoseconds>(_startTime).time_since_epoch().count();
    uint64_t end = std::chrono::time_point_cast<std::chrono::nanoseconds>(endTime).time_since_epoch().count();

    Profiler::addRecord({_name, threadId, begin, end});
}

} // namespace atta
