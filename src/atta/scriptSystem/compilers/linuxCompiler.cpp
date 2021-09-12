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

	void LinuxCompiler::updateTargets()
	{
		fs::path projectDir = FileManager::getProjectDirectory();
		fs::path buildDir = projectDir / "build";
		fs::path tempFile = buildDir / "atta.temp";
		//---------- Build cmake ----------//
		compileAll();

		//---------- Get targets ----------//
		_targetFiles.clear();
		std::string getTargetCommand = "cmake --build . --target help > " + tempFile.filename().string();
		fs::path prevPath = fs::current_path();
		fs::current_path(buildDir);
		std::system(getTargetCommand.c_str());
		fs::current_path(prevPath);

		std::ifstream tempIn(tempFile.filename());
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
				std::string target = line.substr(space+1);
				findTargetFiles(target);
			}
		}
		tempIn.close();
	}

	void LinuxCompiler::compileTarget(std::string target)
	{
		std::chrono::time_point<std::chrono::system_clock> begin = std::chrono::system_clock::now();

		LOG_DEBUG("LinuxCompiler", "Compile target $0", target);
		if(_targetFiles.find(target) == _targetFiles.end())
		{
			LOG_WARN("LinuxCompiler", "Could not find target $0", target);
			return;
		}

		fs::path projectDir = FileManager::getProjectDirectory();
		fs::path buildDir = projectDir / "build";
		fs::path tempFile = buildDir / "atta.temp";
		fs::path errorFile = buildDir / "atta.error";

		// Compile all if never compiled
		if(!fs::exists(buildDir))
		{
			compileAll();
			return;
		}

		fs::path prevPath = fs::current_path();
		fs::current_path(buildDir);
		std::string command = "cmake --build . --target "+target;
		command += " > " + tempFile.filename().string();
		command += " 2> " + errorFile.filename().string();
		std::system(command.c_str());
		fs::current_path(prevPath);

		//std::string makeCommand = "make";
		//makeCommand += " > " + tempFile.filename().string();
		//makeCommand += " 2> " + errorFile.filename().string();
		//std::system(makeCommand.c_str());

		std::stringstream errorSS;
		std::ifstream errorIn(errorFile.filename());
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
		LOG_INFO("LinuxCompiler", "Time to link: $0 ms", micro.count()/1000.0f);
	}

	void LinuxCompiler::compileAll()
	{
		LOG_DEBUG("LinuxCompiler", "Compile all targets");

		fs::path projectDir = FileManager::getProjectDirectory();
		fs::path buildDir = projectDir / "build";
		fs::path tempFile = buildDir / "atta.temp";
		fs::path errorFile = buildDir / "atta.error";

		// Create build directory if does not exists
		if(!fs::exists(buildDir))
			fs::create_directory(buildDir);

		fs::path prevPath = fs::current_path();
		fs::current_path(buildDir);
		std::string buildCommand = "cmake -DCMAKE_CUDA_ARCHITECTURES=75 .. ";
		buildCommand += "> " + tempFile.filename().string() + " ";
		buildCommand += "2> " + errorFile.filename().string();
		std::system(buildCommand.c_str());

		std::string makeCommand = "make";
		makeCommand += " > " + tempFile.filename().string();
		makeCommand += " 2> " + errorFile.filename().string();
		std::system(makeCommand.c_str());
		fs::current_path(prevPath);

		//---------- Show default output ----------//
		std::stringstream tempSS;
		std::ifstream tempIn(tempFile.filename());
		tempSS << tempIn.rdbuf();
		tempIn.close();
		std::string tempStr = tempSS.str();

		if(tempStr.size() > 0)
			LOG_VERBOSE("LinuxCompiler", "Build output: \n$0", tempStr);

		//---------- Show error ----------//
		std::stringstream errorSS;
		std::ifstream errorIn(errorFile.filename());
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
	}

	void LinuxCompiler::findTargetFiles(std::string target)
	{
		_targetFiles[target] = std::vector<fs::path>();

		fs::path projectDir = FileManager::getProjectDirectory();
		fs::path buildDir = projectDir / "build";
		fs::path dependFile = buildDir / "CMakeFiles" / (target+".dir").c_str() / "DependInfo.cmake";

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
						LOG_DEBUG("LinuxCompiler", "Updated target files $0:\n $1", target, _targetFiles[target]);
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
