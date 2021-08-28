//--------------------------------------------------
// Atta Log
// log.inl
// Date: 2021-08-11
// By Breno Cunha Queiroz
//--------------------------------------------------
namespace atta
{
	template<class...Args>
	void Log::verbose(std::string tag, std::string text, Args&&... args)
	{
		if(logLevel <= LOG_LEVEL_VERBOSE)
			log(COLOR_BOLD_WHITE, tag, COLOR_RESET_WHITE, text, args...);
	}

	template<class...Args>
	void Log::debug(std::string tag, std::string text, Args&&... args)
	{
		if(logLevel <= LOG_LEVEL_DEBUG)
			log(COLOR_BOLD_BLUE, tag, COLOR_RESET_BLUE, text, args...);
	}

	template<class...Args>
	void Log::success(std::string tag, std::string text, Args&&... args)
	{
		if(logLevel <= LOG_LEVEL_SUCCESS)
			log(COLOR_BOLD_GREEN, tag, COLOR_RESET_GREEN, text, args...);
	}

	template<class...Args>
	void Log::info(std::string tag, std::string text, Args&&... args)
	{
		if(logLevel <= LOG_LEVEL_INFO)
			log(COLOR_BOLD_CYAN, tag, COLOR_RESET_CYAN, text, args...);
	}

	template<class...Args>
	void Log::warning(std::string tag, std::string text, Args&&... args)
	{
		if(logLevel <= LOG_LEVEL_WARNING)
			log(COLOR_BOLD_YELLOW, tag, COLOR_RESET_YELLOW, text, args...);
	}

	template<class...Args>
	void Log::error(std::string tag, std::string text, Args&&... args)
	{
		if(logLevel <= LOG_LEVEL_ERROR)
			log(COLOR_BOLD_RED, tag, COLOR_RESET_RED, text, args...);
	}

	template<typename T>
	std::string getArgStr(T t)
	{
		std::stringstream ss;
		ss << t;
		return ss.str();
	}

	template<class...Args>
	void Log::log(const char* tagColor, std::string tag, const char* textColor, std::string text, Args&&... args)
	{
		// Replace arguments
		std::vector<std::string> argsStr;
		(argsStr.push_back(getArgStr(args)), ...);

		std::stringstream finalText;
		for(size_t i = 0; i < text.size(); i++)
		{
			if(text[i] == '[')
			{
				i++;// Skip [
				if(text[i] == '*')
				{
					finalText << COLOR_BOLD;
					i++;// Skip *
				}
				else if(text[i] == ']')
				{
					finalText << textColor;// Return color to default if found []
					i++;// Skip ]
					continue;
				}
				else
					finalText << COLOR_RESET;

				switch(text[i])
				{
					case 'w':
						finalText << COLOR_WHITE;
						break;
					case 'r':
						finalText << COLOR_RED;
						break;
					case 'g':
						finalText << COLOR_BLUE;
						break;
					case 'b':
						finalText << COLOR_BLUE;
						break;
					case 'y':
						finalText << COLOR_YELLOW;
						break;
					case 'm':
						finalText << COLOR_MAGENTA;
						break;
					case 'c':
						finalText << COLOR_RESET << COLOR_CYAN;
						break;
					case 'k':
						finalText << COLOR_BLACK;
						break;
					default:
						finalText << '[' << text[i];
						i--;
				}
				i++;// Skip color and ]
			}
			else if(text[i] == '$')
			{
				size_t idx = static_cast<size_t>(text[++i]-'0');
				if(idx < sizeof...(Args))
					finalText << argsStr[idx];
			}
			else
				finalText << text[i];
		}

		// Print [tag] text
		std::string output = std::string(tagColor) + "[" + tag + "] " + textColor + finalText.str() + COLOR_RESET + "\n";
		std::cout << output;
	}
}