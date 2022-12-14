#include <windows.h>

#include <imgui_internal.h>
#include <Walnut/CoreMacro.h>
#include <Walnut/Application.h>
#include <Walnut/EntryPoint.h>
#include <Walnut/Image.h>
#include <Walnut/Input/Input.h>
#include <Walnut/KeyEvents.h>
#include <Walnut/GLFWWindowEvents.h>
#include <Walnut/Random.h>

#include "ImGuiUtils.h"
#include "Events/ActionEvent.h"
#include "RenderUtils.h"
#include "ServiceLocator.h"
#include "Objects/Objects.h"
#include "Managers/ColorsManager.h"
#include "Managers/StyleManager.h"
#include "Managers/CursorManager.h"
#include "Managers/AppBehaviourManager.h"
#include "Managers/CursorPosRenderManager.h"
#include "Managers/PreviewColorManager.h"

// Windows
static bool isGuidWindowOpened = false;
static bool isAboutWindowOpened = false;
static bool isTextWindowOpened = false;
static bool isPerformanceWindowOpened = false;

#ifdef WL_DEBUG
static bool isDemoWindowOpened = false;
#endif

// Popups
static bool isColorCommentPopup = false;

//General todos:
//TODO: maybe implement a localization support.
//TODO: try to refactor this file to make it smaller.

static uint32_t allocCount = 0;
void* operator new (size_t size)
{
	allocCount++;
#ifdef WL_DEBUG
	std::cout << "Allocation " << size << " bytes\n";
#endif
	return malloc(size);
}

static uint32_t deleteCount = 0;
void operator delete (void* memory, size_t size)
{
	deleteCount++;
	free(memory);
}

class ColorPickerLayer : public Walnut::Layer
{
public:
	ColorPickerLayer(const ServiceLocator::ManagerLocator& managerLocator)
	{
		using namespace Managers;
		mStyleManager = managerLocator.resolve<StyleManager>();
		mCursorManager = managerLocator.resolve<CursorManager>();
		mColorsManager = managerLocator.resolve<ColorsManager>();
		mAppBehaviourManager = managerLocator.resolve<AppBehaviourManager>();
		mCursorPosRenderManager = managerLocator.resolve<CursorPosRenderManager>();
		mPreviewColorManager = managerLocator.resolve<PreviewColorManager>();
	}
	
	virtual void OnAttach() override
	{
		mStyleManager->SetAppLookPreferences();
		mCursorManager->Init(&mPerformanceWindowData);
		mCursorPosRenderManager->Init(&mPerformanceWindowData);
	}

	virtual void OnDetach() override
	{
		// some resources should be cleaned manually
		mCursorPosRenderManager->CleanUpResources();
	}
	
