//--------------------------------------------------
// Atta Script Module
// linuxCompiler.h
// Date: 2021-09-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifdef ATTA_OS_LINUX

#include <atta/file/manager.h>
#include <atta/script/compilers/linuxCompiler.h>

#include <chrono>

namespace atta::script {

LinuxCompiler::LinuxCompiler() : _compiler("g++") {
    // Prefer to use clang++ because it is faster
    if (std::system("clang++ 2> /dev/null") == 0)
        _compiler = "clang++";
}

LinuxCompiler::~LinuxCompiler() {}

void LinuxCompiler::compileAll() {
    std::chrono::time_point<std::chrono::system_clock> begin = std::chrono::system_clock::now();

    fs::path projectDir = file::getProject()->getDirectory();
    fs::path buildDir = projectDir / "build";

    // Create build directory if does not exists
    if (!fs::exists(buildDir))
        fs::create_directory(buildDir);

    // Create makefiles
    fs::path prevPath = fs::current_path();
    fs::current_path(buildDir);

#ifdef ATTA_DEBUG_BUILD
    std::string buildCommand = "cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=" + _compiler + " ..";
#else
    std::string buildCommand = "cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=" + _compiler + " ..";
#endif
    runCommand(buildCommand, true, true);

    // Build makefile
    std::string makeCommand = "make -j";
    runCommand(makeCommand, true, true);
    fs::current_path(prevPath);

    // Update files related to all targets
    updateTargets();

    // Show time
    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    auto micro = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
    LOG_INFO("script::LinuxCompiler", "Time to compile all: [w]$0[] ms", micro.count() / 1000.0f);
}

void LinuxCompiler::compileTarget(StringId target) {
    std::chrono::time_point<std::chrono::system_clock> begin = std::chrono::system_clock::now();

    //  Check target
    if (_targetFiles.find(target) == _targetFiles.end()) {
        LOG_WARN("script::LinuxCompiler", "Could not find target $0", target);
        return;
    }

    fs::path projectDir = file::getProject()->getDirectory();
    fs::path buildDir = projectDir / "build";

    // Compile all if never compiled
    if (!fs::exists(buildDir)) {
        compileAll();
        return;
    }

    // Compile target
    fs::path prevPath = fs::current_path();
    fs::current_path(buildDir);
    std::string command = "cmake --build . --parallel --target " + target.getString();
    std::string output = runCommand(command, true, true);
    fs::current_path(prevPath);

    if (output.find("Error: could not load cache") != std::string::npos) {
        // Empty build directory, should recompile all targets
        compileAll();
        return;
    }

    // Update files related to this target
    findTargetFiles(target);

    // Show time
    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    auto micro = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
    LOG_INFO("script::LinuxCompiler", "Time to compile target [*w]$1[]: [w]$0[] ms", micro.count() / 1000.0f, target);
}

void LinuxCompiler::updateTargets() {
    fs::path projectDir = file::getProject()->getDirectory();
    fs::path buildDir = projectDir / "build";
    fs::path tempFile = buildDir / "atta.temp";

    //---------- Get targets ----------//
    _targetFiles.clear();
    std::string getTargetCommand = "cmake --build . --target help > " + fs::absolute(tempFile).string();
    fs::path prevPath = fs::current_path();
    fs::current_path(buildDir);
    std::system(getTargetCommand.c_str());
    fs::current_path(prevPath);

    std::ifstream tempIn(tempFile.string());
    std::string line;

    bool isTarget = false;
    while (std::getline(tempIn, line)) {
        if (line == "... rebuild_cache") {
            isTarget = true;
            continue;
        }

        if (line.find(".o") != std::string::npos || line.find("cmake_pch") != std::string::npos) {
            isTarget = false;
            continue;
        }

        size_t space = line.find(' ');
        // Find files related to this target
        if (isTarget) {
            StringId target = StringId(line.substr(space + 1));
            findTargetFiles(target);
        }
    }
    tempIn.close();
}

void LinuxCompiler::findTargetFiles(StringId target) {
    fs::path projectDir = file::getProject()->getDirectory();
    fs::path buildDir = projectDir / "build";
    fs::path dependFile = buildDir / "CMakeFiles" / (target.getString() + ".dir").c_str() / "DependInfo.cmake";
    std::vector<fs::path> cmakeTargetFiles;

    // Get list of files from cmake
    std::ifstream dependIn(dependFile);
    std::string line;
    while (std::getline(dependIn, line)) {
        if (line.find("CMAKE_DEPENDS_DEPENDENCY_FILES") != std::string::npos) {
            std::getline(dependIn, line);
            // Line now has a list of string with the name of each file that this target depends on

            size_t end = -1;
            while (true) {
                size_t start = line.find("\"", end + 1);
                if (start == std::string::npos) {
                    std::getline(dependIn, line);
                    if (line.find(")") != std::string::npos) {
                        // Finish if reached end of list
                        break;
                    }
                    // Next line is still the list, continue checking dependencies
                    end = -1;
                    start = line.find("\"", end + 1);
                }
                end = line.find("\"", start + 1);

                std::string possibleFile = line.substr(start + 1, end - start - 1);
                if (possibleFile.find(".o") == std::string::npos && possibleFile.find("cmake_pch") == std::string::npos &&
                    possibleFile.find(projectDir.filename()) != std::string::npos) {
                    cmakeTargetFiles.push_back(possibleFile);
                }
            }
            std::cout << std::flush;
            break;
        }
    }
    dependIn.close();

    // Get all files that where included
    std::set<fs::path> targetFiles(cmakeTargetFiles.begin(), cmakeTargetFiles.end());
    for(fs::path file : cmakeTargetFiles)
    {
        std::vector<fs::path> includedFiles = getIncludedFiles(file);
        targetFiles.insert(includedFiles.begin(), includedFiles.end());
    }

    // Update files related to this target
    _targetFiles[target] = {};
    for(fs::path file : targetFiles)
        _targetFiles[target].push_back(file);
}

std::string LinuxCompiler::runCommand(std::string cmd, bool print, bool keepColors) {
    std::array<char, 512> buffer;
    std::string result;
    keepColors = false;

    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(((keepColors ? "unbuffer " : "") + cmd + " 2>&1").c_str(), "r"), pclose);
    if (!pipe) {
        LOG_WARN("script::LinuxCompiler", "Could not open pipe");
        return "";
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        if (print) {
            // Print each line
            std::string line = buffer.data();
            line.resize(line.size() - 1); // Remove '\n'
            LOG_DEBUG("script::LinuxCompiler", "[w]$0", line);
        }
        // Add buffer to result string
        result += buffer.data();
    }
    return result;
}

} // namespace atta::script

#endif // ATTA_OS_LINUX
