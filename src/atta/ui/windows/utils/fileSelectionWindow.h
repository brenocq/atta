// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

#include <atta/ui/windows/window.h>

namespace atta::ui {

class FileSelectionWindow : public Window<FileSelectionWindow> {
  public:
    void setPath(fs::path currPath);
    static fs::path getChosenFile();

  private:
    FileSelectionWindow();

    void setPathImpl(fs::path currPath);
    void renderImpl();

    void renderFileTree();

    fs::path _currPath;
    fs::path _selectedFile;
    fs::path _chosenFile;
    std::array<char, 200> _currPathBuffer;

    friend Window<FileSelectionWindow>;
};

} // namespace atta::ui
