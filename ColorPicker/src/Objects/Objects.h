#pragma once

#include <string>

namespace Objects
{
    struct Color
    {
        Color() = default;
        
        Color(int _r, int _g, int _b)
            : r(_r), g(_g), b(_b) {}

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
}
