//--------------------------------------------------
// Atta UI System
// fileSelectionWindow.h
// Date: 2022-01-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_SYSTEM_LAYERS_EDITOR_WINDOWS_UTILS_FILE_SELECTION_WINDOW_H
#define ATTA_UI_SYSTEM_LAYERS_EDITOR_WINDOWS_UTILS_FILE_SELECTION_WINDOW_H
#include <atta/uiSystem/layers/editor/window.h>

namespace atta::ui
{
    class FileSelectionWindow : public Window<FileSelectionWindow>
    {
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
}

#endif// ATTA_UI_SYSTEM_LAYERS_EDITOR_WINDOWS_UTILS_FILE_SELECTION_WINDOW_H
