#ifndef FENCE_H
#define FENCE_H

#include "device.h"

class Fence
{
	public:
		Fence(const Fence&) = delete;
		Fence& operator = (const Fence&) = delete;
		Fence& operator = (Fence&&) = delete;

		Fence(Device* device, bool signaled);
		Fence(Fence&& other) noexcept;
		~Fence();

		VkFence handle() const { return _fence; }
		Device* device() const { return _device; }

		void reset();
		void wait(uint64_t timeout) const;

	private:
		Device* _device;
		VkFence _fence;
};

#endif// FENCE_H
