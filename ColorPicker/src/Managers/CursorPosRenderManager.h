#pragma once

#include <memory>

namespace Walnut
{
    class Image;
}

namespace Objects
{
    struct CursorPoint;
    struct PerformanceWindowData;
}

namespace Managers
{
    class CursorPosRenderManager
    {
    public:
        CursorPosRenderManager();
        ~CursorPosRenderManager();

        void Init(Objects::PerformanceWindowData* windowData);
        void CaptureScreen(const Objects::CursorPoint& point);
        void CleanUpResources();

        const Walnut::Image& GetImage() const { return *mImage; }

        bool HasImage() const { return mImage != nullptr; }
        
        void RenderMousePosition(const Objects::CursorPoint& cursorPoint);
        
    private:
        Objects::PerformanceWindowData* mPerformanceWindowData = nullptr;
        
        std::shared_ptr<Walnut::Image> mImage;
        uint32_t* mImageData = nullptr;
    };
}
