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

        void ClearPickedColors();
        void AddPickedColor(Objects::Color color);
        void SetSelectedColor(int index);
        
    private:
        std::vector<Objects::Color> mPickedColors;
        int mSelectedColorIndex = 0;
    };
}