	virtual void OnUIRender() override
	{
        ImGui::Begin("ColorPicker", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNavInputs);
        {
        	mCursorManager->FillCursorPoint(mCursorPoint);
        	
	        char buf[64];
	        sprintf_s(buf, "[%d, %d]", mCursorPoint.x, mCursorPoint.y);
	        
		    char colorBuf[64];
        	const Objects::Color& color = mCursorPoint.color;
		    sprintf_s(colorBuf, "%d, %d, %d", color.r, color.g, color.b);

        	const auto& colorPreviewInfo = mStyleManager->GetColorPreviewUIInfo();
		   	ImGui::ColorButton("MyColor", ImVec4((float)color.r / 255, (float)color.g / 255, (float)color.b / 255, 255),
		   		ImGuiColorEditFlags_NoTooltip,ImVec2(colorPreviewInfo.mXSize, colorPreviewInfo.mYSize));

            ImGui::SameLine();
        	const auto& zoomRegionUIInfo = mStyleManager->GetZoomRegionUIInfo();
        	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)); // remove padding from render viewport
            ImGui::BeginChild("##ZoomField", ImVec2(zoomRegionUIInfo.mXSize, zoomRegionUIInfo.mYSize), true);
            {
        		//TODO: nice to have: we can create a separate window with rendering of cursor pos with custom zooming and ability to freeze position.
        		mCursorPosRenderManager->RenderMousePosition(mCursorPoint);
        		
        		if (mCursorPosRenderManager->HasImage())
        		{
        			const Walnut::Image& renderImage = mCursorPosRenderManager->GetImage();
	                ImGui::Image(renderImage.GetDescriptorSet(), {
		                             static_cast<float>(renderImage.GetWidth()),
		                             static_cast<float>(renderImage.GetHeight())
	                             });
        		}

                const ImVec2 window_pos = ImGui::GetWindowPos();
                const ImVec2 window_size = ImGui::GetWindowSize();
                const ImVec2 window_center = ImVec2(window_pos.x + window_size.x * 0.5f, window_pos.y + window_size.y * 0.5f);
        		ImGui::GetForegroundDrawList()->AddLine(ImVec2(window_center.x - zoomRegionUIInfo.mXSize / 2, window_center.y),
        			ImVec2(window_center.x + zoomRegionUIInfo.mXSize / 2, window_center.y),IM_COL32(255, 0, 0, 200), 1);
        		ImGui::GetForegroundDrawList()->AddLine(ImVec2(window_center.x, window_center.y - zoomRegionUIInfo.mXSize / 2),
					ImVec2(window_center.x, window_center.y + zoomRegionUIInfo.mXSize / 2),IM_COL32(255, 0, 0, 200), 1);
            }
        	ImGui::PopStyleVar();
        	ImGui::EndChild();
            
