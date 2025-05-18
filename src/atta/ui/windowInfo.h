//--------------------------------------------------
// Atta UI Module
// windowInfo.h
// Date: 2025-05-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_WINDOW_INFO_H
#define ATTA_UI_WINDOW_INFO_H

namespace atta::ui {

/// Captures everything we need to serialize/restore an ImGui window
struct WindowInfo {
    /// How this window is split in its dock parent
    enum class SplitDirection {
        LEFT,
        RIGHT,
        UP,
        DOWN,
        CENTER,
        NONE // E.g. when floating, or uninitialized
    };

    std::string name; ///< ImGui window “Name##ID”
    vec2 size;        ///< Width & height

    // Floating info
    vec2 position;  ///< Top-left corner (only if floating)
    bool collapsed; ///< Is it collapsed right now?

    // Docking info
    std::string parentName;     ///< Dock parent’s window ID (only if docked)
    SplitDirection parentSplit; ///< Parent node dock split direction (only if docked)

    WindowInfo() : position(0, 0), size(0, 0), collapsed(false), parentSplit(SplitDirection::NONE) {}
};

} // namespace atta::ui

#endif // ATTA_UI_WINDOW_INFO_H
