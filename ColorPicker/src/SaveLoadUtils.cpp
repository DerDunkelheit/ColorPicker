#include "SaveLoadUtils.h"

#include <assert.h>
#include <iostream>

#include "ColorsUtils.h"
#include "Objects/Objects.h"

static const char* PICKED_COLORS_FILE_NAME = "PickedColors.txt";
static char DELIMITER = ',';

namespace SaveLoadUtils
{
    std::vector<Objects::Color> ReadColorsVector()
    {
        assert(std::filesystem::exists(PICKED_COLORS_FILE_NAME));

        std::vector<Objects::Color> returnData;
        if (std::filesystem::exists(PICKED_COLORS_FILE_NAME))
        {
            std::ifstream pickedColorsFile(PICKED_COLORS_FILE_NAME);

            std::string line;
            while (std::getline (pickedColorsFile, line))
            {
                const size_t pos = line.find(DELIMITER);
                if (pos != std::string::npos)
                {
                    Objects::Color color = (ColorsUtils::GetColorFromHex(line.substr(0, pos).c_str()));
                    const std::string comment = line.substr(pos + 1, line.length() - pos + 1);
                    color.comment = comment;

                    returnData.push_back(color);
                }
                else
                {
                    returnData.push_back(ColorsUtils::GetColorFromHex(line.c_str()));
                }
            }
            
            pickedColorsFile.close();
        }

        return returnData;
    }

    void SaveColorsVector(const std::vector<Objects::Color>& colors)
    {
        assert(std::filesystem::exists(PICKED_COLORS_FILE_NAME));

        if (std::filesystem::exists(PICKED_COLORS_FILE_NAME))
        {
            std::ofstream pickedColorsFile(PICKED_COLORS_FILE_NAME);
            
            for (auto& color : colors)
            {
                if (color.HasComment())
                {
                    pickedColorsFile << ColorsUtils::CreateHexColorFromRGB(color) << "," << color.comment << "\n";
                }
                else
                {
                    pickedColorsFile << ColorsUtils::CreateHexColorFromRGB(color) << "\n";
                }
            }

            pickedColorsFile.close();
        }
    }

    void ClearColorsVector()
    {
        assert(std::filesystem::exists(PICKED_COLORS_FILE_NAME));

        if (std::filesystem::exists(PICKED_COLORS_FILE_NAME))
        {
            std::ofstream pickedColorsFile(PICKED_COLORS_FILE_NAME);
            pickedColorsFile.clear();
            
            pickedColorsFile.close();
        }
    }
}
