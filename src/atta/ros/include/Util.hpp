#pragma once
#include <string>
#include <algorithm>

inline void removeSpacesFrom(std::string& str) {
    std::replace(str.begin(), str.end(), ' ', '_');
}