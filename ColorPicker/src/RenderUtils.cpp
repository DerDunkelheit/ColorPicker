#include "RenderUtils.h"

#include "ImGuiUtils.h"

namespace RenderUtils
{
    void RenderGuideWindow(bool* p_open)
    {
        ImGui::Begin("Guide", p_open, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse);
        {
            
        }
        ImGui::End();
    }

    void RenderAboutWindow(bool* p_open)
    {
        ImGui::Begin("About", p_open, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse);
        {
            
        }
        ImGui::End();
    }

    void RenderTextWindow(bool* p_open)
    {
        ImGui::Begin("Text", p_open, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse);
        {
            
        }
        ImGui::End();
    }
}
