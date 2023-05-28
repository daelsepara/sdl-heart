#ifndef __ITEMS__HPP__
#define __ITEMS__HPP__

#include <vector>

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
        LANTERN,
        STASIS_BOMB,
        VINE_KILLER,
        BATTERY_UNIT,
        EXALTED_ENHANCER,
        VIRID_MYSTERY,
        MASK_OF_OCCULTATION,
        PEERLESS_PERCEPTIVATE,
        CROSSBOW,
        First = BARYSAL_GUN,
        LAST = CROSSBOW
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
    auto LANTERN = Item::Base("LANTERN", "LANTERN", Item::Type::LANTERN);
    auto STASIS_BOMB = Item::Base("STASIS BOMB", "STASIS BOMB", Item::Type::STASIS_BOMB);
    auto VINE_KILLER = Item::Base("VINE KILLER", "VINE KILLER", Item::Type::VINE_KILLER);
    auto BATTERY_UNIT = Item::Base("BATTERY UNIT", "BATTERY UNIT", Item::Type::BATTERY_UNIT, 6);
    auto EXALTED_ENHANCER = Item::Base("EXALTED ENHANCER", "Toughens your skin so that you gain 5 Life Points, even above your initial score. However it also slows your reflexes so that you must lose the [AGILITY] skill if you have it.", Item::Type::EXALTED_ENHANCER);
    auto VIRID_MYSTERY = Item::Base("VIRID MYSTERY", "An antidote designed to reverse unwanted genetic changes.", Item::Type::VIRID_MYSTERY);
    auto MASK_OF_OCCULTATION = Item::Base("MASK OF OCCULTATION", "Gives the ability to alter your appearance and colouring.", Item::Type::MASK_OF_OCCULTATION);
    auto PEERLESS_PERCEPTIVATE = Item::Base("PEERLESS PERCEPTIVATE", "Confers the ability to see in almost total darkness.", Item::Type::PEERLESS_PERCEPTIVATE);
    auto CROSSBOW = Item::Base("CROSSBOW", "CROSSBOW", Item::Type::CROSSBOW);

    std::vector<Item::Type> UniqueItems = {
        Item::Type::SHORT_SWORD,
        Item::Type::SPECULUM_JACKET,
        Item::Type::MANTRAMUKTA_CANNON,
        Item::Type::VADE_MECUM,
        Item::Type::LITTLE_GAIA,
        Item::Type::VIRID_MYSTERY,
        Item::Type::EXALTED_ENHANCER,
        Item::Type::MASK_OF_OCCULTATION,
        Item::Type::PEERLESS_PERCEPTIVATE};

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

    int FIND_TYPE(std::vector<Item::Base> list, Item::Type item)
    {
        auto found = -1;

        for (auto i = 0; i < list.size(); i++)
        {
            if (list[i].Type == item)
            {
                found = i;

                break;
            }
        }

        return found;
    }

    int COUNT_TYPES(std::vector<Item::Base> &list, Item::Type item)
    {
        auto found = 0;

        for (auto i = 0; i < list.size(); i++)
        {
            if (list[i].Type == item)
            {
                found++;
            }
        }

        return found;
    }

    int FIND_LEAST(std::vector<Item::Base> &list, Item::Type item)
    {
        auto found = -1;

        auto min = 255;

        for (auto i = 0; i < list.size(); i++)
        {
            if (list[i].Type == item)
            {
                if (list[i].Charge < min)
                {
                    found = i;

                    min = list[i].Charge;
                }
            }
        }

        return found;
    }

} // namespace Item
#endif
