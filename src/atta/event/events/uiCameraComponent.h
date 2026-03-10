// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once
#include <atta/component/components/cameraSensor.h>
#include <atta/event/event.h>

namespace atta::event {

/// Published when the camera component "View" button is clicked
/** When the SensorManager received this event, it created a window to show the camera image **/
class UiCameraComponent : public EventTyped<SID("UiCameraComponent")> {
  public:
    enum class UiEvent { NONE = 0, VIEW_BUTTON_CLICKED };

    component::CameraSensor* component;
    UiEvent uiEvent;
};

} // namespace atta::event
