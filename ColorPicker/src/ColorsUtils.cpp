#include "ColorsUtils.h"

#include <cstdio>
#include <cassert>

#include "Objects/Objects.h"

namespace ColorsUtils
{
    Objects::Color GetColorFromHex(const char* hexColor)
    {
        int r, g, b;
        sscanf_s(hexColor, "%02X%02X%02X", &r, &g, &b);
        Objects::Color returnColor(r, g, b);
        ValidateColor(returnColor);
        
        return returnColor;
    }

    std::string CreateHexColorFromRGB(const Objects::Color& color)
    {
        char hexCol[16];

        snprintf(hexCol, sizeof(hexCol), "%02x%02x%02x", color.r, color.g, color.b);

        return hexCol;
    }

    bool InRange(int low, int high, int x)
    {
        return (x - high) * (x - low) <= 0;
    }

    void ValidateColor(const Objects::Color& color)
    {
        constexpr int minValue = 0;
        constexpr int maxValue = 255;

        assert(InRange(minValue, maxValue, color.r));
        assert(InRange(minValue, maxValue, color.g));
        assert(InRange(minValue, maxValue, color.b));
    }
}
