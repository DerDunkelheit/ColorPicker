#pragma once

#include <string>

namespace Objects
{
    struct Color
    {
        Color() = default;
        
        Color(int _r, int _g, int _b)
            : r(_r), g(_g), b(_b) {}

        static Color WhiteColor() { return Color{255, 255, 255}; }

        bool HasComment() const { return !comment.empty(); }
        
        int r = -1;
        int g = -1;
        int b = -1;
        std::string comment;
    };
    
    struct CursorPoint
    {
        long x = -1;
        long y = -1;

        Color color;
    };

    struct PerformanceWindowData
    {
        float captureScreenTime = 0.0f;
        float FillCursorPointTime = 0.0f;
    };
}
