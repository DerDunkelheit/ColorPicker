#include "Utils.h"

#ifdef WL_PLATFORM_WINDOWS
#include <windows.h>
#include <shellapi.h>
#endif

namespace Utils
{
    void OpenWebPage(const char* webUrl)
    {
#ifdef WL_PLATFORM_WINDOWS
        // Note: executable path must use backslashes!
        ShellExecuteA(nullptr, "open", webUrl, nullptr, nullptr, SW_SHOWDEFAULT);
#endif
    }
}
