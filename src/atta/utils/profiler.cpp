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
    std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();
    getInstance()._start = std::chrono::time_point_cast<std::chrono::nanoseconds>(now).time_since_epoch().count();

    clearRecords();
    getInstance()._recording = true;
}

void Profiler::stopRecording() {
    getInstance()._recording = false;

    std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();
    getInstance()._stop = std::chrono::time_point_cast<std::chrono::nanoseconds>(now).time_since_epoch().count();
}

void Profiler::clearRecords() { getInstance()._records.clear(); }

void Profiler::addRecord(Record record) {
    if (!isRecording())
        return;
    getInstance()._records.push_back(record);
}

const std::vector<Profiler::Record>& Profiler::getRecords() { return getInstance()._records; }
bool Profiler::isRecording() { return getInstance()._recording; }

std::vector<StringId> Profiler::calcNames() {
    std::unordered_set<StringId> s;

    const std::vector<Record>& records = getRecords();
    for (const Record& r : records)
        s.insert(r.name);

    return std::vector<StringId>(s.begin(), s.end());
}

std::vector<Profiler::ThreadId> Profiler::calcThreadIds() {
    std::unordered_set<ThreadId> s;

    const std::vector<Record>& records = getRecords();
    for (const Record& r : records)
        s.insert(r.threadId);

    return std::vector<ThreadId>(s.begin(), s.end());
}

std::vector<std::pair<StringId, std::vector<Profiler::Record>>> Profiler::calcRecordsByName(size_t startIdx) {
    std::unordered_map<StringId, std::vector<Record>> m;

    const std::vector<Record>& records = getRecords();
    for (size_t i = startIdx; i < records.size(); i++)
        m[records[i].name].push_back(records[i]);

    return std::vector<std::pair<StringId, std::vector<Profiler::Record>>>(m.begin(), m.end());
}

std::vector<std::pair<Profiler::ThreadId, std::vector<Profiler::Record>>> Profiler::calcRecordsByThreadId(size_t startIdx) {
    std::unordered_map<ThreadId, std::vector<Record>> m;

    const std::vector<Record>& records = getRecords();
    for (size_t i = startIdx; i < records.size(); i++)
        m[records[i].threadId].push_back(records[i]);

    return std::vector<std::pair<ThreadId, std::vector<Record>>>(m.begin(), m.end());
}

Profiler::Time Profiler::getStart() { return getInstance()._start; }

Profiler::Time Profiler::getStop() { return getInstance()._stop; }

Profiler::Time Profiler::getTotalTime() {
    const auto& r = getRecords();
    if (isRecording() && r.size())
        return r.back().end - getStart();
    else
        return getStop() - getStart();
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

void HSVtoRGB(float H, float S, float V, uint8_t& R, uint8_t& G, uint8_t& B) {
    float C = S * V;
    float X = C * (1 - std::abs(std::fmod(H / 60.0, 2) - 1));
    float m = V - C;
    float r, g, b;

    if (H >= 0 && H < 60)
        r = C, g = X, b = 0;
    else if (H >= 60 && H < 120)
        r = X, g = C, b = 0;
    else if (H >= 120 && H < 180)
        r = 0, g = C, b = X;
    else if (H >= 180 && H < 240)
        r = 0, g = X, b = C;
    else if (H >= 240 && H < 300)
        r = X, g = 0, b = C;
    else
        r = C, g = 0, b = X;

    R = (r + m) * 255;
    G = (g + m) * 255;
    B = (b + m) * 255;
}

void Profiler::getFuncColor(StringId name, uint8_t& r, uint8_t& g, uint8_t& b) {
    struct RGB {
        uint8_t r, g, b;
    };
    static std::unordered_map<StringId, RGB> savedColors;
    if (savedColors.find(name) != savedColors.end()) {
        RGB c = savedColors[name];
        r = c.r, g = c.g, b = c.b;
    } else {
        float h = float(name.getId() % 360);
        float s = 0.8f;
        float v = 0.8f;
        HSVtoRGB(h, s, v, r, g, b);
        savedColors[name] = {r, g, b};
    }
}

std::string Profiler::cropFuncName(std::string name) {
    int endPos = 0;
    int startPos = 0;

    for (int i = 0; i < name.size(); i++)
        if (name[i] == '(') {
            endPos = i;
            break;
        }

    for (int i = endPos; i >= 0; i--)
        if (name[i] == ' ') {
            startPos = i + 1;
            break;
        }

    if (startPos && endPos)
        name = name.substr(startPos, endPos - startPos);
    return name;
}

//---------- ProfilerRecord ----------//
bool ProfilerRecord::_record = false;
ProfilerRecord::ProfilerRecord(StringId name) : _name(name) {
    // Handle to start recording at the beginning of atta::Atta::loop
    if (!_record && Profiler::isRecording() && _name == SID("void atta::Atta::loop()"))
        _record = true;

    if (_record)
        _startTime = std::chrono::high_resolution_clock::now();
}

ProfilerRecord::~ProfilerRecord() {
    // Handle to stop recording at the end of atta::Atta::loop
    if (_record && !Profiler::isRecording() && _name == SID("void atta::Atta::loop()"))
        _record = false;

    if (!_record)
        return;

    std::chrono::time_point<std::chrono::high_resolution_clock> endTime = std::chrono::high_resolution_clock::now();

    Profiler::ThreadId threadId = std::hash<std::thread::id>{}(std::this_thread::get_id());
    Profiler::Time begin = std::chrono::time_point_cast<std::chrono::nanoseconds>(_startTime).time_since_epoch().count();
    Profiler::Time end = std::chrono::time_point_cast<std::chrono::nanoseconds>(endTime).time_since_epoch().count();

    Profiler::addRecord({_name, threadId, begin, end});
}

} // namespace atta
