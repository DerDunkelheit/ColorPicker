#include "ColorsManager.h"

#include "Objects/Objects.h"

namespace Managers
{
    ColorsManager::ColorsManager()
    {
       
    }

    bool ColorsManager::CanClearColors() const
    {
        return !mPickedColors.empty();
    }

    void ColorsManager::ClearPickedColors()
    {
        mPickedColors.clear();

        SetSelectedColor(0);
    }

    void ColorsManager::AddPickedColor(Objects::Color color)
    {
        mPickedColors.push_back(color);
    }

    void ColorsManager::SetSelectedColor(int index)
    {
        //TODO: input validation ??
        mSelectedColorIndex = index;
    }
}
