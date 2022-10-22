#pragma once

#include <vector>

namespace Objects
{
    struct Color;
}

namespace Managers
{
    class ColorsManager
    {
    public:
        ColorsManager();

        const std::vector<Objects::Color> GetPickedColors() const { return mPickedColors; }
        int GetSelectedColorIndex() const { return mSelectedColorIndex; }
        bool CanClearColors() const;
        bool CanOpenTextWindow() const { return mPickedColors.size() > 1; }
        
        bool* GetAutoSaveColorsListMemory() { return &mAutoSaveColorsList; }

        void ClearPickedColors();
        void AddPickedColor(Objects::Color color);
        void SetSelectedColor(int index);
        void SaveAutoSaveColorsList();

    private:
        void Load();
        
    private:
        std::vector<Objects::Color> mPickedColors;
        int mSelectedColorIndex = 0;

        bool mAutoSaveColorsList = false;
    };
}
