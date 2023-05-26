//--------------------------------------------------
// Atta Component Module
// infraredSensor.cpp
// Date: 2022-11-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/infraredSensor.h>

namespace atta::component {

template <>
ComponentDescription& TypedRegistry<InfraredSensor>::getDescription() {
    static ComponentDescription desc = {
        "Infrared Sensor",
        {
            {AttributeType::FLOAT32, offsetof(InfraredSensor, measurement), "measurement"},
            {AttributeType::FLOAT32, offsetof(InfraredSensor, measurementTime), "measurementTime"},
            {AttributeType::BOOL, offsetof(InfraredSensor, enabled), "enabled"},
            {AttributeType::FLOAT32, offsetof(InfraredSensor, resolution), "resolution", 0.0f, 1.0f, 0.001f},
            {AttributeType::FLOAT32, offsetof(InfraredSensor, lowerLimit), "lowerLimit", 0.0f, 1.0f, 0.001f},
            {AttributeType::FLOAT32, offsetof(InfraredSensor, upperLimit), "upperLimit", 0.0f, 1.0f, 0.001f},
            {AttributeType::FLOAT32, offsetof(InfraredSensor, odr), "odr", 0.01f, 2000.0f, 1.0f},
            {AttributeType::FLOAT32, offsetof(InfraredSensor, gaussianStd), "gaussianStd", 0.0f, 1.0f, 0.001f},
        },
        1024, // Max instances
    };

    return desc;
}

} // namespace atta::component
