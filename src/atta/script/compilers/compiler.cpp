//--------------------------------------------------
// Atta Script Module
// compiler.cpp
// Date: 2021-09-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/file/interface.h>
#include <atta/script/compilers/compiler.h>

namespace atta::script {

std::vector<StringId> Compiler::getTargets() const {
    std::vector<StringId> targets;

    for (auto& target : _targetFiles)
        targets.push_back(target.first);

    return targets;
}

std::vector<fs::path> Compiler::getIncludedFiles(fs::path file) {
    fs::path projectDir = file::getProject()->getDirectory();
    fs::path projectDirSrc0 = projectDir / "src";
    fs::path projectDirSrc1 = projectDir / "source";
    std::vector<fs::path> possibleBases = {file.parent_path(), projectDirSrc0, projectDirSrc1, projectDir};
    std::unordered_set<std::string> allFiles;
    std::stack<fs::path> s;
    s.push(file);

    while (!s.empty()) {
        file = s.top();
        s.pop();
        std::ifstream in(file);
        std::string line;
        // For each file
        while (std::getline(in, line)) {
            if (line[0] == '#' && line.find("#include") != std::string::npos) {
                // If found #include, get file name
                size_t posBegin = line.find('"') != std::string::npos ? line.find('"') : line.find('<');
                if (posBegin != std::string::npos) {
                    size_t posEnd = line.find('"', posBegin+1) != std::string::npos ? line.find('"', posBegin+1) : line.find('>', posBegin+1);
                    if (posEnd != std::string::npos) {
                        std::string possibleFile = line.substr(posBegin + 1, posEnd - posBegin - 1);
                        for (auto base : possibleBases) {
                            fs::path possiblePath = base / possibleFile;
                            // Check if file exists
                            if (fs::exists(possiblePath)) {
                                if (allFiles.find(possiblePath.string()) == allFiles.end()) {
                                    allFiles.insert(possiblePath.string());
                                    s.push(possiblePath);
                                }
                                break;
                            }
                        }
                    }
                }
            }
        }
        in.close();
    }

    return std::vector<fs::path>(allFiles.begin(), allFiles.end());
}

} // namespace atta::script
