#ifndef __CHARACTER__HPP__
#define __CHARACTER__HPP__

#include <string>
#include <vector>

#include "codewords.hpp"
#include "items.hpp"
#include "skills.hpp"
#include "vehicle.hpp"

namespace Character
{
    enum class Type
    {
        CUSTOM = 0,
        EXPLORER,
        BOUNTY_HUNTER,
        SPY,
        TRADER,
        VISIONARY,
        SCIENTIST,
        MUTANT
    };

    class Base
    {
    public:
        std::string Name = "";

        std::string Description = "";

        Character::Type Type = Character::Type::CUSTOM;

        int Life = 10;

        int Money = 10;

        int ITEM_LIMIT = 8;

        int MAX_LIFE_LIMIT = 10;

        int SKILLS_LIMIT = 4;

        Vehicle::Type Vehicle = Vehicle::Type::NONE;

        std::vector<Skill::Base> Skills = std::vector<Skill::Base>();

        std::vector<Item::Base> Items = std::vector<Item::Base>();

        std::vector<Codeword::Type> Codewords = std::vector<Codeword::Type>();

        std::vector<Item::Base> LostItems = std::vector<Item::Base>();

        std::vector<Skill::Base> LostSkills = std::vector<Skill::Base>();

        std::vector<Item::Type> PotionsApplied = std::vector<Item::Type>();

        int LostMoney = 0;

        int StoryID = 0;

#if defined(_WIN32) || defined(__arm__)
        long long Epoch = 0;
#else
        long Epoch = 0;
#endif

        Base()
        {
        }

        Base(const char *name, Character::Type type, const char *description, std::vector<Skill::Base> skills, std::vector<Item::Base> items, int money)
        {
            Name = name;
            Type = type;
            Description = description;
            Skills = skills;
            Items = items;
            Money = money;
        }

        Base(const char *name, Character::Type type, const char *description, std::vector<Skill::Base> skills, std::vector<Item::Base> items, std::vector<Codeword::Type> codewords, int life, int money)
        {
            Name = name;
            Type = type;
            Description = description;
            Skills = skills;
            Items = items;
            Codewords = codewords;
            Life = life;
            Money = money;
        }
    };

    auto EXPLORER = Character::Base("The Explorer", Character::Type::EXPLORER, "Others might mourn the collapse of civilization, but for you it only opens up new areas of mystery in the world.", {Skill::CLOSE_COMBAT, Skill::LORE, Skill::STREETWISE, Skill::SURVIVAL}, {}, 30);
    auto BOUNTY_HUNTER = Character::Base("The Bounty Hunter", Character::Type::BOUNTY_HUNTER, "Times are hard and the strong prey upon the weak. It is left to the likes of you to enforce the law.", {Skill::CUNNING, Skill::PILOTING, Skill::SHOOTING, Skill::STREETWISE}, {Item::BARYSAL_GUN}, 30);
    auto SPY = Character::Base("The Spy", Character::Type::SPY, "Even as the world dies a slow death, governments vie with one another for the wealth and power that remain. You steal secrets and trade them to the highest bidder.", {Skill::AGILITY, Skill::CYBERNETICS, Skill::ROGUERY, Skill::STREETWISE}, {}, 30);
    auto TRADER = Character::Base("The Trader", Character::Type::TRADER, "Few dare cross the icy wastes between cities, so a daring adventurer can make a tidy profit.", {Skill::ESP, Skill::LORE, Skill::SHOOTING, Skill::STREETWISE}, {Item::BARYSAL_GUN, Item::PSIONIC_FOCUS}, 30);
    auto VISIONARY = Character::Base("The Visionary", Character::Type::VISIONARY, "Cursed with second sight, you know that mankind has no future unless something is done to save the world.", {Skill::CLOSE_COMBAT, Skill::CUNNING, Skill::ESP, Skill::PARADOXING}, {Item::PSIONIC_FOCUS}, 30);
    auto SCIENTIST = Character::Base("The Scientist", Character::Type::SCIENTIST, "Most people understand nothing of the machines created by their ancestors, but you've learned that knowledge is power.", {Skill::CYBERNETICS, Skill::LORE, Skill::PILOTING, Skill::SURVIVAL}, {}, 30);
    auto MUTANT = Character::Base("The Mutant", Character::Type::MUTANT, "Born with strange powers, you are more than human. Others would kill you if they knew your secret.", {Skill::AGILITY, Skill::CUNNING, Skill::PARADOXING, Skill::ROGUERY}, {Item::PSIONIC_FOCUS}, 30);

