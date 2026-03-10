// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

#include <atta/ui/windows/window.h>
#include <string>
#include <vector>

namespace atta::ui {

class ExamplesWindow : public Window<ExamplesWindow> {
  public:
    struct ExampleInfo {
        std::string title;
        std::string description;
        fs::path attaFile;
    };

  private:
    ExamplesWindow();
    void renderImpl();

    void scanExamples();
    static bool parseReadme(const fs::path& readme, std::string& title, std::string& description);

    std::vector<ExampleInfo> _examples;

    friend Window<ExamplesWindow>;
};

} // namespace atta::ui
