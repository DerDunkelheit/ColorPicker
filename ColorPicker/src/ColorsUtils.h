#pragma once

#include <string>

namespace Objects
{
    struct Color;
}

namespace ColorsUtils
{
    Objects::Color GetColorFromHex(const char* hexColor);
    std::string CreateHexColorFromRGB(const Objects::Color& color);

    void ValidateColor(const Objects::Color& color);
}