//--------------------------------------------------
// Atta Component Module
// infraredSensor.h
// Date: 2021-11-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_COMPONENTS_INFRARED_SENSOR_H
#define ATTA_COMPONENT_COMPONENTS_INFRARED_SENSOR_H

#include <atta/component/components/component.h>
#include <atta/component/interface.h>

namespace atta::component {

/// %Component to create simulated infra-red sensor
/** Simulation of infra-red sensor.
 * The measurement value is updated automatically by the sensor
 * module with a frequency close to ODR.
 *
 * The measurement value comes from the physics system, so the entities
 * must have a collider component to be detected. When 2D physics is
 * being used, the measurement will be in the 2D plane.
 *
 * The infra-red shooting direction is the same as the entity Z axis,
 * you can change the direction by changing the Transform.
 */
struct InfraredSensor final : public Component {
    /// Sensor measurement
    /** Measured in meters.
     * If it is the first measurement, check measurementTime to see if the first measurement was
     * already generated
     **/
    float measurement = 0.0f;
    /// Time the last measurement was generated
    /** If less than zero, no measurement was made yet **/
    float measurementTime = -1.0f;
    /// Enable sensor measurements
    /** Can be used to enable/disable the sensor measurement. If it was disabled and turned into
     * enabled, the measurement will be made in the next simulation step.
     **/
    bool enabled = true;
    /// Resolution
    /** The minimum possible difference between two measurements in meters **/
    float resolution = 0.01f;
    /// Lower measurement range
    /** If the distance is lower than the lower range, the measurement is clipped to the lower range **/
    float lowerRange = 0.0f;
    /// Upper measurement range
    /** If the distance is higher than the upper range, the measurement is clipped to the upper range **/
    float upperRange = 1000.0f; ///< Near plane
    /// Output data rate
    /** Frequency in which the measurement is collected, in Hz **/
    float odr = 2000.0f;
    /// Gaussian standard deviation
    float gaussianStd = 0.0f;
};
ATTA_REGISTER_COMPONENT(InfraredSensor);
template <>
ComponentDescription& TypedComponentRegistry<InfraredSensor>::getDescription();

} // namespace atta::component

#endif // ATTA_COMPONENT_COMPONENTS_INFRARED_SENSOR_H
