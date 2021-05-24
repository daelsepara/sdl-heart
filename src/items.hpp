#ifndef __ITEMS__HPP__
#define __ITEMS__HPP__

#include <map>

namespace Item
{
    enum class Type
    {
        NONE = -1,
        BARYSAL_GUN,
        PSIONIC_FOCUS,
        STUN_GRENADE,
        SHORT_SWORD,
        FUR_COAT,
        COLD_WEATHER_SUIT,
        FLASHLIGHT,
        ROPE,
        SPECULUM_JACKET,
        MANTRAMUKTA_CANNON,
        ANTIDOTE_PILLS,
        ID_CARD,
        VADE_MECUM,
        LITTLE_GAIA,
        GAS_MASK,
        BINOCULARS,
        POLARIZED_GOGGLES,
        FOOD_PACK,
        MEDICAL_KIT,
        KNIFE,
        First = BARYSAL_GUN,
        LAST = KNIFE
    };

    class Base
    {
    public:
        std::string Name;
        std::string Description;

        Item::Type Type = Item::Type::NONE;

        int Charge = -1;

        Base()
        {
        }

        Base(const char *name, const char *description, Item::Type type)
        {
            Name = name;
            Description = description;
            Type = type;
        }

        Base(const char *name, const char *description, Item::Type type, int charge)
        {
            Name = name;
            Description = description;
            Type = type;
            Charge = charge;
        }
    };

    Item::Base MAKE_BARYSAL_GUN(int charge)
    {
        return Item::Base("BARYSAL GUN", "BARYSAL GUN", Item::Type::BARYSAL_GUN, charge);
    }

    // Item defaults
    auto PSIONIC_FOCUS = Item::Base("PSIONIC FOCUS", "PSIONIC FOCUS", Item::Type::PSIONIC_FOCUS);
    auto BARYSAL_GUN = MAKE_BARYSAL_GUN(6);
    auto STUN_GRENADE = Item::Base("STUN GRENADE", "STUN GRENADE", Item::Type::STUN_GRENADE);
    auto SHORT_SWORD = Item::Base("SHORT SWORD", "SHORT SWORD", Item::Type::SHORT_SWORD);
    auto FUR_COAT = Item::Base("FUR COAT", "FUR COAT", Item::Type::FUR_COAT);
    auto COLD_WEATHER_SUIT = Item::Base("COLD-WEATHER SUIT", "COLD-WEATHER SUIT", Item::Type::COLD_WEATHER_SUIT);
    auto FLASHLIGHT = Item::Base("FLASHLIGHT", "FLASHLIGHT", Item::Type::FLASHLIGHT);
    auto ROPE = Item::Base("ROPE", "ROPE", Item::Type::ROPE);
    auto SPECULUM_JACKET = Item::Base("SPECULUM JACKET", "SPECULUM JACKET", Item::Type::SPECULUM_JACKET);
    auto MANTRAMUKTA_CANNON = Item::Base("MANTRAMUKTA CANNON", "MANTRAMUKTA CANNON", Item::Type::MANTRAMUKTA_CANNON);
    auto ANTIDOTE_PILLS = Item::Base("ANTIDOTE PILLS", "ANTIDOTE PILLS", Item::Type::ANTIDOTE_PILLS);
    auto ID_CARD = Item::Base("ID CARD", "ID CARD", Item::Type::ID_CARD);
    auto VADE_MECUM = Item::Base("VADE MECUM", "VADE MECUM", Item::Type::VADE_MECUM);
    auto LITTLE_GAIA = Item::Base("LITTLE GAIA", "LITTLE GAIA", Item::Type::LITTLE_GAIA);
    auto GAS_MASK = Item::Base("GAS MASK", "GAS MASK", Item::Type::GAS_MASK);
    auto BINOCULARS = Item::Base("BINOCULARS", "BINOCULARS", Item::Type::BINOCULARS);
    auto POLARIZED_GOGGLES = Item::Base("POLARIZED GOGGLES", "POLARIZED GOGGLES", Item::Type::POLARIZED_GOGGLES);
    auto FOOD_PACK = Item::Base("FOOD PACK", "FOOD PACK", Item::Type::FOOD_PACK);
    auto MEDICAL_KIT = Item::Base("MEDICAL KIT", "MEDICAL KIT", Item::Type::MEDICAL_KIT);
    auto KNIFE = Item::Base("KNIFE", "KNIFE", Item::Type::KNIFE);

    std::vector<Item::Type> UniqueItems = {
        Item::Type::SHORT_SWORD,
        Item::Type::SPECULUM_JACKET,
        Item::Type::MANTRAMUKTA_CANNON,
        Item::Type::VADE_MECUM,
        Item::Type::LITTLE_GAIA};

    bool IsUnique(Item::Type item)
    {
        auto unique = false;

        for (auto i = 0; i < Item::UniqueItems.size(); i++)
        {
            if (Item::UniqueItems[i] == item)
            {
                unique = true;

                break;
            }
        }

        return unique;
    }

    void REMOVE(std::vector<Item::Base> &items, Item::Base item)
    {
        if (items.size() > 0)
        {
            for (auto i = 0; i < items.size(); i++)
            {
                if (items[i].Type == item.Type)
                {
                    items.erase(items.begin() + i);

                    break;
                }
            }
        }
    }

    void ADD(std::vector<Item::Base> &items, Item::Base item)
    {
        if (items.size() > 0)
        {
            auto found = 0;

            for (auto i = 0; i < items.size(); i++)
            {
                if (items[i].Type == item.Type)
                {
                    found++;
                }
            }

            if (found == 0)
            {
                items.push_back(item);
            }
        }
        else
        {
            items.push_back(item);
        }
    }

    bool VERIFY(std::vector<Item::Base> &items, Item::Base item)
    {
        auto has = false;

        if (items.size() > 0)
        {
            for (auto i = 0; i < items.size(); i++)
            {
                if (items[i].Type == item.Type)
                {
                    has = true;

                    break;
                }
            }
        }

        return has;
    }

    int FIND(std::vector<int> list, int item)
    {
        auto found = -1;

        for (auto i = 0; i < list.size(); i++)
        {
            if (list[i] == item)
            {
                found = i;

                break;
            }
        }

        return found;
    }

} // namespace Item
#endif
