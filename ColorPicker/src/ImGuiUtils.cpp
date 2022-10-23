#include "ImGuiUtils.h"

#include <iostream>

#include "imgui.h"
#include "Objects/Objects.h"

namespace ImGuiUtils
{
    float CalculateCenterWidth(const char* text)
    {
        const auto contentWidth = ImGui::GetContentRegionAvail().x;
        const auto textWidth   = ImGui::CalcTextSize(text).x;

        return contentWidth / 2 - textWidth / 2;
    }
    
    void TextCentered(const char* text)
    {
        ImGui::SetCursorPosX(CalculateCenterWidth(text));
        ImGui::Text(text);
    }

    bool ButtonCentered(const char* label)
    {
        ImGui::SetCursorPosX(CalculateCenterWidth(label));
        return ImGui::Button(label);
    }

    void CustomSpacing(float value)
    {
        ImGui::Dummy(ImVec2(0, value));
    }

    void SelectableColor(ImU32 color)
    {
        ImVec2 min = ImGui::GetItemRectMin();
        ImVec2 max = ImGui::GetItemRectMax();
        ImGui::GetWindowDrawList()->AddRectFilled(min, max, color);
    }

    void SelectedColor(ImU32 color)
    {
        ImVec2 min = ImGui::GetItemRectMin();
        ImVec2 max = ImGui::GetItemRectMax();
        ImGui::GetWindowDrawList()->AddRect(min, max, color);
    }

    void HelpMarker(const char* desc)
    {
        ImGui::TextDisabled("(?)");
        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(desc);
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
    }

    void CopyColorToClipboard(const Objects::Color& color)
    {
        ImGui::LogToClipboard();
        
        char copiedBuf[64];
        color.HasComment() ?
            sprintf_s(copiedBuf, "%d, %d, %d, %s", color.r, color.g, color.b, color.comment.c_str())
            : sprintf_s(copiedBuf, "%d, %d, %d", color.r, color.g, color.b);
                			
        ImGui::LogText(copiedBuf);
        ImGui::LogFinish();
    }
}
