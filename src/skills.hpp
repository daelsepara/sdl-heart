#ifndef __SKILLS__HPP__
#define __SKILLS__HPP__

#include <vector>

#include "items.hpp"

namespace Skill
{
    enum class Type
    {
        NONE = -1,
        AGILITY,
        CLOSE_COMBAT,
        CUNNING,
        CYBERNETICS,
        ESP,
        LORE,
        PARADOXING,
        PILOTING,
        ROGUERY,
        SHOOTING,
        STREETWISE,
        SURVIVAL,
        First = AGILITY,
        Last = SURVIVAL
    };

    class Base
    {
    public:
        const char *Name = NULL;
        const char *Description = NULL;
        Skill::Type Type;
        Item::Type Requirement = Item::Type::NONE;

        Base(const char *name, const char *description, Skill::Type type, Item::Type item)
        {
            Name = name;
            Type = type;
            Description = description;
            Requirement = item;
        }

        Base(const char *name, const char *description, Skill::Type type)
        {
            Name = name;
            Type = type;
            Description = description;
            Requirement = Item::Type::NONE;
        }
    };

    std::vector<Skill::Base> ALL = {};

    int FIND(std::vector<Skill::Base> &skills, Skill::Type skill)
    {
        auto found = -1;

        if (skills.size() > 0 && skill != Skill::Type::NONE)
        {
            for (auto i = 0; i < skills.size(); i++)
            {
                if (skills[i].Type == skill)
                {
                    found = i;

                    break;
                }
            }
        }

        return found;
    }

    bool VERIFY(std::vector<Skill::Base> &skills, Skill::Base skill)
    {
        return FIND(skills, skill.Type) >= 0;
    }

    void ADD(std::vector<Skill::Base> &skills, Skill::Base skill)
    {
        if (!VERIFY(skills, skill))
        {
            skills.push_back(skill);
        }
    }

    void REMOVE(std::vector<Skill::Base> &skills, Skill::Base skill)
    {
        auto result = FIND(skills, skill.Type);

        if (result >= 0)
        {
            skills.erase(skills.begin() + result);
        }
    }

} // namespace Skill

#endif
