//--------------------------------------------------
// Atta Parallel
// barrier.h
// Date: 2021-01-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/parallel/barrier.h>
namespace atta
{
	Barrier::Barrier(int qtyThreads):
		_qtyThreads(qtyThreads), _count(qtyThreads), _up(false)
	{

	}

	Barrier::~Barrier()
	{

	}

	void Barrier::wait()
	{
		std::unique_lock<std::mutex> lock{_mutex};
		if(!_up)// Count down
		{
			if (--_count == 0)
			{
				// Last thread to reach the barrier -> wake up all of the oher ones
				_up = true;// Unlock barrier to count up
				_cv.notify_all();
			}
			else
			{
				// Give up the lock and wait other threads to reach the barrier
				_cv.wait(lock, [this] { return _up == true; });
			}
		}
		else// Count up
		{
			if (++_count == _qtyThreads)
			{
				// Last thread to reach the barrier -> wake up all of the oher ones
				_up = false;// Unlock barrier to count down
				_cv.notify_all();
			}
			else
			{
				// Give up the lock and wait other threads to reach the barrier
				_cv.wait(lock, [this] { return _up == false; });
			}
		}
	}
}
