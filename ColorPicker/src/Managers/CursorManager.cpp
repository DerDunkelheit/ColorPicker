#include "CursorManager.h"

#include "Objects/Objects.h"

namespace Managers
{
    CursorManager::CursorManager()
    {
        hDC = GetDC(nullptr);
        if (hDC == nullptr)
        {
            static_assert(true);
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
                static_assert(true);
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
            static_assert(true);
        }
    }
}
