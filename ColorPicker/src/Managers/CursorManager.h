#pragma once

#include <dinput.h>

namespace Objects
{
    struct CursorPoint;
    struct PerformanceWindowData;
}

namespace Managers
{
    class CursorManager
    {
    public:
        CursorManager();

        void Init(Objects::PerformanceWindowData* windowData);
        
        void FillCursorPoint(Objects::CursorPoint& pointOut) const;

    private:
        Objects::PerformanceWindowData* mPerformanceWindowData = nullptr;
        
        HDC hDC = nullptr;
    };
}
