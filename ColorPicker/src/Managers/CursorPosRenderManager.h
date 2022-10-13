#pragma once

#include <memory>

namespace Walnut
{
    class Image;
}

namespace Objects
{
    struct CursorPoint;
}

namespace Managers
{
    class CursorManager;
}

namespace Managers
{
    class CursorPosRenderManager
    {
    public:
        CursorPosRenderManager(std::shared_ptr<Managers::CursorManager> cursorManager);
        CursorPosRenderManager();
        ~CursorPosRenderManager();

        std::shared_ptr<Walnut::Image> GetImage() const { return mImage; }
        
        void RenderMousePosition(const Objects::CursorPoint& cursorPoint);
        
    private:
        std::shared_ptr<Walnut::Image> mImage;
        uint32_t* mImageData = nullptr;
        uint32_t mViewportWidth = 120;
        uint32_t mViewportHeight = 120;

        float mLastRenderTime = 0.0f;

        std::shared_ptr<Managers::CursorManager> mCursorManager;
    };
}
