#include "CursorManager.h"

#include "Objects/Objects.h"

namespace Managers
{
    CursorManager::CursorManager()
    {
        hDC = GetDC(nullptr);
        if (hDC == nullptr)
        {
            __debugbreak();
        }
    }

    void CursorManager::FillCursorPoint(Objects::CursorPoint& pointOut) const
    {
        POINT point;
        if (GetCursorPos(&point))
        {
            COLORREF color;
            pointOut.x = point.x;
            pointOut.y = point.y;
            color = GetPixel(hDC, point.x, point.y);
            if (color == CLR_INVALID)
            {
                // handle error
                __debugbreak();
            }
            else
            {
                
                pointOut.color.r = GetRValue(color);
                pointOut.color.g = GetGValue(color);
                pointOut.color.b = GetBValue(color);
            }
        }
        else
        {
            __debugbreak();
        }
    }
}
