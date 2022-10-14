#include "CursorPosRenderManager.h"
#include "CursorManager.h"

#include "Walnut/Image.h"
#include "Walnut/Random.h"
#include "Walnut/Timer.h"

#include "Objects/Objects.h"

namespace Managers
{
    CursorPosRenderManager::CursorPosRenderManager(std::shared_ptr<Managers::CursorManager> cursorManager)
        : mCursorManager(cursorManager) {}

    CursorPosRenderManager::CursorPosRenderManager()
    {
        
    }

    CursorPosRenderManager::~CursorPosRenderManager()
    {
        
    }

    uint32_t PerPixel(glm::vec2 coord)
    {
        auto test = Walnut::Random::UInt();
        test |= 0xff000000;

        return test;
    }

    void CursorPosRenderManager::RenderMousePosition(const Objects::CursorPoint& cursorPoint)
    {
        Walnut::Timer timer;
		
        if (mImage == nullptr || mViewportWidth != mImage->GetWidth() || mViewportHeight != mImage->GetHeight())
        {
            mImage = std::make_shared<Walnut::Image>(mViewportWidth, mViewportHeight, Walnut::ImageFormat::RGBA);
            delete[] mImageData;
            mImageData = new uint32_t[mViewportWidth * mViewportHeight];
        }

        // Iteration through all pixels
        for (uint32_t y = 0; y < mImage->GetHeight(); y++)
        {
            for (uint32_t x = 0; x < mImage->GetWidth(); x++)
            {
                //glm::vec2 coord = { (float)x / (float)mImage->GetWidth(), (float)y / (float)mImage->GetHeight() };

                //Objects::CursorPoint currentPoint;
                //TODO: we cannot do it each frame. we have to replace it to different thread
                //mCursorManager->FillCursorPoint(currentPoint);
                
                //Get color in rgb for pixel with x and y coord.
                uint32_t colorForPixel = Walnut::Random::UInt();
                colorForPixel |= 0xff000000;
                mImageData[x + y * mImage->GetWidth()] = colorForPixel;
            }
        }
        
        mImage->SetData(mImageData);

        mLastRenderTime = timer.ElapsedMillis();
        auto a = mLastRenderTime;
    }
}
