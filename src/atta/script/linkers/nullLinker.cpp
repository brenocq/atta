// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#include <atta/script/linkers/nullLinker.h>

namespace atta::script {

void NullLinker::linkTarget(StringId target, Script** script, ProjectScript** projectScript, std::string& name) {
    *script = nullptr;
    *projectScript = nullptr;
    name = "";
}

} // namespace atta::script
