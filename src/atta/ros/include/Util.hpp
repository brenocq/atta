#pragma once
#include <cctype>
#include <string>
#include <algorithm>
#include <atta/component/components/name.h>
#include <atta/component/entity.h>
#include <atta/component/components/relationship.h>
#include <atta/component/components/transform.h>
#include <atta/component/components/infraredSensor.h>
#include <atta/component/components/rigidBody.h>
namespace atta::ros {
inline std::string nameOf(int key){
    if (auto* cName = atta::component::getComponent<atta::component::Name>(key)) {
        std::string name = cName->name;
        std::replace(name.begin(), name.end(), ' ', '_');
        return name;
    } else {

        return "Entity_" + std::to_string(key);
    }
}

std::string shortenTopic(std::string fullName){
    size_t count = std::count(fullName.begin(), fullName.end(), '/');
    // check if count is bigger than the number of entites we want visible in topic
    // "root/a/b/c/d/e/f/g" ===> "root/a/.../f/g" {4 entites = 4 "/"} 
    if(count > 4){
        //LOG_INFO("Ros","topic name is too big,");
        // Find position of second '/'
        size_t firstSlash = fullName.find('/');
        if (firstSlash == std::string::npos) return fullName;

        size_t secondSlash = fullName.find('/', firstSlash + 1);
        if (secondSlash == std::string::npos) return fullName;

        // Find position of second-to-last '/'
        size_t lastSlash = fullName.rfind('/');
        if (lastSlash == std::string::npos || lastSlash <= secondSlash) return fullName;

        size_t secondLastSlash = fullName.rfind('/', lastSlash - 1);
        if (secondLastSlash == std::string::npos || secondLastSlash <= secondSlash) return fullName;

        // Extract the first character of the segment being collapsed
        size_t startOfMiddle = secondSlash + 1;
        char firstChar = fullName[startOfMiddle];
        std::string replacement = "/" + std::string(1, firstChar) + "___/";

        fullName.replace(secondSlash, secondLastSlash - secondSlash + 1, replacement);
    }
    return fullName;
}

std::string getFullName(int key) {
    std::string name = nameOf(key);

    auto rel = atta::component::getComponent<atta::component::Relationship>(key);
    if (!rel) {
        LOG_INFO("Ros", "No Relationship component for key: " + std::to_string(key));
        return name;
    }

    atta::component::Entity parent = rel->getParent();
    if (!parent.exists()) {
        LOG_INFO("Ros", "Reached top-level entity (no parent) for key: " + std::to_string(key));
        return name;
    }

    int parentId = parent.getId();
    LOG_INFO("Ros", "Parent found, key: " + std::to_string(key) + ", parent ID: " + std::to_string(parentId));
    return getFullName(parentId) + "/" + name;
}

}