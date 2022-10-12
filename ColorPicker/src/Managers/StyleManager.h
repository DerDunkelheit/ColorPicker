#pragma once

namespace Objects
{
    struct Color;
}

struct ImVec4;

namespace Managers
{
    class StyleManager
    {
    public:
        //TODO: add additional parameters such as border colors etc... and would be also great add them to file to allow runtime UI changes.
        struct ColorPreviewUInfo
        {
            float mXSize = 90;
            float mYSize = 50;
        };
        struct ZoomRegionUIInfo
        {
            float mXSize = 120;
            float mYSize = 120;
        };
        struct PickedColorsUIInfo
        {
            float mXSize = 200;
            float mYSize = 120;
        };
    public:
        ~StyleManager();
        
        static void SetAppLookPreferences();
        static ImVec4 GetColorThemeBasedOnColor(const Objects::Color& color); 

        const ColorPreviewUInfo& GetColorPreviewUIInfo() const { return mColorPreviewInfo; }
        const ZoomRegionUIInfo& GetZoomRegionUIInfo() const { return mZoomRegionUIInfo; }
        const PickedColorsUIInfo& GetPickedColorsUIInfo() const { return mPickedColorsUIInfo; }

        float GetColorsComboWidth() const { return mColorsComboWidth; }
        float GetHotkeyWidth() const { return mHotkeyWidth; }
        
    private:
        ColorPreviewUInfo mColorPreviewInfo;
        ZoomRegionUIInfo mZoomRegionUIInfo;
        PickedColorsUIInfo mPickedColorsUIInfo;

        float mColorsComboWidth = 75;
        float mHotkeyWidth = 120;
    };
}