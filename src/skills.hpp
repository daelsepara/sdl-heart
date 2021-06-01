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

    auto AGILITY = Skill::Base("AGILITY", "The ability to perform acrobatic feats, run, climb, balance and leap. A character with this skills is nimble and dexterous.", Skill::Type::AGILITY);
    auto CLOSE_COMBAT = Skill::Base("CLOSE COMBAT", "The use of a range of martial arts incorporating elements of karate, ju-jitsu and t'ai-chi.", Skill::Type::CLOSE_COMBAT);
    auto CUNNING = Skill::Base("CUNNING", "The ability to think on your feet and devise clever ploys for getting out of trouble. Useful in countless situations.", Skill::Type::CUNNING);
    auto CYBERNETICS = Skill::Base("CYBERNETICS", "The ability to program and operate computers -- almost a forgotten science in the apocalyptic world of the 23rd century.", Skill::Type::CYBERNETICS);
    auto ESP = Skill::Base("ESP", "The ability to sense danger and read other people's minds. You must possess a PSIONIC FOCUS to use this skill.", Skill::Type::ESP, Item::Type::PSIONIC_FOCUS);
    auto LORE = Skill::Base("LORE", "A combination of history, legend and general knowledge which gives you a good basis for dealing with the unknown.", Skill::Type::LORE);
    auto PARADOXING = Skill::Base("PARADOXING", "The ability to mentally affect the normal laws of nature. A slower and less reliable technique than ESP, but with sometimes miraculous effects. You must possess a PSIONIC FOCUS to use this skill.", Skill::Type::PARADOXING, Item::Type::PSIONIC_FOCUS);
    auto PILOTING = Skill::Base("PILOTING", "The ability to handle virtually any vehicle from an air-sled up to a space shuttle.", Skill::Type::PILOTING);
    auto ROGUERY = Skill::Base("ROGUERY", "Stealth and espionage skills: picking pockets, opening locks, and skulking unseen in the shadows.", Skill::Type::ROGUERY);
    auto SHOOTING = Skill::Base("SHOOTING", "Expertise with long-range weaponry. You must possess a charged BARYSAL GUN to use this skill.", Skill::Type::SHOOTING, Item::Type::BARYSAL_GUN);
    auto STREETWISE = Skill::Base("STREETWISE", "With this skill you are never at a loss in cities. What others see as the squalor and menace of narrow neon-lit streets is opportunity to you.", Skill::Type::STREETWISE);
    auto SURVIVAL = Skill::Base("SURVIVAL", "A talent which enables you to cope in desolate and uninhabited regions: forests, deserts, swamps and mountain peaks.", Skill::Type::SURVIVAL);

    std::vector<Skill::Base> ALL = {AGILITY, CLOSE_COMBAT, CUNNING, CYBERNETICS, ESP, LORE, PARADOXING, PILOTING, ROGUERY, SHOOTING, STREETWISE, SURVIVAL};

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

    int FIND_LIST(std::vector<int> selection, int item)
    {
        auto found = -1;

        for (auto i = 0; i < selection.size(); i++)
        {
            if (selection[i] == item)
            {
                found = i;

                break;
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
