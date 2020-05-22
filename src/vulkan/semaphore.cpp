#include "semaphore.h"

Semaphore::Semaphore(Device* device) :
	_device(device)
{
	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	if(vkCreateSemaphore(_device->handle(), &semaphoreInfo, nullptr, &_semaphore) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[Semaphore] Failed to create semaphore!";
		exit(1);
	}
}

Semaphore::Semaphore(Semaphore&& other) noexcept :
	_device(other._device),
	_semaphore(other._semaphore)
{
	other._semaphore = nullptr;
}

Semaphore::~Semaphore()
{
	if (_semaphore != nullptr)
		vkDestroySemaphore(_device->handle(), _semaphore, nullptr);
}
