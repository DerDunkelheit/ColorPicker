#include "RenderUtils.h"

#include "ImGuiUtils.h"
#include "Utils.h"
#include "Objects/Objects.h"

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
            ImGuiUtils::TextCentered("Author: Aleksandr Samarin");
            ImGui::Selectable("Project Github page: github.com/DerDunkelheit/ColorPicker");
            if (ImGui::IsItemHovered())
            {
                ImGui::SetTooltip("Click to open, right click to copy URL");

                if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                {
                    Utils::OpenWebPage("https://github.com\\DerDunkelheit\\ColorPicker");
                }
                else if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
                {
                    ImGui::LogToClipboard();
                    ImGui::LogText("github.com/DerDunkelheit/ColorPicker");
                    ImGui::LogFinish();
                }
            }
            ImGui::Spacing();
            ImGui::TextWrapped(
                "It was a hobby/learning project with an aim to get smooth image preview capturing when picking a color."
                "This project was also a place to implement and test new techies and approaches in programming with C++");
        }
        ImGui::End();
    }

    void RenderTextWindow(bool* p_open)
    {
        //TODO: finish this idea.
        ImGui::Begin("Text", p_open, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse);
        {
            ImGuiIO& io = ImGui::GetIO();

            const char* items[] = { "25", "50" };
            static int item_current = 0;
            ImGui::Text("Font size: ");
            ImGui::SameLine();
            ImGui::Combo("##FontSize", &item_current, items, IM_ARRAYSIZE(items));

            ImGui::BeginChild("##TextPreview", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true);
            {
                switch (item_current)
                {
                case 0:
                    ImGui::PushFont(io.Fonts->Fonts[1]);
                    break;
                
                case 1:
                    ImGui::PushFont(io.Fonts->Fonts[2]);
                    break;
                }
                ImGui::Text("Hello with another font");
                ImGui::PopFont();
            }
            ImGui::EndChild();
        }
        ImGui::End();
    }

    void RenderPerformanceWindow(bool* p_open, const Objects::PerformanceWindowData& windowData)
    {
        ImGui::Begin("Performance", p_open, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse);
        {
            ImGui::Text("CaptureScreen() time: %.8f", windowData.captureScreenTime);
            ImGui::Text("FillCursorPoint() time: %.8f", windowData.FillCursorPointTime);
        }
        ImGui::End();
    }
}
