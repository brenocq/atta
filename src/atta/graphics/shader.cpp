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

    processASL();
}

fs::path Shader::getFile() const { return _file; }

const BufferLayout& Shader::getVertexBufferLayout() const { return _vertexLayout; }
const BufferLayout& Shader::getPerFrameBufferLayout() const { return _perFrameLayout; }
const BufferLayout& Shader::getPerDrawBufferLayout() const { return _perDrawLayout; }
const BufferLayout& Shader::getImageBufferLayout() const { return _imageLayout; }

void Shader::processASL() {
    LOG_DEBUG("gfx::Shader", "Preprocess ASL: [w]$0", _file.string());

    // Remove comments
    _aslCode = removeComments(_aslCode);

    // Populate vertex layout
    populateVertexLayout();

    // Populate perFrame layout
    populatePerFrameLayout();

    // Populate perDraw layout
    populatePerDrawLayout();

    // Populate uniform layout
    populateImageLayout();

    // Process ASL code to generate ICode and APICode
    _shaderCodes.clear();
    std::set<ShaderType> shaderEntrypoints = detectEntrypoints(_aslCode);
    for (ShaderType type : shaderEntrypoints) {
        ShaderCode shaderCode;
        shaderCode.iCode = removeUnusedFunctions(generateICode(type, _aslCode));
        shaderCode.apiCode = "";
        // LOG_DEBUG("gfx::Shader", "[y]$0\n[w]$1", type, shaderCode.iCode);
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

std::set<Shader::ShaderType> Shader::detectEntrypoints(std::string code) {
    std::map<ShaderType, int> shaderOccurrences;

    // Regular expressions to match function signatures
    std::regex fragmentRegex(R"(vec4\s+fragment\s*\(\s*\))");
    std::regex vertexRegex(R"(vec4\s+vertex\s*\((\s*\w+\s+\w+\s*,?)*\s*\))");

    // Search for the function signatures in the code and count occurrences
    for (std::sregex_iterator it(code.begin(), code.end(), fragmentRegex), end; it != end; ++it) {
        shaderOccurrences[ShaderType::FRAGMENT]++;
    }
    for (std::sregex_iterator it(code.begin(), code.end(), vertexRegex), end; it != end; ++it) {
        shaderOccurrences[ShaderType::VERTEX]++;
    }

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
            iCode = input + iCode + "void main() { POSITION = vertex(" + params + ");}";

            // Replace
            iCode = std::regex_replace(iCode, std::regex(R"(\bPOSITION)"), "gl_Position");
            iCode = std::regex_replace(iCode, std::regex(R"(\bPOINT_SIZE)"), "gl_PointSize");

            break;
        }
        case FRAGMENT: {
            iCode = aslCode + "out vec4 COLOR;\nvoid main() { COLOR = fragment();}";

            // Replace
            iCode = std::regex_replace(iCode, std::regex(R"(\bDEPTH)"), "gl_FragDepth");
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

            // LOG_DEBUG("Shader", "Found vertex input: $0 $1", type, name);
            BufferLayout::Element::Type t = BufferLayout::Element::typeFromString(type);
            _vertexLayout.push(t, name);

            // Move to the next match
            start = match.suffix().first;
        }
    }
}

void Shader::populatePerFrameLayout() {
    std::regex regex(R"(\bperFrame\s+(\w+)\s+(\w+)\s*;)");
    std::smatch match;

    auto start = _aslCode.cbegin();
    auto end = _aslCode.cend();
    while (std::regex_search(start, end, match, regex)) {
        std::string type = match[1];
        std::string name = match[2];
        if (type != "sampler2D" && type != "samplerCube") {
            LOG_DEBUG("Shader", "Found perFrame: $0 $1", type, name);
            BufferLayout::Element::Type t = BufferLayout::Element::typeFromString(type);
            _perFrameLayout.push(t, name);
        }

        // Move to the next match
        start = match.suffix().first;
    }
}

void Shader::populatePerDrawLayout() {
    std::regex regex(R"(\bperDraw\s+(\w+)\s+(\w+)\s*;)");
    std::smatch match;

    auto start = _aslCode.cbegin();
    auto end = _aslCode.cend();
    while (std::regex_search(start, end, match, regex)) {
        std::string type = match[1];
        std::string name = match[2];
        if (type != "sampler2D" && type != "samplerCube") {
            LOG_DEBUG("Shader", "Found perDraw: $0 $1", type, name);
            BufferLayout::Element::Type t = BufferLayout::Element::typeFromString(type);
            _perDrawLayout.push(t, name);
        }

        // Move to the next match
        start = match.suffix().first;
    }
}

void Shader::populateImageLayout() {
    std::regex regex(R"(\buniform\s+(sampler2D|samplerCube)\s+(\w+)\s*;)");
    std::smatch match;

    auto start = _aslCode.cbegin();
    auto end = _aslCode.cend();
    while (std::regex_search(start, end, match, regex)) {
        std::string type = match[1];
        std::string name = match[2];
        // LOG_DEBUG("Shader", "Found image uniform: $0 $1", type, name);
        BufferLayout::Element::Type t = BufferLayout::Element::typeFromString(type);
        _imageLayout.push(t, name);

        // Move to the next match
        start = match.suffix().first;
    }
}

} // namespace atta::graphics
