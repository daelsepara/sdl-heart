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
        {Codeword::Type::BLUE, "BLUE"},
        {Codeword::Type::CAMOUFLAGE, "CAMOUFLAGE"},
        {Codeword::Type::DIAMOND, "DIAMOND"},
        {Codeword::Type::ENKIDU, "ENKIDU"},
        {Codeword::Type::FOCUS, "FOCUS"},
        {Codeword::Type::HOURGLASS, "HOURGLASS"},
        {Codeword::Type::HUMBABA, "HUMBABA"},
        {Codeword::Type::LUNAR, "LUNAR"},
        {Codeword::Type::MALLET, "MALLET"},
        {Codeword::Type::NEMESIS, "NEMESIS"},
        {Codeword::Type::PROTEUS, "PROTEUS"},
        {Codeword::Type::RED, "RED"},
        {Codeword::Type::SCOTOPIC, "SCOTOPIC"},
        {Codeword::Type::SCYTHE, "SCYTHE"},
        {Codeword::Type::TALOS, "TALOS"},
        {Codeword::Type::URUK, "URUK"},
        {Codeword::Type::YELLOW, "YELLOW"}};

} // namespace Codeword
#endif
