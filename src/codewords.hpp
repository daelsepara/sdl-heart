#ifndef __CODEWORDS__HPP__
#define __CODEWORDS__HPP__

#include <map>

namespace Codeword
{
    enum class Type
    {
        NONE = -1
    };

    std::map<Codeword::Type, const char *> Descriptions = {};

} // namespace Codeword
#endif
