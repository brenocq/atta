// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

#include <atta/event/event.h>

namespace atta::event {

using Callback = std::function<void(Event&)>;
#define BIND_EVENT_FUNC(x) (void*)this, std::bind(&x, this, std::placeholders::_1)

template <typename E>
void subscribe(void* source, Callback&& callback);
template <typename E>
void unsubscribe(void* source, Callback&& callback);
void publish(Event& event);
void clear();

} // namespace atta::event

#include <atta/event/manager.h>

namespace atta::event {

template <typename E>
void subscribe(void* source, Callback&& callback) {
    Manager::getInstance().subscribeImpl(E::type, source, std::move(callback));
}

template <typename E>
void unsubscribe(void* source, Callback&& callback) {
    Manager::getInstance().unsubscribeImpl(E::type, source, std::move(callback));
}

} // namespace atta::event
