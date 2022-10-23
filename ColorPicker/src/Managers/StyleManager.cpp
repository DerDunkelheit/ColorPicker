#include "StyleManager.h"

#include <imgui.h>

#include "Objects/Objects.h"

namespace Managers
{
    StyleManager::~StyleManager()
    {
        
    }

    void StyleManager::SetAppLookPreferences()
    {
        ImGuiStyle& style = ImGui::GetStyle();

        //TODO: would be great to read those vars from a file.
        style.WindowMinSize = ImVec2(450, 295);

        style.Colors[ImGuiCol_TitleBg] = ImColor(255, 101, 53);
        style.Colors[ImGuiCol_TitleBgActive] = ImColor(255, 101, 53);

        style.Colors[ImGuiCol_WindowBg] = ImColor(16, 16 ,16);
        style.Colors[ImGuiCol_ChildBg] = ImColor(24, 24, 24);
        style.Colors[ImGuiCol_Text] = ImColor(255, 255, 255);
        style.Colors[ImGuiCol_CheckMark] = ImColor(255, 255, 255);

        style.Colors[ImGuiCol_Header] = ImColor(35,35,35);
        style.Colors[ImGuiCol_HeaderActive] = ImColor(33,33,33);
        style.Colors[ImGuiCol_HeaderHovered] = ImColor(33,33,33);

        style.Colors[ImGuiCol_Button] = ImColor(35,35,35);
        style.Colors[ImGuiCol_ButtonActive] = ImColor(38,38,38);
        style.Colors[ImGuiCol_ButtonHovered] = ImColor(40,40,40);

        style.Colors[ImGuiCol_Border] = ImColor(0, 0, 0, 255);

        style.Colors[ImGuiCol_FrameBg] = ImColor(30, 30, 30);
        style.Colors[ImGuiCol_FrameBgHovered] = ImColor(35, 35, 35);
        style.Colors[ImGuiCol_FrameBgActive] = ImColor(35, 35, 35);

        style.Colors[ImGuiCol_SliderGrab] = ImColor(115, 115, 115);
        style.Colors[ImGuiCol_SliderGrabActive] = ImColor(128, 128, 128);

        style.FrameRounding = 6;

        style.ScrollbarSize = 10;
    }

    ImVec4 StyleManager::GetColorThemeBasedOnColor(const Objects::Color& color)
    {
        if (color.r > 200 || color.g > 200 || color.b > 200)
        {
            return {0,0,0, 255};
        }

        return {255,255,255, 255};
    }
}
