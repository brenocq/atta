// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#ifndef ATTA_UI_PANELS_TOP_BAR_LOCAL_WINDOWS_VERSION_WINDOW_H
#define ATTA_UI_PANELS_TOP_BAR_LOCAL_WINDOWS_VERSION_WINDOW_H

#include <atta/physics/interface.h>
#include <atta/ui/windows/window.h>

namespace atta::ui {

class VersionWindow : public Window<VersionWindow> {
  private:
    VersionWindow();
    void renderImpl();

    void checkInstalledVersions();
    std::set<std::string> _installedVersions;

    friend Window<VersionWindow>;
};

} // namespace atta::ui

#endif // ATTA_UI_PANELS_TOP_BAR_LOCAL_WINDOWS_VERSION_WINDOW_H
