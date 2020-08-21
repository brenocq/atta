//----------------------------------------
// Robot Simulator
// log.cpp
// Date: 2020-08-20
// By Breno Cunha Queiroz
//----------------------------------------
#include "log.h"
#include <stdio.h>

Log::Log(std::string tag, Level level):
	_tag(tag), _level(level)
{

}

Log::~Log()
{

}

void Log::debug(const char* format, ...)
{
	//logging(LOG_BOLD(LOG_COLOR_GREEN), LOG_RESET_COLOR, format);
}

void Log::debug_pair(std::string label, std::string text)
{
    //printf("\t%s%s: %s%s\n", LOG_RESET_COLOR, label.c_str(),
	//						LOG_RESET_COLOR, text.c_str() );
}

void Log::success(const char* format, ...)
{
	//logging(LOG_BOLD(LOG_COLOR_GREEN), LOG_COLOR(LOG_COLOR_GREEN), format);
}

void Log::info(const char* format, ...)
{
	//logging(LOG_BOLD(LOG_COLOR_CYAN), LOG_COLOR(LOG_COLOR_CYAN), format);
}

void Log::info_pair(std::string label, std::string text)
{
    //printf("\t%s%s: %s%s\n", LOG_COLOR(LOG_COLOR_CYAN), label.c_str(),
	//						LOG_RESET_COLOR, text.c_str() );
}

void Log::warning(const char* format, ...)
{
	//logging(LOG_BOLD(LOG_COLOR_BROWN), LOG_COLOR(LOG_COLOR_BROWN), format);
}

void Log::error(const char* format, ...)
{
	//logging(LOG_BOLD(LOG_COLOR_RED), LOG_COLOR(LOG_COLOR_RED), format);
}


void Log::logging(const char* tag_color, const char* text_color, const char* format, ...)
{
	// Print tag
    printf("%s[%s]%s ", tag_color, _tag.c_str(), text_color);

	// Init list and remove tagColor and textColor from buffer
	va_list args;
	va_start(args, format);
	va_arg(args, const char*);
	va_arg(args, const char*);

	// Print fomatted data
	vprintf(format, args);
	va_end(args);
    //printf("\n%s", LOG_RESET_COLOR);
}
