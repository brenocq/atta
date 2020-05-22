#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "device.h"

class Semaphore
{
	public:
	Semaphore(const Semaphore&) = delete;
	Semaphore& operator = (const Semaphore&) = delete;
	Semaphore& operator = (Semaphore&&) = delete;

	Semaphore(Device* device);
	Semaphore(Semaphore&& other) noexcept;
	~Semaphore();

	VkSemaphore handle() const { return _semaphore; }
	Device* device() const { return _device; }

	private:
	Device* _device;
	VkSemaphore _semaphore;
};

#endif// SEMAPHORE_H