            //-----------------------------------------------------------
            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_Border, ImColor(110, 110, 128, 255).Value);
        	const auto& pickedColorsUIInfo = mStyleManager->GetPickedColorsUIInfo();
            ImGui::BeginChild("##ColorList", ImVec2(pickedColorsUIInfo.mXSize, pickedColorsUIInfo.mYSize), true);
            {
                for (int i = 0; i < mColorsManager->GetPickedColors().size(); i++)
                {
	                const bool selected = mColorsManager->GetSelectedColorIndex() == i;
                	const auto& pickedColors = mColorsManager->GetPickedColors();
                    const Objects::Color& currentColor = pickedColors[i];
                	
                	char selectableLabel[64];
                	sprintf_s(selectableLabel, "## %d", i);
                    
                    if (ImGui::Selectable(selectableLabel))
                    {
                        mColorsManager->SetSelectedColor(i);
                    	mPreviewColorManager->SetPreviewColor(pickedColors[i]);
                    }

                	if (ImGui::BeginPopupContextItem())
                	{
                		mColorsManager->SetSelectedColor(i);

                		ImGui::Text("This a popup for %d, %d, %d", currentColor.r, currentColor.g, currentColor.b);
                		if (ImGui::Button("Copy to clipboard"))
                		{
                			ImGuiUtils::CopyColorToClipboard(currentColor);
                			
                			ImGui::CloseCurrentPopup();	
                		}
                		if (ImGui::Button("Add comment"))
                		{
                			isColorCommentPopup = true;
                			
                			ImGui::CloseCurrentPopup();
                		}
                		if (ImGui::Button("Delete color"))
                		{
                			ImGui::CloseCurrentPopup();

                			mColorsManager->SetSelectedColor(-1);
                			mColorsManager->RemoveColor(i);
                			ImGui::EndPopup();
                			break;
                		}

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
                	currentColor.HasComment() ?
                		sprintf_s(colorBuf, "%d, %d, %d %s", currentColor.r, currentColor.g, currentColor.b, currentColor.comment.c_str())
                	    : sprintf_s(colorBuf, "%d, %d, %d", currentColor.r, currentColor.g, currentColor.b);
                	
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
        	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1);
        	ImGui::PushStyleColor(ImGuiCol_Border, ImColor(110, 110, 128, 255).Value);
        	if (mColorsManager->GetSelectedColorIndex() < 0)
        	{
        		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
        	}
            if (ImGui::Button("Copy value", ImVec2(ImGui::GetContentRegionAvail().x, 25)))
            {
            	ImGuiUtils::CopyColorToClipboard(mColorsManager->GetSelectedColor());
            }
        	ImGui::PopStyleVar();
        	ImGui::PopStyleColor();

        	if (mColorsManager->GetSelectedColorIndex() < 0)
        	{
        		ImGui::PopItemFlag();
        		ImGui::PopStyleVar();
        	}
            ImGuiUtils::CustomSpacing(5);
            ImGui::Separator();
			//-------------------------------------------------------------------

        	ImGui::BeginChild("##PreviewColorRegion", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true);
        	{
	            const Objects::Color& previewColor = mPreviewColorManager->GetPreviewColor();
        		ImGui::ColorButton("PreviewColor", ImVec4((float)previewColor.r /255, (float)previewColor.g/255, (float)previewColor.b/ 255, 255),
				   0,ImVec2(96, 54));

        		ImGui::SameLine();
        		ImGui::BeginChild("##PreviewFormatChild", ImVec2(ImGui::GetContentRegionAvail().x, 54), true);
        		{
        			static int e = 0;
                    ImGui::SameLine(0, 30);
        			ImGui::SetCursorPosY(15);
                    ImGui::RadioButton("RGB", &e, 0); ImGui::SameLine(0, 30);
                    ImGui::RadioButton("HSV", &e, 1); ImGui::SameLine(0, 30);
                    ImGui::RadioButton("HSL", &e, 2);
        		}
        		ImGui::EndChild();

        		int i1 = previewColor.r;
        		int i2 = previewColor.g;
        		int i3 = previewColor.b;
        		static int maxVal = 255; //TODO: get from manager based on current color displaying format
        								//TODO: get format for slider from manager based on color format as well.
        		static float sliderSize = 300;
        		ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - sliderSize / 2);
        		ImGui::PushItemWidth(sliderSize);
        		if (ImGui::SliderInt("##first slider", &i1, 0, maxVal, "R: %d"))
        		{
        			if (i1 > maxVal) i1 = maxVal;
        			if (i1 < 0) i1 = 0;

        			mPreviewColorManager->UpdatePreviewColorFirstChannel(i1);
        		}
        		ImGui::SameLine(); ImGuiUtils::HelpMarker("CTRL+click to input value.");
        		ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - sliderSize / 2);
        		ImGui::PushItemWidth(sliderSize);
        		if (ImGui::SliderInt("##second slider", &i2, 0, maxVal, "G: %d"))
        		{
        			if (i2 > maxVal) i2 = maxVal;
        			if (i2 < 0) i2 = 0;

        			mPreviewColorManager->UpdatePreviewColorSecondChannel(i2);
        		}
        		ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - sliderSize / 2);
        		ImGui::PushItemWidth(sliderSize);
        		if (ImGui::SliderInt("##third slider", &i3, 0, maxVal, "B: %d"))
        		{
        			if (i3 > maxVal) i3 = maxVal;
        			if (i3 < 0) i3 = 0;

        			mPreviewColorManager->UpdatePreviewColorThirdChannel(i3);
        		}

        		ImGuiUtils::CustomSpacing(3);
        		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1);
        		ImGui::PushStyleColor(ImGuiCol_Border, ImColor(110, 110, 128, 255).Value);
        		if (ImGui::Button("Copy Preview Color", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y)))
        		{
        			ImGuiUtils::CopyColorToClipboard(mPreviewColorManager->GetPreviewColor());
        		}
        		ImGui::PopStyleVar();
        		ImGui::PopStyleColor();
        	}
        	ImGui::EndChild();
        }
        ImGui::End();

		// Popups ---------------------------------------------------------
		if (isColorCommentPopup) ImGui::OpenPopup("Comment selected color");
		
        const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		if (ImGui::BeginPopupModal("Comment selected color", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			auto AddComment = [this](const char* commentBuf)
			{
				isColorCommentPopup = false;
				mColorsManager->CommentSelectedColor(commentBuf);
				
				ImGui::CloseCurrentPopup();
			};
			
			static char commentBuf[128] = "";
			if (!ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0)) ImGui::SetKeyboardFocusHere(0);
			if (ImGui::InputTextWithHint("##commentColorInputText", "Your comment...", commentBuf, IM_ARRAYSIZE(commentBuf), ImGuiInputTextFlags_EnterReturnsTrue))
			{
				AddComment(commentBuf);
				memset(commentBuf, 0, IM_ARRAYSIZE(commentBuf));
			}
			ImGui::Separator();

			if (ImGui::Button("Add", ImVec2(120, 0)))
			{
				AddComment(commentBuf);
				memset(commentBuf, 0, IM_ARRAYSIZE(commentBuf));
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); isColorCommentPopup = false; }
			ImGui::EndPopup();
		}
		//-----------------------------------------------------------------

		// Another windows-------------------------------------------------