    std::vector<Character::Base> Classes = {EXPLORER, BOUNTY_HUNTER, SPY, TRADER, VISIONARY, SCIENTIST, MUTANT};

    int FIND_ITEM(Character::Base &player, Item::Type item)
    {
        auto found = -1;

        if (player.Items.size() > 0)
        {
            for (auto i = 0; i < player.Items.size(); i++)
            {
                if (player.Items[i].Type == item && player.Items[i].Charge != 0)
                {
                    found = i;

                    break;
                }
            }
        }

        return found;
    }

    int FIND_SKILL(Character::Base &player, Skill::Type skill)
    {
        auto found = -1;

        if (player.Skills.size() > 0)
        {
            for (auto i = 0; i < player.Skills.size(); i++)
            {
                if (player.Skills[i].Type == skill)
                {
                    found = i;

                    break;
                }
            }
        }

        return found;
    }

    bool VERIFY_ITEMS(Character::Base &player, std::vector<Item::Type> items)
    {
        auto found = 0;

        if (items.size() > 0)
        {
            for (auto i = 0; i < items.size(); i++)
            {
                auto result = Character::FIND_ITEM(player, items[i]);

                if (result >= 0)
                {
                    found++;
                }
            }
        }

        return found >= items.size();
    }

    int COUNT_ITEMS(Character::Base &player, std::vector<Item::Base> items)
    {
        auto found = 0;

        for (auto i = 0; i < items.size(); i++)
        {
            if (Character::FIND_ITEM(player, items[i].Type) >= 0)
            {
                found++;
            }
        }

        return found;
    }

    bool VERIFY_ITEMS_ANY(Character::Base &player, std::vector<Item::Base> items)
    {
        return Character::COUNT_ITEMS(player, items) > 0;
    }

    // Checks if player has the skill and the required item
    bool VERIFY_SKILL(Character::Base &player, Skill::Type skill)
    {
        auto found = false;

        if (player.Skills.size() > 0 && skill != Skill::Type::NONE)
        {
            for (auto i = 0; i < player.Skills.size(); i++)
            {
                if (player.Skills[i].Type == skill)
                {
                    if (player.Skills[i].Requirement != Item::Type::NONE)
                    {
                        found = Character::VERIFY_ITEMS(player, {player.Skills[i].Requirement});
                    }
                    else
                    {
                        found = true;
                    }

                    break;
                }
            }
        }

        return found;
    }

    bool VERIFY_ANY_SKILLS(Character::Base &player, std::vector<Skill::Type> skills)
    {
        auto found = false;

        if (player.Skills.size() > 0 && skills.size() > 0)
        {
            for (auto i = 0; i < skills.size(); i++)
            {
                if (Character::VERIFY_SKILL(player, skills[i]))
                {
                    found = true;

                    break;
                }
            }
        }

        return found;
    }

    bool VERIFY_ALL_SKILLS(Character::Base &player, std::vector<Skill::Type> skills)
    {
        auto found = 0;

        if (player.Skills.size() > 0 && skills.size() > 0)
        {
            for (auto i = 0; i < skills.size(); i++)
            {
                if (Character::VERIFY_SKILL(player, skills[i]))
                {
                    found++;
                }
            }
        }

        return found == skills.size();
    }

    bool HAS_SKILL(Character::Base &player, Skill::Type skill)
    {
        auto found = false;

        if (player.Skills.size() > 0 && skill != Skill::Type::NONE)
        {
            for (auto i = 0; i < player.Skills.size(); i++)
            {
                if (player.Skills[i].Type == skill)
                {
                    found = true;

                    break;
                }
            }
        }

        return found;
    }

    int FIND_SKILL_ITEMS(Character::Base &player, Skill::Type skill, std::vector<Item::Type> items)
    {
        auto found = 0;

        if (player.Skills.size() > 0 && skill != Skill::Type::NONE && items.size() > 0)
        {
            for (auto i = 0; i < player.Skills.size(); i++)
            {
                if (player.Skills[i].Type == skill)
                {
                    for (auto j = 0; j < items.size(); j++)
                    {
                        if (Character::FIND_ITEM(player, items[j]) >= 0)
                        {
                            found++;
                        }
                    }
                }
            }
        }

        return found;
    }

    // verify that player has the skill and ANY of the items
    bool VERIFY_SKILL_ANY(Character::Base &player, Skill::Type skill, std::vector<Item::Type> items)
    {
        return Character::FIND_SKILL_ITEMS(player, skill, items) > 0;
    }

