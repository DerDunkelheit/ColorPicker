#include "ColorsManager.h"

#include <assert.h>
#include <filesystem>

#include "Objects/Objects.h"
#include "SaveLoadUtils.h"

namespace Managers
{
    ColorsManager::ColorsManager()
    {
       Load();
    }

    bool ColorsManager::CanClearColors() const
    {
        return !mPickedColors.empty();
    }

    void ColorsManager::ClearPickedColors()
    {
        mPickedColors.clear();

        SetSelectedColor(0);

        SaveLoadUtils::ClearColorsVector();
    }

    void ColorsManager::AddPickedColor(Objects::Color color)
    {
        mPickedColors.push_back(color);

        if (mAutoSaveColorsList)
        {
            SaveLoadUtils::SaveColorsVector(mPickedColors);
        }
    }

    void ColorsManager::SetSelectedColor(int index)
    {
        assert(index >= 0);
        
        mSelectedColorIndex = index;
    }

    void ColorsManager::SaveAutoSaveColorsList()
    {
       SaveLoadUtils::WriteValue<bool>("managers", "AutoSave", mAutoSaveColorsList);
    }

    void ColorsManager::Load()
    {
        mAutoSaveColorsList = SaveLoadUtils::ReadValue<bool>("managers", "AutoSave");

        mPickedColors = SaveLoadUtils::ReadColorsVector();
    }
}
