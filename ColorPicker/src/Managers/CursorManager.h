#pragma once

#include <dinput.h>

namespace Objects
{
    struct CursorPoint;
}

namespace Managers
{
    class CursorManager
    {
    public:
        CursorManager();
        
        //TODO: to separate header.
        
        void FillCursorPoint(Objects::CursorPoint& pointOut) const;

    private:
        HDC hDC = nullptr;
    };
}
