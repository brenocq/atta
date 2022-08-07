//--------------------------------------------------
// Atta Config
// config.cpp
// Date: 2022-03-12
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/core/config.h>

namespace atta {
Config& Config::getInstance() {
    static Config config;
    return config;
}

void Config::init() { getInstance().initImpl(); }
void Config::initImpl() { _dt = 0.0005; }
} // namespace atta
