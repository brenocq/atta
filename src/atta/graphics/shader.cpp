//--------------------------------------------------
// Atta Graphics Module
// shader.cpp
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/file/manager.h>
#include <atta/graphics/shader.h>
#include <regex>

namespace atta::graphics {

Shader::Shader(const fs::path& file) : _file(file) {
    fs::path absolutePath = file::solveResourcePath(_file);

    // Check file
    if (!fs::exists(absolutePath)) {
        LOG_ERROR("gfx::Shader", "Shader file [w]$0[] was not found", file.string());
        return;
    }

    if (file.extension() != ".asl") {
        LOG_ERROR("gfx::Shader", "Shader file [w]$0[] does not have [w].asl[] extension", file.string());
        return;
    }

    // Read file
    std::stringstream ss;
    std::ifstream f(absolutePath);
    ss << f.rdbuf();
    f.close();
    _aslCode = ss.str();

    // Set buffer layout alignment
    _vertexLayout.setAlignmentType(BufferLayout::AlignmentType::DEFAULT);
    _perFrameLayout.setAlignmentType(BufferLayout::AlignmentType::STD140);
    _perDrawLayout.setAlignmentType(BufferLayout::AlignmentType::STD430);
    _perFrameImageLayout.setAlignmentType(BufferLayout::AlignmentType::STD430);
    _perDrawImageLayout.setAlignmentType(BufferLayout::AlignmentType::STD430);

    processASL();
}

fs::path Shader::getFile() const { return _file; }

const BufferLayout& Shader::getVertexBufferLayout() const { return _vertexLayout; }
const BufferLayout& Shader::getPerFrameLayout() const { return _perFrameLayout; }
const BufferLayout& Shader::getPerDrawLayout() const { return _perDrawLayout; }
const BufferLayout& Shader::getPerFrameImageLayout() const { return _perFrameImageLayout; }
const BufferLayout& Shader::getPerDrawImageLayout() const { return _perDrawImageLayout; }

void Shader::processASL() {
    // Remove comments
    _aslCode = removeComments(_aslCode);

    // Replace defines
    _aslCode = replaceDefines(_aslCode);

    // Parse custom types
    parseCustomTypes();

    // Populate vertex layout
    populateVertexLayout();

    // Populate fragment layout
    populateFragmentLayout();

    // Populate descriptor layouts
    populateDescriptorLayouts();

    // Process ASL code to generate ICode and APICode
    _shaderCodes.clear();
    std::set<ShaderType> shaderEntrypoints = detectEntrypoints(_aslCode);
    for (ShaderType type : shaderEntrypoints) {
        ShaderCode shaderCode;
        shaderCode.iCode = removeUnusedFunctions(generateICode(type, _aslCode));
        shaderCode.apiCode = "";
        _shaderCodes[type] = shaderCode;
    }
}

std::string Shader::removeComments(std::string code) {
    // Remove single-line comments
    code = std::regex_replace(code, std::regex("//[^\n]*"), "");
    // Remove multi-line comments
    code = std::regex_replace(code, std::regex(R"(/\*([\s\S]*?)\*/)"), "");
    return code;
}

std::string Shader::replaceDefines(std::string code) {
    std::map<std::string, std::string> defines;

    // Populate defines
    std::regex defineRegex(R"(#define\s+(\w+)\s+(\w+)\s+)");
    for (std::sregex_iterator it(code.begin(), code.end(), defineRegex), end; it != end; it++) {
        std::smatch match = *it;
        defines[match[1]] = match[2];
    }

    // Remove #define lines
    code = std::regex_replace(code, defineRegex, "");

    // Replace defines
    for (const auto& [name, value] : defines)
        code = std::regex_replace(code, std::regex("\\b" + name + "\\b"), value);
    return code;
}

std::set<Shader::ShaderType> Shader::detectEntrypoints(std::string code) {
    std::map<ShaderType, int> shaderOccurrences;

    // Regular expressions to match function signatures
    std::regex fragmentRegex(R"(void\s+fragment\s*\((\s*out\s+\w+\s+\w+\s*,?)*\s*\))");
    std::regex vertexRegex(R"(vec4\s+vertex\s*\((\s*\w+\s+\w+\s*,?)*\s*\))");

    // Search for the function signatures in the code and count occurrences
    for (std::sregex_iterator it(code.begin(), code.end(), fragmentRegex), end; it != end; ++it)
        shaderOccurrences[ShaderType::FRAGMENT]++;
    for (std::sregex_iterator it(code.begin(), code.end(), vertexRegex), end; it != end; ++it)
        shaderOccurrences[ShaderType::VERTEX]++;

    std::set<ShaderType> result = {};
    for (auto [type, numOccurences] : shaderOccurrences) {
        if (numOccurences == 1) {
            result.insert(type);
        } else {
            LOG_ERROR("gfx::Shader",
                      "Error found when parsing [w]$0[], there should be only one [w]$1[] entrypoint, but $2 were found. Default entrypoint will "
                      "be used instead",
                      _file.string(), type, numOccurences);
        }
    }
    return result;
}

std::string addToFuncBegin(std::string code, std::string funcName, std::string toAdd) {
    std::regex openBracketRegex("void\\s+" + funcName + "\\s*\\(\\s*\\)\\s*\\{");
    std::smatch match;
    if (std::regex_search(code, match, openBracketRegex)) {
        // Get the position of the opening curly brace
        size_t openBracket = match.position() + match.length();
        code.insert(openBracket + 1, "\n" + toAdd);
    }
    return code;
}

std::string addToFuncEnd(std::string code, std::string funcName, std::string toAdd) {
    std::regex openBracketRegex("void\\s+" + funcName + "\\s*\\(\\s*\\)\\s*\\{");
    std::smatch match;
    if (std::regex_search(code, match, openBracketRegex)) {
        // Get the position of the opening curly brace
        size_t openBracket = match.position() + match.length();

        int nested = 1; // Counter to keep track of nested {}
        size_t insertPosition = std::string::npos;
        for (size_t i = openBracket; i < code.size(); i++) {
            if (code[i] == '{') {
                nested++;
            } else if (code[i] == '}') {
                nested--;
                if (nested == 0) {
                    // Set the insert position to the current index
                    insertPosition = i;
                    break;
                }
            }
        }
        code.insert(insertPosition - 1, "\n" + toAdd);
    }
    return code;
}

std::string Shader::generateICode(ShaderType type, std::string aslCode) {
    std::string iCode = aslCode;
    switch (type) {
        case VERTEX: {
            std::string input;
            std::string params;
            for (size_t i = 0; i < _vertexLayout.getElements().size(); i++) {
                BufferLayout::Element element = _vertexLayout.getElements()[i];
                std::string typeStr = BufferLayout::Element::typeToString(element.type);
                input += "in " + typeStr + " " + element.name + ";\n";
                params += element.name;
                if (i != _vertexLayout.getElements().size() - 1)
                    params += ", ";
            }
            iCode = input + iCode + "void main() { gl_Position = vertex(" + params + "); }";
            break;
        }
        case FRAGMENT: {
            std::string output;
            std::string params;
            for (size_t i = 0; i < _fragmentLayout.getElements().size(); i++) {
                BufferLayout::Element element = _fragmentLayout.getElements()[i];
                std::string typeStr = BufferLayout::Element::typeToString(element.type);
                output += "out " + typeStr + " " + element.name + ";\n";
                params += element.name;
                if (i != _fragmentLayout.getElements().size() - 1)
                    params += ", ";
            }

            iCode = output + aslCode + "void main() { fragment(" + params + "); }";
            break;
        }
        default:
            LOG_WARN("gfx::Shader", "Unknown type $0 when generating ICode", type);
    }
    return iCode;
}

std::string Shader::removeUnusedFunctions(std::string code) {
    struct Func {
        bool used = false;
        size_t start = 0;
        size_t end = 0;
    };
    // Detect all functions and mark them as unused
    std::map<std::string, Func> usedFunctions;

    std::regex functionDefRegex(R"((\w+)\s+(\w+)\s*\(\s*([^)]*)\s*\)\s*\{)");
    std::smatch match;
    auto it = code.cbegin();
    auto end = code.cend();
    size_t offset = 0;
    while (std::regex_search(it, end, match, functionDefRegex)) {
        std::string name = match[2].str();
        Func f = {};

        // Set as unused
        f.used = false;

        // Get start
        f.start = offset + match.position();

        // Get end
        int nested = 0; // Counter to keep track of nested {}
        for (size_t i = f.start; i < code.size(); i++) {
            if (code[i] == '{') {
                nested++;
            } else if (code[i] == '}') {
                nested--;
                if (nested == 0) {
                    f.end = i;
                    break;
                }
            }
        }

        usedFunctions[name] = f;

        // Advance
        offset = f.end;
        it = code.cbegin() + offset;
    }

    // Starting from main, detect used functions
    std::regex functionCallRegex(R"((\w+)\s*\(\s*([^)]*)\s*\))");
    std::stack<std::string> stackCall;
    stackCall.push("main");
    while (!stackCall.empty()) {
        // Get first function from stack
        std::string currFunc = stackCall.top();
        stackCall.pop();
        Func& func = usedFunctions[currFunc];

        // Mark as used
        func.used = true;

        // Detect called functions inside the function
        auto start = code.cbegin() + func.start;
        auto end = code.cbegin() + func.end;
        while (std::regex_search(start, end, match, functionCallRegex)) {
            std::string calledFunc = match[1].str();

            // Add to stack call if it is valid and not used before
            if (usedFunctions.find(calledFunc) != usedFunctions.end() && usedFunctions[calledFunc].used == false)
                stackCall.push(calledFunc);

            // Advance
            start = match.suffix().first;
        }
    }

    for (auto [name, func] : usedFunctions) {
        if (!func.used)
            for (size_t i = func.start; i <= func.end; i++)
                code[i] = ' ';
    }

    // Create final code without unused functions
    return code;
}

void Shader::parseCustomTypes() {
    std::regex structRegex(R"(struct\s+(\w+)\s*\{([^\}]*)\}\s*;)");
    std::regex memberRegex(R"((\w+)\s+(\w+)(\[\d+\])?\s*;)");

    // Match structs
    std::smatch structMatch;
    auto structStart = _aslCode.cbegin();
    while (std::regex_search(structStart, _aslCode.cend(), structMatch, structRegex)) {
        std::string structName = structMatch[1];
        std::string members = structMatch[2];

        // Check if already defined
        if (_customTypes.find(structName) != _customTypes.end()) {
            LOG_WARN("gfx::Shader", "Struct [w]$0[] has multiple definitions", structName);
            structStart = structMatch.suffix().first;
            continue;
        }

        BufferLayout structLayout;
        structLayout.setAlignmentType(BufferLayout::AlignmentType::STD430);

        // Extract members from struct
        std::smatch memberMatch;
        auto memberStart = members.cbegin();
        while (std::regex_search(memberStart, members.cend(), memberMatch, memberRegex)) {
            std::string memberType = memberMatch[1];
            std::string memberName = memberMatch[2];
            size_t memberArraySize = 0;
            if (memberMatch[3] != "") {
                std::string noBracket = std::string(memberMatch[3]).substr(1, std::string(memberMatch[3]).size() - 2);
                memberArraySize = std::stoul(noBracket);
            }

            BufferLayout::Element::Type type = BufferLayout::Element::typeFromString(memberType);
            for (size_t a = 0; a < std::max(size_t(1), memberArraySize); a++) {
                std::string fullMemberName = memberName;
                // Handle array types
                if (memberArraySize) {
                    fullMemberName += "[" + std::to_string(a) + "]";
                    // Array start alignment
                    structLayout.pushStructArrayAlign();
                }

                if (type != BufferLayout::Element::Type::NONE) {
                    // Handle basic type
                    if (type == BufferLayout::Element::Type::SAMPLER_2D || type == BufferLayout::Element::Type::SAMPLER_CUBE) {
                        LOG_WARN("gfx::Shader", "Struct [w]$0[] uses struct [w]$1[], but this type is not supported in structs", memberType,
                                 BufferLayout::Element::typeToString(type));
                        memberStart = memberMatch.suffix().first;
                        continue;
                    }
                    structLayout.push(type, fullMemberName);
                } else {
                    // Handle custom type
                    if (_customTypes.find(structName) != _customTypes.end()) {
                        LOG_WARN("gfx::Shader", "Struct [w]$0[] uses struct [w]$1[], which was not defined yet", structName, memberType);
                        memberStart = memberMatch.suffix().first;
                        continue;
                    }

                    // Handle struct start alignment
                    structLayout.pushStructArrayAlign();

                    const BufferLayout& memberStructLayout = _customTypes[memberType];
                    for (size_t i = 0; i < memberStructLayout.getElements().size(); i++) {
                        const BufferLayout::Element& e = memberStructLayout.getElements().at(i);
                        std::string name = fullMemberName + "." + e.name;
                        structLayout.push(e.type, name, e.align);
                    }

                    // Handle struct end alignment
                    structLayout.pushStructArrayAlign();
                }

                // Handle array end alignment
                if (memberArraySize)
                    structLayout.pushStructArrayAlign();
            }

            memberStart = memberMatch.suffix().first;
        }

        _customTypes[structName] = structLayout;
        structStart = structMatch.suffix().first;
    }
}

void Shader::populateVertexLayout() {
    std::regex regexVertex(R"(\bvertex\s*\()");
    std::smatch match;
    auto start = _aslCode.cbegin();
    auto end = _aslCode.cend();
    if (std::regex_search(start, end, match, regexVertex)) {
        // Calculate start and end
        uint32_t paramStart = match.position() + match.length() - 1;
        uint32_t paramEnd = paramStart;
        while (paramEnd < _aslCode.size() - 1 && _aslCode[paramEnd] != ')')
            paramEnd++;

        start = _aslCode.cbegin() + paramStart;
        end = _aslCode.cbegin() + paramEnd;

        std::regex regexParam(R"(\s*(\w+)\s+(\w+))");
        while (std::regex_search(start, end, match, regexParam)) {
            std::string type = match[1].str();
            std::string name = match[2].str();

            BufferLayout::Element::Type t = BufferLayout::Element::typeFromString(type);
            _vertexLayout.push(t, name);

            // Move to the next match
            start = match.suffix().first;
        }
    }
}

void Shader::populateFragmentLayout() {
    std::regex regexFragment(R"(void\s+fragment\s*\()");
    std::smatch match;
    auto start = _aslCode.cbegin();
    auto end = _aslCode.cend();
    if (std::regex_search(start, end, match, regexFragment)) {
        // Calculate start and end
        uint32_t paramStart = match.position() + match.length() - 1;
        uint32_t paramEnd = paramStart;
        while (paramEnd < _aslCode.size() - 1 && _aslCode[paramEnd] != ')')
            paramEnd++;

        start = _aslCode.cbegin() + paramStart;
        end = _aslCode.cbegin() + paramEnd;

        std::regex regexParam(R"(out\s+(\w+)\s+(\w+))");
        while (std::regex_search(start, end, match, regexParam)) {
            std::string type = match[1].str();
            std::string name = match[2].str();

            BufferLayout::Element::Type t = BufferLayout::Element::typeFromString(type);
            _fragmentLayout.push(t, name);

            // Move to the next match
            start = match.suffix().first;
        }
    }
}

void Shader::populateDescriptorLayouts() {
    std::regex regex(R"(\b(perFrame|perDraw)\s+(\w+)\s+(\w+)(\[\d+\])?\s*;)");
    std::smatch match;

    auto start = _aslCode.cbegin();
    auto end = _aslCode.cend();
    while (std::regex_search(start, end, match, regex)) {
        std::string freq = match[1];
        std::string type = match[2];
        std::string name = match[3];
        size_t arraySize = 0;
        if (match[4] != "") {
            std::string noBracket = std::string(match[4]).substr(1, std::string(match[4]).size() - 2);
            arraySize = std::stoul(noBracket);
        }
        BufferLayout::Element::Type t = BufferLayout::Element::typeFromString(type);

        for (size_t i = 0; i < std::max(size_t(1), arraySize); i++) {
            std::string fullName = name;
            // Handle array types;
            if (arraySize)
                fullName += "[" + std::to_string(i) + "]";

            // Handle array start alignment
            if (arraySize && type != "sampler2D" && type != "samplerCube") {
                if (freq == "perFrame")
                    _perFrameLayout.pushStructArrayAlign();
                else if (freq == "perDraw")
                    _perDrawLayout.pushStructArrayAlign();
            }

            if (t != BufferLayout::Element::Type::NONE) {
                // Handle basic types
                if (type == "sampler2D" || type == "samplerCube") {
                    if (freq == "perFrame")
                        _perFrameImageLayout.push(t, fullName);
                    else if (freq == "perDraw")
                        _perDrawImageLayout.push(t, fullName);
                } else {
                    if (freq == "perFrame")
                        _perFrameLayout.push(t, fullName);
                    else if (freq == "perDraw")
                        _perDrawLayout.push(t, fullName);
                }
            } else if (_customTypes.find(type) != _customTypes.end()) {
                // Handle custom types
                // NOTE: There should be no sampler2D or samplerCube in the struct
                const BufferLayout& structLayout = _customTypes[type];

                // Handle struct start alignment
                if (freq == "perFrame")
                    _perFrameLayout.pushStructArrayAlign();
                else if (freq == "perDraw")
                    _perDrawLayout.pushStructArrayAlign();

                for (size_t i = 0; i < structLayout.getElements().size(); i++) {
                    const BufferLayout::Element& e = structLayout.getElements().at(i);
                    std::string memberName = fullName + "." + e.name;
                    if (freq == "perFrame")
                        _perFrameLayout.push(e.type, memberName, e.align);
                    else if (freq == "perDraw")
                        _perDrawLayout.push(e.type, memberName, e.align);
                }

                // Handle struct end alignment
                if (freq == "perFrame")
                    _perFrameLayout.pushStructArrayAlign();
                else if (freq == "perDraw")
                    _perDrawLayout.pushStructArrayAlign();
            }

            // Handle array end alignment
            if (arraySize && type != "sampler2D" && type != "samplerCube") {
                if (freq == "perFrame")
                    _perFrameLayout.pushStructArrayAlign();
                else if (freq == "perDraw")
                    _perDrawLayout.pushStructArrayAlign();
            }
        }

        // Keep track of types and names
        if (type != "sampler2D" && type != "samplerCube") {
            if (freq == "perFrame") {
                if (arraySize)
                    _perFrameLayoutMembers.emplace_back(type, name, arraySize);
                else
                    _perFrameLayoutMembers.emplace_back(type, name);
            } else if (freq == "perDraw") {
                if (arraySize)
                    _perFrameLayoutMembers.emplace_back(type, name, arraySize);
                else
                    _perDrawLayoutMembers.emplace_back(type, name);
            }
        }

        // Move to the next match
        start = match.suffix().first;
    }
}

Shader::LayoutMember::LayoutMember(std::string type_, std::string name_) : type(type_), name(name_), isArray(false), arraySize(0) {}
Shader::LayoutMember::LayoutMember(std::string type_, std::string name_, size_t arraySize_)
    : type(type_), name(name_), isArray(true), arraySize(arraySize_) {}

} // namespace atta::graphics
