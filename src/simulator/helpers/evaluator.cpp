//--------------------------------------------------
// Atta Evaluator
// evaluator.cpp
// Date: 2021-01-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "evaluator.h"
#include "log.h"

namespace atta
{
	//------------------------------//
	//---------- EVALUATOR ---------//
	//------------------------------//

	void Evaluator::reportCounterImpl(std::string name, int64_t val)
	{
		_counters[name]+=val;
		//Log::debug("Eval", "counter[$0]=$1", name, _counters[name]);
	}

	void Evaluator::showReportsImpl()
	{
		std::string reportStr = "";
		reportStr+="[*c]Counts:\n";
		for(auto report : _counters)
		{
			std::string currReport("    [w]- [c]"+report.first+":");
			while(currReport.size()<30)
				currReport+=" ";
			currReport += "[w]" + std::to_string(report.second)+"\n";

			reportStr += currReport;
		}

		Log::info("Evaluator", "Report: \n"+reportStr);
	}

	void Evaluator::clearImpl()
	{
		_counters.clear();
	}

	//------------------------------//
	//------- LOCAL EVALUATOR ------//
	//------------------------------//
	LocalEvaluator::LocalEvaluator(std::string description):
		_finished(false), _description(description)
	{
		_startTime = std::chrono::high_resolution_clock::now();
	}

	LocalEvaluator::~LocalEvaluator()
	{
		if(!_finished)
		{
			stop();
			print();
		}
	}

	void LocalEvaluator::stop()
	{
		auto endTime = std::chrono::high_resolution_clock::now();

		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(_startTime).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTime).time_since_epoch().count();
		_duration = end-start;

		_finished = true;
	}

	void LocalEvaluator::print()
	{
		Log::debug("LocalEvaluator", std::string(_description+" - [w]$0ms"), getMs());
	}
}
