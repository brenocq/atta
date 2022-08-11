//--------------------------------------------------
// Atta Script Module
// interface.h
// Date: 2022-08-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_INTERFACE_H
#define ATTA_SCRIPT_INTERFACE_H

#include <atta/script/projectScript.h>
#include <atta/script/script.h>

namespace atta::script {

void startUp();
void shutDown();

Script* getScript(StringId target);
std::vector<StringId> getScriptSids();

ProjectScript* getProjectScript();
StringId getProjectScriptSid();

#ifdef ATTA_STATIC_PROJECT
ProjectScript* registerProjectScript(std::string name, ProjectScript* projectScript);
Script* registerScript(std::string name, Script* script);
#endif

} // namespace atta::script

#endif // ATTA_SCRIPT_INTERFACE_H
