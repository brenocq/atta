// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz

namespace atta::ui {

template <typename T>
void registerComponentUI(const ComponentUIFunc& renderFunc) {
    registerComponentUI(cmp::getId<T>(), renderFunc);
}

template <typename T>
std::optional<ComponentUIFunc> getComponentUI() {
    return getComponentUI(cmp::getId<T>());
}

} // namespace atta::ui
