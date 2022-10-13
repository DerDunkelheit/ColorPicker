#include <windows.h>

#include <imgui_internal.h>
#include <Walnut/Application.h>
#include <Walnut/EntryPoint.h>
#include <Walnut/Image.h>
#include <Walnut/Input/Input.h>

#include "ImGuiUtils.h"
#include "RenderUtils.h"
#include "Events/ActionEvent.h"
#include "ServiceLocator.h"
#include "Managers/ColorsManager.h"
#include "Objects/Objects.h"
#include "Managers/StyleManager.h"
#include "Managers/CursorManager.h"
#include "Walnut/KeyEvents.h"
#include "Managers/AppBehaviourManager.h"
#include "Managers/CursorPosRenderManager.h"
#include "Walnut/GLFWWindowEvents.h"
#include "Walnut/Random.h"
#include "Walnut/Timer.h"

//TODO: to separate file.
#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

static bool isDemoWindowOpened = false;
static bool isGuidWindowOpened = false;
static bool isAboutWindowOpened = false;
static bool isTextWindowOpened = false;

static bool isImageRenderingEnable = false;

class ColorPickerLayer : public Walnut::Layer
{
public:
	ColorPickerLayer(const ServiceLocator::ManagerLocator& managerLocator)
	{
		mStyleManager = managerLocator.resolve<Managers::StyleManager>();
		mCursorManager = managerLocator.resolve<Managers::CursorManager>();
		mColorsManager = managerLocator.resolve<Managers::ColorsManager>();
		mAppBehaviourManager = managerLocator.resolve<Managers::AppBehaviourManager>();

		//Cuz we use static layerLocator we cannot use resolve here, cuz all frames are destroyed when we release image data.
		//TODO: come up with smt.
		mCursorPosRenderManager = std::make_shared<Managers::CursorPosRenderManager>(mCursorManager);
	}
	
	virtual void OnAttach() override
	{
		using namespace Managers;
		mStyleManager->SetAppLookPreferences();
	}
	
	virtual void OnUIRender() override
	{
        ImGui::Begin("ColorPicker", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNavInputs);
        {
        	Objects::CursorPoint cursorPoint;
        	mCursorManager->FillCursorPoint(cursorPoint);
        	
	        char buf[64];
	        sprintf_s(buf, "[%d, %d]", cursorPoint.x, cursorPoint.y);
	        
		    char colorBuf[64];
        	Objects::Color color = cursorPoint.color;
		    sprintf_s(colorBuf, "%d, %d, %d", color.r, color.g, color.b);

        	const auto& colorPreviewInfo = mStyleManager->GetColorPreviewUIInfo();
		   	ImGui::ColorButton("MyColor", ImVec4((float)color.r / 255, (float)color.g / 255, (float)color.b / 255, 255),
		   		0,ImVec2(colorPreviewInfo.mXSize, colorPreviewInfo.mYSize));

            ImGui::SameLine();
        	const auto& zoomRegionUIInfo = mStyleManager->GetZoomRegionUIInfo();
        	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)); // remove padding from render viewport
            ImGui::BeginChild("##ZoomField", ImVec2(zoomRegionUIInfo.mXSize, zoomRegionUIInfo.mYSize), true);
            {
        		//TODO: nice to have: we can create a separate window with rendering of cursor pos with custom zooming and ability to freeze position.
        		mCursorPosRenderManager->RenderMousePosition(cursorPoint);

        		auto mousePosImage = mCursorPosRenderManager->GetImage();
        		if (mousePosImage != nullptr)
        			ImGui::Image(mousePosImage->GetDescriptorSet(), { static_cast<float>(mousePosImage->GetWidth()), static_cast<float>(mousePosImage->GetHeight()) });
            }
        	ImGui::PopStyleVar();
        	ImGui::EndChild();
            
