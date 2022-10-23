#pragma once

#include "Objects/Objects.h"

namespace Managers
{
    class PreviewColorManager
    {
    public:
        PreviewColorManager();

        const Objects::Color& GetPreviewColor() const { return mPreviewColor; }
        
        void SetPreviewColor(const Objects::Color& color) { mPreviewColor = color; }
        void UpdatePreviewColorFirstChannel(int newValue);
        void UpdatePreviewColorSecondChannel(int newValue);
        void UpdatePreviewColorThirdChannel(int newValue);

    private:
        Objects::Color mPreviewColor = Objects::Color::WhiteColor();
    };
}
