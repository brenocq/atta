//--------------------------------------------------
// Atta Evaluator
// evaluator.h
// Date: 2021-01-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_HELPERS_EVALUATOR_H
#define ATTA_HELPERS_EVALUATOR_H

#include <string>
#include <chrono>
#include <map>

namespace atta
{
	// Used to obtain global stats on simulator performance
	// (Must deal with concurrency updates by the threads)
	class Evaluator
	{
		public:
			Evaluator(const Evaluator&) = delete;
        	void operator=(const Evaluator&) = delete;

			static Evaluator& get()
			{
				static Evaluator instance;
				return instance;
			}

			static void reportCounter(std::string name, int64_t val=1) { get().reportCounterImpl(name, val); } 
			static void showReports() { get().showReportsImpl(); }
			static void clear() { get().clearImpl(); }

		private:
			Evaluator(){}

			void reportCounterImpl(std::string name, int64_t val);
			void showReportsImpl();
			void clearImpl();

    		std::map<std::string, int64_t> _counters;
	};

	// Used to obtain stats that will be displayed at the end of the scope
	// Ex: Time to load model mesh
	class LocalEvaluator
	{
		public:
			LocalEvaluator(std::string description="Finished evaluator");
			~LocalEvaluator();

			void stop();
			void print();
			double getMs() const { return _duration*0.001; }

		private:
			std::string _description;
			bool _finished;
			std::chrono::time_point<std::chrono::high_resolution_clock> _startTime;
			long int _duration;
	};
}
#endif// ATTA_HELPERS_EVALUATOR_H
