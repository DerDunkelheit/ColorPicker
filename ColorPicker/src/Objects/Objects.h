#pragma once

namespace Objects
{
    struct Color
    {
        Color() = default;
        
        Color(int _r, int _g, int _b)
            : r(_r), g(_g), b(_b) {}
        
        int r = -1;
        int g = -1;
        int b = -1;
    };
    
    struct CursorPoint
    {
        long x = -1;
        long y = -1;

        Color color;
    };
}
