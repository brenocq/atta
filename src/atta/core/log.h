//--------------------------------------------------
// Atta Log
// log.h
// Date: 2021-08-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_CORE_LOG_H
#define ATTA_CORE_LOG_H

//---------------------------------//
//------ Terminal Color Code ------//
//---------------------------------//
#define COLOR_RESET   		"\033[0m"
#define COLOR_BOLD   		"\033[1m"
#define COLOR_BLACK   		"\033[30m"      		/* Black */
#define COLOR_RED     		"\033[31m"      		/* Red */
#define COLOR_GREEN   		"\033[32m"      		/* Green */
#define COLOR_YELLOW  		"\033[33m"      		/* Yellow */
#define COLOR_BLUE    		"\033[34m"      		/* Blue */
#define COLOR_MAGENTA 		"\033[35m"      		/* Magenta */
#define COLOR_CYAN    		"\033[36m"      		/* Cyan */
#define COLOR_WHITE   		"\033[37m"      		/* White */
#define COLOR_RESET_BLACK   "\033[0m\033[30m"      	/* Black */
#define COLOR_RESET_RED     "\033[0m\033[31m"      	/* Red */
#define COLOR_RESET_GREEN   "\033[0m\033[32m"      	/* Green */
#define COLOR_RESET_YELLOW  "\033[0m\033[33m"      	/* Yellow */
#define COLOR_RESET_BLUE    "\033[0m\033[34m"      	/* Blue */
#define COLOR_RESET_MAGENTA "\033[0m\033[35m"      	/* Magenta */
#define COLOR_RESET_CYAN    "\033[0m\033[36m"      	/* Cyan */
#define COLOR_RESET_WHITE   "\033[0m\033[37m"      	/* White */
#define COLOR_BOLD_BLACK   	"\033[1m\033[30m"      	/* Bold Black */
#define COLOR_BOLD_RED     	"\033[1m\033[31m"      	/* Bold Red */
#define COLOR_BOLD_GREEN   	"\033[1m\033[32m"      	/* Bold Green */
#define COLOR_BOLD_YELLOW  	"\033[1m\033[33m"      	/* Bold Yellow */
#define COLOR_BOLD_BLUE    	"\033[1m\033[34m"      	/* Bold Blue */
#define COLOR_BOLD_MAGENTA 	"\033[1m\033[35m"      	/* Bold Magenta */
#define COLOR_BOLD_CYAN    	"\033[1m\033[36m"      	/* Bold Cyan */
#define COLOR_BOLD_WHITE   	"\033[1m\033[37m"      	/* Bold White */

namespace atta
{
	//---------------------------------//
	//---------- Description ----------//
	//---------------------------------//
	// Create colored output logs with parameter pack
	// Colors: [w]  -> white
	//         [r]  -> red
	//         [g]  -> green
	//         [b]  -> blue
	//         [c]  -> cyan
	//         [m]  -> magenta
	//         [y]  -> yellow
	//         [k]  -> black
	//         [*w]  -> bold white
	//         [*r]  -> bold red
	//         [*g]  -> bold green
	//         [*b]  -> bold blue
	//         [*c]  -> bold cyan
	//         [*m]  -> bold magenta
	//         [*y]  -> bold yellow
	//         [*k]  -> bold black
	//         []   -> reset to default color
	//
	// Argument substitution: $0 -> first argument
	//						  $1 -> second argument
	//						  ...

	//---------------------------------//
	//------------ Examples -----------//
	//---------------------------------//
	// Log::error("Window", "$0 surface could not be created. [w]Error Code: $1", "Main", 123);
	//
	// Output:
	// /*boldred*/[Window] /*red*/Main surface could not be created. /*white*/Error Code: 123
	//----------------
	// Log::info("Cool", "[r]R[*g]a[y]i[*c]n[m]b[*w]o[c]w []$0", "output");
	//
	// Output:
	// [Cool] Rainbow output // But with colors

	//---------------------------------//
	//----------- Log class -----------//
	//---------------------------------//
	class Log final
	{
	public:
		enum LogLevel {
			LOG_LEVEL_VERBOSE = 0,
			LOG_LEVEL_DEBUG,
			LOG_LEVEL_SUCCESS,
			LOG_LEVEL_INFO,
			LOG_LEVEL_WARNING,
			LOG_LEVEL_ERROR,
			LOG_LEVEL_NONE,
		};
		const static LogLevel logLevel = LOG_LEVEL_VERBOSE;

		template<class...Args>
		static void verbose(std::string tag, std::string text, Args&&... args);
		template<class...Args>
		static void debug(std::string tag, std::string text, Args&&... args);
		template<class...Args>
		static void success(std::string tag, std::string text, Args&&... args);
		template<class...Args>
		static void info(std::string tag, std::string text, Args&&... args);
		template<class...Args>
		static void warning(std::string tag, std::string text, Args&&... args);
		template<class...Args>
		static void error(std::string tag, std::string text, Args&&... args);

	private:
		//---------- Main log function ----------//
		template<class...Args>
		static void log(const char* tagColor, std::string tag, const char* textColor, std::string text, Args&&... args);
	};
}

#define LOG_VERBOSE(...) atta::Log::verbose(__VA_ARGS__)
#define LOG_DEBUG(...) atta::Log::debug(__VA_ARGS__)
#define LOG_SUCCESS(...) atta::Log::success(__VA_ARGS__)
#define LOG_INFO(...) atta::Log::info(__VA_ARGS__)
#define LOG_WARN(...) atta::Log::warning(__VA_ARGS__)
#define LOG_ERROR(...) atta::Log::error(__VA_ARGS__)

#include <atta/core/log.inl>
#endif// ATTA_CORE_LOG_H
