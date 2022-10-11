#include <windows.h>

#include <imgui_internal.h>
#include <Walnut/Application.h>
#include <Walnut/EntryPoint.h>
#include <Walnut/Image.h>
#include <Walnut/Input/Input.h>

#include "ImGuiUtils.h"
#include "Events/ActionEvent.h"
#include "ServiceLocator.h"
#include "Managers/ColorsManager.h"
#include "Objects/Objects.h"
#include "Managers/StyleManager.h"
#include "Managers/CursorManager.h"
#include "Walnut/KeyEvents.h"

//TODO: to separate file.
#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

// windows toggles todo: to style manager
static bool isDemoWindowOpened = false;
static bool isGuidWindowOpened = false;
static bool isAboutWindowOpened = false;

class ColorPickerLayer : public Walnut::Layer
{
public:
	ColorPickerLayer(const ServiceLocator::ManagerLocator& managerLocator)
		:mManagerLocator(managerLocator) { }
	
	virtual void OnAttach() override
	{
		using namespace Managers;
		mManagerLocator.resolve<StyleManager>()->SetAppLookPreferences();
	}
	
	virtual void OnUIRender() override
	{
        ImGui::Begin("ColorPicker", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNavInputs);
        {
        	Objects::CursorPoint cursorPoint;
        	mManagerLocator.resolve<Managers::CursorManager>()->FillCursorPoint(cursorPoint);
        	
	        char buf[64];
	        sprintf_s(buf, "Mouse Pos: x: %d, y: %d", cursorPoint.x, cursorPoint.y);
	        ImGui::Text(buf);
	        
		    char colorBuf[64];
        	Objects::Color color = cursorPoint.color;
		    sprintf_s(colorBuf, "%d, %d, %d", color.r, color.g, color.b);
		    ImGui::Text(colorBuf);
        	
		   	ImGui::ColorButton("MyColor", ImVec4((float)color.r / 255, (float)color.g / 255, (float)color.b / 255, 255), 0,ImVec2(75, 75));

            ImGui::SameLine();
            ImGui::BeginChild("##ZoomField", ImVec2(120, 120), true);
            {
                ImGui::Text("TODO: implement\nzooming...");
            }
            ImGui::EndChild();
            
            //-----------------------------------------------------------
            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_Border, ImColor(80, 0, 0, 255).Value);
            ImGui::BeginChild("##ColorList", ImVec2(200, 120), true);
            {
                const auto colorManager = mManagerLocator.resolve<Managers::ColorsManager>();
                for (int i = 0; i < colorManager->GetPickedColors().size(); i++)
                {
	                const bool selected = colorManager->GetSelectedColorIndex() == i;
                	auto pickedColors = colorManager->GetPickedColors();
                    const Objects::Color& currentColor = pickedColors[i];

                	//todo: string in render loop.
                    std::string label = std::to_string(i);
                    
                    if (ImGui::Selectable(label.data()))
                    {
                        colorManager->SetSelectedColor(i);
                    }

                	if (ImGui::BeginPopupContextItem())
                	{
                		//TODO: implement popup logic
                		ImGui::Text("This a popup for %d, %d, %d", currentColor.r, currentColor.g, currentColor.b);
                		if (ImGui::Button("Copy to clipboard")) ImGui::CloseCurrentPopup();
                		if (ImGui::Button("Add comment")) ImGui::CloseCurrentPopup();
                		if (ImGui::Button("Delete color")) ImGui::CloseCurrentPopup();

                		ImGui::EndPopup();
					}

                    if (selected)
                    {
                        ImGuiUtils::SelectableColor(IM_COL32(currentColor.r, currentColor.g, currentColor.b, 255));
                        ImGuiUtils::SelectedColor(IM_COL32(255, 255, 255, 255));
                    }
                    else
                    {
                        //TODO change selected border to dark when color is to bright.
                        ImGuiUtils::SelectableColor(IM_COL32(currentColor.r, currentColor.g, currentColor.b, 255));
                    }

                    ImGui::SameLine();
                    //TODO change color text to dark when color is to bright.
                    char colorBuf[64];
                    sprintf_s(colorBuf, "%d, %d, %d", currentColor.r, currentColor.g, currentColor.b);
                    ImGui::Text(colorBuf);
                }
            }
            ImGui::PopStyleColor();
            ImGui::EndChild();
            //-----------------------------------------------------------

            //-------------------------------------------------------------------
            ImGuiUtils::CustomSpacing(10);
            const char* items[] = { "RGB" };
            static int item_current = 0;
            ImGui::PushItemWidth(75); //TODO: we use 75 like with color button, replace it to manager.
            ImGui::Combo("##ColorOptions", &item_current, items, IM_ARRAYSIZE(items));

            ImGui::SameLine();
            ImGui::PushItemWidth(120); // same todo:
            ImGui::Text("Hotkey: X");

            ImGui::SameLine();
            ImGui::Button("Copy value", ImVec2(ImGui::GetContentRegionAvail().x, 25));
            ImGuiUtils::CustomSpacing(5);
            ImGui::Separator();
			//-------------------------------------------------------------------
        }
        ImGui::End();

