#pragma once

#include <vector>
#include <filesystem>
#include <mini/ini.h>

namespace Objects
{
    struct Color;
}

namespace SaveLoadUtils
{
    inline const char* INI_FILE_NAME = "Settings.ini";

    template <typename T>
    T ReadValue(const char* tag, const char* property)
    {
        T defaultReturn {};
        if (std::filesystem::exists(INI_FILE_NAME))
        {
            mINI::INIFile file(INI_FILE_NAME);
            mINI::INIStructure ini;
            file.read(ini);

            std::string& item = ini[tag][property];

            if (item.empty()) { __debugbreak(); } // invalid tag or property

            //TODO: maybe use try catch??? 
            if (std::is_same_v<T, bool>)
            {
                bool value = false;
                std::istringstream(item) >> std::boolalpha >> value;

                return value;
            }
            else if (std::is_same_v<T, int>)
            {
                return std::stoi(item);
            }

            __debugbreak(); // tag and property were present but type was incorrect 
        }
        else
        {
            __debugbreak();
        }
        
        return defaultReturn;
    }

    template <typename T>
    void WriteValue(const char* tag, const char* property, T newValue)
    {
        if (std::filesystem::exists(INI_FILE_NAME))
        {
            mINI::INIFile file(INI_FILE_NAME);
            mINI::INIStructure ini;

            if (std::is_same_v<T, bool>)
            {
                ini[tag][property] = newValue == true ? "true" : "false";
                file.write(ini);

                return;
            }

            __debugbreak(); // others types aren't supported yet.
        }
        else
        {
            __debugbreak();
        }
    }

    std::vector<Objects::Color> ReadColorsVector();
    void SaveColorsVector(const std::vector<Objects::Color>& colors);
}
