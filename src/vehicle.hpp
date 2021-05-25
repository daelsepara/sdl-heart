#ifndef __VEHICLE__HPP__
#define __VEHICLE__HPP__

#include <map>

namespace Vehicle
{
    enum class Type
    {
        NONE = -1,
        BURREK,
        MANTA_SKY_CAR
    };

    std::map<Vehicle::Type, const char *> Descriptions = {
        {Vehicle::Type::NONE, "NONE"},
        {Vehicle::Type::BURREK, "BURREK"},
        {Vehicle::Type::MANTA_SKY_CAR, "MANTA SKY CAR"}};
}

#endif
