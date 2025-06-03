#pragma once
#include <cctype>
#include <string>
#include <algorithm>
#include <atta/component/components/name.h>
#include <atta/component/entity.h>
#include <atta/component/components/relationship.h>
#include <atta/component/components/transform.h>
#include <atta/component/components/infraredSensor.h>

inline std::string nameOf(int key){
    
    if (auto* cName = atta::component::getComponent<atta::component::Name>(key)) {
        std::string name = cName->name;
        std::replace(name.begin(), name.end(), ' ', '_');
        return name;
    } else {

        return "Entity_" + std::to_string(key);
    }
}