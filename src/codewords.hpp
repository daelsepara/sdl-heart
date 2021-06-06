#ifndef __CODEWORDS__HPP__
#define __CODEWORDS__HPP__

#include <map>

namespace Codeword
{
    enum class Type
    {
        NONE = -1,
        BLUE,
        CAMOUFLAGE,
        DIAMOND,
        ENKIDU,
        FOCUS,
        HOURGLASS,
        HUMBABA,
        LUNAR,
        MALLET,
        NEMESIS,
        PROTEUS,
        RED,
        SCOTOPIC,
        SCYTHE,
        TALOS,
        URUK,
        YELLOW,
        SHORTSWORD
    };

    std::map<Codeword::Type, const char *> Descriptions = {
        {Codeword::Type::BLUE, "Blue"},
        {Codeword::Type::CAMOUFLAGE, "Camouflage"},
        {Codeword::Type::DIAMOND, "Diamond"},
        {Codeword::Type::ENKIDU, "Enkidu"},
        {Codeword::Type::FOCUS, "Focus"},
        {Codeword::Type::HOURGLASS, "Hourglass"},
        {Codeword::Type::HUMBABA, "Humbaba"},
        {Codeword::Type::LUNAR, "Lunar"},
        {Codeword::Type::MALLET, "Mallet"},
        {Codeword::Type::NEMESIS, "Nemesis"},
        {Codeword::Type::PROTEUS, "Proteus"},
        {Codeword::Type::RED, "Red"},
        {Codeword::Type::SCOTOPIC, "Scotopic"},
        {Codeword::Type::SCYTHE, "Scythe"},
        {Codeword::Type::TALOS, "Talos"},
        {Codeword::Type::URUK, "Uruk"},
        {Codeword::Type::YELLOW, "Yellow"},
        {Codeword::Type::SHORTSWORD, "Shortsword"}};

    std::vector<Codeword::Type> Invisible = {Codeword::Type::SHORTSWORD};

    bool IsInvisible(Codeword::Type codeword)
    {
        auto invisible = false;

        for (auto i = 0; i < Codeword::Invisible.size(); i++)
        {
            if (Codeword::Invisible[i] == codeword)
            {
                invisible = true;

                break;
            }
        }

        return invisible;
    }

} // namespace Codeword
#endif