    // verify that player has the skill and ALL of the items
    bool VERIFY_SKILL_ALL(Character::Base &player, Skill::Type skill, std::vector<Item::Type> items)
    {
        return Character::FIND_SKILL_ITEMS(player, skill, items) == items.size();
    }

    bool VERIFY_SKILL_ITEM(Character::Base &player, Skill::Type skill, Item::Type item)
    {
        return Character::VERIFY_SKILL_ALL(player, skill, {item});
    }

    int FIND_CODEWORD(Character::Base &player, Codeword::Type codeword)
    {
        auto found = -1;

        if (player.Codewords.size() > 0)
        {
            for (auto i = 0; i < player.Codewords.size(); i++)
            {
                if (player.Codewords[i] == codeword)
                {
                    found = i;

                    break;
                }
            }
        }

        return found;
    }

    int FIND_CODEWORDS(Character::Base &player, std::vector<Codeword::Type> codewords)
    {
        auto found = 0;

        if (player.Codewords.size() > 0 && codewords.size() > 0)
        {
            for (auto i = 0; i < codewords.size(); i++)
            {
                auto result = Character::FIND_CODEWORD(player, codewords[i]);

                if (result >= 0)
                {
                    found++;
                }
            }
        }

        return found;
    }

    bool VERIFY_CODEWORDS_ANY(Character::Base &player, std::vector<Codeword::Type> codewords)
    {
        return Character::FIND_CODEWORDS(player, codewords) > 0;
    }

    bool VERIFY_CODEWORDS_ALL(Character::Base &player, std::vector<Codeword::Type> codewords)
    {
        return Character::FIND_CODEWORDS(player, codewords) == codewords.size();
    }

    bool VERIFY_CODEWORD(Character::Base &player, Codeword::Type codeword)
    {
        return Character::VERIFY_CODEWORDS_ALL(player, {codeword});
    }

    bool VERIFY_LIFE(Character::Base &player, int threshold = 0)
    {
        return player.Life > threshold;
    }

    bool VERIFY_POSSESSIONS(Character::Base &player)
    {
        return player.Items.size() <= player.ITEM_LIMIT;
    }

    void GET_ITEMS(Character::Base &player, std::vector<Item::Base> items)
    {
        player.Items.insert(player.Items.end(), items.begin(), items.end());
    }

    void GET_CODEWORDS(Character::Base &player, std::vector<Codeword::Type> codewords)
    {
        for (auto i = 0; i < codewords.size(); i++)
        {
            if (!Character::VERIFY_CODEWORD(player, codewords[i]))
            {
                player.Codewords.push_back(codewords[i]);
            }
        }
    }

    void REMOVE_CODEWORD(Character::Base &player, Codeword::Type codeword)
    {
        if (Character::VERIFY_CODEWORD(player, codeword))
        {
            auto result = FIND_CODEWORD(player, codeword);

            if (result >= 0)
            {
                player.Codewords.erase(player.Codewords.begin() + result);
            }
        }
    }

    void GET_UNIQUE_ITEMS(Character::Base &player, std::vector<Item::Base> items)
    {
        for (auto i = 0; i < items.size(); i++)
        {
            if (!Character::VERIFY_ITEMS(player, {items[i].Type}))
            {
                player.Items.push_back(items[i]);
            }
        }
    }

    void LOSE_ITEMS(Character::Base &player, std::vector<Item::Type> items)
    {
        if (player.Items.size() > 0 && items.size() > 0)
        {
            for (auto i = 0; i < items.size(); i++)
            {
                auto result = Character::FIND_ITEM(player, items[i]);

                if (result >= 0)
                {
                    player.Items.erase(player.Items.begin() + result);
                }
            }
        }
    }

    void LOSE_SKILLS(Character::Base &player, std::vector<Skill::Type> skills)
    {
        if (player.Skills.size() > 0 && skills.size() > 0)
        {
            for (auto i = 0; i < skills.size(); i++)
            {
                auto result = Character::FIND_SKILL(player, skills[i]);

                if (result >= 0)
                {
                    player.LostSkills.push_back(player.Skills[result]);

                    player.Skills.erase(player.Skills.begin() + result);
                }
            }
        }
    }

    void GAIN_LIFE(Character::Base &player, int life)
    {
        player.Life += life;

        if (player.Life < 0)
        {
            player.Life = 0;
        }
        else if (player.Life > player.MAX_LIFE_LIMIT)
        {
            player.Life = player.MAX_LIFE_LIMIT;
        }
    }

