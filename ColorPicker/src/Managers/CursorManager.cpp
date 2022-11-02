#include "CursorManager.h"

#include "Objects/Objects.h"
#include "Walnut/Timer.h"

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

    void CursorManager::Init(Objects::PerformanceWindowData* windowData)
    {
        mPerformanceWindowData = windowData;
    }

    void CursorManager::FillCursorPoint(Objects::CursorPoint& pointOut) const
    {
        POINT point;
        Walnut::Timer timer;
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
        mPerformanceWindowData->FillCursorPointTime = timer.Elapsed();
    }
}
