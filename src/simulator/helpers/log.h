//----------------------------------------
// Robot Simulator
// log.h
// Date: 2020-08-20
// By Breno Cunha Queiroz
//----------------------------------------
#ifndef LOG_H
#define LOG_H
#include <string>
#include <cstdarg>

class Log
{
	public:
		enum class Level
		{
			DEBUG,
			INFO,
			WARNING,
			ERROR
		};

		Log(std::string tag, Level level=Level::DEBUG);
		~Log();

		void debug(const char* format, ...);
		void debug_pair(std::string label, std::string text);
		void success(const char* format, ...);
		void info(const char* format, ...);
		void info_pair(std::string label, std::string text);
		void warning(const char* format, ...);
		void error(const char* format, ...);

	private:
		void logging(const char* tag_color, const char* text_color, const char* format, ...);
		std::string _tag;
		Level _level;
};

#endif// LOG_H
