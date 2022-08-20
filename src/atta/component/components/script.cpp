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
ComponentDescription& TypedComponentRegistry<Script>::getDescription() {
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

} // namespace atta::component