            //-----------------------------------------------------------
            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_Border, ImColor(80, 0, 0, 255).Value);
        	const auto& pickedColorsUIInfo = mStyleManager->GetPickedColorsUIInfo();
            ImGui::BeginChild("##ColorList", ImVec2(pickedColorsUIInfo.mXSize, pickedColorsUIInfo.mYSize), true);
            {
                for (int i = 0; i < mColorsManager->GetPickedColors().size(); i++)
                {
	                const bool selected = mColorsManager->GetSelectedColorIndex() == i;
                	auto pickedColors = mColorsManager->GetPickedColors();
                    const Objects::Color& currentColor = pickedColors[i];

                	//todo: string in render loop.
                    std::string label = std::to_string(i);
                    
                    if (ImGui::Selectable(label.data()))
                    {
                        mColorsManager->SetSelectedColor(i);
                    }

                	if (ImGui::BeginPopupContextItem())
                	{
                		mColorsManager->SetSelectedColor(i);
                		
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
                        ImGuiUtils::SelectedColor(ImGui::GetColorU32(mStyleManager->GetColorThemeBasedOnColor(currentColor)));
                    }
                    else
                    {
                        ImGuiUtils::SelectableColor(IM_COL32(currentColor.r, currentColor.g, currentColor.b, 255));
                    }

                    ImGui::SameLine();
                    char colorBuf[64];
                    sprintf_s(colorBuf, "%d, %d, %d", currentColor.r, currentColor.g, currentColor.b);
                	ImGui::TextColored(mStyleManager->GetColorThemeBasedOnColor(currentColor), colorBuf);
                }
            }
            ImGui::PopStyleColor();
            ImGui::EndChild();

        	// to be hones, looks like hack...
			ImGui::SetCursorPosY(ImGui::GetContentRegionAvail().y / 2 - 80);
        	ImGui::Text(colorBuf);
        	ImGui::Text(buf);
        	ImGuiIO& io = ImGui::GetIO();
        	ImGui::Text("%.1f FPS", io.Framerate);
        	ImGui::SetCursorPosY(ImGui::GetContentRegionAvail().y / 2 );
            //-----------------------------------------------------------

            //-------------------------------------------------------------------
            const char* items[] = { "RGB" };
            static int item_current = 0;
            ImGui::PushItemWidth(mStyleManager->GetColorsComboWidth());
            ImGui::Combo("##ColorOptions", &item_current, items, IM_ARRAYSIZE(items));

            ImGui::SameLine();
            ImGui::PushItemWidth(mStyleManager->GetHotkeyWidth());
            ImGui::Text("Hotkey: X");

            ImGui::SameLine();
            if ( ImGui::Button("Copy value", ImVec2(ImGui::GetContentRegionAvail().x, 25))) { }
            ImGuiUtils::CustomSpacing(5);
            ImGui::Separator();
			//-------------------------------------------------------------------

        	ImGui::BeginChild("##PreviewColorRegion", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true);
        	{
        		ImGui::ColorButton("PreviewColor", ImVec4(255 /255, 255/255, 255/ 255, 255),
				   0,ImVec2(96, 54));

        		ImGui::SameLine();
        		ImGui::BeginChild("##test", ImVec2(ImGui::GetContentRegionAvail().x, 54), true);
        		{
        			ImGuiUtils::CustomSpacing(3);
        			
        			static int e = 0;
        			ImGui::RadioButton("RGB", &e, 0); ImGui::SameLine(0,25);
        			ImGui::RadioButton("HSV", &e, 1); ImGui::SameLine(0,25);
        			ImGui::RadioButton("HSL", &e, 2);
        		}
        		ImGui::EndChild();

        		static int i1 = 0;
        		static int i2 = 0;
        		static int i3 = 0;
        		static int maxVal = 255; //TODO: get from manager based on current color displaying format
        								//TODO: get format for slider from manager based on color format as well.
        		static float sliderSize = 300;
        		ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - sliderSize / 2);
        		ImGui::PushItemWidth(sliderSize);
        		if (ImGui::SliderInt("##first slider", &i1, 0, maxVal, "R: %d"))
        		{
        			if (i1 > maxVal) i1 = maxVal;
        			if (i1 < 0) i1 = 0;
        		}
        		ImGui::SameLine(); ImGuiUtils::HelpMarker("CTRL+click to input value.");
        		ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - sliderSize / 2);
        		ImGui::PushItemWidth(sliderSize);
        		if (ImGui::SliderInt("##second slider", &i2, 0, maxVal, "G: %d"))
        		{
        			if (i2 > maxVal) i2 = maxVal;
        			if (i2 < 0) i2 = 0;
        		}
        		ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - sliderSize / 2);
        		ImGui::PushItemWidth(sliderSize);
        		if (ImGui::SliderInt("##third slider", &i3, 0, maxVal, "B: %d"))
        		{
        			if (i3 > maxVal) i3 = maxVal;
        			if (i3 < 0) i3 = 0;
        		}

        		ImGuiUtils::CustomSpacing(3);
        		ImGui::Button("Copy Preview Color", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));
        	}
        	ImGui::EndChild();
        }
        ImGui::End();

		if (isDemoWindowOpened)
		{
			ImGui::ShowDemoWindow(&isDemoWindowOpened);
		}
		if (isGuidWindowOpened)
		{
			RenderUtils::RenderGuideWindow(&isGuidWindowOpened);
		}
		if (isAboutWindowOpened)
		{
			RenderUtils::RenderAboutWindow(&isAboutWindowOpened);
		}
		if (isTextWindowOpened)
		{
			RenderUtils::RenderTextWindow(&isTextWindowOpened);
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
			mCursorManager->FillCursorPoint(cursorPoint);

			mColorsManager->AddPickedColor(cursorPoint.color);
		}

		return handled;
	}
	