#ifdef WL_DEBUG
		if (isDemoWindowOpened)
		{
			ImGui::ShowDemoWindow(&isDemoWindowOpened);
		}
#endif
		
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
		if (isPerformanceWindowOpened)
		{
			RenderUtils::RenderPerformanceWindow(&isPerformanceWindowOpened, mPerformanceWindowData);
		}
		//-----------------------------------------------------------------
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
			mColorsManager->AddPickedColor(mCursorPoint.color);
		}

		return handled;
	}
	
private:
	Objects::PerformanceWindowData mPerformanceWindowData;
	Objects::CursorPoint mCursorPoint;
	
	std::shared_ptr<Managers::StyleManager> mStyleManager;
	std::shared_ptr<Managers::ColorsManager> mColorsManager;
	std::shared_ptr<Managers::CursorManager> mCursorManager;
	std::shared_ptr<Managers::AppBehaviourManager> mAppBehaviourManager;
	std::shared_ptr<Managers::CursorPosRenderManager> mCursorPosRenderManager;
	std::shared_ptr<Managers::PreviewColorManager> mPreviewColorManager;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Color Picker";
	spec.Width = 450;
	spec.Height = 450;
	spec.CanResize = false;
	
	static bool isStayOnTop = false;

	using namespace Managers;
	ServiceLocator::ManagerLocator managerLocator;
	managerLocator.registerInstance<StyleManager>(new StyleManager);
	managerLocator.registerInstance<CursorManager>(new CursorManager);
	managerLocator.registerInstance<ColorsManager>(new ColorsManager);
	managerLocator.registerInstance<AppBehaviourManager>(new AppBehaviourManager);
	managerLocator.registerInstance<CursorPosRenderManager>(new CursorPosRenderManager());
	managerLocator.registerInstance<PreviewColorManager>(new PreviewColorManager);
	
	Walnut::Application* app = new Walnut::Application(spec);
	std::shared_ptr<ColorPickerLayer> mainLayer = std::make_shared<ColorPickerLayer>(managerLocator);
	app->PushLayer(mainLayer);
	app->SetMenubarCallback([app, managerLocator]()
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
			ImGui::MenuItem("Show performance window", nullptr, &isPerformanceWindowOpened);
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Color List"))
		{
			auto colorsManager = managerLocator.resolve<ColorsManager>();
			if (ImGui::MenuItem("Auto Save", nullptr, colorsManager->GetAutoSaveColorsListMemory()))
			{
				colorsManager->SaveAutoSaveColorsList();
			}
			if (ImGui::MenuItem("Save List"))
			{
				colorsManager->SaveColorList();
			}
			if (ImGui::MenuItem("Clear All",nullptr, nullptr, colorsManager->CanClearColors()))
			{
				colorsManager->ClearPickedColors();
			}
			
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Tools"))
		{
			auto colorsManager = managerLocator.resolve<ColorsManager>();
			ImGui::MenuItem("Text", nullptr, &isTextWindowOpened, colorsManager->CanOpenTextWindow());
			
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