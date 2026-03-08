// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

namespace atta::ui {

/// Drawer used to draw sensor lines
class SensorDrawer {
  public:
    void update(); ///< Update sensor lines
  private:
    void updateCameras();
    void updateInfrareds();
};

} // namespace atta::ui
