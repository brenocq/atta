//----------------------------------------
// Atta Log
// log.h
// Date: 2020-08-20
// By Breno Cunha Queiroz
//----------------------------------------
#ifndef LOG_H
#define LOG_H
#include <string>
#include <vector>
#include <tuple>
#include <iostream>
//---------------------------------//
//---------- Description ----------//
//---------------------------------//
// Create colored output logs with parameter pack
// Color examples: [w]  -> white
//                 [r]  -> red
//                 [*c] -> bold cyan
//                 [*y] -> bold yellow
//                 []   -> reset to log default color
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
//------ Terminal Color Code ------//
//---------------------------------//
// https://stackoverflow.com/questions/9158150/colored-output-in-c/9158263
// The following are UBUNTU/LINUX, and MacOS ONLY terminal color codes.
// TODO add Windows support
#define COLOR_RESET   		"\033[0m"
#define COLOR_BLACK   		"\033[0m\033[30m"      	/* Black */
#define COLOR_RED     		"\033[0m\033[31m"      	/* Red */
#define COLOR_GREEN   		"\033[0m\033[32m"      	/* Green */
#define COLOR_YELLOW  		"\033[0m\033[33m"      	/* Yellow */
#define COLOR_BLUE    		"\033[0m\033[34m"      	/* Blue */
#define COLOR_MAGENTA 		"\033[0m\033[35m"      	/* Magenta */
#define COLOR_CYAN    		"\033[0m\033[36m"      	/* Cyan */
#define COLOR_WHITE   		"\033[0m\033[37m"      	/* White */
#define COLOR_BOLD_BLACK   	"\033[1m\033[30m"      	/* Bold Black */
#define COLOR_BOLD_RED     	"\033[1m\033[31m"      	/* Bold Red */
#define COLOR_BOLD_GREEN   	"\033[1m\033[32m"      	/* Bold Green */
#define COLOR_BOLD_YELLOW  	"\033[1m\033[33m"      	/* Bold Yellow */
#define COLOR_BOLD_BLUE    	"\033[1m\033[34m"      	/* Bold Blue */
#define COLOR_BOLD_MAGENTA 	"\033[1m\033[35m"      	/* Bold Magenta */
#define COLOR_BOLD_CYAN    	"\033[1m\033[36m"      	/* Bold Cyan */
#define COLOR_BOLD_WHITE   	"\033[1m\033[37m"      	/* Bold White */

//---------------------------------//
//----------- Log class -----------//
//---------------------------------//
class Log
{
	public:
		template<class...Args>
		static void debug(std::string tag, std::string text, Args&&... args)
		{
			log(COLOR_BOLD_MAGENTA, tag, COLOR_MAGENTA, text, args...);
		}

		template<class...Args>
		static void verbose(std::string tag, std::string text, Args&&... args)
		{
			log(COLOR_BOLD_WHITE, tag, COLOR_WHITE, text, args...);
		}

		template<class...Args>
		static void success(std::string tag, std::string text, Args&&... args)
		{
			log(COLOR_BOLD_GREEN, tag, COLOR_GREEN, text, args...);
		}

		template<class...Args>
		static void info(std::string tag, std::string text, Args&&... args)
		{
			log(COLOR_BOLD_CYAN, tag, COLOR_CYAN, text, args...);
		}

		template<class...Args>
		static void warning(std::string tag, std::string text, Args&&... args)
		{
			log(COLOR_BOLD_YELLOW, tag, COLOR_YELLOW, text, args...);
		}

		template<class...Args>
		static void error(std::string tag, std::string text, Args&&... args)
		{
			log(COLOR_BOLD_RED, tag, COLOR_RED, text, args...);
		}

	private:
		static void replace(std::string& str, const std::string& oldStr, const std::string& newStr);
		
		//---------- Convert parameters to string ----------//
		static std::string toString(const char* str);
		static std::string toString(std::string str);

		template<class T>
		static std::string toString(std::vector<T> vec)
		{
			std::string str = "{";
			for(unsigned int i=0; i<vec.size(); i++)
			{
				str+=toString(vec[i]);
				if(i<vec.size()-1)
					str+=", ";
			}
			str+="}";

			return str;
		}

		template<class T>
		static std::string toString(T t)
		{
			return std::to_string(t);
		}

		//---------- Main log function ----------//
		template<class...Args>
		static void log(const char* tagColor, std::string tag, const char* textColor, std::string text, Args&&... args)
		{
			//int size = argSize(args...);
			
			// Replace special sequences by color (TODO more efficient algorithm)
			replace(text, "[k]", std::string(COLOR_BLACK));
			replace(text, "[r]", std::string(COLOR_RED));
			replace(text, "[g]", std::string(COLOR_GREEN));
			replace(text, "[y]", std::string(COLOR_YELLOW));
			replace(text, "[b]", std::string(COLOR_BLUE));
			replace(text, "[m]", std::string(COLOR_MAGENTA));
			replace(text, "[c]", std::string(COLOR_CYAN));
			replace(text, "[w]", std::string(COLOR_WHITE));
			replace(text, "[*k]", std::string(COLOR_BOLD_BLACK));
			replace(text, "[*r]", std::string(COLOR_BOLD_RED));
			replace(text, "[*g]", std::string(COLOR_BOLD_GREEN));
			replace(text, "[*y]", std::string(COLOR_BOLD_YELLOW));
			replace(text, "[*b]", std::string(COLOR_BOLD_BLUE));
			replace(text, "[*m]", std::string(COLOR_BOLD_MAGENTA));
			replace(text, "[*c]", std::string(COLOR_BOLD_CYAN));
			replace(text, "[*w]", std::string(COLOR_BOLD_WHITE));
			replace(text, "[]", std::string(textColor));

			// Replace arguments
			std::vector<std::string> argsStr;
			(argsStr.push_back(toString(std::forward<Args>(args))), ...);

			for(unsigned int i=0; i<argsStr.size(); i++)
				replace(text, "$"+std::to_string(i), argsStr[i]);

			// Print [tag] text
			std::cout << tagColor << "[" << tag << "] ";
			std::cout << textColor << text << " ";

			// Finish
			std::cout << COLOR_RESET << std::endl;
		}
};

//#undef COLOR_RESET
//#undef COLOR_BLACK
//#undef COLOR_RED
//#undef COLOR_GREEN
//#undef COLOR_YELLOW
//#undef COLOR_BLUE
//#undef COLOR_MAGENTA
//#undef COLOR_CYAN
//#undef COLOR_WHITE
//#undef COLOR_BOLD_BLACK
//#undef COLOR_BOLD_RED
//#undef COLOR_BOLD_GREEN
//#undef COLOR_BOLD_YELLOW
//#undef COLOR_BOLD_BLUE
//#undef COLOR_BOLD_MAGENTA
//#undef COLOR_BOLD_CYAN
//#undef COLOR_BOLD_WHITE

#endif// LOG_H