		if (isDemoWindowOpened)
		{
			ImGui::ShowDemoWindow(&isDemoWindowOpened);
		}
	}

	virtual void OnEvent(Event& event) override
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<ActionEvent>(BIND_EVENT_FN(ColorPickerLayer::HandleAction));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(ColorPickerLayer::HandleInput));
	}

private:
	bool HandleAction(const ActionEvent& actionEvent)
	{
		bool handled = false;
		
		return handled;
	}

	bool HandleInput(const KeyPressedEvent& keyPressed)
	{
		bool handled = false;
		//TODO: support for shortcuts and custom user's input key
		//TODO: input listening even on minimized window.
		if (keyPressed.GetKeyCode() == static_cast<int32_t>(Walnut::KeyCode::X))
		{
			Objects::CursorPoint cursorPoint;
			mManagerLocator.resolve<Managers::CursorManager>()->FillCursorPoint(cursorPoint);

			mManagerLocator.resolve<Managers::ColorsManager>()->AddPickedColor(cursorPoint.color);
		}

		return handled;
	}
	
private:
	const ServiceLocator::ManagerLocator& mManagerLocator;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Color Picker";
	spec.Width = 450;
	spec.Height = 480;
	spec.CanResize = false;

	//Create locator here and all managers and put as an arg to the layer.

	// logic toggles for ui
	static bool isAutoColorsSaveEnabled = true;

	using namespace Managers;
	static ServiceLocator::ManagerLocator managerLocator;
	
	managerLocator.registerInstance<StyleManager>(new StyleManager);
	managerLocator.registerInstance<CursorManager>(new CursorManager);
	managerLocator.registerInstance<ColorsManager>(new ColorsManager);
	
	Walnut::Application* app = new Walnut::Application(spec);
	std::shared_ptr<ColorPickerLayer> mainLayer = std::make_shared<ColorPickerLayer>(managerLocator);
	app->PushLayer(mainLayer);
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("Options"))
		{
#ifdef WL_DEBUG
			ImGui::MenuItem("ShowDemoWindow", nullptr, &isDemoWindowOpened);
#endif // WL_DEBUG
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Color List"))
		{
			//TODO: mb different manager???
			auto colorsManager = managerLocator.resolve<ColorsManager>();
			if (ImGui::MenuItem("Clear All",nullptr, nullptr, colorsManager->CanClearColors()))
			{
				managerLocator.resolve<ColorsManager>()->ClearPickedColors();
			}
			if (ImGui::MenuItem("Auto Save", nullptr, &isAutoColorsSaveEnabled))
			{
				bool enabled = isAutoColorsSaveEnabled;
				// todo: use manager
			}
				
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			ImGui::MenuItem("GuideWindow", nullptr, &isGuidWindowOpened);
			ImGui::MenuItem("About", nullptr, &isAboutWindowOpened);
			
			ImGui::EndMenu();
		}
	});
	return app;
}