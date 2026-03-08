// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
namespace atta::component {

template <typename T>
T* Manager::getComponentImpl(Entity entity) {
    static int index = -1;
    if (index == -1)
        index = TypedComponentRegistry<T>::getInstance().getIndex();

    return reinterpret_cast<T*>(getComponentByIndex(index, entity));
}

template <typename T>
T* Manager::addComponentImpl(Entity entity) {
    return reinterpret_cast<T*>(addComponentByIdImpl(COMPONENT_POOL_SSID(T), entity));
}

} // namespace atta::component
