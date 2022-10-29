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
}

const std::vector<Profiler::Record>& Profiler::getRecords() { return getInstance()._records; }
bool Profiler::isRecording() { return getInstance()._recording; }

std::vector<const char*> Profiler::calcNames() {
    std::unordered_set<const char*> s;

    const std::vector<Record>& records = getRecords();
    for (const Record& r : records)
        s.insert(r.name);

    return std::vector<const char*>(s.begin(), s.end());
}

std::vector<Profiler::ThreadId> Profiler::calcThreadIds() {
    std::unordered_set<ThreadId> s;

    const std::vector<Record>& records = getRecords();
    for (const Record& r : records)
        s.insert(r.threadId);

    return std::vector<ThreadId>(s.begin(), s.end());
}

std::vector<std::pair<const char*, std::vector<Profiler::Record>>> Profiler::calcRecordsByName(size_t startIdx) {
    std::unordered_map<const char*, std::vector<Record>> m;

    const std::vector<Record>& records = getRecords();
    for (size_t i = startIdx; i < records.size(); i++)
        m[records[i].name].push_back(records[i]);

    return std::vector<std::pair<const char*, std::vector<Profiler::Record>>>(m.begin(), m.end());
}

std::vector<std::pair<Profiler::ThreadId, std::vector<Profiler::Record>>> Profiler::calcRecordsByThreadId(size_t startIdx) {
    std::unordered_map<ThreadId, std::vector<Record>> m;

    const std::vector<Record>& records = getRecords();
    for (size_t i = startIdx; i < records.size(); i++)
        m[records[i].threadId].push_back(records[i]);

    return std::vector<std::pair<ThreadId, std::vector<Record>>>(m.begin(), m.end());
}

Profiler::Time Profiler::getTotalTime() {
    const auto& r = getRecords();
    return r.size() ? r.back().end - r.front().begin : 0;
}

std::string Profiler::getTimeString(Time time) {
    constexpr uint64_t nsToMs = 1000 * 1000;
    constexpr uint64_t nsToS = 1000 * nsToMs;
    constexpr uint64_t nsToM = 60 * nsToS;
    constexpr uint64_t nsToH = 60 * nsToM;
    int h = time / nsToH;
    int m = (time % nsToH) / nsToM;
    int s = (time % nsToM) / nsToS;
    int ms = (time % nsToS) / nsToMs;
    char buff[16];
    snprintf(buff, sizeof(buff), "%02d:%02d:%02d.%03d", h, m, s, ms);
    return std::string(buff);
}

std::string Profiler::cropFuncName(std::string name) {
    int endPos = 0;
    int startPos = 0;
    for (int i = name.size() - 1; i >= 0; i--) {
        if (!endPos && name[i] == '(')
            endPos = i;
        if (endPos && name[i] == ' ') {
            startPos = i + 1;
            break;
        }
    }
    if (startPos && endPos)
        name = name.substr(startPos, endPos - startPos);
    return name;
}

//---------- ProfilerRecord ----------//
ProfilerRecord::ProfilerRecord(const char* name) : _name(name) { _startTime = std::chrono::high_resolution_clock::now(); }
ProfilerRecord::~ProfilerRecord() {
    if (!Profiler::isRecording())
        return;

    std::chrono::time_point<std::chrono::high_resolution_clock> endTime = std::chrono::high_resolution_clock::now();

    Profiler::ThreadId threadId = std::hash<std::thread::id>{}(std::this_thread::get_id());
    Profiler::Time begin = std::chrono::time_point_cast<std::chrono::nanoseconds>(_startTime).time_since_epoch().count();
    Profiler::Time end = std::chrono::time_point_cast<std::chrono::nanoseconds>(endTime).time_since_epoch().count();

    Profiler::addRecord({_name, threadId, begin, end});
}

} // namespace atta
