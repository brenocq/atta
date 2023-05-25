//--------------------------------------------------
// Atta Script Module
// nullLinker.cpp
// Date: 2021-09-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/script/linkers/nullLinker.h>

namespace atta::script {

void NullLinker::linkTarget(StringId target) {}

void NullLinker::releaseTarget(StringId target) {}

} // namespace atta::script