private:
	std::shared_ptr<Managers::StyleManager> mStyleManager;
	std::shared_ptr<Managers::ColorsManager> mColorsManager;
	std::shared_ptr<Managers::CursorManager> mCursorManager;
	std::shared_ptr<Managers::AppBehaviourManager> mAppBehaviourManager;
	std::shared_ptr<Managers::CursorPosRenderManager> mCursorPosRenderManager;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Color Picker";
	spec.Width = 450;
	spec.Height = 450;
	spec.CanResize = false;
	
	static bool isAutoColorsSaveEnabled = true;
	static bool isStayOnTop = false;

	using namespace Managers;
	static ServiceLocator::ManagerLocator managerLocator;

	//todo: maybe store it inside app manager???
	managerLocator.registerInstance<StyleManager>(new StyleManager);
	managerLocator.registerInstance<CursorManager>(new CursorManager);
	managerLocator.registerInstance<ColorsManager>(new ColorsManager);
	managerLocator.registerInstance<AppBehaviourManager>(new AppBehaviourManager);
	managerLocator.registerInstance<CursorPosRenderManager>(new CursorPosRenderManager);
	
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
			if (ImGui::MenuItem("Stay on Top", nullptr, &isStayOnTop))
			{
				WindowFlagEvent flagEvent("StayOnTop", isStayOnTop);
				app->OnEvent(flagEvent);
			}
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Tools"))
		{
			ImGui::MenuItem("Text", nullptr, &isTextWindowOpened);
			
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Color List"))
		{
			auto colorsManager = managerLocator.resolve<ColorsManager>();
			if (ImGui::MenuItem("Clear All",nullptr, nullptr, colorsManager->CanClearColors()))
			{
				managerLocator.resolve<ColorsManager>()->ClearPickedColors();
			}
			if (ImGui::MenuItem("Auto Save", nullptr, &isAutoColorsSaveEnabled))
			{
				bool enabled = isAutoColorsSaveEnabled;
				// todo: use manager different manage?
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