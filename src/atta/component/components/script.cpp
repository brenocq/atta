//--------------------------------------------------
// Atta Component Module
// script.cpp
// Date: 2021-11-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/script.h>
#include <atta/script/interface.h>

namespace atta::component {

template <>
ComponentDescription& TypedRegistry<Script>::getDescription() {
    static ComponentDescription desc = {"Script",
                                        {
                                            {AttributeType::STRINGID, offsetof(Script, sid), "sid", {}, {}, {}, {}},
                                        }};

    return desc;
}

Script::Script() {
    if (!script::getScriptSids().empty())
        sid = script::getScriptSids()[0];
}

void Script::set(std::string script) { sid = StringId(script); }

} // namespace atta::component