    int COMBAT_DAMAGE(Character::Base &player, int life)
    {
        if (life < 0)
        {
            // reduce damage due to SHORT SWORD TRAINING
            if (Character::VERIFY_SKILL(player, Skill::Type::CLOSE_COMBAT) && Character::VERIFY_ITEMS(player, {Item::Type::SHORT_SWORD}) && Character::VERIFY_CODEWORD(player, Codeword::Type::SHORTSWORD))
            {
                life++;
            }
        }

        Character::GAIN_LIFE(player, life);

        return life;
    }

    void GAIN_MONEY(Character::Base &player, int money)
    {
        player.Money += money;

        if (player.Money < 0)
        {
            player.Money = 0;
        }
    }

    void LOSE_POSSESSIONS(Character::Base &player)
    {
        player.LostItems = player.Items;

        player.Items.clear();
    }

    void LOSE_ALL(Character::Base &player)
    {
        player.LostMoney = player.LostMoney;
        player.Money = 0;

        Character::LOSE_POSSESSIONS(player);
    }

    bool CHECK_VEHICLE(Character::Base &player, Vehicle::Type vehicle)
    {
        return (player.Vehicle == vehicle);
    }

    void LOSE_VEHICLE(Character::Base &player)
    {
        player.Vehicle = Vehicle::Type::NONE;
    }

    bool FIRE_WEAPON(Character::Base &player, Item::Type weapon)
    {
        auto fired = false;

        for (auto i = 0; i < player.Items.size(); i++)
        {
            if (player.Items[i].Type == weapon && player.Items[i].Charge > 0)
            {
                player.Items[i].Charge--;

                fired = true;

                break;
            }
        }

        return fired;
    }

    bool FIRE_BARYSAL(Character::Base &player, int count)
    {
        auto fired = false;

        for (auto i = 0; i < count; i++)
        {
            fired = Character::FIRE_WEAPON(player, Item::Type::BARYSAL_GUN);
        }

        return fired;
    }

    void APPLY_VIRUS(Character::Base &player, Item::Type virus)
    {
        player.PotionsApplied.push_back(virus);
    }

    void REMOVE_VIRUS(Character::Base &player, Item::Type virus)
    {
        for (auto i = 0; i < player.PotionsApplied.size(); i++)
        {
            if (player.PotionsApplied[i] == virus)
            {
                player.PotionsApplied.erase(player.PotionsApplied.begin() + i);

                break;
            }
        }
    }

    bool IS_APPLIED(Character::Base &player, Item::Type virus)
    {
        bool applied = false;

        for (auto i = 0; i < player.PotionsApplied.size(); i++)
        {
            if (player.PotionsApplied[i] == virus)
            {
                applied = true;

                break;
            }
        }

        return applied;
    }

    void EXALTED_ENHANCER(Character::Base &player, bool reverse)
    {
        if (reverse)
        {
            player.MAX_LIFE_LIMIT -= 5;

            Character::GAIN_LIFE(player, 0);

            for (auto i = 0; i < player.LostSkills.size(); i++)
            {
                if (player.LostSkills[i].Type == Skill::Type::AGILITY)
                {
                    player.Skills.push_back(player.LostSkills[i]);

                    player.LostSkills.erase(player.LostSkills.begin() + i);

                    break;
                }
            }
        }
        else
        {
            player.MAX_LIFE_LIMIT += 5;

            Character::GAIN_LIFE(player, 5);

            Character::LOSE_SKILLS(player, {Skill::Type::AGILITY});

            Character::APPLY_VIRUS(player, Item::Type::EXALTED_ENHANCER);
        }
    }

    void MASK_OF_OCCULTATION(Character::Base &player, bool reverse)
    {
        if (reverse)
        {
            Character::REMOVE_CODEWORD(player, Codeword::Type::CAMOUFLAGE);
        }
        else
        {
            Character::GET_CODEWORDS(player, {Codeword::Type::CAMOUFLAGE});

            Character::APPLY_VIRUS(player, Item::Type::MASK_OF_OCCULTATION);
        }
    }

    void PEERLESS_PERCEPTIVATE(Character::Base &player, bool reverse)
    {
        if (reverse)
        {
            Character::REMOVE_CODEWORD(player, Codeword::Type::SCOTOPIC);
        }
        else
        {
            Character::GET_CODEWORDS(player, {Codeword::Type::SCOTOPIC});

            Character::APPLY_VIRUS(player, Item::Type::PEERLESS_PERCEPTIVATE);
        }
    }

} // namespace Character

#endif
