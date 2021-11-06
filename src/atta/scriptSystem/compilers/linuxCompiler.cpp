//--------------------------------------------------
// Atta Script System
// linuxCompiler.h
// Date: 2021-09-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifdef ATTA_OS_LINUX
#include <atta/scriptSystem/compilers/linuxCompiler.h>
#include <atta/fileSystem/fileManager.h>

#include <chrono>

namespace atta
{
    LinuxCompiler::LinuxCompiler()
    {

    }

    LinuxCompiler::~LinuxCompiler()
    {

    }

    void LinuxCompiler::compileAll()
    {
        std::chrono::time_point<std::chrono::system_clock> begin = std::chrono::system_clock::now();
        //LOG_DEBUG("LinuxCompiler", "Compile all targets");

        fs::path projectDir = FileManager::getProject()->getDirectory();
        fs::path buildDir = projectDir / "build";
        fs::path tempFile = buildDir / "atta.temp";
        fs::path errorFile = buildDir / "atta.error";
        tempFile = fs::absolute(tempFile);
        errorFile = fs::absolute(errorFile);

        // Create build directory if does not exists
        if(!fs::exists(buildDir))
            fs::create_directory(buildDir);

        fs::path prevPath = fs::current_path();
        fs::current_path(buildDir);
#ifdef ATTA_DEBUG_BUILD
        std::string buildCommand = "cmake -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_CUDA_ARCHITECTURES=75 -DCMAKE_BUILD_TYPE=Debug ..";
#else
        std::string buildCommand = "cmake -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_CUDA_ARCHITECTURES=75 -DCMAKE_BUILD_TYPE=Release ..";
#endif
        buildCommand += " > " + tempFile.string() + " ";
        buildCommand += "2> " + errorFile.string();
        std::system(buildCommand.c_str());

        std::string makeCommand = "make -j";
        makeCommand += " > " + tempFile.string();
        makeCommand += " 2> " + errorFile.string();
        std::system(makeCommand.c_str());
        fs::current_path(prevPath);

        //---------- Show default output ----------//
        std::stringstream tempSS;
        std::ifstream tempIn(tempFile);
        tempSS << tempIn.rdbuf();
        tempIn.close();
        std::string tempStr = tempSS.str();

        if(tempStr.size() > 0)
            LOG_VERBOSE("LinuxCompiler", "Build output: \n$0", tempStr);

        //---------- Show error ----------//
        std::stringstream errorSS;
        std::ifstream errorIn(errorFile);
        errorSS << errorIn.rdbuf();
        errorIn.close();
        std::string errorStr = errorSS.str();

        if(errorStr.size() > 0)
        {
            LOG_WARN("LinuxCompiler", "Error while compiling project!\n$0", errorStr);
            return;
        }

        fs::remove(tempFile);
        fs::remove(errorFile);

        std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
        auto micro = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        //LOG_INFO("LinuxCompiler", "Time to compile all: $0 ms", micro.count()/1000.0f);
    }

    void LinuxCompiler::compileTarget(StringId target)
    {
        std::chrono::time_point<std::chrono::system_clock> begin = std::chrono::system_clock::now();

        //LOG_DEBUG("LinuxCompiler", "Compile target $0", target);
        if(_targetFiles.find(target) == _targetFiles.end())
        {
            LOG_WARN("LinuxCompiler", "Could not find target $0", target);
            return;
        }

        fs::path projectDir = FileManager::getProject()->getDirectory();
        fs::path buildDir = projectDir / "build";
        fs::path tempFile = buildDir / "atta.temp";
        fs::path errorFile = buildDir / "atta.error";
        tempFile = fs::absolute(tempFile);
        errorFile = fs::absolute(errorFile);

        // Compile all if never compiled
        if(!fs::exists(buildDir))
        {
            compileAll();
            return;
        }

        fs::path prevPath = fs::current_path();
        fs::current_path(buildDir);
        std::string command = "cmake --build . --parallel --target "+target.getString();
        //std::string command = "make "+target.getString()+"/fast -j";
        command += " > " + tempFile.string();
        command += " 2> " + errorFile.string();
        std::system(command.c_str());
        fs::current_path(prevPath);

        //std::string makeCommand = "make";
        //makeCommand += " > " + tempFile.filename().string();
        //makeCommand += " 2> " + errorFile.filename().string();
        //std::system(makeCommand.c_str());

        std::stringstream errorSS;
        std::ifstream errorIn(errorFile);
        errorSS << errorIn.rdbuf();
        errorIn.close();
        std::string errorStr = errorSS.str();

        if(errorStr.size() > 0)
        {
            LOG_WARN("LinuxCompiler", "Error while compiling target $0\n$1", target, errorStr);
            return;
        }

        fs::remove(tempFile);
        fs::remove(errorFile);

        std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
        auto micro = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        //LOG_INFO("LinuxCompiler", "Time to compile target $1: $0 ms", micro.count()/1000.0f, target);
    }

    void LinuxCompiler::updateTargets()
    {
        fs::path projectDir = FileManager::getProject()->getDirectory();
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
        while(std::getline(tempIn, line))
        {
            if(line == "... rebuild_cache")
            {
                isTarget = true;
                continue;
            }
            if(line.find(".o") != std::string::npos)
            {
                isTarget = false;
                continue;
            }

            size_t space = line.find(' ');
            if(isTarget)
            {
                StringId target = StringId(line.substr(space+1));
                findTargetFiles(target);
            }
        }
        tempIn.close();
    }


    void LinuxCompiler::findTargetFiles(StringId target)
    {
        _targetFiles[target] = std::vector<fs::path>();

        fs::path projectDir = FileManager::getProject()->getDirectory();
        fs::path buildDir = projectDir / "build";
        fs::path dependFile = buildDir / "CMakeFiles" / (target.getString()+".dir").c_str() / "DependInfo.cmake";

        std::ifstream dependIn(dependFile);
        std::string line;

        while(std::getline(dependIn, line))
        {
            if(line.find("CMAKE_DEPENDS_DEPENDENCY_FILES") != std::string::npos)
            {
                std::getline(dependIn, line);
                // Line now has a list of string with the name of each file that this target depends on

                size_t end = -1;
                while(true)
                {
                    size_t start = line.find("\"", end+1);
                    if(start == std::string::npos)	
                    {
                        //LOG_DEBUG("LinuxCompiler", "Updated target files $0:\n $1", target, _targetFiles[target]);
                        return;
                    }
                    end = line.find("\"", start+1);

                    std::string possibleFile = line.substr(start+1, end-start-1);
                    if(possibleFile.find(".o") == std::string::npos && 
                            possibleFile.find(projectDir.filename()) != std::string::npos)
                    {
                        _targetFiles[target].push_back(possibleFile);

                        // If .cpp, add .h too
                        // TODO Find a better way to track header files
                        size_t pos;
                        if((pos=possibleFile.find(".cpp")) != std::string::npos)
                        {
                            possibleFile.replace(pos,4,".h");
                            _targetFiles[target].push_back(possibleFile);
                        }
                    }
                }
                std::cout << std::flush;
            }
        }
        dependIn.close();
    }
}

#endif// ATTA_OS_LINUX
