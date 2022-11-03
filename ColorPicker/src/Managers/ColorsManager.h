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

        const std::vector<Objects::Color>& GetPickedColors() const { return mPickedColors; }
        const Objects::Color& GetSelectedColor() const;
        int GetSelectedColorIndex() const { return mSelectedColorIndex; }
        bool CanClearColors() const;
        bool CanOpenTextWindow() const { return mPickedColors.size() > 1; }
        
        bool* GetAutoSaveColorsListMemory() { return &mAutoSaveColorsList; }

        void RemoveColor(int index);
        void ClearPickedColors();
        void SaveColorList() const;
        void AddPickedColor(const Objects::Color& color);
        void SetSelectedColor(int index);
        void CommentSelectedColor(const char* comment);
        
        void SaveAutoSaveColorsList() const;

    private:
        void Load();
        
    private:
        std::vector<Objects::Color> mPickedColors;
        int mSelectedColorIndex = -1;

        bool mAutoSaveColorsList = false;
    };
}
