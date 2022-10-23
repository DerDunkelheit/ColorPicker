#include "PreviewColorManager.h"

#include "ColorsUtils.h"

namespace Managers
{
    PreviewColorManager::PreviewColorManager()
    {

    }

    // TODO: We support only RGB format now. We have to do some converter for different formats in the future.
    void PreviewColorManager::UpdatePreviewColorFirstChannel(int newValue)
    {
        mPreviewColor.r = newValue;

#ifdef WL_DEBUG
        ColorsUtils::ValidateColor(mPreviewColor);
#endif
    }

    void PreviewColorManager::UpdatePreviewColorSecondChannel(int newValue)
    {
        mPreviewColor.g = newValue;

#ifdef WL_DEBUG
        ColorsUtils::ValidateColor(mPreviewColor);
#endif
    }

    void PreviewColorManager::UpdatePreviewColorThirdChannel(int newValue)
    {
        mPreviewColor.b = newValue;

#ifdef WL_DEBUG
        ColorsUtils::ValidateColor(mPreviewColor);
#endif
    }
}
