#include "CursorPosRenderManager.h"

#include <Walnut/Image.h>
#include <atlimage.h>
#include <filesystem>
#include <Walnut/Timer.h>

#include "Objects/Objects.h"

namespace Managers
{
    static const char* IMAGE_NAME = "mousePos.png";
    
    CursorPosRenderManager::CursorPosRenderManager() = default;

    CursorPosRenderManager::~CursorPosRenderManager()
    {
        CleanUpResources();
    }

    void CursorPosRenderManager::Init(Objects::PerformanceWindowData* windowData)
    {
        mPerformanceWindowData = windowData;

        if (!std::filesystem::exists(IMAGE_NAME))
        {
            CaptureScreen({0, 0});
        }
        
        mImage = std::make_shared<Walnut::Image>(IMAGE_NAME);
    }

    void CursorPosRenderManager::CaptureScreen(const Objects::CursorPoint& point)
    {
        //TODO: bad way. we save image to file every frame.
        // TODO: try to use bitmap directly to imageData
        Walnut::Timer saveImageTimer;
        HDC hScreenDC = GetDC(nullptr);
        HDC hMemoryDC = CreateCompatibleDC(hScreenDC);
        int width = GetDeviceCaps(hScreenDC,HORZRES);
        int height = GetDeviceCaps(hScreenDC,VERTRES);
        int customSize = 120;
        HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, customSize, customSize);
        HBITMAP hOldBitmap = static_cast<HBITMAP>(SelectObject(hMemoryDC, hBitmap));
        BitBlt(hMemoryDC, 0, 0, customSize, customSize, hScreenDC, point.x - customSize / 2, point.y - customSize / 2, SRCCOPY);
        hBitmap = static_cast<HBITMAP>(SelectObject(hMemoryDC, hOldBitmap));

        CImage image;
        image.Attach(hBitmap);
        image.Save(L"mousePos.png");

        DeleteDC(hMemoryDC);
        DeleteDC(hScreenDC);

        mPerformanceWindowData->captureScreenTime = saveImageTimer.Elapsed();
    }

    void CursorPosRenderManager::CleanUpResources()
    {
        mImage.reset();
    }

    void CursorPosRenderManager::RenderMousePosition(const Objects::CursorPoint& cursorPoint)
    {
        CaptureScreen(cursorPoint);
        mImage->SetImageData(IMAGE_NAME);
    }
}
