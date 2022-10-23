#include "ColorsManager.h"

#include <cassert>
#include <filesystem>

#include "Objects/Objects.h"
#include "SaveLoadUtils.h"

namespace Managers
{
    ColorsManager::ColorsManager()
    {
       Load();
    }

    const Objects::Color& ColorsManager::GetSelectedColor() const
    {
        assert(mPickedColors.size() > 0);
        assert(mSelectedColorIndex >= 0);

        return mPickedColors[mSelectedColorIndex];
    }

    bool ColorsManager::CanClearColors() const
    {
        return !mPickedColors.empty();
    }

    void ColorsManager::RemoveColor(int index)
    {
        assert(mPickedColors.size() > index);

        mPickedColors.erase(mPickedColors.begin() + index);
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
        assert(index >= -1);
        
        mSelectedColorIndex = index;
    }

    void ColorsManager::CommentSelectedColor(const char* comment)
    {
        assert(mSelectedColorIndex > -1);

        mPickedColors[mSelectedColorIndex].comment = comment;

        SaveLoadUtils::SaveColorsVector(mPickedColors);
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
