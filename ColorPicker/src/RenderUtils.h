#pragma once

namespace Objects
{
    struct PerformanceWindowData;
}

namespace RenderUtils
{
    void RenderGuideWindow(bool* p_open);
    void RenderAboutWindow(bool* p_open);
    void RenderTextWindow(bool* p_open);
    void RenderPerformanceWindow(bool* p_open, const Objects::PerformanceWindowData& windowData);
}
