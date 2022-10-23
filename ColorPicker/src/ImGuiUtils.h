#pragma once

#include "imgui.h"

namespace Objects
{
    struct Color;
}

namespace ImGuiUtils
{
    void TextCentered(const char* text);
    bool ButtonCentered(const char* label);

    void CustomSpacing(float value);

    void SelectableColor(ImU32 color);
    void SelectedColor(ImU32 color);

    void HelpMarker(const char* desc);

    void CopyColorToClipboard(const Objects::Color& color);
}
