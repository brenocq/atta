//--------------------------------------------------
// Atta Sensor Module
// managerCamera.cpp
// Date: 2021-09-25
// By Breno Cunha Queiroz
//--------------------------------------------------
namespace atta::sensor {

void Manager::unregisterInfrareds() { _infrareds.clear(); }

void Manager::unregisterInfrared(cmp::Entity entity) {
    bool found = false;
    for (int i = _infrareds.size() - 1; i >= 0; i--) {
        if (_infrareds[i].entity == entity) {
            _infrareds.erase(_infrareds.begin() + i);
            found = true;
            break;
        }
    }

    if (!found)
        LOG_WARN("sensor::Manager", "Could not unregister infrared from entity [w]$0[], infrared was not registered before", entity);
}

void Manager::registerInfrareds() {
    for (cmp::Entity entity : cmp::getEntitiesView()) {
        cmp::InfraredSensor* infrared = cmp::getComponent<cmp::InfraredSensor>(entity);
        if (infrared) {
            // Check if infrared was not registered yet
            bool found = false;
            for (auto ir : _infrareds)
                if (ir.entity == entity) {
                    found = true;
                    break;
                }
            // Register new infrared
            if (!found)
                registerInfrared(entity, infrared);
        }
    }
}

void Manager::registerInfrared(cmp::Entity entity, cmp::InfraredSensor* infrared) {
    // Do not register prototype infrared sensors
    if (cmp::Entity(entity).isPrototype())
        return;

    InfraredInfo infraredInfo{};
    infraredInfo.entity = entity;
    infraredInfo.component = infrared;
    _infrareds.push_back(infraredInfo);
}

void Manager::initializeInfrared(InfraredInfo& infraredInfo) {
    cmp::InfraredSensor* ir = infraredInfo.component;
    // Start with random time (used to distribute measurements across time)
    ir->measurementTime = -(rand() / float(RAND_MAX)) / ir->odr;
}

void Manager::updateInfrareds(float dt) {
    for (InfraredInfo& iri : _infrareds) {
        cmp::InfraredSensor* ir = iri.component;

        // Check if it is enabled
        if (!ir->enabled)
            continue;

        // Check if should take new measurement
        float change = Config::getTime() - ir->measurementTime;
        float interval = 1.0f / ir->odr;
        if (change >= interval) {
            float measurement = 0.0f;

            //----- Measurement from physics module -----//
            cmp::Entity entity = iri.entity;
            cmp::Transform* t = entity.get<cmp::Transform>();
            if (t == nullptr) {
                LOG_WARN("sensor::Manager", "Could not measure infrared sensor because entity does not have a transform component");
                continue;
            }

            // Calculate ray direction
            component::Transform worldTrans = cmp::Transform::getEntityWorldTransform(entity);
            vec3 rayDir = vec3(1.0f, 0.0f, 0.0f);
            worldTrans.orientation.rotateVector(rayDir);

            // Perform ray cast
            vec3 begin = worldTrans.position;
            vec3 end = begin + rayDir * ir->upperLimit;
            std::vector<phy::RayCastHit> hits = phy::rayCast(begin, end, true);
            if (hits.size())
                measurement = hits[0].distance;
            else
                measurement = ir->upperLimit;

            //----- Post-process measurement -----//
            // Apply gaussian noise
            if (ir->gaussianStd > 0.0f) {
                std::default_random_engine generator(int(Config::getTime() / Config::getDt()));
                std::normal_distribution<float> dist(measurement, ir->gaussianStd);
                measurement = dist(generator);
            }

            // Round to resolution
            if (ir->resolution > 0.0f)
                measurement = int(measurement / ir->resolution) * ir->resolution;

            // Clip limits
            measurement = std::min(std::max(measurement, ir->lowerLimit), ir->upperLimit);

            ir->measurement = measurement;
            ir->measurementTime = Config::getTime();
        }
    }
}

} // namespace atta::sensor
