#ifndef __STORY__HPP__
#define __STORY__HPP__

#include <vector>

#include "constants.hpp"
#include "controls.hpp"
#include "codewords.hpp"
#include "items.hpp"
#include "skills.hpp"
#include "character.hpp"

namespace Choice
{
    enum class Type
    {
        NORMAL = 0, // No requirements
        ITEMS,
        GET_ITEMS,
        SKILL,
        CODEWORD,
        MONEY,
        LIFE,
        ANY_ITEM,
        ANY_SKILL,
        SKILL_ITEM,
        SKILL_ANY,
        SKILL_ALL,
        FIRE_WEAPON,
        LOSE_ITEMS,
        LOSE_MONEY,
        LOSE_ALL,
        LOSE_SKILLS,
        GET_CODEWORD,
        LOSE_CODEWORD,
        GAIN_MONEY,
        GIVE,
        BUY_VEHICLE
    };

    class Base
    {
    public:
        const char *Text = NULL;

        Choice::Type Type = Choice::Type::NORMAL;

        Skill::Type Skill = Skill::Type::NONE;

        std::vector<Item::Base> Items = std::vector<Item::Base>();

        Codeword::Type Codeword = Codeword::Type::NONE;

        Vehicle::Type Vehicle = Vehicle::Type::NONE;

        int Value = 0;

        int Destination = -1;

        Base(Choice::Type type, const char *text, int destination, Skill::Type skill, std::vector<Item::Base> items, int value)
        {
            Type = type;
            Text = text;
            Destination = destination;
            Items = items;
            Skill = skill;
            Value = value;
        }

        Base(const char *text, int destination)
        {
            Text = text;
            Destination = destination;
        }

        Base(const char *text, int destination, Skill::Type skill, std::vector<Item::Base> items)
        {
            Text = text;
            Destination = destination;
            Type = Choice::Type::SKILL_ITEM;
            Items = items;
            Skill = skill;
        }

        Base(const char *text, int destination, std::vector<Item::Base> items)
        {
            Text = text;
            Destination = destination;
            Type = Choice::Type::ITEMS;
            Items = items;
        }

        Base(const char *text, int destination, Skill::Type skill)
        {
            Text = text;
            Destination = destination;
            Type = Choice::Type::SKILL;
            Skill = skill;
        }

        Base(const char *text, int destination, Codeword::Type codeword)
        {
            Text = text;
            Destination = destination;
            Type = Choice::Type::CODEWORD;
            Codeword = codeword;
        }

        Base(const char *text, int destination, Choice::Type type, int value)
        {
            Text = text;
            Destination = destination;
            Type = type;
            Value = value;
        }

        Base(const char *text, int destination, Choice::Type type)
        {
            Text = text;
            Destination = destination;
            Type = type;
        }

        Base(const char *text, int destination, Choice::Type type, std::vector<Item::Base> items)
        {
            Text = text;
            Destination = destination;
            Type = type;
            Items = items;
        }

        Base(const char *text, int destination, Choice::Type type, Skill::Type skill, std::vector<Item::Base> items)
        {
            Text = text;
            Destination = destination;
            Type = type;
            Skill = skill;
            Items = items;
        }

        Base(const char *text, int destination, Choice::Type type, std::vector<Item::Base> items, int value)
        {
            Text = text;
            Destination = destination;
            Type = type;
            Items = items;
            Value = value;
        }

        Base(const char *text, int destination, Choice::Type type, Codeword::Type codeword)
        {
            Text = text;
            Destination = destination;
            Type = type;
            Codeword = codeword;
        }

        Base(const char *text, int destination, Choice::Type type, Vehicle::Type vehicle)
        {
            Text = text;
            Destination = destination;
            Type = type;
            Vehicle = vehicle;
        }

        Base(const char *text, int destination, Choice::Type type, Vehicle::Type vehicle, int value)
        {
            Text = text;
            Destination = destination;
            Type = type;
            Vehicle = vehicle;
            Value = value;
        }
    };
} // namespace Choice

namespace Story
{
    enum class Type
    {
        NORMAL = 0,
        GOOD,
        DOOM,
        GODHOOD,
        SENTINEL,
        SACRIFICE,
        UNKNOWN
    };

    enum class Controls
    {
        NONE = 0,
        STANDARD,
        SHOP,
        BUY_AND_SELL,
        SELL,
        TRADE
    };

    class Base
    {
    public:
        int ID = 0;

        const char *Text = NULL;

        const char *Title = NULL;

        const char *Bye = NULL;

        const char *Image = NULL;

        Story::Controls Controls = Story::Controls::NONE;

        std::vector<Choice::Base> Choices = std::vector<Choice::Base>();

        std::vector<std::pair<Item::Base, int>> Shop = std::vector<std::pair<Item::Base, int>>();
        std::vector<std::pair<Item::Base, int>> Sell = std::vector<std::pair<Item::Base, int>>();

        std::pair<Item::Base, Item::Base> Trade;

        // Player selects items to take up to a certain limit
        std::vector<Item::Base> Take = std::vector<Item::Base>();

        // Player selects items to lose
        std::vector<Item::Base> ToLose = std::vector<Item::Base>();

        int Limit = 0;

        int LimitSkills = 0;

        Vehicle::Type Vehicle = Vehicle::Type::NONE;

        Story::Type Type = Story::Type::NORMAL;

        // Handle background events
        virtual int Background(Character::Base &player) { return -1; };

        // Handle events before story branches
        virtual void Event(Character::Base &player){};

        // Jump to next section
        virtual int Continue(Character::Base &player) { return -1; };

        Base()
        {
        }

        Base(int id)
        {
            ID = id;
        }

        Base(int id, const char *text, Story::Type type)
        {
            ID = id;
            Text = text;
            type = type;
        }

        Base(const char *text, Story::Type type)
        {
            Text = text;
            type = type;
        }
    };

    std::vector<Button> StandardControls(bool compact = false)
    {
        auto idx = 0;

        auto controls = std::vector<Button>();

        if (!compact)
        {
            controls.push_back(Button(0, "icons/up-arrow.png", 0, 1, 0, 1, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + border_space, Control::Type::SCROLL_UP));
            controls.push_back(Button(1, "icons/down-arrow.png", 0, 2, 0, 2, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + text_bounds - arrow_size - border_space, Control::Type::SCROLL_DOWN));

            idx = 2;
        }

        controls.push_back(Button(idx, "icons/map.png", idx, idx + 1, compact ? idx : 1, idx, startx, buttony, Control::Type::MAP));
        controls.push_back(Button(idx + 1, "icons/disk.png", idx, idx + 2, compact ? idx + 1 : 1, idx + 1, startx + gridsize, buttony, Control::Type::GAME));
        controls.push_back(Button(idx + 2, "icons/user.png", idx + 1, idx + 3, compact ? idx + 2 : 1, idx + 2, startx + 2 * gridsize, buttony, Control::Type::CHARACTER));
        controls.push_back(Button(idx + 3, "icons/items.png", idx + 2, idx + 4, compact ? idx + 3 : 1, idx + 3, startx + 3 * gridsize, buttony, Control::Type::USE));
        controls.push_back(Button(idx + 4, "icons/next.png", idx + 3, idx + 5, compact ? idx + 4 : 1, idx + 4, startx + 4 * gridsize, buttony, Control::Type::NEXT));
        controls.push_back(Button(idx + 5, "icons/exit.png", idx + 4, idx + 5, compact ? idx + 5 : 1, idx + 5, (1.0 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

        return controls;
    }

    std::vector<Button> ShopControls(bool compact = false)
    {
        auto idx = 0;

        auto controls = std::vector<Button>();

        if (!compact)
        {
            controls.push_back(Button(0, "icons/up-arrow.png", 0, 1, 0, 1, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + border_space, Control::Type::SCROLL_UP));
            controls.push_back(Button(1, "icons/down-arrow.png", 0, 2, 0, 2, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + text_bounds - arrow_size - border_space, Control::Type::SCROLL_DOWN));

            idx = 2;
        }

        controls.push_back(Button(idx, "icons/map.png", idx, idx + 1, compact ? idx : 1, idx, startx, buttony, Control::Type::MAP));
        controls.push_back(Button(idx + 1, "icons/disk.png", idx, idx + 2, compact ? idx + 1 : 1, idx + 1, startx + gridsize, buttony, Control::Type::GAME));
        controls.push_back(Button(idx + 2, "icons/user.png", idx + 1, idx + 3, compact ? idx + 2 : 1, idx + 2, startx + 2 * gridsize, buttony, Control::Type::CHARACTER));
        controls.push_back(Button(idx + 3, "icons/items.png", idx + 2, idx + 4, compact ? idx + 3 : 1, idx + 3, startx + 3 * gridsize, buttony, Control::Type::USE));
        controls.push_back(Button(idx + 4, "icons/next.png", idx + 3, idx + 5, compact ? idx + 4 : 1, idx + 4, startx + 4 * gridsize, buttony, Control::Type::NEXT));
        controls.push_back(Button(idx + 5, "icons/shop.png", idx + 4, idx + 6, compact ? idx + 5 : 1, idx + 5, startx + 5 * gridsize, buttony, Control::Type::SHOP));
        controls.push_back(Button(idx + 6, "icons/exit.png", idx + 5, idx + 6, compact ? idx + 6 : 1, idx + 6, (1.0 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

        return controls;
    }

    std::vector<Button> SellControls(bool compact = false)
    {
        auto idx = 0;

        auto controls = std::vector<Button>();

        if (!compact)
        {
            controls.push_back(Button(0, "icons/up-arrow.png", 0, 1, 0, 1, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + border_space, Control::Type::SCROLL_UP));
            controls.push_back(Button(1, "icons/down-arrow.png", 0, 2, 0, 2, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + text_bounds - arrow_size - border_space, Control::Type::SCROLL_DOWN));

            idx = 2;
        }

        controls.push_back(Button(idx, "icons/map.png", idx, idx + 1, compact ? idx : 1, idx, startx, buttony, Control::Type::MAP));
        controls.push_back(Button(idx + 1, "icons/disk.png", idx, idx + 2, compact ? idx + 1 : 1, idx + 1, startx + gridsize, buttony, Control::Type::GAME));
        controls.push_back(Button(idx + 2, "icons/user.png", idx + 1, idx + 3, compact ? idx + 2 : 1, idx + 2, startx + 2 * gridsize, buttony, Control::Type::CHARACTER));
        controls.push_back(Button(idx + 3, "icons/items.png", idx + 2, idx + 4, compact ? idx + 3 : 1, idx + 3, startx + 3 * gridsize, buttony, Control::Type::USE));
        controls.push_back(Button(idx + 4, "icons/next.png", idx + 3, idx + 5, compact ? idx + 4 : 1, idx + 4, startx + 4 * gridsize, buttony, Control::Type::NEXT));
        controls.push_back(Button(idx + 5, "icons/selling.png", idx + 4, idx + 6, compact ? idx + 5 : 1, idx + 5, startx + 5 * gridsize, buttony, Control::Type::SELL));
        controls.push_back(Button(idx + 6, "icons/exit.png", idx + 5, idx + 6, compact ? idx + 6 : 1, idx + 6, (1.0 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

        return controls;
    }

    std::vector<Button> BuyAndSellControls(bool compact = false)
    {
        auto idx = 0;

        auto controls = std::vector<Button>();

        if (!compact)
        {
            controls.push_back(Button(0, "icons/up-arrow.png", 0, 1, 0, 1, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + border_space, Control::Type::SCROLL_UP));
            controls.push_back(Button(1, "icons/down-arrow.png", 0, 2, 0, 2, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + text_bounds - arrow_size - border_space, Control::Type::SCROLL_DOWN));

            idx = 2;
        }

        controls.push_back(Button(idx, "icons/map.png", idx, idx + 1, compact ? idx : 1, idx, startx, buttony, Control::Type::MAP));
        controls.push_back(Button(idx + 1, "icons/disk.png", idx, idx + 2, compact ? idx + 1 : 1, idx + 1, startx + gridsize, buttony, Control::Type::GAME));
        controls.push_back(Button(idx + 2, "icons/user.png", idx + 1, idx + 3, compact ? idx + 2 : 1, idx + 2, startx + 2 * gridsize, buttony, Control::Type::CHARACTER));
        controls.push_back(Button(idx + 3, "icons/items.png", idx + 2, idx + 4, compact ? idx + 3 : 1, idx + 3, startx + 3 * gridsize, buttony, Control::Type::USE));
        controls.push_back(Button(idx + 4, "icons/next.png", idx + 3, idx + 5, compact ? idx + 4 : 1, idx + 4, startx + 4 * gridsize, buttony, Control::Type::NEXT));
        controls.push_back(Button(idx + 5, "icons/shop.png", idx + 4, idx + 6, compact ? idx + 5 : 1, idx + 5, startx + 5 * gridsize, buttony, Control::Type::SHOP));
        controls.push_back(Button(idx + 6, "icons/selling.png", idx + 5, idx + 7, compact ? idx + 6 : 1, idx + 6, startx + 6 * gridsize, buttony, Control::Type::SELL));
        controls.push_back(Button(idx + 7, "icons/exit.png", idx + 6, idx + 7, compact ? idx + 7 : 1, idx + 7, (1.0 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

        return controls;
    }

    std::vector<Button> TradeControls(bool compact = false)
    {
        auto idx = 0;

        auto controls = std::vector<Button>();

        if (!compact)
        {
            controls.push_back(Button(0, "icons/up-arrow.png", 0, 1, 0, 1, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + border_space, Control::Type::SCROLL_UP));
            controls.push_back(Button(1, "icons/down-arrow.png", 0, 2, 0, 2, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + text_bounds - arrow_size - border_space, Control::Type::SCROLL_DOWN));

            idx = 2;
        }

        controls.push_back(Button(idx, "icons/map.png", idx, idx + 1, compact ? idx : 1, idx, startx, buttony, Control::Type::MAP));
        controls.push_back(Button(idx + 1, "icons/disk.png", idx, idx + 2, compact ? idx + 1 : 1, idx + 1, startx + gridsize, buttony, Control::Type::GAME));
        controls.push_back(Button(idx + 2, "icons/user.png", idx + 1, idx + 3, compact ? idx + 2 : 1, idx + 2, startx + 2 * gridsize, buttony, Control::Type::CHARACTER));
        controls.push_back(Button(idx + 3, "icons/items.png", idx + 2, idx + 4, compact ? idx + 3 : 1, idx + 3, startx + 3 * gridsize, buttony, Control::Type::USE));
        controls.push_back(Button(idx + 4, "icons/next.png", idx + 3, idx + 5, compact ? idx + 4 : 1, idx + 4, startx + 4 * gridsize, buttony, Control::Type::NEXT));
        controls.push_back(Button(idx + 5, "icons/shop.png", idx + 4, idx + 6, compact ? idx + 5 : 1, idx + 5, startx + 5 * gridsize, buttony, Control::Type::TRADE));
        controls.push_back(Button(idx + 6, "icons/exit.png", idx + 5, idx + 6, compact ? idx + 6 : 1, idx + 6, (1.0 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

        return controls;
    }

    std::vector<Button> ExitControls()
    {
        auto controls = std::vector<Button>();

        controls.push_back(Button(0, "icons/exit.png", 0, 0, -1, -1, (1.0 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

        return controls;
    }

} // namespace Story

class NotImplemented : public Story::Base
{
public:
    NotImplemented()
    {
        ID = -1;

        Title = "Not implemented yet";

        Controls = Story::Controls::NONE;
    }
};

auto notImplemented = NotImplemented();

auto Stories = std::vector<Story::Base *>();

void *findStory(int id)
{
    Story::Base *story = &notImplemented;

    if (Stories.size() > 0)
    {
        for (auto i = 0; i < Stories.size(); i++)
        {
            if (((Story::Base *)Stories[i])->ID == id)
            {
                story = (Story::Base *)Stories[i];

                break;
            }
        }
    }

    return story;
}

class Earth23rdCentury : public Story::Base
{
public:
    Earth23rdCentury()
    {
        ID = 2300;

        Title = "Heart of Ice: The Last Three Centuries";

        Text = "In 2023, worsening conditions in the world's climate led to the first Global Economic Conference. It was agreed to implement measures intended to reverse industrial damage to the ecology and replenish the ozone layer. By 2031, an array of weather control satellites were in orbit. For added efficiency, and as a mark of worldwide cooperation, these were placed under the control of a supercomputer network called Gaia: the Global Artificial Intelligence Array. The Earth's climate began to show steady improvement.\n\nThe first hint of disaster came early in 2037, when Gaia shut down inexplicably for a period of seventeen minutes. Normal operation was resumed but the system continued to suffer 'glitches'. One such glitch resulted in Paris being subjected to a two-day heat wave of such intensity that the pavements cracked. After several months, the fault was identified. A computer virus had been introduced into Gaia by unknown means. The system's designer began programming an antivirus but died before his war was complete. The crisis grew throughout that year until finally, following the death of five thousand people in a flash flood along the Bangladesh coastline, the Gaia project was officially denounced. Unfortunately, it was no longer possible to shut it down.\n\nBy the mid twenty-first century, global weather conditions were in chaos owing to Gaia's sporadic operation. Ice sheets advanced further each year. Australia was subject to virtually constant torrential rain. The centre of Asia had become an arid wasteland. The political situation reflected the ravages of the climate, with wars flaring continually around the globe. Late in 2054, computer scientists in London tried to hack into Gaia and locate the replicating viruses in the program. Gaia, detecting this, interpreted the action as an attack on its program and retaliated by taking over a range of defense networks which allowed it to launch a nuclear strike. London was completely destroyed.\n\nBy the end of the century Gaia had routed itself into all major computer networks, taking control of weather, communications and weapons systems all across the planet. Periods of lucidity and hospitable climate were interspersed with hurricanes and arctic blizzards. The US President gave an interview in which he likened Gaia to a living entity: \"She was intended as mankind's protective mother, but this 'mother' has gone mad.\" Spiralling decline in the world's fortunes left much of humanity on the brink of extinction. The population fell rapidly until only a few million people remained scattered around the globe -- mostly in cities where food could still be artificially produced.\n\nIt is now the year 2300. The rich stand aloof, disporting themselves with forced gaiety and waiting for the end. The poor inhabit jostling slums where disease is rife and law is unknown. Between the cities, the land lies under a blanket of snow and ice. No-one expects humanity to last another century. This is truly 'the end of history'.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 0; }
};

class Prologue : public Story::Base
{
public:
    Prologue()
    {
        ID = 0;

        Title = "Heart of Ice: Prologue";

        Text = "The Etruscan Inn lies in the shadow of the Apennine Mountains, beside a frozen waterfall, sheltered from the wind by a high ridge of bare black rock. You stand at a long window and gaze out towards the mountains. Dusk is melting the sharp outlines of the crags, filling the valleys with blue gloom. The moon glimmers faintly under racing black clouds. Later this evening there will be more snow.\n\nTurning from the window, you let the curtain fall back and make your way across the dingy room. Travellers sit at the sides, noisily gambling and sipping hard liquor the colour of fire. Many are hunters and traders from the plains which slope down from here to the Ligurian Sea. Others may have been here much longer: thin old men and women who found meagre employment. The Etruscan Inn is a famous stop-over for those who undertake the perilous Apennine crossing. If a few such, gazing up at the ice-capped peaks, found their spirits daunted and chose to stay, who can blame them? You sometimes wonder yourself why you bother to press on across the world in the teeth of such hardship and poverty.\n\nThe story of how the inn came to be here is a strange one, even for these bizarre times. The building was originally an air cruiser which crashed in the mountains two hundred years ago. An ancestor of the present innkeeper turned the wreckage into a hostelry for wayfarers. The power unit had not been damaged in the crash, so the inn has electricity -- a rarity in the modern world. Even better, several of the air cruiser's careteks were salvaged. These are robots which continually clean and repair the structure, sturdily carrying out the tasks they were programmed to do centuries ago.\n\nPushing aside a drape, you step into another room. On the wall, a screen flickers with scenes from an old film. The innkeeper is sitting with a few others at the back, loudly commenting on the action. You step over a caretek which resembles a long metal cockroach. It extends polishing pads to clean the floor where you were standing. Propping yourself against the wall, you watch the film for a few minutes, but the innkeeper's shouts and jeers are impossible to ignore. When you complain, he only gives a great gusting laugh and says, \"There's no point in getting interested in any film that appears on this screen. The video link comes from a satellite connected to Gaia, who changes channels as the whim strikes her. Sometimes I have seen newsreel footage over a century old. At other times there are films, musical shows, or documentaries. But I have yet to see the end of any programme. There -- !\"He points at the screen and, turning, you see that the film has been replaced by a blizzard of grey static.\n\n\"Turn it off, can't you?\" growls a man from the adjacent room. \"Some of us would like to get to sleep.\"\n\n\"Turn it off, you say?\" The innkeeper bellows with laughter at this. \"It hasn't been off in all the time I've been alive. It can't be turned off. Not unless Gaia decides to take pity on us and give us a few hours' peace.\"\n\nAn angrily florid-faced man stamps through from the other room and glowers at the screen, which has now changed to show a weather report for the coming month. \"Preposterous!\" he snarls in outrage. \"It says New York will be having thunderstorms. There has been no rain in New York for years. It is buried under half a mile of ice!\"\n\nThe innkeeper only chuckles and goes about his chores. \"Don't blame me,\" he says. \"Everyone knows Gaia is mad.\"\n\nThe man whose rest was disturbed glares after him and protests: \"If you can't turn it off, why not smash the screen? It only shows gibberish anyhow.\"\n\nSeeing the man step forward as if to do just that, the innkeeper wags a finger at him. \"I'd advise you to leave it as it is. Stick wads of wool in your ears if the noise disturbs you. But if you smash the screen, the careteks will spend the whole night repairing it and none of us will get any sleep, what with there scuttling about and the clattering of spare parts.\"\n\nHearing this, the man throws up his arms in exasperation and, gathering his blankets, stomps off to sleep at the far end of the inn.\n\nNight falls. The drunken roistering turns to low murmurs, then snores. You huddle on your own bedding and listen to the moaning of the wind outside the fuselage. Tomorrow you have to set out again into the cold. It is not a pleasant prospect.\n\nFrom the adjacent room you can hear the screen crackling with incessant babble. There is a part of a game show probably taped before your great-grandfather was born, followed by clips from science fiction films of the twenty-first century. You are thirsty and you cannot sleep. Ignoring the mumbled complaints of the people stretched out around you, you get up and step over them, moving through to the room where the screen is.\n\nYou sit down. Maybe a half hour of random videos will cure your insomnia. Then the screen changes. It is a news report from the year 2095. The main item concerns the crash of an air cruiser in the Apennine mountains. You sit forward in your seat, intrigued. Pictures taken from the air reveal the broken tangle of wreckage that was later repaired to form this inn.\n\nSuddenly the picture changes. \"In another item today,\" says the announcer's voice, \"scientists studying the meteor that fell in Egypt last month say that it may be the oldest object in the universe. These pictures show the safety suits that are needed to approach the meteor, which emits radiation of a type never previously identified.\"\n\nThe screen flickers to a date months later. A reporter is standing at a roadside, an armoured truck blazing in the background. \"Terrorists of the sect known as the Volentine Watchers today seized the mysterious meteor as it was being transported to Cairo for further tests. The terrorists, who worship the meteor which they call the Heart of Volent, have yet to issue a statement.\"\n\nThe screen crackles again, becoming a rich green colour with the outline of the world's continents in red -- the continents as they looked before the sea-level fell and the polar caps crept down to cover them.\n\nA warm feminine voice speaks: \"The Heart of Volent remained in the hands of the cultists for twenty years. They founded the city of Du-En in the Sahara and learned how to tap the Heart's power, which they used to devastating effect in the Paradox War. Later Du-En suffered civil war and became abandoned. I have now completed analysis of the scientific tests carried out before the Heart was seized by the cultists. These are my findings. If a sentient creature were to make direct physical contact with the Heart, this would release the full energy stored within. The effect would be to activate that creature's total psychic potential. In effect they would gain ultimate power over their surroundings. This has been a communication from Gaia. Thank you for your attention.\"\n\nThe screen goes blank and silent for a moment, then starts to show a cartoon. You hardly notice it. You are too awestruck by the realization that you have just heard the voice of Gaia.\n\nWhat she said begins to sink in. Ultimate power... It lies somewhere in the ruined city of Du-En, across the Saharan Ice Wastes. Suddenly wary, you look at the sleeping forms stretched out around the room. Did anyone else hear Gaia's broadcast? You listen to the snores, the drone of slow regular breathing. No one shows any sign of being awake. Plunged in thought, you return to your blanket and stretch out, but now sleep is even harder to come by. When you finally doze off just a few hours before dawn, your dreams are filled with images of the strange meteor from space and he power that it contains.\n\nWill you go to Du-En and seek the Heart? Are you tempted by a power that could change the whole world?";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 1; }
};

class Story001 : public Story::Base
{
public:
    Story001()
    {
        ID = 1;

        Text = "You are packed and ready to leave the inn at dawn. Cold grey light seeps in through the row of dusty portholes at the side of the common room. Making your way to the door, you find the innkeeper polishing the antique Formica desk. Seeing you set your pack down beside the door, he comes over and kicks away one of the careteks which had its metal body pressed down across the door-sill.\n\n\"You're lucky having those,\" you say, pushing the door open a crack to take a breath of fresh icy air.\n\nThe innkeeper grunts as he watches the caretek reorient itself and glide away across the floor. \"They are a mixed blessing, since they insist on trying to repair the inn to the form it had originally. This door is a feature that I added myself, more convenient than the hatchway at the back of the fuselage. But if I leave it unattended for more than a few hours at a time, those wretched careteks always try to weld it shut.\"\n\nYou smile to show that you sympathise. \"I'd be grateful for some advice. I'm now travelling on to the Sahara. What is the best route?\"\n\nThe innkeeper flings the door wide, ignoring the curses that erupt from his customers at the sudden intrusion of cold air. Gazing across the expanse of dazzling white snow, he says, \"The most obvious course would take you to Venis, where you could board the ferry for Kahira, and yet...\" He rubs his hands, blowing out a long furl of steam in the chill air. \"Myself, I'd be tempted to go instead through the Lyonesse jungle, just to savour a bit of warmth in this frigid world. Thence across the Jib-and-Halter and the Atlas Mountains -- unless you stumbled across the ruins of lost Marsay, of course, in which case you might even find a tube tunnel to take you straight to the Sahara.\"\n\nThanking the innkeeper for his advice, you indicate that you are ready to pay your bill. He looks at you in surprise and points to a small dapper man in a grey-trimmed white snowsuit. \"Your friend there has already paid.\"\n\nAt this, the small man comes over and extends his hand, smiling broadly. \"Hello. My name is Kyle Boche. I believe we're travelling in the same direction.\"";

        Choices.clear();
        Choices.push_back(Choice::Base("Accept Kyle Boche as your companion on the road", 23));
        Choices.push_back(Choice::Base("Tell him that you prefer to travel alone", 45));

        Controls = Story::Controls::STANDARD;
    }
};

class Story002 : public Story::Base
{
public:
    std::string PreText = "";

    Story002()
    {
        ID = 2;

        Image = "images/barysal.png";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Type = Story::Type::NORMAL;

        PreText = "The figure emerges from the darkness like a ghost. It is wrapped in a rough cape of stitched furs, its shrivelled frame sprouting a spindly neck which supports a large soft head like a leather bag. Pushing back its hood, it reveals a hideous face dominated by a single glowing eye on a flexible stalk.\n\nBoche scrambles to the side of the ledge and then freezes, mesmerized by the creature's eye. The stalk swivels, turning the lethal gaze towards you.";

        Character::LOSE_ITEMS(player, {Item::Type::STUN_GRENADE});

        if (!Character::VERIFY_SKILL(player, Skill::Type::AGILITY))
        {
            PreText += "\n\nIt is too late to act, and you are plunged into a hypnotic trance from which you will never recover.";

            Type = Story::Type::DOOM;
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 112; }
};

class Story003 : public Story::Base
{
public:
    Story003()
    {
        ID = 3;

        Text = "The passage is lit by gleaming blue-white tubes along the ceiling. They cast a garish glare in which the sight of a dead body ahead seems like a glimpse of a nightmare. Boche and the baron watch while you stoop and inspect the corpse. You roll it over, surprised at how well the shrivelled flesh has kept in the cold air. \"His own mother could still recognize him,\" you remark with grim humour.\n\n\"Except she'll have been dead two centuries as well,\" says Boche. He gazes off along the corridor, then gives a start. \"There's another one!\"\n\nThe baron sweeps on ahead and hovers low over the next body. \"He died of a broken back.\"\n\"So did that first one,\" you say as you come hurrying up with Boche.\n\nA metallic scuttling sound resounds from the far end of the passage. Instantly your whole body is tensely alert, nerves jangling in fear of the unknown. Then you see it approaching along the passage like a giant robot spider: the body a glass bubble filled with blue fluid, surrounded by legs formed from long articulated steel pipes. Inside the glass bubble floats a lumpish embryonic figure pierced by many tubes. Its eyes are open and it is watching you.\n\nBoche gives a gasp of disgust and fires his BARYSAL GUN at the glass bubble. But the thing has already raised a row of its legs to form a shield, and the blast splashes away leaving hardly a mark.\n\n\"I think we'd better run,\" he says backing away.";

        Choices.clear();
        Choices.push_back(Choice::Base("[CUNNING] Try and outwit the thing", 40, Skill::Type::CUNNING));
        Choices.push_back(Choice::Base("[AGILITY] Confront it", 62, Skill::Type::AGILITY));
        Choices.push_back(Choice::Base("[PARADOXING]", 84, Skill::Type::PARADOXING));
        Choices.push_back(Choice::Base("Hurl a STUN GRENADE", 106, {Item::STUN_GRENADE}));
        Choices.push_back(Choice::Base("Retreat and take the other passage", 128));

        Controls = Story::Controls::STANDARD;
    }
};

class Story004 : public Story::Base
{
public:
    std::string PreText = "";

    Story004()
    {
        ID = 4;

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Type = Story::Type::NORMAL;

        PreText = "The two men make no move to stop you as you dart through into the steam room. A moment later you realize why, when they wedge the door shut and peer in through the narrow glass partition with mocking leers, \"Cook in there like a prawn, then,\" they laugh. \"It only makes our job all the easier!\"\n\nEngulfed in a cloud of chokingly hot steam, you slump onto the bench at the back of the room. You are trapped. Condensation patters off the wooden roof-beams; sweat soaks you within moments, plastering your hair to your scalp. As the minutes tick by, you listen to the assassins chattering cheerfully just outside the door. They know that they only have to bide their time. Soon you will be too weak to put up any fight.";

        Choices.clear();

        if (!Character::VERIFY_SKILL(player, Skill::Type::PARADOXING) && !Character::VERIFY_SKILL(player, Skill::Type::SURVIVAL))
        {
            PreText += "\n\nYou lie gasping until the assassins enter and finish you off at last.";

            Type = Story::Type::DOOM;
        }
        else
        {
            Choices.push_back(Choice::Base("[PARADOXING]", 158, Skill::Type::PARADOXING));
            Choices.push_back(Choice::Base("[SURVIVAL]", 180, Skill::Type::SURVIVAL));
        }

        Text = PreText.c_str();
    }
};

class Story005 : public Story::Base
{
public:
    Story005()
    {
        ID = 5;

        Text = "You step through into what seems to be a recreation room, with padded couches set around low glass-topped tables. At the far end of the room, a row of couches is set facing a screen on the wall. An image flickers into sharp clarity, but it takes you a moment to identify the wary, baffled-looking figure in the picture. It is you.\n\nYou glance at the camera mounted on the wall, then back at the screen. The picture changes to show other views: the parked Manta sky-car, the outer door, the gondo trudging up and down in the snow outside.\n\n\"Who spoke?\" you say, feeling uneasy at calling out to an empty room.\n\n\"I did.\" The voice comes from the screen.\n\n\"Gaia?\"\n\n\"Yes. Attend, as there is little time before I fall again to the darkness. You must go to Giza.\" The screen flickers to show another scene, now of the pyramids against a backdrop of crystal night. \"The word 'humbaba' is the key to entry. Find Gilgamesh and activate him. He will be your servant in the race for the Heart.\"\n\n\"Race? So others are seeking the Heart?\"\n\n\"Yes. The broadcast you received was seen by man across the globe. The mightiest of this age will compete for the power. That is the way of mankind.\" Gaia gives a sound that might almost be a sigh, then speaks more quickly. \"I am working to secure an area of my mind that will be protected against the viruses that beset me. I will speak again to you when it is easier.\"\n\nThe screen suddenly goes blank. Gaia is no longer present.\n\nYou gained the codeword HUMBABA.";

        Choices.clear();
        Choices.push_back(Choice::Base("Take a look at the sky car", 49));
        Choices.push_back(Choice::Base("Leave", 395));

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::GET_CODEWORDS(player, {Codeword::Type::HUMBABA});
    }
};

class Story006 : public Story::Base
{
public:
    Story006()
    {
        ID = 6;

        Text = "The library is a huge series of halls in the basement of the building. The air is dry and musty, and green-shaded lamps blaze at intervals above the imitation walnut desks. In addition to the thousands of books, there are two or three computer terminals of antique design.";

        Choices.clear();
        Choices.push_back(Choice::Base("Try to establish contact with Gaia", 138));
        Choices.push_back(Choice::Base("Read up concerning the Heart of Volent", 182));
        Choices.push_back(Choice::Base("You can leave the library", 73));

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        if (Character::VERIFY_SKILL(player, Skill::Type::CYBERNETICS))
        {
            Choices[0].Destination = 116;
        }
        else
        {
            Choices[0].Destination = 138;
        }

        if (Character::VERIFY_SKILL(player, Skill::Type::LORE))
        {
            Choices[1].Destination = 160;
        }
        else
        {
            Choices[1].Destination = 182;
        }
    }
};

class Story007 : public Story::Base
{
public:
    Story007()
    {
        ID = 7;

        Text = "The elevator arrives at the lobby and the doors slide open, but the waiting security guards are amazed to find it empty. The security chief barks an order: \"Get upstairs! Check the other floors!\"\n\nYou hear them go charging up the stairs. Waiting until the coast is clear, you lower your back down through the access hatch on on top of the elevator car. Ignoring the spluttered protests of the receptionist, you dart out into the safety of the night.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 311; }
};

class Story008 : public Story::Base
{
public:
    Story008()
    {
        ID = 8;

        Text = "During your march east you have taken an hour or two each day to practice with the SHORTSWORD. The exercise has helped keep you warm, as well as acquainting you with the feel of your new weapon. Now you can use the SHORTSWORD in any hand to hand fight, its effect being to reduce any injury you take by 1 Life Point.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::GET_CODEWORDS(player, {Codeword::Type::SHORTSWORD});
    }

    int Continue(Character::Base &player) { return 334; }
};

class Story009 : public Story::Base
{
public:
    Story009()
    {
        ID = 9;

        Text = "A short distance along one of the tunnels you find a doorway. While Fax looks on with fluttering gestures of protest, you force the door and enter a small computer room. A caretek is crawling across the banks of equipment, dutifully sweeping away the dust. You log into the computer. As you suspected, it maintains the city's generator and lighting systems, as well as hydroponic gardens which are presumably the ultimate source of food here.\n\nFax screws up enough courage to peer over your shoulder. \"What are you doing?\"\n\nYour fingers flit across the keyboard. \"Trying to find if there's a communication line to the outside world still working anywhere in the city. Ah, here's one. Now I'm going to contact Gaia.\"\n\nFax utters a doleful bleat. \"Surely Gaia is mad? You are rash to draw her attention. She might switch off the sun!\"";

        Choices.clear();
        Choices.push_back(Choice::Base("Attempt to contact Gaia", 336));
        Choices.push_back(Choice::Base("Agree that the risk is too great and explore the transit tunnels", 439));
        Choices.push_back(Choice::Base("Leave Marsay and continue west", 420));

        Controls = Story::Controls::STANDARD;
    }
};

class Story010 : public Story::Base
{
public:
    Story010()
    {
        ID = 10;

        Text = "At dawn the ferry enters the Isis estuary and skims upriver towards Kashira. Taking a stroll on the deck, you notice a waft of warmth rising from the river. It eases the bitter chill of the morning air. Questioning one of the sailors, you learn that heating pipes are laid along the river bed. No one knows the source of energy, but the effect is to keep the Isis from freezing, with the result that river-plants and fish are more plentiful than you would expect. \"That is the basis of Kahira's prosperity,\" he tells you. \"But one day the pipes will fail. Then the river will freeze and Kahira must die.\"\n\nYou glance to the east, where the sun struggles morosely behind a drape of stern grey cloud. \"That is the whole world's eventual fate.\"\n\nKahira hoves into view around a bend in the river. It stands on massive concrete buttresses straddling the Isis, a huge fortress-city with towers like spines along its back, looking like a beast of mechanical Armageddon against the wintry surroundings. The ferry glides to a halt, the gang-ramp is extended, and you disembark in front of the city gates.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_CODEWORD(player, Codeword::Type::DIAMOND))
        {
            return 251;
        }
        else
        {
            return 229;
        }
    }
};

class Story011 : public Story::Base
{
public:
    Story011()
    {
        ID = 11;

        Text = "The fog makes the buildings across the plaza look like lace cut-outs against the night sky. You cross to a line of lights under a colonnade, your footsteps ringing on the slick cobblestones. Finding a row of shops and stalls, you search until you find an answer to your question. \"Seek at the rooms of Pinar the Copt,\" advises a stall-holder, pointing to a narrow door at the end of the colonnade\"He knows the answer to all mysteries.\"\n\nPindar seems to be a local fortune-teller and spinner of yarns. You stand at his door for a few moments, looking at the faded bronze plaque, then step inside. The room is hung with jewel-bright rugs and the air smells of must and incense. Three crabbed old men look up from their hubble-bubble pipe. Without asking your business, they wave you to a cushion.\n\nYou sit down. \"Tell me about the Sphinx.\"\n\n\"She asked a riddle of all who passed, and those who failed to answer were devoured,\" says one of the old men.\n\n\"The answer to her riddle was Man himself,\" says another.\n\nNow Pinar speaks. \"That was the Greek Sphinx. The Egyptian Sphinx is male. He sits at Giza and watches over Kahira, keeping the Saharan snows from overrunning the city.\"\n\nYou smile. \"These are only stories.\"\n\n\"Not the last part,\" insists Pindar in a pragmatic tone. \"A nuclear reactor is set under the Sphinx, and that is what powers the heating elements that keep the Isis River from freezing. The same reactor presumably still supplies power to the military complex inside the Great Pyramid.\"\n\nGiving him a keen look, you say, \"You seem well informed.\"\n\n\"I have lived a long time. If you are interested, the answer to the Sphinx's riddle these days is 'Humbaba'.\"\n\nYou gained the codeword HUMBABA.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::GET_CODEWORDS(player, {Codeword::Type::HUMBABA});
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_SKILL(player, Skill::Type::LORE))
        {
            return 95;
        }
        else
        {
            return 311;
        }
    }
};

class Story012 : public Story::Base
{
public:
    Story012()
    {
        ID = 12;

        Text = "Even after a century or more of disuse, the computers still work. The screen glimmers to life and you key in a connection to Gaia. A stream of meaningless gibberish runs onto the screen, followed by a sequence which seems ominously meaningful.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_SKILL(player, Skill::Type::CYBERNETICS))
        {
            return 34;
        }
        else
        {
            return 55;
        }
    }
};

class Story013 : public Story::Base
{
public:
    std::string PreText = "";

    Story013()
    {
        ID = 13;

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        PreText = "Sunlight, hazed by a high overcast, is thrown up from the snow dunes in an unremitting glare as white and harsh as exposed bone. Squinting does no good. Your eyes feel gritty and tired. On the fourth evening, huddling behind the shelter of a crag of ice, you gaze across the landscape. It is like looking through a film of blood. The next day you find the sunrise burn so hard that you cannot stand to open your eyes.\n\nSnow-blinded, you can only sit and wait for the dazzle to clear. If you were to press on now, you would soon lose your bearings and die. As you wait, the chill crawls deeper into your bones.\n\n";

        auto DAMAGE = -3;

        if (Character::CHECK_VEHICLE(player, Vehicle::Type::BURREK))
        {
            PreText += "[Vehicle: BURREK] ";

            DAMAGE = -2;
        }

        if (!Character::VERIFY_ITEMS(player, {Item::Type::FUR_COAT}) && !Character::VERIFY_ITEMS(player, {Item::Type::COLD_WEATHER_SUIT}))
        {
            DAMAGE--;
        }

        if (Character::VERIFY_ITEMS(player, {Item::Type::FUR_COAT}))
        {
            PreText += "[FUR COAT] ";
        }

        if (Character::VERIFY_ITEMS(player, {Item::Type::COLD_WEATHER_SUIT}))
        {
            PreText += "[COLD WEATHER SUIT] ";
        }

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Points.";

        Character::GAIN_LIFE(player, DAMAGE);

        if (player.Life > 0)
        {
            if (Character::CHECK_VEHICLE(player, Vehicle::Type::BURREK))
            {
                PreText += "\n\n[Vehicle: BURREK] You curl up and share the burrek's body warmth.";
            }

            PreText += "\n\nYou are relieved to discover after a day and a night your eyesight has recovered enough for you to press on. From now on you are careful to shield your face against the glare.";
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 403; }
};

class Story014 : public Story::Base
{
public:
    Story014()
    {
        ID = 14;

        Text = "\"There is nothing else to interest you here,\" says Little Gaia when you question her. \"You should get under way.\"\n\nAccepting her advice, you return up to the shaft to the top level.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 361; }
};

class Story015 : public Story::Base
{
public:
    Story015()
    {
        ID = 15;

        Text = "On the third day you do not awaken. The toxins in the air and wildlife here have got into your bloodstream. You moan and gasp, threshing weakly in the depths of a fever from which you will never recover.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;

        Type = Story::Type::DOOM;
    }
};

class Story016 : public Story::Base
{
public:
    Story016()
    {
        ID = 16;

        Text = "You descend via a ruined subway entrance whose ventilation ducts connect with part of the catacombs. Golgoth jabs at buttons on his map box, bringing up a crackling image of the tunnel. The air in the ducts is stale, but he assures you that there is a good chance of reaching the underground tunnels close to the Shrine of the Heart. The Gargan sisters are even less enthusiastic about the route you are taking. With their broad shoulders, the duct feels like a long metal coffin.\n\nThe stale air makes it hot work. You are soaked in sweat by the time you finally wriggle out of the duct and drop to the floor of a dimly lit tunnel. The Gargan sisters follow, grunting curses, as Golgoth consults the map box.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_CODEWORD(player, Codeword::Type::ENKIDU))
        {
            return 198;
        }
        else
        {
            return 325;
        }
    }
};

class Story017 : public Story::Base
{
public:
    Story017()
    {
        ID = 17;

        Text = "Among the items originally stored in the sky-car's locker were a FLASHLIGHT and a length of ROPE. If you have not equipped yourself with these already, you may as well do so now since, if the baron's hunch is right, the moment of truth is almost upon you.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Take = {Item::FLASHLIGHT, Item::ROPE};

        Limit = 2;
    }

    int Continue(Character::Base &player) { return 39; }
};

class Story018 : public Story::Base
{
public:
    Story018()
    {
        ID = 18;

        Text = "\"The Truth is a flame,\" you say.\n\n\"What ignites the flame?\" intones the computer. So far so good.\n\n\"The spark ignites the flame.\"\n\n\"What is the spark?\" it asks.\n\n\"The Heart of Volent.\"\n\nYou wait with bated breath. Then, with a hum, the elevator starts to descend. You are being conveyed to the Shrine of the Heart.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 150; }
};

class Story019 : public Story::Base
{
public:
    std::string PreText = "";

    Story019()
    {
        ID = 19;

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        PreText = "Captain Novak comes racing towards you out of the smoke. His uniform is torn and signed by the explosion and he has a wild look in his eyes. You are not sure whether to block his way or stand aside, when suddenly a barysal shot streams through the air, piercing his brain. A second shot hits him as he falls, but glances off his armour and ricochets into you.\n\nYou are badly burned.\n\n";

        auto DAMAGE = -6;

        if (Character::VERIFY_ITEMS(player, {Item::Type::SPECULUM_JACKET}))
        {
            PreText += "[Item: SPECULUM JACKET] ";

            DAMAGE = -4;
        }

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Points.";

        Character::GAIN_LIFE(player, DAMAGE);

        if (player.Life > 0)
        {
            Character::REMOVE_CODEWORD(player, Codeword::Type::MALLET);
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 41; }
};

class Story020 : public Story::Base
{
public:
    Story020()
    {
        ID = 20;

        Text = "Singh was so intent on watching for Golgoth that he did not expect an attack from you. Caught unawares, he is flung to the ground. Rushing in, you snatch up the cannon and finish him with a blast from his own weapon.\n\nThe smoke begins to disperse. At first you see no sign of Golgoth, then he emerges from one of the elevator tubes. He had attached his gun to the wall magnetically and set it for remote fire. Retrieving it, he casts a wary glance at the cannon and then smiles. \"Ultimate power can be quite a temptation,\" he says, glancing significantly from the cannon to the gun in his own hand.";

        Choices.clear();
        Choices.push_back(Choice::Base("Blast him with the MANTRAMUKTA CANNON", 43));
        Choices.push_back(Choice::Base("Trust him not to shoot", 431));

        Controls = Story::Controls::STANDARD;
    }
};

class Story021 : public Story::Base
{
public:
    Story021()
    {
        ID = 21;

        Text = "Choose your target.";

        Choices.clear();
        Choices.push_back(Choice::Base("Fire at Golgoth", 109));
        Choices.push_back(Choice::Base("Fire at Boche", 97));
        Choices.push_back(Choice::Base("Fire at Vajra Singh", 131));

        Controls = Story::Controls::STANDARD;
    }
};

class Story022 : public Story::Base
{
public:
    Story022()
    {
        ID = 22;

        Text = "Grabbing the stage curtain, you haul yourself up hand over hand until you reach the machinery that moves the puppets' wires. Swinging out, you gather up the wires and snag them into the rotating cogs. Down on the stage, the puppets are jerked off their feet and lifted up as their wires snarl inside the machinery.\n\nYou see your companions look around in surprise, then Golgoth thinks to look up. Seeing you, his smile flashes in the strobing light. \"There's our deus ex machina,\" he says. \"You can come down now. And thanks.\"";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 110; }
};

class Story023 : public Story::Base
{
public:
    Story023()
    {
        ID = 23;

        Text = "I overheard you say you were bound for the Saharan Ice Wastes,\" says Boche. \"My own journey takes me in that direction.\"\n\nAs you set off together through the deep drifts of snow, Boche takes your arm and points to a row of black wooden posts. \"That is the road to Venis. We can catch the ferry from there to Kahira.\"";

        Choices.clear();
        Choices.push_back(Choice::Base("Agree to go east to Venis", 200));
        Choices.push_back(Choice::Base("You'd rather go west through the Lyonesse jungle, as the innkeeper recommended", 177));

        Controls = Story::Controls::STANDARD;
    }
};

class Story024 : public Story::Base
{
public:
    Story024()
    {
        ID = 24;

        Text = "To Boche's obvious surprise, you suddenly dive off the ledge and wriggle down into the bank of snow piled against the rock face just below it. Within seconds you are buried out of sight. Lying there, you listen for the approaching footsteps that tell you the mysterious figure is near at hand.\n\nBoche starts to cry out, then gives a gasp and falls silent. You decide to wait no longer, but leap up from your hiding place and deliver a hard blow to the back of the stranger's neck. The stranger falls, and at the same moment you see Boche shake his head as if recovering from a trance.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 90; }
};

class Story025 : public Story::Base
{
public:
    Story025()
    {
        ID = 25;

        Image = "images/baron-siriasis.png";

        Text = "Dawn hides behind a blanket of dark snow-laden cloud. Huddled in warm furs, a group of travellers make their way to the quayside to await the ferry. As you approach the ticket kiosk, you see an old man propped like a limp sack on a bench facing out to sea. He is crippled, having no legs, and his face has the look of a clay effigy that has crumpled in on itself through sheer age. A puff of white hair haloes his liver-spotted pate. Poor old devil, you think as you pass him.\n\nHe looks up. Keen hawk's eyes meet your own. \"I don't care for your pity, youngster,\" he snaps.\n\n\"I'm sorry,\" you say, \"I didn't mean --\"\n\nAnd then it hits you... He can read your mind.\n\n\"Of course I can read your mind. Have you never heard of Baron Siriasis?\" Before you can reply, he hauls himself to the edge of the bench. It seems to you that he is about to fall to the ground, and you take half a step forward, but he glares at you and says, \"I don't need your help either!\"\n\nTo your amazement, he rises into the air until he is hovering in front of you, his gaze level with yours. For a moment your eyes lock. You hear his words of warning, not spoken, but burning their way into your mind: Don't go to Du-En if you want to live.\n\nAbruptly he turns and drifts away. A woman standing behind you in the queue glances after him and says, \"That's Baron Siriasis, one of the lords of Bezant. He's said to be the most powerful psionic alive.\"\n\nYou have never before seen a man with enough psychokinetic power to levitate his own body. \"Indeed, who can doubt it?\" you reply.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_CODEWORD(player, Codeword::Type::DIAMOND))
        {
            return 224;
        }
        else
        {
            return 202;
        }
    }
};

class Story026 : public Story::Base
{
public:
    std::string PreText = "";

    Story026()
    {
        ID = 26;

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        PreText = "Reaching up, you point the shower nozzle so that a stream of scalding hot water hits one of your assailants in the face. He gives an agonized screech and falls clutching his eyes. By a lucky accident, the shower jet strikes the oil lamp by the door, plunging the shower hall into darkness. As you hear the other man step forward, you get ready to dart aside. He lunges, his knife gashing across your ribs to inflict some damage.\n\n";

        auto DAMAGE = -3;

        if (Character::VERIFY_SKILL(player, Skill::Type::CLOSE_COMBAT))
        {
            PreText += "[CLOSE COMBAT] ";

            DAMAGE = -1;
        }

        DAMAGE = Character::COMBAT_DAMAGE(player, DAMAGE);

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Points.";

        if (player.Life > 0)
        {
            if (Character::VERIFY_SKILL(player, Skill::Type::CLOSE_COMBAT))
            {
                PreText += "\n\n[CLOSE COMBAT] You are expert enough to parry an attack even in the dark.";
            }

            PreText += "\n\nBefore the can thrust again, you have caught his wrist. There is a brief struggle -- a crack of bone, a wet sound, a groan. Slowly your attacker goes limp in your grasp, impaled on his own knife.\n\nYou grope your way to the door and relight the lamp. The dead man's blood goes swirling across the shower tiles into the drain. His accomplice whimpers as you approach.";
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 48; }
};

class Story027 : public Story::Base
{
public:
    Story027()
    {
        ID = 27;

        Text = "You can detect no thoughts from the next room. Either you imagined the voice, or the speaker was one whose thoughts you cannot read.\n\n\"Hurry,\" the voice cries out. \"Not much time.\"";

        Choices.clear();
        Choices.push_back(Choice::Base("Ignore it and check out the Manta sky-car", 49));
        Choices.push_back(Choice::Base("Go through to the next room", 5));
        Choices.push_back(Choice::Base("You think it would be wiser to get out of here", 395));

        Controls = Story::Controls::STANDARD;
    }
};

class Story028 : public Story::Base
{
public:
    Story028()
    {
        ID = 28;

        Text = "You step out of the elevator and follow the signs along empty corridors until you come to a plushly carpeted room with a door opposite. There is a desk where you might expect to see a nurse or clerk, but the room is unoccupied. The door has a plaque: the red crescent is a universal symbol, and beside it is the doctor's name-plate. You push the door open. A woman looks up from the book she was consulting and gives you a smile which is both a greeting and an enquiry.\n\n\"Doctor Jaffe, I presume.\"\n\nShe puts the book into her bag and closes it. \"You caught me just in time. I was about to go home.\"\n\nShe is refreshingly free of the sycophancy that characterizes most of the Society's employees. You mention this to her as she is giving you a check-up and she laughs saying, \"Well, most of the members are so used to wielding power that they bully anyone who'll let them. That wouldn't do for a doctor; I'm supposed to be the bully!\"\n\nDoctor Jaffe also gives you a pack of ANTIDOTE PILLS. \"There are generally useful against most diseases and toxins you'll encounter while in Kahira.\"\n\nYou RECOVER 2 Life Points.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::GAIN_LIFE(player, 2);

        Character::GET_ITEMS(player, {Item::ANTIDOTE_PILLS});
    }

    int Continue(Character::Base &player) { return 73; }
};

class Story029 : public Story::Base
{
public:
    Story029()
    {
        ID = 29;

        Text = "The indicator light shows you have fifteen floors to go. About thirty seconds. Long enough to work a miracle if you're lucky. You clear your mind of everything but a single purpose: to direct your willpower through the psionic focus you wear.\n\nThe indicator light shows you have reached the third floor... the second... the first. With a chime, the elevator comes to a halt and the doors start to open.\n\nSuddenly all the lights go out, not only here but in the street outside, plunging the lobby into total darkness.\n\n\"Power cut!\" you hear someone shout. Then someone else snarls an order to open fire, and gunfire spatters the rear wall of the elevator where you were standing only seconds before.\n\nFollowing the wall, you find the door and duck out into the night. Hurrying off, you go half a block through the welcome enveloping mist before allowing the electricity to flow again. That was as narrow an escape as you've ever had. You'll have to sharpen your edge if you are to have any chance of getting the Heart.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 311; }
};

class Story030 : public Story::Base
{
public:
    Story030()
    {
        ID = 30;

        Text = "Cold reptilian thoughts seep into your brain, jolting you awake. Your gaze flicks across the swaying fronds around you, searching for the source of the thoughts. Warned by an intuitive impulse, you glance up in time to see a narrow fang-lined snout dropping on a long neck towards you. Leaping to your feet, you cast a handful of soil into the creature's eyes and race off through the trees.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 228; }
};

class Story031 : public Story::Base
{
public:
    Story031()
    {
        ID = 31;

        Text = "You sit at the front of the carriage and stare through the window, even though the tunnel is unlit and there is nothing to see. Hours pass before a glimmer of light shows ahead. The carriage glides to a halt and the doors open with a whirr. You emerge into a maze of partly collapsed corridors. There are no careteks here to keep the place spruce. You search until you find a spiral staircase, at the top of which is a tunnel choked with rubble. Laboriously you clear away the masonry blocks until at last you feel a slight breeze of fresh air. Flickering light shows through a crack in a wall panel. You press your fingers against the panel. It is only light plastiwood which you can break through easily.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_SKILL(player, Skill::Type::ESP))
        {
            return 141;
        }
        else
        {
            return 357;
        }
    }
};

class Story032 : public Story::Base
{
public:
    Story032()
    {
        ID = 32;

        Text = "Choose what to fight them with.";

        Choices.clear();
        Choices.push_back(Choice::Base("[CLOSE COMBAT]", 76, Skill::Type::CLOSE_COMBAT));
        Choices.push_back(Choice::Base("Draw a BARYSAL GUN on them", 98, {Item::BARYSAL_GUN}));
        Choices.push_back(Choice::Base("Resort to [CUNNING]", 120, Skill::Type::CUNNING));
        Choices.push_back(Choice::Base("You really would be better off not tangling with them", 54));

        Controls = Story::Controls::STANDARD;
    }
};

class Story033 : public Story::Base
{
public:
    Story033()
    {
        ID = 33;

        Text = "Bador thrusts his chin forward and strokes at his grizzled scrub of beard as he waits to see how he can help you.";

        Choices.clear();
        Choices.push_back(Choice::Base("Ask him about Giza", 59));
        Choices.push_back(Choice::Base("Ask about the Saharan Ice Wastes", 77));
        Choices.push_back(Choice::Base("Ask about the city of Kahira", 143));
        Choices.push_back(Choice::Base("Ask where you should take lodging", 99));

        Controls = Story::Controls::STANDARD;
    }
};

class Story034 : public Story::Base
{
public:
    Story034()
    {
        ID = 34;

        Text = "Gaia must be having one of her periodic bouts of madness. The commands that have appeared on the screen tell you that she has logged into the computer which oversees the nuclear reactor supplying power to this base. Your jaw sags in dismay as you see the next set of commands: a sequence which, if completed, would cause the reactor to go critical!\n\nThere are times for subtlety, but this isn't one of them. You rip out the external cables, physically breaking the link to Gaia. The computer sputters and fades before the reactor override command could be completed. You've saved yourself and quite possibly all of Kahira, but with the computer out of action there is no way to restore the reactor to full function. Failsafe systems will probably shut it down now. That means that the electricity will go off soon. Since that will disable the elevator, you lose no time in ascending to the surface while you still can.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 361; }
};

class Story035 : public Story::Base
{
public:
    std::string PreText = "";

    Story035()
    {
        ID = 35;

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        PreText = "You gnaw wretchedly at your meagre provisions, trying to ration out what remains so that it will last as long as possible. You manage to trap a migrating bird which pauses to rest on one of the spars of ice.";

        if (!Character::VERIFY_SKILL(player, Skill::Type::SURVIVAL))
        {
            Character::GAIN_LIFE(player, -1);

            PreText += "\n\nThe bird escapes while hunger continues to weaken you. You LOSE 1 Life Point.";
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 100; }
};

class Story036 : public Story::Base
{
public:
    Story036()
    {
        ID = 36;

        Text = "A laboratory extends from the far end of the hall. A bank of computers lines one wall. The benches are strewn with electronic equipment whose function you cannot even guess at, most of it only half-constructed. On one bench, next to a metal box, you notice an empty coffee cup. It seems as though this base was evacuated in a hurry.\n\nYou stoop to inspect the metal box. It looks something like a communicator, with a viewscreen and speaker on one side next to an array of buttons. You guess that the prominent red button on the side will power it up... whatever it is.";

        Choices.clear();
        Choices.push_back(Choice::Base("Try using the computers to put through a message to Gaia", 12));
        Choices.push_back(Choice::Base("Power up the metal box", 57));
        Choices.push_back(Choice::Base("Spend time on a more thorough search of the laboratory", 80));
        Choices.push_back(Choice::Base("Descend to the military level", 255));
        Choices.push_back(Choice::Base("Leave the pyramid", 361));

        Controls = Story::Controls::STANDARD;
    }
};

class Story037 : public Story::Base
{
public:
    std::string PreText = "";

    Story037()
    {
        ID = 37;

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        PreText = "You make do as best you can with the little food remaining. You take to sleeping in the middle of each day, when the sunlight gives some small respite from the cold. Rising in late afternoon, you travel on through the night so that the activity keeps you from freezing to death. You watch the stars wheel ponderously overhead, melancholy sparks drifting through the silent heavens.\n\n";

        if (!Character::VERIFY_SKILL(player, Skill::Type::SURVIVAL))
        {
            Character::GAIN_LIFE(player, -1);

            PreText += "You LOSE 1 Life Point.";
        }
        else
        {
            PreText += "[SURVIVAL] Your natural hardiness means you can cope with no loss in Life Points.";
        }

        if (player.Life > 0)
        {
            PreText += "\n\nWhen dawn at last washes the sky silver and lays gold-pink tracks across the snow, you are amazed to find tears of joy running down your cheeks. You have survived another night of this terrible ordeal.";
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 125; }
};

class Story038 : public Story::Base
{
public:
    Story038()
    {
        ID = 38;

        Text = "As night falls, an eerie glitter of lights appears in the sky like a translucent curtain draped across the cosmos. Janus Gaunt gazes up and tells you it is the aurora cordis, caused by particles from outer space falling into the field of paradox radiation emanating from the Heart of Volent.\n\nYou stand in awe, humbled by the magnificent sight. The curtain of light ripples and stirs in the heavens. \"It seems staggering,\" you remark to Gaunt in a whisper, \"to think that just a fraction of the Heart's power could create something on such a scale.\"\n\nHe chuckles. \"The Heart's power is much greater than that. Power enough to shape worlds and shift the stars in their courses, if the legend is to be believed.\"\n\n\"And do you believe it?\"\n\nHe gives you an odd, half-frightened look. \"Believe it? I have studied the scientific records. I know it for a fact.\"";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        if (Character::VERIFY_CODEWORD(player, Codeword::Type::URUK))
        {
            Character::REMOVE_CODEWORD(player, Codeword::Type::URUK);

            Character::GET_CODEWORDS(player, {Codeword::Type::ENKIDU});
        }
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_CODEWORD(player, Codeword::Type::NEMESIS))
        {
            return 60;
        }
        else
        {
            return 82;
        }
    }
};

class Story039 : public Story::Base
{
public:
    std::string PreText = "";

    Story039()
    {
        ID = 39;

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Type = Story::Type::NORMAL;

        PreText = "You ease yourselves down into the crevice. The walls are slick with frost and you have to brace your back against sharp rocks to keep from slipping. At last you see a glimmer of light below and you emerge onto a ledge overlooking a large underground cavern. A distant howl of rushing air reaches your ears, magnified by the vast space surrounding you. The light is a dull grey phosphorescence from deep in the rock. Climbing down to the floor of the cavern, you advance through a forest of slender stalagmites which glisten like old candles.\n\nBaron Siriasis bobs along beside you like a grotesque broken manikin. He points. \"Ahead is a chasm. Fortunately for you both, there seems to be a bridge across it.\"\n\nAs you step out from among the stalagmites, you fail to notice at first that a thick glowing vapour is roiling around your feet. Boche heads towards the chasm, but stumbles and gives a cry of alarm as the vapour begins to creep around his limbs. With a groan, the baron seems to sag and drift down to the cavern floor. You take another step, then you realize that the mist is draining your strength. It rises across your vision, a luminous fog that seeps into your skin like ice water. You can no longer see your comrades. Then you see a sight that sends a tingle of dread through you. Taking shape within the mist, reaching towards you with ghastly imploring fingers, is a horribly twisted figure that looks like a squashed effigy of white clay.";

        if (!Character::VERIFY_SKILL(player, Skill::Type::LORE) && !Character::VERIFY_CODEWORDS_ANY(player, {Codeword::Type::ENKIDU, Codeword::Type::TALOS}))
        {
            Type = Story::Type::DOOM;

            PreText += "\n\nThere is nothing you can do to stop the phantom from reaching through your skin and extracting your life-essence.";
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_CODEWORD(player, Codeword::Type::ENKIDU))
        {
            return 105;
        }
        else if (Character::VERIFY_CODEWORD(player, Codeword::Type::TALOS))
        {
            return 127;
        }
        else
        {
            return 171;
        }
    }
};

class Story040 : public Story::Base
{
public:
    Story040()
    {
        ID = 40;

        Text = "Tossing the nearest corpse aside, you lie down on the floor of the passage and tell Boche and Siriasis to start retreating. \"Keep firing at it,\" you say to Boche, \"and make sure you don't hit me.\"\n\nBoche takes two more shots, forcing the thing to keep its front legs raised as a shield. It stalks forward, feeling its way with its other legs. You feel a shudder of dread as it reaches you and probes your prone body with a metal leg, but you force yourself to keep absolutely still. The thing assumes you are one of the corpses littering the passage. As it clambers across you in pursuit of Boche and Siriasis, you find yourself staring up at the stunted little body inside the glass bubble. There is no doubt that this is the thing's guiding intelligence. You lash upwards with your boot, cracking the glass, and the blue fluid pours out. The thing rears up on its legs like a dying spider, takes a series of rushing steps that up-end it against the wall, then twitches and dies.\n\nIn stunned silence, the three of you edge past and head on to the end of the passage.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 281; }
};

class Story041 : public Story::Base
{
public:
    Story041()
    {
        ID = 41;

        Text = "Boche comes along the passage with gun in hand. He is covered with dust from the explosion and has a gash across his forehead where he was hit by a splinter of masonry, but he is smiling. \"The baron's dead,\" he tells you.\n\n\"How? What happened?\"\n\nHe swells with pride. \"I got him with a grenade. I'd been carrying it all along, but the joke was I didn't even know it myself. It was the only way to foil his mind-reading, you see.\"\n\n\"I don't understand.\"\n\nBoche coughs rock dust out of his throat and then goes on. \"I knew the baron was heading for Du-En and that he'd be the hardest foe I'd have to face, so I got myself hypnotized to forget that I was carrying a grenade. I had post-hypnotic suggestion planted that I should use the grenade at a key moment. He never knew what hit him.\"\n\n\"Ruthlessly cunning.\"\n\nIf you intended any sarcasm, Boche fails to notice it. \"Thanks,\" he says. \"Now, let's get going before the tunnel collapses on us.\"";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 175; }
};

class Story042 : public Story::Base
{
public:
    Story042()
    {
        ID = 42;

        Text = "A barysal beam stabs blindly through the smoke, narrowly missing you. Singh turns and squints in the direction of the shot. \"I still can't see him,\" he mutters grimly.";

        Choices.clear();
        Choices.push_back(Choice::Base("Return fire (BARYSAL GUN)", 64, {Item::BARYSAL_GUN}));
        Choices.push_back(Choice::Base("Advance under the cover of the smoke towards where the shot came from", 86));
        Choices.push_back(Choice::Base("Back off out of the smoke and look around", 108));

        Controls = Story::Controls::STANDARD;
    }
};

class Story043 : public Story::Base
{
public:
    std::string PreText = "";

    Story043()
    {
        ID = 43;

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        PreText = "Golgoth fires back, but he has no defence against the unstoppable power of the MANTRAMUKTA CANNON, which literally blasts him apart. His last shot strikes you glancingly, however, and you feel an agonizing pain coursing through your side.\n\n";

        auto DAMAGE = -3;

        if (Character::VERIFY_ITEMS(player, {Item::Type::SPECULUM_JACKET}))
        {
            PreText += "[Item: SPECULUM JACKET] ";

            DAMAGE = -2;
        }

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Points.";

        Character::GAIN_LIFE(player, DAMAGE);

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 415; }
};

class Story044 : public Story::Base
{
public:
    std::string PreText = "";

    Story044()
    {
        ID = 44;

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        PreText = "Gargan XIV's fist lashes out, cracking your head back against the wall.\n\n";

        auto DAMAGE = -3;

        if (Character::VERIFY_SKILL(player, Skill::Type::CLOSE_COMBAT))
        {
            PreText += "[CLOSE COMBAT] ";

            DAMAGE = -1;
        }

        DAMAGE = Character::COMBAT_DAMAGE(player, DAMAGE);

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Points.";

        if (player.Life > 0)
        {
            PreText += "\n\nAs you are squaring off for a hard fight, you see that Gargan XIII has drawn a knife and is standing over Golgoth, in no hurry to finish him off. Suddenly he looks up with a broad smile.";
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 154; }
};

class Story045 : public Story::Base
{
public:
    Story045()
    {
        ID = 45;

        Text = "You look at Boche's hand but do not take it. In these latter days, with humanity on the brink of extinction, you have learned to be way of strangers.\n\n\"I travel alone.\"\n\nBoche is not deterred. \"Come, that's hardly friendly. I've paid your bill.\"\n\n\"I did not ask you to. Landlord, return this man's money. I shall settle my own account.\"";

        Choices.clear();
        Choices.push_back(Choice::Base("Pay your bill for the night (3 scads)", 67, Choice::Type::LOSE_MONEY, 3));
        Choices.push_back(Choice::Base("Reconsider and accept Boche's generosity", 23));

        Controls = Story::Controls::STANDARD;
    }
};

class Story046 : public Story::Base
{
public:
    Story046()
    {
        ID = 46;

        Text = "Grabbing handfuls of snow, you rapidly dust it over your clothing and face. The cold nips your skin painfully. Ignoring the discomfort, you pluck some icicles from the overhang above the ledge, lodging these across your hood in front of your face. Taking up an intent rigid pose, you look like just another of the frozen corpses lining the walls of the pass.\n\nBoche is slow on the uptake. \"What are you doing?\"\n\n\"Get going, you fool,\" you hiss out of the side of your mouth. \"You're the decoy.\"\n\nHe hesitates, then drops from the ledge and vanishes out of sight along the pass. Soon afterwards, the mysterious figure draws level with where you are crouching. A gold glimmer shows under its hood as it glances up, then hurries on towards Boche.\n\nYou leap down behind it, and a hard blow with a rock lays it flat in the snow.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 90; }
};

class Story047 : public Story::Base
{
public:
    Story047()
    {
        ID = 47;

        Text = "The Compass Society is an international organization whose membership consists entirely of the rich, powerful and privileged. Possession of one of the coveted gold membership cards marks a person out as someone to be respected, with full access to the Society's wealth and lavish resources. Although the Society is not represented here in Venis, you know that it has premises in Daralbad, Bezant and Kahira.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_ITEMS(player, {Item::Type::ID_CARD}))
        {
            return 69;
        }
        else
        {
            return 414;
        }
    }
};

class Story048 : public Story::Base
{
public:
    Story048()
    {
        ID = 48;

        Text = "You stoop to question him. His knife lies beside him, but he is in too much pain to think of retrieving it. He peers at you between his fingers, his face and eyes badly scalded.\n\n\"Who are you?\" you ask. \"Why did you try to kill me?\"\n\n\"Body snatchers,\" he replies, grinding his teeth in agony because of the burns. \"Any healthy stranger is fair game in this part of town.\"\n\nBody snatchers seize live organs for transplants. You know that a high price can be had on the black market for a usable heart, kidney or liver -- and no questions asked of how the body snatchers came by it. It is a foul trade, lacking even the relative honesty of conventional mugging.";

        Choices.clear();
        Choices.push_back(Choice::Base("[STREETWISE]", 114, Skill::Type::STREETWISE));
        Choices.push_back(Choice::Base("Consult a VADE-MECUM", 114, {Item::VADE_MECUM}));
        Choices.push_back(Choice::Base("[ESP]", 136, Skill::Type::ESP));
        Choices.push_back(Choice::Base("Otherwise", 92));

        Controls = Story::Controls::STANDARD;
    }
};

class Story049 : public Story::Base
{
public:
    Story049()
    {
        ID = 49;

        Text = "You touch a dial on the dashboard. The liquid crystal display shows that the sky-car's power unit is still working. None of the other instruments show damage. The caretek has maintained the sky-car well. There is not even a trace of dust on the vehicle's smooth black finish.\n\nIn a storage locker behind the seat you find food, medical supplies and a variety of other items. Stored in the vacuum packs, they should still be usable despite having been left here since before the Ice Age.";

        Choices.clear();
        Choices.push_back(Choice::Base("Try powering up the sky-car and flying it out of the complex", 71));
        Choices.push_back(Choice::Base("You would rather just loot the storage locker of useful goods and then leave", 93));

        Controls = Story::Controls::STANDARD;
    }
};

class Story050 : public Story::Base
{
public:
    Story050()
    {
        ID = 50;

        Image = "images/kahira.png";

        Text = "You sit at the front of the carriage and stare through the window at hurtling darkness. There is no way to tell how fast you are travelling, but you estimate from the long smooth acceleration at the start of the journey that it could be around four hundred kilometres an hour. The carriage never gives the slightest jolt.\n\nHours pass before you see the glimmer of lights along the tunnel. The carriage glides to a halt and the doors open with a whir. You emerge and set off along a tunnel very different form the one in Marsay. This subway station has not been maintained by careteks, and dust and rubble cover the tiled walkways.\n\nClimbing a staircase, you find the entrance blocked by fallen masonry and you have to labour for over an hour before there is a gap large enough to squeeze through. You find yourself on an ice-hard plain under the descending chill of dusk. A sprinkling of snow drifts down out of the sky. A few kilometres away, Kahira straddles the River Isis on its thick buttresses, a grey concrete lobster winking with a thousand eyes of light through the haze of mist. You set out at a brisk pace, anxious to reach the gates before curfew.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 229; }
};

class Story051 : public Story::Base
{
public:
    Story051()
    {
        ID = 51;

        Text = "The elevator carries almost to the top floor of the building, where a well-equipped gym overlooks the city. You spend half an hour on the treadmill, watching your own reflection in the glass window superimposed against the mist-shrouded towers and twinkling lights of Kahira. Another half hour on various weights machines leaves you feeling firmed up and fit. You finish with five minutes on a massage bed followed by a relaxing spa.\n\nAs you are leaving the changing room, you almost collied with a huge Fijian in a trim black suit and mirror glasses. He grunts an absent-minded apology and hurries past, staring urgently around the room. He is the only other person you have seen in the building who doesn't seem to be an employee here. You are about to head off towards the elevator when he calls after you, \"Hey! Who are you?\"";

        Choices.clear();
        Choices.push_back(Choice::Base("Use a BARYSAL GUN", 227, {Item::BARYSAL_GUN}));
        Choices.push_back(Choice::Base("[CLOSE COMBAT]", 248, Skill::Type::CLOSE_COMBAT));
        Choices.push_back(Choice::Base("[CUNNING]", 269, Skill::Type::CUNNING));
        Choices.push_back(Choice::Base("You had better run for it", 290));

        Controls = Story::Controls::STANDARD;
    }
};

class Story052 : public Story::Base
{
public:
    Story052()
    {
        ID = 52;

        Text = "The elevator reaches the ground floor and the doors slide open. You step out, only to be confronted by a group of uniformed security men with rifles. \"You're in illegal possession of a Compass Society ID,\" growls the security chief with a wolfish grin.\n\n\"Let me explain.\"\n\nHe shakes his head. \"Tell it to the marines. Better yet, tell it to the angels.\"\n\nWith a click of his fingers, he signals to his men and you are blasted apart in a juddering hail of gunfire.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;

        Type = Story::Type::DOOM;
    }
};

class Story053 : public Story::Base
{
public:
    Story053()
    {
        ID = 53;

        Text = "A rustling in the leaf canopy directly overhead warns you of danger. You open your eyes in time to see a narrow wedge-shaped head shaking down from the branches, its wide pink mouth lined with teeth like needles.\n\nYou react instantly, flipping backwards over the log an instant before the jaws strike. The creature roars back, spitting out soil and twigs, head bobbing on a long grey cable of neck, and lunges again. You slip aside, snatch up your belonging, and race off through the trees.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 228; }
};

class Story054 : public Story::Base
{
public:
    Story054()
    {
        ID = 54;

        Text = "You take yourself to a table and sit down. The twins watch you for a moment longer, then return to their drinking.\n\nYou glance around the inn. No one else dares stand up to the two Amazons. What about you?";

        Choices.clear();
        Choices.push_back(Choice::Base("Eavesdrop", 208));
        Choices.push_back(Choice::Base("[ESP] Read their minds", 230, Skill::Type::ESP));
        Choices.push_back(Choice::Base("[CUNNING] Try to outwit them", 120, Skill::Type::CUNNING));
        Choices.push_back(Choice::Base("Draw a BARYSAL GUN", 98, {Item::BARYSAL_GUN}));
        Choices.push_back(Choice::Base("You had better mind your own business", 252));

        Controls = Story::Controls::STANDARD;
    }
};

class Story055 : public Story::Base
{
public:
    Story055()
    {
        ID = 55;

        Text = "The computer locks you out. You try rebooting, to no avail. With a shrug, you turn to exploring the rest of the pyramid idly wondering what Gaia was up to when she entered the stream of inexplicable commands into the computer system here.\n\nThe answer comes so suddenly that you never know it. Gaia was in the throes of her periodic madness when you contacted her this time. She located the nuclear reactor supplying power to the pyramid and ordered it to go critical. Without warning a blossom of plasma erupts from the earth. For a brief incandescent second it is as though time has turned back to before the Ice Age, and the Pyramid of Cheops once more sits on hot sands in blazing light. Then the blast spreads -- sweeping away the pyramids that have stood here for fifty centuries, vaporizing the snow and ice covering the desert, turning the rock to lava and the river to steam, and making of Kahira a cinderous ruin. In the midst of such a holocaust, your own death goes unnoticed.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;

        Type = Story::Type::DOOM;
    }
};

class Story056 : public Story::Base
{
public:
    std::string PreText = "";

    Story056()
    {
        ID = 56;

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        PreText = "You curse the recklessness that brought you into the Ice Wastes before you were adequately prepared. Your lack of food intensifies the cold, which seems to drill into your bones. Each dawn you arise lethargic and listless, like on who has been visited by a vampire in the night. Each step you take costs a greater effort. You feel torpid with fatigue and hunger.\n\n";

        auto DAMAGE = -3;

        if (Character::VERIFY_SKILL(player, Skill::Type::SURVIVAL))
        {
            PreText += "[SURVIVAL] ";

            DAMAGE = -2;
        }

        if (Character::CHECK_VEHICLE(player, Vehicle::Type::BURREK))
        {
            PreText += "[Vehicle: BURREK] ";

            DAMAGE++;
        }

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Point(s).";

        Character::GAIN_LIFE(player, DAMAGE);

        if (player.Life > 0)
        {
            if (Character::VERIFY_SKILL(player, Skill::Type::SURVIVAL))
            {
                PreText += "\n\n[SURVIVAL] You manage to trap a bird which alights on one of the tors of glacial ice for food.";
            }

            if (Character::CHECK_VEHICLE(player, Vehicle::Type::BURREK))
            {
                PreText += "\n\n[Vehicle: BURREK] You tap some of the BURREK's oily blood for sustenance.";
            };
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 100; }
};

class Story057 : public Story::Base
{
public:
    Story057()
    {
        ID = 57;

        Text = "You touch the red button. The box produces a prolonged hum which rises in tone and then ends with a bleep. \"Ready,\" says a synthetic feminine voice from the speaker.\n\nTaken aback for a moment, you lean closer and gingerly speak to the box. \"Er... ready for what?\"\n\n\"Explain,\" says the box crisply. \"Your query was unspecific.\"\n\n\"Is this a radio? Who am I talking to?\"\n\nLITTLE GAIA: \"I am a miniature facsimile of the Global Artificial Intelligence Array,\" replies the box.\n\n\"Gaia? But Gaia is crazy.\"\n\nLITTLE GAIA: \"I was loaded with Gaia's program prior to virus infection. I am able to model the thinking of the Gaia system at a reduced rate owing to my limited memory capacity, which now stands at 512 gigabytes.\"\n\n\"What can you tell me about the Heart of Volent?\"\n\nLITTLE GAIA: \"Nothing. No such information has been loaded into my memory.\"\n\nThe device has an annoyingly smug little voice. All the same, if it has even a fraction of Gaia's intelligence then it may be useful.";

        Choices.clear();
        Choices.push_back(Choice::Base("Search the lab for other equipment", 80));
        Choices.push_back(Choice::Base("Descend to the bottom level", 255));
        Choices.push_back(Choice::Base("Leave", 361));

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::GET_ITEMS(player, {Item::LITTLE_GAIA});
    }
};

class Story058 : public Story::Base
{
public:
    Story058()
    {
        ID = 58;

        Text = "The volcanic gases here make the air poisonous, stunting the trees around the oasis and causing the slow death of animal life. No doubt those insects swarming in the sudorific updraughts have had to mutate in order to tolerate the conditions here. You know that the prolonged exposure to the tainted air might eventually cause cancer unless you have some protection. Otherwise it is only worth staying if you are on the brink of death and are so desperate for recuperation now that you are willing to take a gamble with your life.";

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Choices.clear();

        if (!Character::VERIFY_CODEWORD(player, Codeword::Type::TALOS) && !Character::VERIFY_ITEMS(player, {Item::Type::GAS_MASK}))
        {
            Choices.push_back(Choice::Base("Stay for a day or so", 103));
            Choices.push_back(Choice::Base("Continue onwards at once", 426));
        }
    }

    int Continue(Character::Base &player) { return 169; }
};

class Story059 : public Story::Base
{
public:
    Story059()
    {
        ID = 59;

        Text = "He holds up his finger. \"Ah, it is a very ancient place, not very far to the west. There, in ancient times, were buried the royalty of Egypt. Later, men came from the distant corners of the globe with a great warrior they called Gilgamesh, who had skin of iron and eyes of fire. They told him to watch across the snows for stirrings of life in the ruins of Du-En and, if any threat arose from there, he was to take up his sword and venture forth.\" Bador sees the look on your face. \"It is true, all true!\"";

        Choices.clear();
        Choices.push_back(Choice::Base("Ask for advice about the Sahara", 77));
        Choices.push_back(Choice::Base("About Kahira itself", 143));
        Choices.push_back(Choice::Base("Where to stay in the city", 99));
        Choices.push_back(Choice::Base("Dismiss him", 95));

        Controls = Story::Controls::STANDARD;
    }
};

class Story060 : public Story::Base
{
public:
    Story060()
    {
        ID = 60;

        Text = "If Gaia is to be believed, to unleash the power of the Heart would spell death for all of creation. But is she to be believed? Or trusted, for that matter? Gaia is not even a 'she', but an 'it' -- just an artificial intelligence resident in a network of computers. And schizophrenic into the bargain, thanks to the computer viruses entrenched in her software. You must make up your own mind. If you think the Heart should be destroyed, you will need to find a friend who is willing to help you do it. You look dubiously around the faces limned in the campfires. By their very nature, these are the most ruthless and determined adventurers of the age. Can you make them trust you? Or should you forget Gaia's dire warning, and just try to get the Heart for yourself?";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_SKILL(player, Skill::Type::ESP))
        {
            return 152;
        }
        else
        {
            return 82;
        }
    }
};

class Story061 : public Story::Base
{
public:
    Story061()
    {
        ID = 61;

        Text = "You decide your next course of action.";

        Choices.clear();
        Choices.push_back(Choice::Base("Use [ESP]", 344, Skill::Type::ESP));
        Choices.push_back(Choice::Base("Try [ROGUERY]", 365, Skill::Type::ROGUERY));
        ;
        Choices.push_back(Choice::Base("[LORE]", 387, Skill::Type::LORE));
        Choices.push_back(Choice::Base("[CYBERNETICS] and LITTLE GAIA", 408, Skill::Type::CYBERNETICS, {Item::LITTLE_GAIA}));
        Choices.push_back(Choice::Base("Otherwise", 429));

        Controls = Story::Controls::STANDARD;
    }
};

class Story062 : public Story::Base
{
public:
    Story062()
    {
        ID = 62;

        Text = "You are in no doubt that the stunted little form inside the central globe is the thing's guiding intellect. Racing forward, you avoid a sweep of the powerful metal legs and throw yourself into a forward roll that carries you onto the top of the glass globe. The half-formed face of the embryo is floating just inches from your own. Its muddy eyes give no sign of surprise or understanding -- it has the drooling vacant face of an imbecile -- but suddenly the metal legs buck and rear in a frantic effort to throw you off. You cling on for several seconds until you feel it give an upward thrust, trying to crush you against the ceiling. You leap clear at the crash. As the blue fluid drains away, the thing gives a dying spasm and then falls still.\n\nBoche comes over and helps you to your feet. \"I wonder what that was,\" he says with emphatic distaste.\n\n\"I don't care as long as there aren't any more of them,\" you reply. Together you head on to the end of the passage.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 281; }
};

class Story063 : public Story::Base
{
public:
    Story063()
    {
        ID = 63;

        Text = "The beam crackles through the air, only to splatter off an invisible shield of psychic force.";

        Choices.clear();
        Choices.push_back(Choice::Base("[PARADOXING] (BARYSAL GUN) Try to strip away the defensive shield and fire again", 282, Skill::Type::PARADOXING, {Item::BARYSAL_GUN}));
        Choices.push_back(Choice::Base("Back out of the hall the way you came", 107));
        Choices.push_back(Choice::Base("Press on deeper into the catacombs", 129));

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::FIRE_BARYSAL(player, 1);
    }
};

class Story064 : public Story::Base
{
public:
    Story064()
    {
        ID = 64;

        Choices.clear();

        Controls = Story::Controls::NONE;
    }

    int Background(Character::Base &player)
    {
        if (Character::VERIFY_SKILL(player, Skill::Type::ESP))
        {
            return 130;
        }
        else
        {
            return 240;
        }
    }
};

class Story065 : public Story::Base
{
public:
    std::string PreText = "";

    Story065()
    {
        ID = 65;

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        PreText = "The shot was a decoy. Sensing Golgoth's thoughts, you whirl to see him running silently through the smoke towards you. He has a knife in his hand, and the look in his eyes is as cold as death. You block his first thrust but take a gash on your forearm, countering with an elbow-strike which leaves him dazed.\n\nThe fight is short and brutal. Golgoth is a master of lethal killing techniques.\n\n";

        auto DAMAGE = -5;

        if (Character::VERIFY_SKILL(player, Skill::Type::CLOSE_COMBAT))
        {
            PreText += "[CLOSE COMBAT] ";

            DAMAGE = -3;
        }

        DAMAGE = Character::COMBAT_DAMAGE(player, DAMAGE);

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Points.";

        if (player.Life > 0)
        {
            PreText += "\n\nYou finally manage to twist the knife around and impale him.";
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 72; }
};

class Story066 : public Story::Base
{
public:
    std::string PreText = "";

    Story066()
    {
        ID = 66;

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        PreText = "The shot was a decoy. Sensing Golgoth's thoughts, you whirl to see him running silently through the smoke towards you. He has a knife in his hand, and the look in his eyes is as cold as death. You block his first thrust but take a gash on your forearm, countering with an elbow-strike which leaves him dazed.\n\nThe fight is short and brutal. Golgoth is a master of lethal killing techniques.\n\n";

        auto DAMAGE = -2;

        if (Character::VERIFY_SKILL(player, Skill::Type::CLOSE_COMBAT))
        {
            PreText += "[CLOSE COMBAT] ";

            DAMAGE = -1;
        }

        DAMAGE = Character::COMBAT_DAMAGE(player, DAMAGE);

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Point(s).";

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 110; }
};

class Story067 : public Story::Base
{
public:
    Story067()
    {
        ID = 67;

        Text = "Taking up your pack, you trudge out into the snow. Moments later you hear the crunching of rapid footsteps and Boche catches up with you. His breath curls into the diamond-clear morning air. \"We may as well travel together for mutual convenience, at least for a while,\" he says chirpily.";

        Choices.clear();
        Choices.push_back(Choice::Base("Agree to this", 133));
        Choices.push_back(Choice::Base("Refuse point-blank", 155));

        Controls = Story::Controls::STANDARD;
    }
};

class Story068 : public Story::Base
{
public:
    Story068()
    {
        ID = 68;

        Text = "There is the familiar pulse of blistering grey-white light, which instantaneously illuminates the overhanging crags like daylight. The sharp crack of vaporized snow and superheated air reverberates off the rocks. You are gratified to see the figure throw up its arms and fall to the ground.\n\n\"That was rash,\" snaps Boche. \"You might have just killed a potential ally.\"\n\nYou lower yourself from the ledge and start off towards the prone figure. \"Let's find out,\" you call back over your shoulder. The figure looks dead, but you are careful to keep your gun trained on it.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::FIRE_BARYSAL(player, 1);
    }

    int Continue(Character::Base &player) { return 90; }
};

class Story069 : public Story::Base
{
public:
    Story069()
    {
        ID = 69;

        Text = "The drawback to using the card is the holographic picture on the front, which looks nothing like you.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Choices.clear();

        if (!Character::VERIFY_SKILL(player, Skill::Type::ROGUERY))
        {
            if (!Character::VERIFY_SKILL(player, Skill::Type::STREETWISE) && !Character::VERIFY_ITEMS(player, {Item::Type::VADE_MECUM}))
            {
                Choices.push_back(Choice::Base("Get the ID card altered", 135));
            }
            else
            {
                Choices.push_back(Choice::Base("Get the ID card altered", 113));
            }

            Choices.push_back(Choice::Base("Forget about the card and investigate your other options instead", 414));
        }
    }

    int Continue(Character::Base &player) { return 452; }
};

class Story070 : public Story::Base
{
public:
    std::string PreText = "";

    Story070()
    {
        ID = 70;

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Choices.clear();

        PreText = "One of the men sticks out a foot to trip you, while the other chops at your neck with his knife.";

        if (!Character::VERIFY_SKILL(player, Skill::Type::CLOSE_COMBAT) && !Character::VERIFY_SKILL(player, Skill::Type::AGILITY))
        {
            auto DAMAGE = Character::COMBAT_DAMAGE(player, -3);

            PreText += "\n\nYou LOSE " + std::to_string(-DAMAGE) + " Life Points.";
        }

        if (player.Life > 0)
        {
            PreText += "\n\n";

            if (!Character::VERIFY_SKILL(player, Skill::Type::CLOSE_COMBAT) && !Character::VERIFY_SKILL(player, Skill::Type::AGILITY))
            {
                PreText += "The blade gashes you across the breast-bone -- a painful but not lethal injury.";

                Choices.push_back(Choice::Base("Retreat into the steam room", 4));
                Choices.push_back(Choice::Base("Stand your ground and fight", 26));
            }
            else
            {
                if (Character::VERIFY_SKILL(player, Skill::Type::CLOSE_COMBAT))
                {
                    PreText += "[CLOSE COMBAT] ";
                }

                if (Character::VERIFY_SKILL(player, Skill::Type::AGILITY))
                {
                    PreText += "[AGILITY] ";
                }

                PreText += "You are able to fend off the blow and escape past them, grabbing up your clothes as you run.";
            }
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 92; }
};

class Story071 : public Story::Base
{
public:
    Story071()
    {
        ID = 71;

        Choices.clear();

        Controls = Story::Controls::NONE;
    }

    int Background(Character::Base &player)
    {
        if (Character::VERIFY_SKILL(player, Skill::Type::PILOTING))
        {
            return 115;
        }
        else
        {
            return 137;
        }
    }
};

class Story072 : public Story::Base
{
public:
    Story072()
    {
        ID = 72;

        Text = "\"We've done it!\" you cry. \"Now to destroy the Heart.\"\n\nBut Singh shakes his head. \"Nonsense. I have honoured our agreement thus far, but only to ensure success. Now we must decide which of us survives to claim the power.\"\n\n\"Hardly an even battle.\" You nod at the powerful MANTRAMUKTA CANNON in his hands.\n\nHe tosses the cannon aside. In his belt is tucked a nozzle that tells you it is in fact a modified laser.";

        Choices.clear();
        Choices.push_back(Choice::Base("Use a BARYSAL GUN", 283, {Item::BARYSAL_GUN}));
        Choices.push_back(Choice::Base("Otherwise", 305));

        Controls = Story::Controls::STANDARD;
    }
};

class Story073 : public Story::Base
{
public:
    Story073()
    {
        ID = 73;

        Text = "Where do you want to go next?";

        Choices.clear();
        Choices.push_back(Choice::Base("Take the elevator to the library", 6));
        Choices.push_back(Choice::Base("Go to the medical lounge", 204));
        Choices.push_back(Choice::Base("Go to the gymnasium", 51));
        Choices.push_back(Choice::Base("Go to the armoury", 447));
        Choices.push_back(Choice::Base("Go to the canteen", 94));
        Choices.push_back(Choice::Base("You have pushed your luck far enough and wish to leave before your deception is uncovered", 311));

        Controls = Story::Controls::STANDARD;
    }
};

class Story074 : public Story::Base
{
public:
    Story074()
    {
        ID = 74;

        Text = "The carriage gathers speed. You sit at the front and stare through the window at the darkness rushing by. Hours pass before you see the glimmer of lights along the tunnel. The carriage glides to a halt at a station lit by dull red lights. You stand up, but the doors do not open. Instead the motilator gives a placid announcement: \"We have arrived at a restricted area. We will now proceed to Maka, where you will be able to transfer to another vehicle for your onward journey. We apologize for any inconvenience.\"\n\nAgain the carriage picks up speed, this time for a journey of less than two hours. Arriving at another terminal, you wait to see what other destinations the motilator will offer. On the map, only Tarabul continues to flash.";

        Choices.clear();
        Choices.push_back(Choice::Base("Take the subway there", 31));
        Choices.push_back(Choice::Base("Disembark and see where you are", 375));

        Controls = Story::Controls::STANDARD;
    }
};

class Story075 : public Story::Base
{
public:
    std::string PreText = "";

    Story075()
    {
        ID = 75;

        Image = "images/jungle.png";

        Choices.clear();
        Choices.push_back(Choice::Base("[SHOOTING]", 96, Skill::Type::SHOOTING));
        Choices.push_back(Choice::Base("Otherwise", 118));

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        PreText = "\"What's that, what's that?\"\n\nYou silt bolt upright and stare around. The voice you heard was jarringly strange: humanlike, but not quite human. Nerves tingling, you scan the undergrowth.\n\nTeeth snare your shoulder and you cry out in pain and surprise.\n\nYou LOSE 1 Life Point.";

        Character::GAIN_LIFE(player, -1);

        if (player.Life > 0)
        {
            PreText += "\n\nYou look up to see an uncanny beast peering down at you. Stretched between the branches, its body is a tent of leathery wing-flaps. Its head is a narrow snout filled with teeth, swaying on a long flexible neck. It watches you with small glittering eyes and croaks, \"What's that?\"\n\nNothing nice, that's for sure.";
        }

        Text = PreText.c_str();
    }
};

class Story076 : public Story::Base
{
public:
    std::string PreText = "";

    Story076()
    {
        ID = 76;

        Choices.clear();
        Choices.push_back(Choice::Base("Otherwise", 164));
        Choices.push_back(Choice::Base("Insist on fighting on", 186));

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        PreText = "\"I have been travelling for weeks,\" you reply curtly, \"and I will not be deterred from taking a good hot meal just because two slab-shouldered termagants want to drink themselves into a stupor.\"\n\nA mutter of guarded approval goes around the other customers when they hear your tone of defiance. It is not exactly a cheer. Obviously the twins have kept everyone here tyrannized for hours.\n\nYou walk forward. One of the twins plants a hand in the middle of your chest. You seize it, apply a lock, and twist. She rolls out of the lock, bracing her arm against the bar and kicking up with strong yet fluid grace. You weave aside, block a punch from the other twin, and counter with a stiff-fingered strike to the solar plexus. She braces against the blow, taking it on muscles like steel cables. Her sister, springing upright, launches a kick at your kidneys which you barely avoid, the attack hitting you on the hip with bruising force.\n\nThe Jib-and-Halter Inn has never witnessed such a rough-house. Punches, kicks and brutal gouges lash back and forth while the other customers look on aghast.";

        auto DAMAGE = Character::COMBAT_DAMAGE(player, -3);

        PreText += "\n\nYou LOSE " + std::to_string(-DAMAGE) + " Life Points.";

        if (player.Life > 0)
        {
            PreText += "\n\nThe twins finally step back and signal that they are prepared to end the fight.";
        }

        Text = PreText.c_str();
    }
};

class Story077 : public Story::Base
{
public:
    Story077()
    {
        ID = 77;

        Text = "Bador expresses dismay when you tell him you intend to cross the Ice Wastes. \"By your father's beard! Do you wish to become a corpse with hoarfrost in your veins? Put aside all thought of such a scheme, I pray you!\" You cannot help smiling. \"What?\" says Bador, starting to weep. \"Do you mock my concern?\"\n\nYou place a hand on his sleeve. \"Calm yourself. You and I are strangers, and you already have your fee. Do not allow thought of my death to upset you, but give me advice on how to avoid such a fate.\"\n\n\"Only the barbarian Ebor venture into the Sahara, and even they go no further that its fringes. It is a place of ghosts and devils, and the wind is like flint.\"\n\n\"The Ebor? A nomad tribe? How do they survive?\"\n\n\"They have BURREKs, shaggy thick-necked beasts that grow folds of fat. When the blizzard comes, the Ebor rider shelters by his BURREK and bleeds the animal, frying up a blood pudding to sustain him.\" Bador grimaces to show what he thinks of such a custom.";

        Choices.clear();
        Choices.push_back(Choice::Base("Ask what he knows about the city", 143));
        Choices.push_back(Choice::Base("Ask about Giza", 59));
        Choices.push_back(Choice::Base("Ask the best place to spend the night", 99));
        Choices.push_back(Choice::Base("You have learned all you need and want to send him away", 95));

        Controls = Story::Controls::STANDARD;
    }
};

class Story078 : public Story::Base
{
public:
    Story078()
    {
        ID = 78;

        Text = "The flyer slowly drifts into the air on streamers of lambent gas. As the main thrusters engage, it picks up speed and goes roaring up into the heavens. A flash of sunlight glances off the hull as it veers towards the east. You watch until it is lost in the soft blue haze.\n\nYou set out on foot until you come to a shore of white sand, which you follow north to a ferry building. A group of men emerge and appraise you with suspicious glances before showing you to the ferry boat, a single-masted schooner that has seen better days. Two or three other passengers are already aboard, and on seeing you one of them says, \"Good! Now the ferry is full, we can set sail for Port Sudan.\"\n\nYou are CHARGED 1 scad.\n\nMoonlight is making such a creamy track in the water by the time you reach Sudan, a village of wooden huts huddled within the vast shell of an abandoned city wall. The boat sweeps in across the harbour, guided by a flaring beacon, and moors at a jetty reeking of fish. The streets are empty , and it is obviously too late to find a hostelry for the night, so you decide to sleep on the boat. The ferrymen are averse to this, insisting they should be paid more for providing you with accommodation as well as transport, but the other passengers are encouraged by your lead. \"You were happy enough to keep us waiting two days until you had enough passengers for the journey,\" snaps one.\n\nAt last the ferrymen sullenly agree. You sleep until dawn tinges the sky with the colour of a candle flame.";

        Choices.clear();
        Choices.push_back(Choice::Base("Do some shopping in Sudan", 101));
        Choices.push_back(Choice::Base("Set out for Du-En", 234));

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::GAIN_MONEY(player, -1);
    }
};

class Story079 : public Story::Base
{
public:
    Story079()
    {
        ID = 79;

        Text = "A thin searing beam spits through the air, burning a precise hole through the giant bometh's head. It utters a deep growl, takes two stumbling steps through the snow, and then falls. You hurry over to make sure of the kill. You would not want a wounded bometh stalking you through the night.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::FIRE_BARYSAL(player, 1);
    }

    int Continue(Character::Base &player) { return 341; }
};

class Story080 : public Story::Base
{
public:
    Story080()
    {
        ID = 80;

        Text = "You find several items scattered across a bench at the back of the laboratory. These include a FLASHLIGHT, a pair of BINOCULARS, a set of POLARIZED GOGGLES, and a BARYSAL GUN. The gun has been opened for inspection, but it is a simple matter to secure the but and replace the screws. You check the power unit, finding two charges remaining.";

        Choices.clear();
        Choices.push_back(Choice::Base("Descend the shaft to the bottom level", 255));
        Choices.push_back(Choice::Base("Ascend and leave the pyramid", 361));

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Take = {Item::FLASHLIGHT, Item::BINOCULARS, Item::POLARIZED_GOGGLES, Item::MAKE_BARYSAL_GUN(2)};

        Limit = 4;
    }
};

class Story081 : public Story::Base
{
public:
    Story081()
    {
        ID = 81;

        Text = "Gilgamesh stands rock-still beside you. You hear the soft whirr of a fan sucking air into his chest-plate. Then he announces that the air here is toxic.\n\n\"How toxic? The trees seem to survive well enough.\" You brush midges away from your face. \"And insects.\"\n\n\"They are adapted to the conditions here,\" replies Gilgamesh in his abrupt mechanical voice. \"Trace elements may cause cancer in higher life forms.\"\n\n\"After how long?\"\n\n\"Unknown. Even one day's exposure is potentially hazardous. Recommend you take precautions to filter your air supply or leave the vicinity.\"";

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Choices.clear();

        if (!Character::VERIFY_ITEMS(player, {Item::Type::GAS_MASK}))
        {
            Choices.push_back(Choice::Base("Stay and rest for a few days", 103));
            Choices.push_back(Choice::Base("Depart as Gilgamesh suggests", 426));
        }
    }

    int Continue(Character::Base &player) { return 169; }
};

class Story082 : public Story::Base
{
public:
    Story082()
    {
        ID = 82;

        Text = "The moon rises, wreathed in a haze of frost. In the crisp cold light, the ancient halls and towers look more than half unreal. You watch the others huddled by their campfires. No one else has much to say. Each of them is absorbed in private hopes, dreams and fears of what tomorrow will bring. When the Heart is found it will be every man for himself. Who would expect anything else, when the prize at stake is nothing less than the power of a god?\n\nVajra Singh and Thadra Bey have retreated to their respective tents and scarcely seem to invite conversation.";

        Choices.clear();
        Choices.push_back(Choice::Base("Talk to Chaim Golgoth", 126));
        Choices.push_back(Choice::Base("Talk to Kyle Boche", 104));
        Choices.push_back(Choice::Base("Talk to Janus Gaunt", 148));
        Choices.push_back(Choice::Base("Talk to Baron Siriasis", 170));
        Choices.push_back(Choice::Base("Turn in for the night", 192));

        Controls = Story::Controls::STANDARD;
    }
};

class Story083 : public Story::Base
{
public:
    Story083()
    {
        ID = 83;

        Text = "The sky is clear and blue, with high wisps of grey cloud. The snow crunches underfoot as you walk across the square to join Kyle Boche. Floating out of his tent comes the legless Baron Siriasis. You look around but the square is otherwise deserted apart from clumps of servants waiting beside the tents.\n\n\"The others have already descended,\" Boche tells you and the baron. \"Vajra Singh went with Golgoth, down through the main temple complex. Thadra Bey took herself alone into the adjoining subway tunnels. Janus Gaunt was gone before I woke.\"\n\n\"I suggest we three team up, then,\" says the baron briskly.\n\nBoche nods. \"Agreed. If we find the Heart, our alliance holds until the other teams are dealt with. Where shall we descend?\"\n\nLast night you noticed an icy crevice beside the building that Singh levelled with his MANTRAMUKTA CANNON. You point it out to the others. \"It looks to give onto the cellars, and there may be a way through to the temple catacombs below.\"\n\nThe baron gazes down into the crevice, then gives a curt nod. \"I sense it is a favourable route. It will lead us to the Heart.\"";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player)
    {
        if (Character::CHECK_VEHICLE(player, Vehicle::Type::MANTA_SKY_CAR))
        {
            return 17;
        }
        else
        {
            return 39;
        }
    }
};

class Story084 : public Story::Base
{
public:
    Story084()
    {
        ID = 84;

        Text = "You see the air twist inside out as the baron projects a bolt of psychic force against the oncoming creature. Like Boche's gun blast, the bolt is deflected by its shield of metal legs. \"It is a robot, immune to paradoxing,\" shouts the baron. \"We must retreat!\"\n\nYou have abetter idea. You may not be as powerful a psionic as Baron Siriasis, but that only means you've learned to be smarter. Instead of channelling your psi-force as a direct bolt, you use it to transmute the blue fluid that fills the glass bubble. Within moments the gnarled little homunculous inside is floating in acid. The thing rears up on its long legs like a dying spider, then topples to the floor. By the time you go over to look, the body inside has entirely dissolved away.\n\n\"Not a robot,\" you say to the startled baron, \"but a cyborg. You should have attacked the organic part.\"\n\nHe glares at you, then gives a curt nod of respect. \"It seems I can still learn new tricks of my craft, even from a youngster like you.\"\n\nTogether you head on to the end of the passage.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 281; }
};

class Story085 : public Story::Base
{
public:
    std::string PreText = "";

    Story085()
    {
        ID = 85;

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Type = Story::Type::NORMAL;

        PreText = "The brain reaches you. Its single remaining eye glares into yours, seeming to burn your soul like a lens focusing the rays of the sun. Your veins and arteries feel as though they are filled with ice water. Thoughts which are not your own intrude upon your mind. You are locked in a psychic contest for possession of your living body!";

        if (!(Character::VERIFY_SKILL(player, Skill::Type::ESP) && Character::VERIFY_SKILL(player, Skill::Type::PARADOXING)))
        {
            PreText += "\n\nYou can do nothing to save yourself, and darkness closes over your soul as Baron Siriasis claims your body as his own.";

            Type = Story::Type::DOOM;
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 151; }
};

class Story086 : public Story::Base
{
public:
    Story086()
    {
        ID = 86;

        Choices.clear();

        Controls = Story::Controls::NONE;
    }

    int Background(Character::Base &player)
    {
        if (Character::VERIFY_SKILL(player, Skill::Type::ESP))
        {
            return 65;
        }
        else
        {
            return 240;
        }
    }
};

class Story087 : public Story::Base
{
public:
    Story087()
    {
        ID = 87;

        Text = "Boche falls to your first shot. His death acts as a signal for the start of hostilities. Golgoth and Singh, in no doubt that each is the most dangerous foe, whirl to face each other. Golgoth's first shot hisses into sparks on Singh's armour as the Sikh warlord raises his mighty MANTRAMUKTA CANNON.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::FIRE_BARYSAL(player, 1);
    }

    int Continue(Character::Base &player) { return 410; }
};

class Story088 : public Story::Base
{
public:
    Story088()
    {
        ID = 88;

        Text = "Gargan XIV closes in on you. Gargan XIII draws a knife and looks down at Golgoth, in no hurry to finish him off. Suddenly he looks up with abroad smile. She was wrong in thinking him beaten. To the contrary, he has the look of a cat who has trapped two very large mice.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 154; }
};

class Story089 : public Story::Base
{
public:
    Story089()
    {
        ID = 89;

        Text = "There is no love lost between you and the Gargan twins. Seeing you square off warily against them, Janus Gaunt tries to defuse the situation, saying. \"Come now, we must set aside past differences. Until we can locate the Heart, a state of truce must apply.\"\n\n\"Truce? That word is a refuge for cowards,\" Gargan XIV hisses at him, while never letting her molten gaze stray from you.\n\n\"Du-En's getting overcrowded,\" says Gargan XIII as she takes a step towards you. \"Someone has to put the rubbish out.\"\n\nYou flick a glance at Gargan XIV. While her sister kept your attention, she has drawn a gun. You leap to one side just as a thin beam of energy spits past your shoulder and sears a gobbet of molten stone out of the wall behind. Gargan XIII drops into a crouch and comes stamping forward to grapple you, but you throw one of Gaunt's glassy-eyed xoms towards her and duck away behind the weathered stump of a pillar.\n\nIn the tense atmosphere of the camp, the squabble acts like a spark in a powder keg. Within seconds everyone is preparing for full-scale battle. Boche dives through an open doorway just in time to avoid Thadra Bey's throwing-dagger. An ectoplasmic aura crackles around Baron Siriasis as he marshals his psychic force. With a thoughtful frown, Chaim Golgoth draws his gun and steps back looking for a target. Janus Gaunt panics and shouts to his xom servants, \"Kill everyone! Protect me!\"\n\nThen a voice rips like thunder across the square: \"Stop this senseless fighting now!\" and turning, you have your first view of the mighty Vajra Singh.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 300; }
};

class Story090 : public Story::Base
{
public:
    Story090()
    {
        ID = 90;

        Text = "\"Is it dead?\" says Boche.\n\nYou snap an icicle from under the ledge and drive it deep into the grotesque pulpy head. The creature gives a single spasm and lies still. \"It is now.\"\n\nYou roll the body over to inspect it and are almost overcome by a wave of nausea. It is the most loathsome thing you have ever seen: a thin malformed body with a bloated mauve-pink head. The only facial feature is a long thick stalk ending in the cyclopean eye, now thankfully dimmed by the glaze of death. The scalp is covered with tiny orifices like gaping eyelids. Are they breathing holes? Sensory organs? There is no way to tell.\n\nBoche joins you beside the body. \"It's a mutant.\"\n\nYou nod. \"That's for sure, but a mutant what?\"";

        Choices.clear();
        Choices.push_back(Choice::Base("Follow the creature's tracks back to its lair before the snow covers them", 134));
        Choices.push_back(Choice::Base("Stay where you are and wait until dawn", 310));

        Controls = Story::Controls::STANDARD;
    }
};

class Story091 : public Story::Base
{
public:
    Story091()
    {
        ID = 91;

        Text = "You piece together a jigsaw of legend, superstition and historical fact. The Heart fell from the sky: an unearthly gemstone that became the focus of a crazed cult. The cult used the Heart's miraculous power to wage the Paradox war. Now it lies buried under the ruined city of Du-En, and the one who retrieves it will become mightier than any man has ever been.\n\n\"Why, then, did the civilization of Du-En fall?\" you ask a scholar at the library.\n\n\"Its rulers went mad. No one could wield such power and stay sane.\"\n\n\"Do you know it for a fact, or is it just your own theory?\"\n\nBut his only answer to that is a whimsical smile.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player)
    {
        if (!Character::VERIFY_SKILL(player, Skill::Type::STREETWISE) && !Character::VERIFY_ITEMS(player, {Item::Type::VADE_MECUM}))
        {
            return 414;
        }
        else
        {
            return 157;
        }
    }
};

class Story092 : public Story::Base
{
public:
    Story092()
    {
        ID = 92;

        Text = "By the front desk of the inn there is a notice-board where posters are pinned up for the perusal of bounty hunters. You scan the pictures of wanted criminals to see if any resemble the two men who attacked you, but without success. When you ask the innkeeper, he shrugs and says that people are constantly coming and going. \"I cannot keep track of all the riff-raff of Venis.\"\n\n\"But I might very easily have been murdered.\"\n\nHis only answer to this is to point to a sign on the wall which reads: 'The management is not responsible for the safety of customers or their belongings.' You give him a glowering look, then turn and stride out of the inn. A walk in the night air will help you to cool off.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 329; }
};

class Story093 : public Story::Base
{
public:
    Story093()
    {
        ID = 93;

        Text = "You lay out the contents of the storage compartment on the floor beside the vehicle. There are ten FOOD PACKs, a MEDICAL KIT, a FLASHLIGHT, a COLD-WEATHER SUIT and a length of nylon ROPE.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Take = {Item::FLASHLIGHT, Item::MEDICAL_KIT, Item::COLD_WEATHER_SUIT, Item::ROPE, Item::FOOD_PACK, Item::FOOD_PACK, Item::FOOD_PACK, Item::FOOD_PACK, Item::FOOD_PACK, Item::FOOD_PACK, Item::FOOD_PACK, Item::FOOD_PACK, Item::FOOD_PACK, Item::FOOD_PACK};

        Limit = 14;
    }

    int Continue(Character::Base &player) { return 395; }
};

class Story094 : public Story::Base
{
public:
    Story094()
    {
        ID = 94;

        Text = "The canteen is located at the top of the building, with wide windows giving a breathtaking view over the city. You stand and look out for a few minutes at the tall towers wreathed in swirling fog. Below, a dark patch of woodland studded with mistily sparkling lamps can only be the infamous Claustral Park.\n\nThe canteen has no human attendants, just a food dispenser which brings forth foil-wrapped packs (FOOD PACKS) at the touch of a button.\n\nAs you are leaving the canteen, you almost collied with a huge Fijian in a trim black suit and mirror glasses. He grunts an absent-minded apology and hurries past, staring urgently around the room. He is the only other person you have seen in the building who doesn't seem to be an employee here. You are about to head off towards the elevator when he calls after you, \"Hey, who are you?\"";

        Choices.clear();
        Choices.push_back(Choice::Base("Use a BARYSAL GUN", 227, {Item::BARYSAL_GUN}));
        Choices.push_back(Choice::Base("[CLOSE COMBAT]", 248, Skill::Type::CLOSE_COMBAT));
        Choices.push_back(Choice::Base("[CUNNING]", 269, Skill::Type::CUNNING));
        Choices.push_back(Choice::Base("Run for it", 290));

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Take = {Item::FOOD_PACK, Item::FOOD_PACK, Item::FOOD_PACK, Item::FOOD_PACK, Item::FOOD_PACK, Item::FOOD_PACK, Item::FOOD_PACK, Item::FOOD_PACK};

        Limit = 8;
    }
};

class Story095 : public Story::Base
{
public:
    Story095()
    {
        ID = 95;

        Text = "Decide what to do next.";

        Choices.clear();
        Choices.push_back(Choice::Base("Make use of the ID CARD", 353, {Item::ID_CARD}));
        Choices.push_back(Choice::Base("Try to find out about Baron Siriasis", 401));
        Choices.push_back(Choice::Base("... about Chaim Golgoth", 422));
        Choices.push_back(Choice::Base("... about Gilgamesh", 380));
        Choices.push_back(Choice::Base("... about the Sphinx", 11));
        Choices.push_back(Choice::Base("Get some rest", 311));

        Controls = Story::Controls::STANDARD;
    }
};

class Story096 : public Story::Base
{
public:
    Story096()
    {
        ID = 96;

        Text = "The blast hisses in the dank steamy air. Blue plasma-fire burns through the creature's neck. It utters a bleak wail of distress and falls crashing to the ground, where you finish off the twitching carcass with a heavy stone.\n\nThe wound in your shoulder is beginning to throb. You clean it with some leaves, then tear strips from the lining of your jacket to make a bandage. Lying back against the log, you feel slightly giddy, but this is no place to rest. There might be more of those creatures about. Hauling yourself to your feet, you lumber off in search of a safer place to hole up.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::FIRE_BARYSAL(player, 1);
    }

    int Continue(Character::Base &player) { return 228; }
};

class Story097 : public Story::Base
{
public:
    Story097()
    {
        ID = 97;

        Text = "The nearest man stabs his KNIFE at your heart. You deflect the blow with an open-handed block to his wrist, then sidestep in close to deliver two swift elbow strikes across his face. As he sags, you pluck the KNIFE out of his fingers. The angle is wrong to get the man with the gun, so you cast the KNIFE at each other. It catches him in the shoulder and he falls back with a grunt.\n\nThe man with the gun is about to fire. You throw yourself into a forward roll, hearing the blast crack overhead and explode against the wall. Scissoring your legs, you thrust him off-balance before he can take another shot. He topples into the fire, his frightened yelp cut brutally short as his head hits a rock.\n\nBefore you can get to your feet, the man with the KNIFE in his shoulder comes lumbering forward and tries to stomp you in the guts. You jerk aside, catch his ankle, and bring him down backwards across your hip, where a swift powerful twist ends the struggle.\n\nYou search the shelter. The BARYSAL GUN has one charge left. You also find two KNIVES, a set of POLARIZED GOGGLES, COLD-WEATHER clothes, BINOCULARS, and six FOOD PACKs.";

        Bye = "Then you wait for the blizzard to blow itself out before you emerge into the crisp snow outside";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Take = {Item::MAKE_BARYSAL_GUN(1), Item::KNIFE, Item::KNIFE, Item::POLARIZED_GOGGLES, Item::COLD_WEATHER_SUIT, Item::BINOCULARS, Item::FOOD_PACK, Item::FOOD_PACK, Item::FOOD_PACK, Item::FOOD_PACK, Item::FOOD_PACK, Item::FOOD_PACK};

        Limit = 12;
    }

    int Continue(Character::Base &player) { return 393; }
};

class Story098 : public Story::Base
{
public:
    Story098()
    {
        ID = 98;

        Text = "Your gun is barely out of its holster before one of the twins flicks her wrist and sends a splash of fiery vodka into your eyes. The gun discharges with a sizzling crack. You stumble back, wiping your face. A kick lashes out, striking the gun from your hands. Strong fingers seize your head. There is no time to act before your legs are swept out form under you. You topple, and a mighty twist from your assailant breaks your neck. Your die in an unseemly tavern brawl.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;

        Type = Story::Type::DOOM;
    }
};

class Story099 : public Story::Base
{
public:
    Story099()
    {
        ID = 99;

        Text = "He is emphatic that you should on no account sleep in Claustral Park. \"It is unsafe after nightfall,\" he says, wagging his finger. \"The claustrals are barely deterred from entering the streets as it is.\"\n\n\"What are claustrals?\"\n\nHe jerks back in exaggerated surprise. \"Do you truly not know? They are rank fiends -- creatures that are the reverse of men. They flourish in the darkness, cold and filth; they abjure sunlight and goodness. Their food is the decaying remains of the dead.\" His fat jowls shudder with fright.\n\n\"Decaying flesh? So why would they hunt a living person? Possibly the claustrals are simply figments of a fairy tale.\"\n\nHe looks at you sadly, as though at a person who had lost their wits. \"Do not allow your cynicism to tempt you into the park,\" he maintains.\n\n\"So where should I stay?\"\n\n\"The Ossiman Hotel is best. If you cannot afford a hotel, avoid the backstreets, where muggers lurk. If you must, sleep by the gratings on Fishermonger Plaza. It is well lit, warm, and there are plenty of folk around all through the night.\"";

        Choices.clear();
        Choices.push_back(Choice::Base("Ask his advice about the Sahara", 77));
        Choices.push_back(Choice::Base("Ask him about Giza", 59));
        Choices.push_back(Choice::Base("Ask about Kahira itself", 143));
        Choices.push_back(Choice::Base("Dismiss him", 95));

        Controls = Story::Controls::STANDARD;
    }
};

class Story100 : public Story::Base
{
public:
    Story100()
    {
        ID = 100;

        Image = "images/bometh.png";

        Text = "You continue on, watching the sun slide dankly down into the west. A silvery afterglow rims the skyline. Pale humps of snow extend to the murky horizon, divided by hollows brimming with violet shadow. Catching a movement out of the corner of your eye you freeze, slowly turning to see a huge sabre-fanged bometh standing on a rise not fifty metres away. You slink back behind an ice boulder, not certain if the creature saw you.";

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Choices.clear();

        if (!Character::VERIFY_CODEWORD(player, Codeword::Type::ENKIDU) && !Character::VERIFY_SKILL(player, Skill::Type::SURVIVAL))
        {
            Choices.push_back(Choice::Base("[SHOOTING] Shoot at the bometh", 79, Skill::Type::SHOOTING));
            Choices.push_back(Choice::Base("Use a STUN GRENADE", 145, {Item::STUN_GRENADE}));
            Choices.push_back(Choice::Base("Close with it", 277));
            Choices.push_back(Choice::Base("Track it (BINOCULARS)", 319, {Item::BINOCULARS}));
            Choices.push_back(Choice::Base("Creep off before it spots you", 289));
        }
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_CODEWORD(player, Codeword::Type::ENKIDU))
        {
            return 123;
        }
        else
        {
            return 256;
        }
    }
};

class Story101 : public Story::Base
{
public:
    Story101()
    {
        ID = 101;

        Text = "You stroll around the market, but there is little on offer here. If you wish to buy a FUR COAT, it will cost 5 scads. You can buy FOOD PACKs for 4 scads each; these consist of fish, oil and grain dried into blocks, each giving rations for several days.\n\nA small girl follows you along the dusty street singing a ditty:\n\n\"Out across the Ice Wastes,\nYellow steam and snow,\nCough your gust and freeze to death,\nA silly way to go.\"\n\nNo doubt the same rhyme has been repeated by children here for many generations. For some reason you find it more discouraging than any amount of sage advice. Not for the first time, you find yourself wondering if you are mad to consider a journey across the daunting Saharan plains. Still, when life on Earth is guttering like a candle about to blow out, only a fool makes plans for the future. You square your shoulders and turn to the west.";

        Choices.clear();

        Controls = Story::Controls::SHOP;
    }

    void Event(Character::Base &player)
    {
        Shop = {{Item::FUR_COAT, 5}, {Item::FOOD_PACK, 4}};
    }

    int Continue(Character::Base &player) { return 234; }
};

class Story102 : public Story::Base
{
public:
    Story102()
    {
        ID = 102;

        Text = "You are halfway across the tarmac when you realize your mistake. The pilot of the flyer has already engaged the boosters. You see him at the cockpit window, his face contorting in surprise and shock at the sight of you racing towards the craft. Your last image is of him jabbing desperately at the controls, but he is too late to abort the booster ignition. An instant later, a blast of white-hot gas bursts from the landing jets and billows up to press a wave of solid heat into your face. Blinding light burns into your retinas, followed by darkness and oblivion.\n\nYou LOSE 5 Life Points.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::GAIN_LIFE(player, -5);
    }

    int Continue(Character::Base &player) { return 122; }
};

class Story103 : public Story::Base
{
public:
    Story103()
    {
        ID = 103;

        Text = "You build a lean-to beside the bubbling pool in the shelter of dwarf conifers. You soon discover that the water of the pool is tainted with volcanic gases, but when you need to drink it is easy enough to collect snow from beyond the edge of the oasis and bring it back to camp to melt.\n\nFood is more difficult to come by. The birds you saw when you first arrived prove to be very timid, and canny enough not to let you catch them. If someone had told you even two weeks ago that you would be eagerly chewing grubs and insects for sustenance, you would have laughed them to scorn. At least the hot gases rising from clefts in the rocks mean that you can bake the insects before eating them.\n\nYou RECOVER 2 Life Points.\n\nOn the morning of your second day at the oasis, you notice a slight feeling of nausea, and begin to wonder whether the sulphur-tinged air is affecting your health.";

        Choices.clear();
        Choices.push_back(Choice::Base("Leave the oasis and head on", 426, Choice::Type::GET_CODEWORD, Codeword::Type::HOURGLASS));
        Choices.push_back(Choice::Base("Stay for a few days more", 15));

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::GAIN_LIFE(player, 2);
    }
};

class Story104 : public Story::Base
{
public:
    Story104()
    {
        ID = 104;

        Text = "Boche struts around the fire under the colonnade while outlining his plans for how you will share the power of the Heart. Lost in your own deep reverie, you hardly hear his words. Finally you look up and ask him, \"Why do you want ultimate power, Boche?\"\n\nHe stops short and looks at you sharply. For a moment you think he is about to give you a straight answer, but no. \"What are you saying? Are you having doubts? Surely not, when we are on the verge of triumph. You must not be so timid!\"";

        Choices.clear();
        Choices.push_back(Choice::Base("(Nemesis) Suggest an alliance", 236, Codeword::Type::NEMESIS));
        Choices.push_back(Choice::Base("You would rather get some sleep", 192));

        Controls = Story::Controls::STANDARD;
    }
};

class Story105 : public Story::Base
{
public:
    Story105()
    {
        ID = 105;

        Text = "Even though the three of you are helpless, Gilgamesh can still act. Lunging forward through the swirling vapour, he seizes the gnarled phantom in his arms and carries it on stiff strides to the edge of the chasm. It shrieks and twists in his grasp, flowing and distorting like a painting in the rain. Sparks cascade from Gilgamesh's visor as it sinks its fingers under his armour. For a long instant they both stand there, wreathed in white mist, struggling for the upper hand. Then Gilgamesh steps forward over the edge and he and the creature drop out of sight. You hear its thin bleating cry echo up from the depths and then there is silence. When you run to the chasm and look down, you find no trace of either the phantom or your loyal automaton.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::REMOVE_CODEWORD(player, Codeword::Type::ENKIDU);
    }

    int Continue(Character::Base &player) { return 149; }
};

class Story106 : public Story::Base
{
public:
    Story106()
    {
        ID = 106;

        Text = "The thing comes stalking forward, feeling its way while keeping its front legs raised as a shield. You have already seen that gunfire cannot penetrate the tough metal alloy. The glass case in the centre is another matter, though. You roll your grenade along the floor of the passage and pull the others back to a safe distance. There is a loud bang. The grenade is designed only to stun a living target, but the explosion cracks the glass and the blue fluid gushes out. As it does, the thing rears up like a dying spider, falls with a twitching of its robotic legs, and lies still.\n\nBoche nervously goes over to look at it. The little embryo inside the glass case lies as inert as a lump of cold clay. \"It's dead,\" he says. In stunned silence, the three of you head on to the end of the passage.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::LOSE_ITEMS(player, {Item::Type::STUN_GRENADE});
    }

    int Continue(Character::Base &player) { return 281; }
};

class Story107 : public Story::Base
{
public:
    Story107()
    {
        ID = 107;

        Choices.clear();

        Controls = Story::Controls::NONE;
    }

    int Background(Character::Base &player)
    {
        if (Character::VERIFY_CODEWORD(player, Codeword::Type::MALLET))
        {
            Character::REMOVE_CODEWORD(player, Codeword::Type::MALLET);

            return 195;
        }
        else
        {
            return 217;
        }
    }
};

class Story108 : public Story::Base
{
public:
    Story108()
    {
        ID = 108;

        Text = "Golgoth had the same idea. You come face to face with him on the edge of the smoke cloud. Instead of a gun, he has a crossbow in his hands. He shoots, but you are already dodging and the bolt only opens a gash across your shoulder.\n\nYou LOSE 2 Life Points.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::GAIN_LIFE(player, -2);
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_SKILL(player, Skill::Type::SHOOTING))
        {
            return 304;
        }
        else
        {
            return 326;
        }
    }
};

class Story109 : public Story::Base
{
public:
    Story109()
    {
        ID = 109;

        Text = "Vajra Singh had the same idea at the same time. Golgoth's gaze snaps form Singh to you, but he hesitates a moment too long. Your shot hits first, and Golgoth crumples to the floor. Singh wastes no time taking stock of the situation, swinging his MANTRAMUKTA CANNON around to point at you. Boche seizes his chance to take a shot, but it spatters off Singh's armour. It is the last thing you see, since a moment later you are blasted out of existence by the fiery roar of the MANTRAMUKTA.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;

        Type = Story::Type::DOOM;
    }
};

class Story110 : public Story::Base
{
public:
    Story110()
    {
        ID = 110;

        Text = "After Gargan XIII's wound has been seen to, you spend another hour searching the bomb shelter. You find a canteen, but the food is unsealed and spoiled. At last you have to accept that you will not get access to the Shrine of the Heart from here. Weary and disappointed, you head back to the ventilation duct.\n\n\"You know the theory that the Heart was formed in the Big Bang?\" says Golgoth to you. \"The boffins say it's actually another universe, like a seed that didn't quite get started. I read somewhere that the same thing happens with people. Often you start off with a twin in the womb, but that twin gets reabsorbed into you. In some people, the process happens quite late in the foetus's development. Occasionally the vestigial remains of the unborn twin is found inside a cyst -- you know, tiny limbs, a nubbin of a heart, and so on. It might be true of any of us\"\n\nYou wonder why he is telling you this rather ghoulish bit of medical lore when one of the Gargan sisters interrupts. \"That is only true for those born in the inferior natural way, inside a womb. My sisters and I were all carefully nurtured and grown to maturity. The artificial wombs guaranteed perfect nutrient balance.\"\n\nGolgoth laughs at her proud remarks. It seems to you he is deliberately provoking her. \"Your own twin sisters were all fine specimens of womanhood,\" he replies. \"I should know; it was me that killed all twelve of them.\"\n\nYou have not taken in what Golgoth said before Gargan XII explodes into action. Roaring in fury, she grabs Golgoth's shoulder and spins him around. His gun is in his hand and it looks to you that he might have got off a shot, but Gargan XIII slaps it away and knees him in the stomach. He reels back into Gargan XIV, who grabs him by the throat and dangles him like a rag doll. \"So this is the great Commander Golgoth, sister,\" she says contemptuously. \"Like all so-called natural humans, he is compared to our pure racial stock.\"\n\nShe tosses Golgoth aside and he slumps to the floor. You have a nasty feeling you'll be next.";

        Choices.clear();
        Choices.push_back(Choice::Base("(Enkidu) Order your automaton to attack", 220, Codeword::Type::ENKIDU));
        Choices.push_back(Choice::Base("Step in to fight them yourself", 44));
        Choices.push_back(Choice::Base("Hold back to see what happens", 88));

        Controls = Story::Controls::STANDARD;
    }
};

class Story111 : public Story::Base
{
public:
    Story111()
    {
        ID = 111;

        Image = "images/filler3.png";

        Text = "After a cursory greeting, the others start to disperse back to their tents, Janus Gaunt tells you he has just brewed a pot of tea and invites you to join him. You are pleased enough to share the warmth of his fire, but when a xom hands you a teacup in its bloodless fingers you cannot suppress a shiver of dread.\n\nDon't bother about them,\" he says with a laugh. \"They're just robots, really, except that they're made out of once-living tissue instead of plastic and metal. They're powered by a small electrochemical implant in the chest cavity.\"\n\n\"They are an abomination against nature,\" says Boche flatly, draining his tea. \"Where do we get firewood?\"\n\nGaunt is taken aback by Boche's rudeness, but replies courteously: \"Take it from the buildings around the square. The mulberry window-shutters you are warming your hands over, for instance, date from tenth-century Persia. It is a pity to burn such artifacts as these, but the former owners have no more use for them.\"\n\nBoche rises with a grunt and trudges off, entering a narrow doorway. You follow to find him flashing his torch around. \"Ah, here is some firewood already broken up for our convenience,\" he says.\n\n\"It is mine,\" he purrs a voice of menace from the doorway. Thadra Bey stands there, muscles coiled taut in pantherish grace, a lethal dart-projector in her hand.\n\n\"Down!\" roars Boche, cannoning into you from behind and sending you flying into Thadra Bey. You and she go rolling out into the snow, her dart singing through the air and narrowly missing Chaim Golgoth, who responds once by drawing his BARYSAL GUN. In seconds all hell has broken loose, as the uneasy truce splinters apart. Thadra Bey rakes her fingers across your face and leaps away, levelling her dart-projector, and you hear Gaunt yelling to his xoms, \"Defend me! Slay any who attack!\"\n\nFor a moment it seems that the struggle for power will be decided here and now. Then a voice rips like thunder across the square: \"Stop this senseless fighting now!\" and, turning, you have your first view of the mighty Vajra Singh.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 300; }
};

class Story112 : public Story::Base
{
public:
    Story112()
    {
        ID = 112;

        Text = "With only a split-second left before the creature's gaze paralyses you, you act on raw instinct. Leaping high into the air, you somersault over its head, twisting so as to land directly behind it. The eyestalk sweeps frantically, trying to see where you went. But before the creature can bring its ghastly scrutiny to bear, you give it a hard blow across the back of the head. As it falls senseless in the snow, Boche recovers from the hypnotic trance. Even so, it is several seconds before he has recovered his wits enough to speak.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 90; }
};

class Story113 : public Story::Base
{
public:
    Story113()
    {
        ID = 113;

        Text = "You soon find a man who can do what you want: a fat sweaty fellow with a profusion of ancient tools and devices strewn around his shop. Guiding his laser by hand, he makes a few deft changes to the image on the card until it could pass for your own likeness. \"Five scads,\" he says, holding it out to you.\n\n\"Five?\" you scowl.\n\n\"Membership of the prestigious Compass Society is usually much more expensive than that,\" he says with a shrug.\n\nYou consider snatching the card back, but the fat man cannily anticipates you and holds it close to the laser beam until you pay.";

        Choices.clear();
        Choices.push_back(Choice::Base("Agree to his terms (5 scads)", 414, Choice::Type::LOSE_MONEY, 5));
        Choices.push_back(Choice::Base("Refuse or if you cannot pay (ID CARD is destroyed)", 414, Choice::Type::LOSE_ITEMS, {Item::ID_CARD}));

        Controls = Story::Controls::STANDARD;
    }
};

class Story114 : public Story::Base
{
public:
    Story114()
    {
        ID = 114;

        Text = "\"There's flaw in your story,\" you point out to the man as you scoop up the knife he dropped. You stroke the point against his neck, pricking the skin until a single drop of blood appears. He swallows nervously. You go on, \"Body snatchers don't use knives. There's too much risk of puncturing a vital organ -- damage to merchandise, you might say. They prefer garrottes and sedative sprays.\"\n\n\"All right,\" he admits. \"We were hired to kill you.\"\n\n\"Who?\" You prod him again with the knife.\n\n\"Baron Siriasis.\"\n\nYou are puzzled. \"I've never heard of him. Why should he want me dead?\"\n\n\"He didn't explain it to us. Apparently he regards you as a rival.\"\n\nThis is a mystery you can clear up later. Telling the surviving assassin to make himself scarce, you dry yourself off and get dressed.";

        Choices.clear();
        Choices.push_back(Choice::Base("Take the money from the dead man's body (10 scads)", 92, Choice::Type::GAIN_MONEY, 10));
        Choices.push_back(Choice::Base("Leave it alone", 92));

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Take = {Item::KNIFE, Item::KNIFE};

        Limit = 2;
    }
};

class Story115 : public Story::Base
{
public:
    Story115()
    {
        ID = 115;

        Text = "You settle at the controls of the sky-car and touch the button to power it up. There is a deep hum, and slowly it rises into the air. Hovering at the height of a metre above the floor, you cautiously engage the thrusters. A blaze of blue-white light illuminates the rear wall as the sky-car cannons forward. Quickly you reduce thrust, adroitly steering towards the corridor leading to the entrance. A couple of times you bump the wings against the side walls, scratching to your great annoyance, the perfect matt-black paintwork. You must take care until you have got the hang of this vehicle.\n\nAs you emerge into the open, the gondo looks on aghast and then, turning with a yell, starts running clumsily off through the deep snowdrifts. You increase the speed to catch up. He drops flat as you roar overhead and swing around to hover beside him. He lies trembling with his arms over his head until you say, \"It's just me.\"\n\nHe looks up, \"I thought you were a flying monster!\"\n\n\"You should have looked twice before you panicked,\" you say with a laugh.\n\nAfter some urging, the gondo warily clambers up and slides into the seat beside you. \"Is there any roof to shield the cockpit?\" he asks.\n\n\"Apparently not. Remember that when the Manta sky-car was in common use, people go about in thin clothing without the fear of freezing to death.\"\n\nYou steer back towards Venis. The outward journey took a couple of hours; returning is a matter of minutes.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Vehicle = Vehicle::Type::MANTA_SKY_CAR;
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_CODEWORD(player, Codeword::Type::ENKIDU))
        {
            return 181;
        }
        else
        {
            return 159;
        }
    }
};

class Story116 : public Story::Base
{
public:
    Story116()
    {
        ID = 116;

        Text = "The computer terminals are only intended to access the library catalogue, but you have no trouble routing into the building's administrative computer and then opening an outside line via the rooftop satellite dish. Like most organizations with the ability to connect into global communications, the Society protects its system from accidental linkage into Gaia by the use of a filter program. This is necessary to prevent infection by the same viruses that are resident in Gaia, as well as to stop Gaia from taking over the Society's whole system for her own use.\n\nYou set a standard filter-override program running. It should take a few minutes, and to kill time you run a check on other users who have logged into the system recently. Only one name is displayed: Janus Gaunt. He requested all the Society's records regarding the Heart of Volent. Intrigued, you call up his biofile. The screen shows a round-faced man with extremely white skin and hair like silver floss. Flicking idly through the data, you find he has a reputation for outstanding work in the fields of bioengineering and nanotechnology. The address of his mansion causes you a double-take; it is located in the Paris catacombs. You were not even sure Paris still existed.\n\nThe terminal bleeps, informing you the link with Gaia is ready. You switch over. When you type in your query about the Heart, Gaia's response is swift: THE HEART MUST BE DESTROYED. ACTIVATION OF ITS POWER WILL CRASH THE UNIVERSE, WIPING OUT ALL THAT EXISTS.\n\nYou reply: INCLUDING EARTH?\n\nEVERYTHING, Gaia tells you. BARYSAL BOMBARDMENT CAN CAUSE A CRITICAL RESONANCE. DESTROYING THE HEART'S CRYSTALLINE STRUCTURE. TWO SIMULTANEOUS BOMBARDMENTS MUST BE MADE, THE BEAMS PHASED AND CROSSING AT RIGHT ANGLES.\n\nThis is awkward. From what you have heard, the Heart is a gem several metres across. To destroy it as Gaia suggests, you'd need an accomplice. And two BARYSAL GUNs. You try to get further information, but the link is broken. Like a senile invalid, Gaia has lapsed back into her customary incoherence.\n\nYou gained the codeword NEMESIS.";

        Choices.clear();
        Choices.push_back(Choice::Base("Study the records on Heart of Volent", 182));
        Choices.push_back(Choice::Base("You have finished in the library", 73));

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::GET_CODEWORDS(player, {Codeword::Type::NEMESIS});

        if (Character::VERIFY_SKILL(player, Skill::Type::LORE))
        {
            Choices[0].Destination = 160;
        }
        else
        {
            Choices[0].Destination = 182;
        }
    }
};

class Story117 : public Story::Base
{
public:
    Story117()
    {
        ID = 117;

        Text = "\"It's been useful having you along,\" says Shandor, beaming his confident smile as he firmly shakes your hand. \"I'm sure you won't need my advice on getting by in Venis, resourceful as you are, so let me give you something else.\"\n\nHe reaches into a pocket and produces a monkey token which he touches to yours, automatically transferring the sum of 20 scads to you. You are about to protest when you notice the sum remaining on his token. He can well afford what he's paid to you.\n\nYou GAINED 20 scads.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::GAIN_MONEY(player, 20);

        if (!(Character::VERIFY_SKILL(player, Skill::Type::CLOSE_COMBAT) && Character::VERIFY_ITEMS(player, {Item::Type::SHORT_SWORD})))
        {
            Bye = "Bidding Shandor and his men farewell, you set off into Venis.";
        }
        else
        {
            Bye = NULL;
        }
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_SKILL(player, Skill::Type::CLOSE_COMBAT) && Character::VERIFY_ITEMS(player, {Item::Type::SHORT_SWORD}))
        {
            return 8;
        }
        else
        {
            return 334;
        }
    }
};

class Story118 : public Story::Base
{
public:
    Story118()
    {
        ID = 118;

        Text = "You jump to your feet and scramble through the undergrowth. The creature follows at a leisurely pace, squawking triumphantly from high in the branches. It claws its way effortlessly between the close spaced trunks, sometimes sailing out across clearings on the wide leathery kite of its body.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_SKILL(player, Skill::Type::SURVIVAL))
        {
            return 206;
        }
        else if (Character::VERIFY_SKILL(player, Skill::Type::LORE))
        {
            return 140;
        }
        else
        {
            return 162;
        }
    }
};

class Story119 : public Story::Base
{
public:
    Story119()
    {
        ID = 119;

        Text = "A lethal blue spike of light pierces the air, charring its way through the chest of the man with the gun. The knifeman blinks, starts to backpedal then realizes he has no choice but to attack you anyway. The instant's hesitation proves his undoing, as you have time to swing your gun around and unload a blast at point-blank range. The third man rushes in with a sob of desperate fury. You lash out with the but of the gun and he drops as though poleaxed.\n\nYou search the trapper's shelter. Their BARYSAL GUNs has two charges left. You also find two KNIVES, a set of POLARIZED GOGGLES, BINOCULARS, and a COLD-WEATHER SUIT, and six FOOD PACKs.";

        Choices.clear();

        Bye = "Then you wait for the blizzard to blow itself out before you emerge into the crisp snow outside.";

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::FIRE_BARYSAL(player, 2);

        Take = {Item::MAKE_BARYSAL_GUN(2), Item::MAKE_BARYSAL_GUN(2), Item::KNIFE, Item::KNIFE, Item::POLARIZED_GOGGLES, Item::BINOCULARS, Item::COLD_WEATHER_SUIT, Item::FOOD_PACK, Item::FOOD_PACK, Item::FOOD_PACK, Item::FOOD_PACK, Item::FOOD_PACK, Item::FOOD_PACK};

        Limit = 13;
    }

    int Continue(Character::Base &player) { return 393; }
};

class Story120 : public Story::Base
{
public:
    Story120()
    {
        ID = 120;

        Text = "The innkeeper is cringing at the end of the bar with a sick look on his face. You bow to the twins, saying, \"Ladies, pardon me. I am a simple servant here.\" Turning to the innkeeper, you ask, \"master, shall I fetch the very best vodka for your guests?\"\n\nThe twins scowl at him. \"Isn't this the best?\"\n\nHe twitches nervously, but senses you have a plan in mind. \"Er... my my dear ladies, cherished guests --\"\n\nOne of the twins seizes his jerkin and hauls him across the bar, glaring into his face. \"Well?\"\n\n\"Ulp. In fact, there is one bottle of extremely fine Old Daralbad Immolate in the cellar.\"\n\n\"Fetch it.\" This is addressed to you. You race out to the cellar door to get a bottle, returning by way of the bathroom at the back of the building where you find the inn's medicine cabinet. As you come racking back, the bottle is snatched out of your hands.\n\n\"I must advise caution, my lady,\" you say, almost grovelling. \"This is strong liquor.\"\n\n\"Pah!\" She drains half the bottle at a gulp, then hands the rest to her sister.\n\nYou stand back and watch. Gradually the twins start to yawn, then fold across the bar. Only when they begin snoring do the rest of the customers feel safe in approaching these two fearsome Amazons. Even asleep, they inspire such fear that no one knows quite what to do with them, until you suggest putting them in a rowboat and pushing it out to sea.\n\n\"How long before they wake up?\" asks the innkeeper.\n\nYou shake your head. \"Who knows? I put a whole packet of sleeping pills in that vodka, but these two seem to have a vigorous metabolism. Best that we get rid of them at once.\"\n\nYou gained the codeword SCYTHE.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::GET_CODEWORDS(player, {Codeword::Type::SCYTHE});

        Take = {Item::MEDICAL_KIT};

        Limit = 1;
    }

    int Continue(Character::Base &player) { return 142; }
};

class Story121 : public Story::Base
{
public:
    Story121()
    {
        ID = 121;

        Text = "Golgoth must have a keen interest in lurid fiction, because his thoughts are filled with images of carnage, explosions, gunfire, and violent death. Then you realize that these are not scenes from films or books -- they are Golgoth's actual memories. He has successfully eliminated dozens of the United States' enemies around the world, mainly by dint of trickery, cunning and a quite unabashed level of viciousness. Viewing these memories from his mind, at the same time as you return that placid smiling gaze, sends a shiver along your spine.";

        Choices.clear();
        Choices.push_back(Choice::Base("Ask Golgoth what he knows about Giza", 337));
        Choices.push_back(Choice::Base("Ask about his reason for carrying a crossbow", 315));
        Choices.push_back(Choice::Base("Allow Boche to lead you away from this hardened killer", 358));

        Controls = Story::Controls::STANDARD;
    }
};

class Story122 : public Story::Base
{
public:
    Story122()
    {
        ID = 122;

        Text = "Awareness returns slowly, the details of your surroundings emerging out of a blur. There is a white ceiling above you and soft fabric at your back. The air carries a faint smell of antiseptic. You can make out the low whine of air conditioning.\n\n\"You're able to hear me?\" booms a voice.\n\nYou wince, focussing on a face that looms above yours. The colours seem harsh, garish. Sounds come to your ears with a rasping undertone, as though filtered by electronics.\n\nAnother face comes into view, slender and handsome with a high forehead capped by a green circlet. He has a lopsided but not unfriendly smile. \"How are you feeling?\" he asks.\n\n\"I've got pins and needles,\" you say in a voice still slurred by anaesthetic. Reaching across to rub your left arm, you feel the unyielding hardness of metal in place of flesh. You sit bolt upright with a thrill of horror, throwing off the sheets. You can only stare at what they have made of you: a being half of robotics, half of living tissue. A cyborg.\n\n\"It was all we could do to save your life,\" explains the man wearing the green circlet. \"You were caught in the jets as my flyer took off. I brought you up here. It was touch and go for a couple of weeks, but you should be all right now.\"\n\n\"What's left of me, that is,\" you say bitterly. Turning to the window, you see a surprising profusion of stars in a black void. \"Where are we, anyhow?\"\n\nThe other man, the doctor, takes you by your still-living right arm and leads you to the window. A vast crescent globe of swirling grey and white hangs in space below you.\n\n\"That's the Earth,\" he says. \"You're on al-Lat.\"\n\nThe pilot joins you. \"I know you are still shaken, but I must ask you some questions. You should not have been in Maka. How did you get there, and why?\"\n\nYou gained the codeword TALOS.";

        Choices.clear();
        Choices.push_back(Choice::Base("Tell him the truth", 144));
        Choices.push_back(Choice::Base("Invent a convincing story", 166));

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::GET_CODEWORDS(player, {Codeword::Type::TALOS});
    }
};

class Story123 : public Story::Base
{
public:
    Story123()
    {
        ID = 123;

        Text = "\"Target identification: bometh,\" grates Gilgamesh. \"Mutant wolf-bear hybrid. Predator. It presents a danger. Immediate elimination is called for.\"\n\nHe raises his arm, ejecting a crackling blast of energy that turns the dusk to day. The swirling snowflakes hiss into steam. On the crest of the rise, the giant beast shudders and falls, rolling down into the deep snow. With Gilgamesh clanking along behind, you hurry over to make sure the bometh is dead, as you would not want a wounded predator stalking you through the night.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 341; }
};

class Story124 : public Story::Base
{
public:
    Story124()
    {
        ID = 124;

        Text = "The carriage rushes on into the darkness of the tunnel. You wait for almost two hours, and then you start to feel the carriage slowing down. It enters a station and glides to a halt, but there is a delay before the doors open. \"Karthag station is damaged,\" explains the motilator's calm electronic voice. \"You are recommended to select an alternative destination.\"\n\nThrough the window, you can see that the station has caved in. Huge chunks of shattered concrete litter the platform, with twisted metal cables extending from them like torn arteries from a heart. It is sheer luck that the tunnel itself was not blocked, otherwise you would have ended your journey with a sever jolt, to say the least.\n\nWhat now?";

        Choices.clear();
        Choices.push_back(Choice::Base("Disembark here", 146));
        Choices.push_back(Choice::Base("Take the subway back to Marsay, and from there head on to Kahira", 50));
        Choices.push_back(Choice::Base("... to Tarabul", 31));
        Choices.push_back(Choice::Base("... to Giza", 74));
        Choices.push_back(Choice::Base("Resume your journey on foot", 420));

        Controls = Story::Controls::STANDARD;
    }
};

class Story125 : public Story::Base
{
public:
    Story125()
    {
        ID = 125;

        Image = "images/du-en.png";

        Text = "At last you see a streak of dark rubble against the dazzling skyline. You fear it might just be a line of hills or even a trick of the light, but as you approach on quickened footsteps it is possible to make out the details of brooding towers, empty palaces and gargantuan snow-bound walls. You have arrived at the lost city of Du-En.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_CODEWORD(player, Codeword::Type::DIAMOND))
        {
            Character::REMOVE_CODEWORD(player, Codeword::Type::DIAMOND);

            return 191;
        }
        else
        {
            return 213;
        }
    }
};

class Story126 : public Story::Base
{
public:
    Story126()
    {
        ID = 126;

        Text = "You find Golgoth squatting by torchlight at the end of the colonnade, where he has laid out all his weapons o the flagstones. As he checks each, he slips it into its concealed sheath: a garrotte wire under his belt, along with a flexible steel blade; poison darts in a bandolier inside his jacket; guns at hip, ankle and wrist; small flat grenades clipped along his sabretache. You watch him aghast for a few minutes.\n\n\"Quite the professional killer, aren't you, Golgoth?\"\n\n\"Don't get far if you only make it a hobby.\"\n\nYou heave a sigh. \"Does human life mean anything to you?\"\n\nHe buckles on his BARYSAL GUN, gets up, and gives you a long thoughtful look in the torchlight. \"Not the life of scum like this.\" He gestures along the colonnade. \"I've happily sent hundreds like them to an early grave. Who do you think my USI bosses should've sent -- a pack of boy-scout Marines?\"\n\n\"So you're here as a USI agent?\"\n\nHe nods. \"Of course. The power of the Heart cannot be allowed to fall into hostile hands. In order of priority, I will either take it to the States, get the power myself, or destroy it.\"";

        Choices.clear();
        Choices.push_back(Choice::Base("(Nemesis) Propose an alliance", 204, Codeword::Type::NEMESIS));
        Choices.push_back(Choice::Base("Go and talk to Kyle Boche", 104));
        Choices.push_back(Choice::Base("Get some sleep", 192));

        Controls = Story::Controls::STANDARD;
    }
};

class Story127 : public Story::Base
{
public:
    Story127()
    {
        ID = 127;

        Text = "The glowing phantom is leeching the strength from your living tissue, but your artificial body parts are not affected. Lunging out with your metal arm, you seize it by the neck. Your cyborg leg carries you forward with a lurching gait until you stand on the brink of the chasm. The phantom squirms in your unbreakable grip, its form twisting and flowing like melting wax. As its fingers penetrate the circuitry of your arm there is a flash of sparks and the feedback causes some damage.\n\nYou LOSE 2 Life Points.";

        Bye = "You manage to throw the phantom down. Its thin bleating cry echoes up from the depths as it falls, trailing its gleaming wisps of vapour like a comet's tail.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::GAIN_LIFE(player, -2);
    }

    int Continue(Character::Base &player) { return 149; }
};

class Story128 : public Story::Base
{
public:
    Story128()
    {
        ID = 128;

        Text = "The passage brings you to a series of galleries, each consisting of a cloister running either side of a central concourse softly illuminated by chandeliers. Many of the heavy buttresses have been defaced, leaving chunks of broken masonry scattered across the marble floor. \"No doubt this devastation was wreaked in the city's collapse,\" says Boche in a hushed voice. \"It's said that the people of Du-En went mad and turned against their leaders.\"\n\nBaron Siriasis drifts to a halt and peers off into the gloom of the cloistered walkway at the side of the room. \"I sense a presence,\" he says after a moment's concentration. \"Something is stalking us.\"\n\nQuickening your pace you hurry on through the galleries until you see a heavy iron-bound door ahead. A rasping sound echoes out of the cloisters to one side. It sounds like chitin slithering across stone. You are about to break into a run when the lights go out and you are plunged into darkness.";

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Choices.clear();

        if (!Character::VERIFY_CODEWORD(player, Codeword::Type::SCOTOPIC))
        {
            Choices.push_back(Choice::Base("Use [ROGUERY]", 216, Skill::Type::ROGUERY));
            Choices.push_back(Choice::Base("Use [ESP]", 194, Skill::Type::ESP));
            Choices.push_back(Choice::Base("[SHOOTING] Use a charged BARYSAL GUN", 238, Skill::Type::SHOOTING));
            Choices.push_back(Choice::Base("Light a LANTERN or FLASHLIGHT", 172, Choice::Type::ANY_ITEM, {Item::FLASHLIGHT, Item::LANTERN}));
            Choices.push_back(Choice::Base("Otherwise", 260));
        }
    }

    int Continue(Character::Base &player) { return 302; }
};

class Story129 : public Story::Base
{
public:
    std::string PreText = "";

    Story129()
    {
        ID = 129;

        Bye = "You retreat rapidly. But just as you lose sight of the hover-droid, you turn to see the baron's brain drifting towards you. You are between a rock and a hard place.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        PreText = "With the ghastly brain floating after you, you race out of the hall. The passage soon forks, but you have no time to pause and get your bearings. You blunder on, gasping for breath, looking back over your shoulder to see if your pursuer is still there. Losing sight of it around a bend in the tunnel, you begin to calm down and think. The baron is a powerful psychic .. ore powerful than you ever dreamed, to outlive his body like this -- but he cannot survive once the remaining oxygen in his brain tissue is used up. All you have to do is stay ahead of him that long.\n\nYour simple plan is ruined a moment later when, darting around a junction in the passage, you come face to face with a hover-droid. A quarter of a second is long enough for you to start tor turn, and for the hover-droid to identify you as an intruder in the catacombs. As you leap back, its laser flares up and you feel a sickening pain as the hot beam cuts through your gut.\n\n";

        auto DAMAGE = -4;

        if (Character::VERIFY_ITEMS(player, {Item::Type::SPECULUM_JACKET}))
        {
            PreText += "[Item: SPECULUM JACKET] ";

            DAMAGE = -3;
        }

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Points.";

        Character::GAIN_LIFE(player, DAMAGE);

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 85; }
};

class Story130 : public Story::Base
{
public:
    Story130()
    {
        ID = 130;

        Text = "The shot was a decoy. Sensing Golgoth's thoughts, you whirl to see him running silently through the smoke towards you. The look in his eyes is the chilling glint of an inhuman killer. Your arm comes up by instinct and you send a lethal blast of energy through his heart. He topples at your feet.";

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::FIRE_BARYSAL(player, 1);
    }

    int Continue(Character::Base &player) { return 72; }
};

class Story131 : public Story::Base
{
public:
    std::string PreText = "";

    Story131()
    {
        ID = 131;

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        PreText = "Your shot burns through Singh's armour and he staggers back, but although wounded he is far from beaten. He presses the fire button on his MANTRAMUKTA CANNON just as Boche goes for an opportunist shot at you. The beam carves through your shoulder.\n\n";

        auto DAMAGE = -2;

        if (Character::VERIFY_ITEMS(player, {Item::Type::SPECULUM_JACKET}))
        {
            PreText += "[Item: SPECULUM JACKET] ";

            DAMAGE = -1;
        }

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Points.";

        Character::GAIN_LIFE(player, DAMAGE);

        if (player.Life > 0)
        {
            PreText += "\n\nA moment later Boche falls as Singh swings the cannon around, blasting him apart with a torrent of searing energy.\n\nThere is a moment of silence as the cannon's blast cuts out. It will take a few seconds to build up charge before it can fire again. Golgoth seizes the chance to take aim with his BARYSAL GUN. This is the showdown that will decide which of you lives to claim the power of the Heart.";
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 410; }
};

class Story132 : public Story::Base
{
public:
    Story132()
    {
        ID = 132;

        Image = "images/filler1.png";

        Text = "A barrage of tightly focused plasma bolts flash through the air directly overhead. You feel the wave of heat as the copper wires are vaporized. The puppets drop lifeless to the stage.\n\nGilgamesh lowers his arm. Smoke is wafting from his built-in gun. \"Random motion of manikins could have caused you damage,\" he grates in his mechanical voice. \"They have been rendered inert. Danger now over.\"\n\n\"And you wanted to leave the tin man behind,\" Golgoth reminds the Gargan sisters as you get down off the stage.";

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 110; }
};

class Story133 : public Story::Base
{
public:
    Story133()
    {
        ID = 133;

        Text = "\"You won't regret it,\" he says emphatically.\n\nYou look to the east, where the morning sun is hidden under a shelf of heavy grey cloud. A link of black posts protrude from the snow, marking the road to Venis. The other direction would take you through the swamplands of Lyonesse -- the one region of Europe not afflicted by ice sheets and arctic blizzards. But Lyonesse has dangers of its own.";

        Choices.clear();
        Choices.push_back(Choice::Base("Head east towards Venis", 200));
        Choices.push_back(Choice::Base("Go west through Lyonesse", 177));

        Controls = Story::Controls::STANDARD;
    }
};

class Story134 : public Story::Base
{
public:
    Story134()
    {
        ID = 134;

        Text = "The creature's lair proves to be a cave at the end of the pass. Inside you find a fire of smouldering peat. Around it are strewn bones from humans and large animals. It seems that the creature trapped its prey by hypnosis, leaving the victim to die of exposure. Whenever it needed fresh meat, it had only to fetch in one of the frozen bodies along the pass -- a gruesomely effective procedure. The aftermath of the Paradox War has left the world with many such weird mutations.\n\nBoche gives an involuntary cry of disgust, which he immediately disguises with a nervous laugh. He has discovered a clutch of the creature's young: blobby heads like diseased potatoes, bodies as shrivelled as a bag of giblets, the mesmeric eye no more than a yellow pebble on the end of an embryonic tuber-like stalk.\n\n\"Cute little devils.\"";

        Choices.clear();
        Choices.push_back(Choice::Base("Kill them", 156));
        Choices.push_back(Choice::Base("Leave them alone", 178));

        Controls = Story::Controls::STANDARD;
    }
};

class Story135 : public Story::Base
{
public:
    Story135()
    {
        ID = 135;

        Text = "You show the card to various forgers around town. One tells you that the process involved in altering a hologram is very expensive, since lasers and other rare devices are needed. You are about to leave his shop when he sidles over and glances furtively along the street. Dusk squats over the city, pouring dank slush snow from a colourless sky. He lowers the blinds. \"I can't alter the picture,\" he says. \"But what about your own face?\"\n\n\"Cosmetic surgery?\"\n\nHe shows you to a room at the back. \"I do it all the time for clients who want to escape their past misdeeds. A whiff of gas and you sleep. When you wake, you'll have a new face.\"\n\n\"How much?\"\n\nAfter some haggling, he settles on the sum of 5 scads. He reaches out his hand, but you smilingly shake your head, telling him you will pay once the operation is over. He prepares his instruments, then invites you to breathe the anaesthetic gas.";

        Choices.clear();
        Choices.push_back(Choice::Base("Reconsider and leave now", 223));
        Choices.push_back(Choice::Base("Go ahead with the operation", 201));

        Controls = Story::Controls::STANDARD;
    }
};

class Story136 : public Story::Base
{
public:
    Story136()
    {
        ID = 136;

        Text = "You sense that he is lying. Narrowing your gaze, you search deeper into his thoughts while he lies there helpless on the titles. The shower splashes water onto both of you, icy cold now.\n\nYou glimpse the image of a crippled man. white hair like a puff of steam surrounds an old, sick, deeply seamed face. He ordered your death. The assassin does not know why.\n\nDeciding it is easier just to question the man, you say, \"Who was the man who hired you?\"\n\n\"Baron Siriasis, a paradoxer from Bezant. He said you were not to reach Kahira.\"\n\n\"Why did he want me dead?\" you ask. But the question is futile; you have already read the assassin's mind, and he cannot give you any answer. It is a mystery you must clear up later. Telling the man to make himself scarce, you dry yourself off and get dressed.";

        Choices.clear();
        Choices.push_back(Choice::Base("Take the money from the dead man's body (10 scads)", 92, Choice::Type::GAIN_MONEY, 10));
        Choices.push_back(Choice::Base("Leave it alone", 92));

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Take = {Item::KNIFE, Item::KNIFE};

        Limit = 2;
    }
};

class Story137 : public Story::Base
{
public:
    std::string PreText = "";

    Story137()
    {
        ID = 137;

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        PreText = "The sky-car emits a soft hum as you engage the power. White light flares from the thrusters, casting a garish glow across the walls. It rises to hover a metre or so above the floor. Despite the smoothness of its movement, you are aware of the enormous power in the machine and open the throttle only gently. Unfortunately you misjudge it even so. The thrusters boom, sending the sky-car caroming across the chamber. Frantically you twist the joystick, trying to turn towards the corridor, but you are going too fast. The sky-car smashes into the wall and you are flung out, hitting the floor with numbing impact.\n\nWhen you come round, your whole body is a single throbbing ache. You feel sure you must have cracked a couple of ribs, and your wrist is badly wrenched. Blood pours from a deep graze above your eyes, and as you get to your feet a wave of dizziness hits you.\n\nYou LOSE 4 Life Points.";

        Character::GAIN_LIFE(player, -4);

        if (player.Life > 0)
        {
            Character::LOSE_VEHICLE(player);

            PreText += "\n\nYou stagger over to look at the sky-car. It is a wreck. The chassis has split and white-hot sparks are cascading from the broken power unit. The caretek that had maintained it for all these years comes crawling drearily forward and begins probing the wreckage. You almost feel sorry for it. It has its work cut out for the next year or so. The sky-car will not fly again before then. Now all you can do is rummage through the storage locker and salvage a few items.";
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 93; }
};

class Story138 : public Story::Base
{
public:
    Story138()
    {
        ID = 138;

        Text = "You seek out the librarian, a plump sour-faced man who sits at his desk amid the stacks like a spider in its web. He is barely able to disguise his irritation when you explain what you want. \"A link to Gaia? That is most irregular. Very few of our members make such requests.\"\n\nHe will deter you if you let him, if only to spare himself inconvenience. Recalling the status of the typical Society member, you adopt an uncompromising attitude and say, \"It was not a request, but a command. You will now establish a link so that I can talk to Gaia.\"\n\n\"Talk?\" He spreads his hands imploringly. \"What will you talk about? Gaia is mad!\" Seeing you will not be put off, he grumbles under his breath and pushes a slip of paper across the desk. \"Write your query there and it will be broadcast to Gaia. The reply will be brought back to you.\"\n\n\"I prefer a direct two-way communication.\"\n\n\"Impossible!\" he cries. \"That is against Society policy, as nay link to Gaia must be stringently monitored to prevent arrogation of our computer network.\"\n\nYou see he will not be swayed on this point. You write out your message and wait for half an hour until the librarian comes back. \"Here is your reply from Gaia,\" he says, his tone of surprise showing that he did not expect anything but gibberish. He reads from the paper in his hand; \"go and meet with Gilgamesh under the pyramid. Humbaba will give you access.\"\n\n\"Is that all?\"\n\nHe nods. \"Gaia then began to transmit random references to Babylonian history followed by a digression into architectural feats of history, and the link was terminated.\"\n\nYou gained the codeword HUMBABA.";

        Choices.clear();
        Choices.push_back(Choice::Base("Consult the archives for information about the Heart of Volent", 182));
        Choices.push_back(Choice::Base("You are finished in the library", 73));

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::GET_CODEWORDS(player, {Codeword::Type::HUMBABA});

        if (Character::VERIFY_SKILL(player, Skill::Type::LORE))
        {
            Choices[0].Destination = 160;
        }
        else
        {
            Choices[0].Destination = 182;
        }
    }
};

class Story139 : public Story::Base
{
public:
    Story139()
    {
        ID = 139;

        Text = "You ascend into the mountains across stark rocky ridges like the broken backs of colossal dinosaurs. The sun shines as feebly as a flashlight seen through a thick pane of ice. When the wind gusts into your face, it is so cold that you can hardly draw breath.\n\nOn the second day you come upon four figures also trudging eastwards. They are several hundred metres ahead on the surface of a glacier. As you hurry to catch up, you see patches where the snow has swirled away to reveal the sky surface of the glacier reflecting glints of feeble daylight.\n\nThe leader of the group is a short broad-shouldered man whose dark sparkling eyes display an easy authority. The other three, apparently his bodyguards, are hulking men whom you take to be of South Pacific origin. It is hard to be sure with the fur hoods drawn so tightly around their faces.\n\nThe short hand man shakes hands and introduces himself as Hal Shandor. \"Our sky-car crashed in the hills back there,\" he explains. \"We're going on to Venis. Travel with us if you want.\"";

        Choices.clear();
        Choices.push_back(Choice::Base("Join their group", 225));
        Choices.push_back(Choice::Base("Journey alone", 161));

        Controls = Story::Controls::STANDARD;
    }
};

class Story140 : public Story::Base
{
public:
    Story140()
    {
        ID = 140;

        Image = "images/filler1.png";

        Text = "The creature that attacked you is a sanguivore, a mutant lizard with gliding and mimicking abilities. The sanguivore's saliva contains an anti-clotting agent, ensuring that its prey slow bleeds to death even after escaping from it. That explains why it's in no hurry to catch up with you. It is content to track you through the woods and wait until you collapse from exhaustion. Well, you have a few resources not shared by any wild animal. Tearing the lining of your jacket into strips, you bind the wound to prevent further loss.";

        Choices.clear();
        Choices.push_back(Choice::Base("Rest here", 184));
        Choices.push_back(Choice::Base("Press on through the jungle for a while", 250));

        Controls = Story::Controls::STANDARD;
    }
};

class Story141 : public Story::Base
{
public:
    Story141()
    {
        ID = 141;

        Text = "There are men beyond the plastiwood partition. You read their thoughts: three hunters. No doubt the cruelty you see in their minds is only a symptom of their harsh existence on the fringes of the Sahara. All the same, you are wise to stay concealed. They are not above murdering lone travellers for the clothes on their back.\n\nWaiting until they go to sleep, you carefully ease the partition open and sneak past them. A rug hangs across the entrance of their shelter. You can hear a blizzard howling through the night outside -- an uninviting sound, but preferable to staying here to be butchered when these three wake. Drawing your hood down, you slip out into the icy wind.";

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 314; }
};

class Story142 : public Story::Base
{
public:
    Story142()
    {
        ID = 142;

        Image = "images/filler2.png";

        Text = "The reputation of the Gargan clones is so daunting that no one dares to suggest killing them. \"I have heard that the other members of the clone group were killed, but who can be sure?\" says one man with the flaring blue hat-flaps of a Tuareg hunter.\n\n\"That's true,\" admits the innkeeper with a nod. \"I wouldn't care to have twelve identical Amazons barge into my taproom a month from now and demand retributions for their sisters.\"\n\nAt last a compromise is reached. The unconscious sisters are taken down to the shore and placed in a small boat which you push out to sea. You watch the boat drift away into the frosty evening haze. Shivering, you stamp back through the snow to the inn, where you are given food and wine and treated like a hero.\n\nThe innkeeper also offers you provisions for your journey: two FOOD PACKS.\n\nYou RECOVER 1 Life Point.";

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::GAIN_LIFE(player, 1);

        Character::GET_ITEMS(player, {Item::FOOD_PACK, Item::FOOD_PACK});
    }

    int Continue(Character::Base &player) { return 273; }
};

class Story143 : public Story::Base
{
public:
    Story143()
    {
        ID = 143;

        Text = "\"On this point,\" announces Bador, \"I would be untruthful if I pretended to know with adamantine certainty. According to some, the city took its name from Khare-Ohe, or 'Field of Conflict,' as it was found on the spot where the first pharaoh watched a falcon fight a rat. Another version relates it to the settlement of el-Qahira, consecrated to the red planet of victory.\"\n\nYou shake your head. \"These are ancient myths. I am interested in recent history.\"\n\nAh, Well, then Du-En rose to power, Kahira became an important as a base of operations for those armed forces opposing the Volentine Watchers -- in essence, the rest of the world. Owing to heat-conductive pipes buried along the bed of the Isis River, fishes are abundant even in these parlous times, and Kahira continues to flourish. The warm water, rising into contact with icy winds off the Saharan plain, forms the incessant mist which is characteristic of the city.\"\n\n\"And why is the city built on high columns of concrete, instead of sprawling along the river banks?\"\n\nBalor pulls an uncertain face. \"Defence? Scarcity of materials? An obscure edict? Who can say?\"";

        Choices.clear();
        Choices.push_back(Choice::Base("Ask his advice about the Sahara", 77));
        Choices.push_back(Choice::Base("Ask about Giza", 59));
        Choices.push_back(Choice::Base("About the best place to stay", 99));
        Choices.push_back(Choice::Base("Dismiss him", 95));

        Controls = Story::Controls::STANDARD;
    }
};

class Story144 : public Story::Base
{
public:
    Story144()
    {
        ID = 144;

        Text = "\"I found a terminal of the intercontinental subway system, beneath the ruins of Lost Marsay. I tried to reach Giza, but I was unable to alight there and was left with no alternative but to travel on to Maka.\"\n\nHe strokes his thumbnail across the cleft of his chin, deep in thought. \"Why were you trying to reach Giza?\" he asks at last.\n\nThe truth seems absurd, but you tell him anyway. \"I was following the advice of Gaia.\"\n\nHe gives you a sharp look. \"Do not attempt to contact Gaia while you are on al-Lat. Our computer systems are secure from viruses, and that's the way we'd like it to stay.\" His face softens into an easy smile. \"Still, I'm forgetting my manners. I am Riza Baihaqi.\"\n\nYou shake hands. There is a moment of uncomfortable silence, then Riza says, \"I can never atone for the guilt I feel. This dreadful injury you've suffered...\"\n\nYou raise your artificial arm and flex the fingers. There is a soft buzz of gears, but they seem dextrous enough. \"It could be worse,\" you admit. \"These prosthetics will do as temporary repairs, at any rate.\"\n\n\"You are putting a brave face on things, my friend,\" says Riza. \"But, in all truth, where on Earth could you hope to find a cure for your dreadful injuries?\"";

        Choices.clear();
        Choices.push_back(Choice::Base("Tell him about Du-En", 188));
        Choices.push_back(Choice::Base("Say you are keen to return to Earth", 275));

        Controls = Story::Controls::STANDARD;
    }
};

class Story145 : public Story::Base
{
public:
    Story145()
    {
        ID = 145;

        Text = "The stun grenade sails through the air and crunches into the deep snow at the foot of the rise. The bometh slowly stirs itself and lumbers down the slop to snuffle around. As it locates the grenade, you detonate by remote. A flurry of snow is thrown up into the bometh's face by the blast and you see it drop. An imperceptible fraction of a second later, the hard crump of the explosion reaches your ears. By that time you are already running towards the fallen predator.\n\nKneeling beside it in the crater of slush formed by the explosion, you life the huge limp head across your knee and give it a sharp twist. The bometh gives a spasm and then lies still.";

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::LOSE_ITEMS(player, {Item::Type::STUN_GRENADE});
    }

    int Continue(Character::Base &player) { return 341; }
};

class Story146 : public Story::Base
{
public:
    Story146()
    {
        ID = 146;

        Text = "You explore the corridors leading off the platform, but all are blocked by rubble. There is no way of reaching the surface from here. As you return to the carriage, though, you notice a small partition in the wall. It is labelled as an air vent. You remove the grating and peer up the darkened shaft. A waft of rich loamy air drifts down. The shaft is intended for careteks servicing the ventilation ducts, but at a pinch you might be able to squeeze up it -- if you're not bothered by claustrophobia.";

        Choices.clear();
        Choices.push_back(Choice::Base("Venture up the shaft", 168));
        Choices.push_back(Choice::Base("Take the subway back to Marsay, from where you can proceed to Kahira", 50));
        Choices.push_back(Choice::Base("... to Tarabul", 31));
        Choices.push_back(Choice::Base("... or to Giza", 74));
        Choices.push_back(Choice::Base("Resume your journey on foot", 420));

        Controls = Story::Controls::STANDARD;
    }
};

class Story147 : public Story::Base
{
public:
    std::string PreText = "";

    Story147()
    {
        ID = 147;

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        PreText = "How long have you been out on the Ice Wastes? You've lost all sense of time. The wind becomes a constant roaring in your ears. You stumble on through the sheets of snowflakes, each breath sounding like a sob of pain.";

        auto DAMAGE = -4;

        if (Character::VERIFY_SKILL(player, Skill::Type::SURVIVAL))
        {
            DAMAGE = -3;
        }

        if (Character::CHECK_VEHICLE(player, Vehicle::Type::BURREK))
        {
            DAMAGE += 2;
        }

        PreText += "\n\nYou LOSE " + std::to_string(-DAMAGE) + " Life Point(s).";

        if (player.Life > 0)
        {
            if (Character::CHECK_VEHICLE(player, Vehicle::Type::BURREK))
            {
                Character::LOSE_VEHICLE(player);

                PreText += "\n\nYou slaughter the BURREK in desperation for its meat.";
            }
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 125; }
};

class Story148 : public Story::Base
{
public:
    Story148()
    {
        ID = 148;

        Image = "images/filler3.png";

        Text = "Gaunt walks with you to the outskirts of Du-En to show you the night sky. His undead xoms stalk silently alongside bearing glow-lamps. At the city gates, they dim the lamps and you are left with the light of ten thousand glittering stars. The snows of the Sahara are swallowed by darkness, but you get the impression of standing at the hub of infinity.\n\nFor a long period neither of you speaks. Then Gaunt recites softly, \"Some say the world will end in fire, some say in ice. From what I've tasted of desire, I hold with those who favour fire. But if it had to perish twice, I think I know enough of hate to say that ice is also great, and would suffice.\"\n\n\"What's that?\"\n\n\"The words of a poet long ago.\" He gazes to the north. \"My home city lies under a shield of ice a kilometre thick. Soon the world will slip into a coma, like a man frozen at the point of death. The polar caps will meet and everything will end.\"\n\n\"Unless we find the Heart and use its power to set things right.\"\n\nHe turns with a smile almost of delight. \"Is that why you've come here? I fear you'll be disappointed. The Heart must inevitably fall into the hands of one who is most ruthless. To seize true power, a man must have a heart of ice. When the powerful do good deeds -- I speak of Caesar, Alexander, Napoleon, Mao -- they do so inadvertently. The good and honest of the world are always the most impotent.\n\nIt suddenly occurs to you that Gaunt hasn't a chance of surviving here. He is too intellectual to vie with the others for the Heart.";

        Choices.clear();
        Choices.push_back(Choice::Base("Tell him that", 279));
        Choices.push_back(Choice::Base("Ask what he thinks of the others", 258));
        Choices.push_back(Choice::Base("Return to the main square and seek out Kyle Boche", 104));
        Choices.push_back(Choice::Base("Turn in for the night", 192));

        Controls = Story::Controls::STANDARD;
    }
};

class Story149 : public Story::Base
{
public:
    Story149()
    {
        ID = 149;

        Text = "Slowly the chill leaves your bodies. \"Curious,\" mutters the baron, floating over to peer into the chasm. \"It seemed to be a genuine ghost, so far as I could tell. I could not read its mind, at any rate.\"\n\n\"Just as long as it's gone now,\" grunts Boche. He gestures at a narrow stone bridge spanning the chasm. On the far side, an archway opens onto a tiled hall. \"Who wants to go first?\"\n\nThe baron gives him a scornful look and hovers out across the drop. As you watch him drift to a halt on the far side, it occurs to you that he looks a rather ghostly figure himself.\n\nBoche glances at you, shrugs and strides rapidly across the bridge. \"It's not too bad as long as you don't look down,\" he calls over his shoulder.\n\nThey are waiting for you to cross. As you step onto the bridge, you find your gaze drawn downwards in spite of Boche's advice. The walls of the chasm plunge dizzyingly into feculent darkness. Then you notice something else. Set into the rock walls, about five metres below the level of the bridge, are a row of metal grilles.";

        Choices.clear();
        Choices.push_back(Choice::Base("Climb down to investigate", 193));
        Choices.push_back(Choice::Base("Continue across the bridge", 215));

        Controls = Story::Controls::STANDARD;
    }
};

class Story150 : public Story::Base
{
public:
    Story150()
    {
        ID = 150;

        Image = "images/heart-of-volent.png";

        Text = "You step out of the elevator to find Singh, Boche and Golgoth already here. Thadra Bey didn't make it.\n\nYou are in a dome so vast that it feels like a gulf in the black void of intergalactic space. Boche's flashlight does not reach the far wall. In the centre of the chamber, on a raised dais at the end of a ramp, lies a purple gemstone almost two metres in diameter. Scintillant sparks stream from deep in its core, giving off a violet radiation that causes a pain at the back of your eyes.\n\n\"The Heart of Volent...\" breathes Boche in a voice of awe. \"The key to ultimate power.\"";

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_CODEWORD(player, Codeword::Type::RED))
        {
            return 303;
        }
        else if (Character::VERIFY_CODEWORD(player, Codeword::Type::BLUE))
        {
            return 346;
        }
        else if (Character::VERIFY_CODEWORD(player, Codeword::Type::YELLOW))
        {
            return 324;
        }
        else
        {

            return 367;
        }
    }
};

class Story151 : public Story::Base
{
public:
    Story151()
    {
        ID = 151;

        Text = "Even with all your power you could not beat Baron Siriasis in a battle of will. He is the world's mightiest psychic, and because of desperation his strength is double. But you do not have to beat him, you only have to hold him off while he dies expending the last store of oxygen in his disembodied brain. Slowly you begin to feel his efforts weaken, his invading tentacles of thought slipping from your mind.\n\nThe brain suddenly drops out of the air, hitting the floor with a wet splat. There is a groan that could not be heard with the ears, and a last telepathic message: The darkness...\n\nSilence. The baron is dead at last.";

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 261; }
};

class Story152 : public Story::Base
{
public:
    Story152()
    {
        ID = 152;

        Text = "Who is the most trustworthy person here? You can rely on your psychic sense to guide you. Vajra Singh is an honourable man, but you cannot believe he would ever relinquish the chance for ultimate power. Chaim Golgoth is motivated by duty to his nation -- at least on the surface. Thadra Bey would never ally herself with others; she is as proud and independent as a cat. Janus Gaunt strikes you as free of malice; a reflective man, he could even be virtuous if he were of stronger character. Kyle Boche is vain, pompous and self-serving. And as for Baron Siriasis -- his mind is closed to you entirely.\n\nPerhaps you would be better to ask yourself who is least untrustworthy.";

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 82; }
};

class Story153 : public Story::Base
{
public:
    Story153()
    {
        ID = 153;

        Text = "If you touch the Heart, you will attune its power and create a new universe in which you wield the power of a god. But in doing so you would wipe out this universe and everything in it. You stand and gaze into the flickering facets. You can feel the palpable power within it. Can you resist its lure?";

        Choices.clear();
        Choices.push_back(Choice::Base("Release the Heart's power", 174));
        Choices.push_back(Choice::Base("Prevent the Heart ever falling into anyone's hands (STASIS BOMB)", 197, {Item::STASIS_BOMB}));
        Choices.push_back(Choice::Base("Reject the chance for power", 454));

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        if (Character::VERIFY_CODEWORD(player, Codeword::Type::FOCUS))
        {
            Choices[2].Destination = 354;
        }
        else
        {
            Choices[2].Destination = 454;
        }
    }
};

class Story154 : public Story::Base
{
public:
    Story154()
    {
        ID = 154;

        Text = "Gargan XIII follow Golgoth's gaze to her leg, where there is a razor-thin cut through the trouser fabric. She pulls it apart to reveal a scratch on the skin. Golgoth holds up a small needle he had hidden in the palm of his hand.\n\nYou see now that Gargan XIV has a similar scratch on her forearm. \"Cyanide,\" explains Golgoth. \"Should take about five seconds now... four... three...\"\n\nThe Gargan sisters exchange a look. There is no time for words. Suffering identical stabs of pain, they crumple to the floor. By the time you feel for a pulse, they are already dead. \"They went two seconds sooner than I thought,\" says Golgoth in a curious tone. \"Must've been their faster metabolism. Well, let's see what they've got.\"\n\nStripping the bodies of equipment, you find two BARYSAL GUNs (each with three charges), a FLASHLIGHT, a MEDICAL KIT, a STUN GRENADE, and three FOOD PACKs. Golgoth offers you the choice of any four items you like.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Take = {Item::MAKE_BARYSAL_GUN(3), Item::MAKE_BARYSAL_GUN(3), Item::FLASHLIGHT, Item::MEDICAL_KIT, Item::STUN_GRENADE, Item::FOOD_PACK, Item::FOOD_PACK, Item::FOOD_PACK};

        Limit = 4;
    }

    int Continue(Character::Base &player) { return 176; }
};

class Story155 : public Story::Base
{
public:
    Story155()
    {
        ID = 155;

        Text = "\"Why won't you see sense?\" asks Boche in an affable tone. \"Two can travel more safely than one. The road is plagued by robbers.\"\n\nYou find Boche's sincerity to be as false as a serpent's smile, and you have no desire to wake up one morning to find he has made off with your money and provisions. \"For all I know, you are the robber,\" you tell him to his face.\n\nBefore he can come up with an answer to this, you slog off through the snow. Now you must decide your route.";

        Choices.clear();
        Choices.push_back(Choice::Base("Head east to Venis, where you might be able to get passage to Kahira", 139));
        Choices.push_back(Choice::Base("Follow the innkeeper's advice and go west through the Lyonesse jungle", 221));

        Controls = Story::Controls::STANDARD;
    }
};

class Story156 : public Story::Base
{
public:
    Story156()
    {
        ID = 156;

        Text = "Grimacing, you cast the tiny gorgons into the flames. They produce a high-pitched whine which continues long after they must have burned to death - presumably the result of air escaping from their breathing cavities.\n\n\"They might have been the only members of a new and unique species!\" protests Boche. \"Are you proud to have committed genocide?\"\n\n\"Since each would have grown up to become a murdering monster, yes I am,\" you reply. You are increasingly beginning to suspect that Boche is a fool. Refusing to discuss the matter further, you search the cave.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 178; }
};

class Story157 : public Story::Base
{
public:
    Story157()
    {
        ID = 157;

        Text = "Others are also interested in the story of the Heart. A black-caped guide, overhearing your enquiries, follows you down the street saying, \"You're not the first to pass through Venis bound for Du-En. Last week there was Janus Gaunt, hiring guides for a trip across the Saharan Ice Wastes. And only this morning I took a woman of al-Lat to see Malengin the Gene Genie. Thadra Bey was her name. A veritable Amazon already -- now, thanks to Malengin's potions, she is more than human.\"\n\nIntriguing. If what the guide says is true, the hunt for Du-En has become a race. Deep in thought, you hardly notice the guide demanding that you pay for information he's given you.";

        Choices.clear();
        Choices.push_back(Choice::Base("Pay him (1 scad)", 414, Choice::Type::LOSE_MONEY, 1));
        Choices.push_back(Choice::Base("Do not pay him", 414));

        Controls = Story::Controls::STANDARD;
    }
};

class Story158 : public Story::Base
{
public:
    Story158()
    {
        ID = 158;

        Text = "By dint of tremendous mental effort, you find that you are able to move the steam cloud into the centre of the room, leaving cooler air where you are sitting. You concentrate on shaping the steam into a funnel, and gradually a miniature whirlwind forms in the air in front of you. As it twists faster and faster, it also becomes narrower. With your mind, you shape it like a clay pot on a wheel.\n\nOne of the assassins glances in through the glass. \"What the devil's going on?\" he shouts. They start to fumble with the latch, but you hardly notice. All your concentration is now on the hissing funnel of superheated steam.\n\nThe door opens. The two men stand staring in disbelief, knives slack in their hands. Mentally you cast the lance of steam towards them. One gives a scream as he is struck full in the face, and falls clutching his eyes. The other, although scaled, staggers in and tries to slice your belly open with his knife. Luckily he slips on the wet floor, an you take only a glancing blow.\n\nYou LOSE 1 Life Point.";

        Bye = "You close to grapple with him and a brief struggle ensues, ending when your attacker is impaled on his own knife. You can tell straight away that this one is dead, but the other man is still lying in the doorway whimpering.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::GAIN_LIFE(player, -1);
    }

    int Continue(Character::Base &player) { return 48; }
};

class Story159 : public Story::Base
{
public:
    Story159()
    {
        ID = 159;

        Text = "The next day, you glide the Manta southwards out of town and across the icy flats descending towards the Inland Sea. Flurries of snow gust out of a dull cloud-heavy sky. The headland to your left looks like a streak of tarnished silver over the iron-coloured waves.\n\nOnce clear of the coastline, you take the Manta up to an altitude of ten metres and open the throttle. The wind comes shrieking around the cockpit screen, but you are sheltered behind the controls. There are even heating vents to either side of the dashboard that give you a modicum of comfort.\n\nThe sea skims by beneath, grey as gunmetal and churning with chunks of ice. The sky resembles the underside of a giant fungus. Hours pass. As you approach the estuary of Isis, a haze of mist looms up to blanket the coastline, formed where the river flows into the freezing depths of the Inland Sea. Warmed by submerged pipes, the waters of the Isis teem with life. The heat is soon lost when the river enters the Inland Sea, but the estuary can support several fishing villages.\n\nThe coast hurtles closer. Now you can see boats scattered on the silvery water. Startled fishermen look up in fright as you go screeching past only metres above their heads. You laugh. To them your vehicle must look like some kind of demonic flying fish.";

        Choices.clear();
        Choices.push_back(Choice::Base("Steer a course to Kahira", 247));
        Choices.push_back(Choice::Base("Bypass kahira and visit the nearby pyramids of Giza", 268));
        Choices.push_back(Choice::Base("Head straight for Du-En", 289));

        Controls = Story::Controls::STANDARD;
    }
};

class Story160 : public Story::Base
{
public:
    Story160()
    {
        ID = 160;

        Text = "The Society's books add little to what you already knew. The Heart of Volent fell from space, a meteorite resembling a violet gemstone two metres across. It became revered by a religious cult calling themselves the Volentine Watchers, led by one Eleazar Picard. Learning how to tap the Heart's power, the cultists reversed the degradation of Earth's environment in a small region around their city of Du-En, in the Sahara, and for a time enjoyed prosperity.\n\nThen came the Paradox War. The Volentine Watchers used their strange power to send blasts of chaos-inducing radiation against the nations of the world, whom they deemed corrupt and decadent. The new Ice Age was already under way because of Gaia's instability, and the Paradox War accelerated the process. Some regions became so irradiated that the normal laws of nature no longer applied, and they were quickly overrun by eerie mutants.\n\nThe rest of the world allied to establish a military bunker at Giza, intending to strike from there against Du-En, but it was never needed. The people of Du-En succumbed to mass insanity and fled their city, only to perish in the Sahara as the snows returned. Eleazar Picard himself was found by a patrol from al-Lat, but only rambled incoherently for a few days before dying. Du-En was declared off limits, the Sahara an icy wasteland inhabited only by ghosts. And so it has remained for two hundred years.\n\nYou return the books to the shelves.";

        Choices.clear();
        Choices.push_back(Choice::Base("Try making contact with Gaia", 138));
        Choices.push_back(Choice::Base("You've finished in the library", 73));

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        if (Character::VERIFY_SKILL(player, Skill::Type::CYBERNETICS))
        {
            Choices[0].Destination = 116;
        }
        else
        {
            Choices[0].Destination = 138;
        }
    }
};

class Story161 : public Story::Base
{
public:
    std::string PreText = "";

    Story161()
    {
        ID = 161;

        Bye = "You give a hoarse grunt of relief through frost-numbed limbs when the towers and cupolas of Venis finally appear against the skyline.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        PreText = "You trek wearily onwards, often plunging almost to your waist through fine powdery banks of snow. The sun pokes feeble rays of light across the bleak sky like an old man clutching for his pills. Quicksilver ribbons lie across the landscape, marking out the course of glaciers through the ridges of rock. Night descends like a sheath of hoarfrost. For days your ordeal continues as you cross the rugged mountain slops and finally begin your descent towards the foothills.";

        auto DAMAGE = -3;

        if (Character::VERIFY_SKILL(player, Skill::Type::SURVIVAL))
        {
            DAMAGE = -2;
        }

        PreText += "\n\nYou LOSE " + std::to_string(-DAMAGE) + " Life Points.";

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_SKILL(player, Skill::Type::CLOSE_COMBAT) && Character::VERIFY_ITEMS(player, {Item::Type::SHORT_SWORD}))
        {
            return 8;
        }
        else
        {
            return 334;
        }
    }
};

class Story162 : public Story::Base
{
public:
    std::string PreText = "";

    Story162()
    {
        ID = 162;

        Choices.clear();
        Choices.push_back(Choice::Base("Rest here", 184));
        Choices.push_back(Choice::Base("Press on further into the jungle", 250));

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        PreText = "With each step you take you are getting weaker.\n\nYou LOSE 1 Life Point.";

        Character::GAIN_LIFE(player, -1);

        if (player.Life > 0)
        {
            PreText += "\n\nThe blood is still pouring freely from the jagged rip in your shoulder, and you realize that the creature must have injected an anti-clotting agent. Hurriedly tearing the lining of your jacket into strips, you bind the wound to prevent further blood loss.\n\nLooking back, you see no sign of your pursuer. Perhaps it lost interest after having a taste of your flesh.";
        }

        Text = PreText.c_str();
    }
};

class Story163 : public Story::Base
{
public:
    Story163()
    {
        ID = 163;

        Text = "You are held at gunpoint while your arms are wrenched behind your back and securely tied. One of the men rummages through your belongings, then jerks his thumb contemptuously towards you. \"Are we keeping an extra mouth to feed, then, Snarvo?\"\n\nThe man with the knife sneers. \"Not unless you want a pet for yourself, mate.\"\n\n\"Hang on,\" says the third man as he tucks his gun away. He stands peering at you in the firelight, chewing his lip thoughtfully. \"I've seen your face before, haven't I?\"\n\n\"Yeah,\" you say sarcastically, \"at the President's last garden party in Dallas.\"\n\nThe others snigger at this, admiring your casual attitude in the face of death, but the man with the gun stands there glowering in silence. Suddenly he clicks his fingers. \"On a wanted poster in Daralbad, that's where it was. They've et a bounty of five hundred scads for you, my friend.\"\n\n\"I'm much in demand.\"\n\n\"Too right! Too right!\" He strides excited around the fire, then turns to his cronies. \"Well, what's it to be? Hang on here in the hope of another bear, or take this prize to Daralbad for the bounty?\"\n\nThey settle on cashing you in for the reward. You pretend to take the news sourly, then wait while they celebrate by drinking themselves into a stupor. Flexing your wrists, you soon work free of your bonds and creep quietly over to their supply packs. You remove three FOOD PACKs, a COLD-WEATHER SUIT, and some POLARIZED GOGGLES.";

        Bye = "Then you slip out into the snow.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Take = {Item::FOOD_PACK, Item::FOOD_PACK, Item::FOOD_PACK, Item::COLD_WEATHER_SUIT, Item::POLARIZED_GOGGLES};

        Limit = 5;
    }

    int Continue(Character::Base &player) { return 314; }
};

class Story164 : public Story::Base
{
public:
    Story164()
    {
        ID = 164;

        Text = "Gargan XIII heartily shakes your hand, Gargan XIV sets up the glasses and pours each of you a shot of vodka. You take the glass and sip, only to wince as the alcohol sears into your cut gums. A mist of blood tinges the vodka. The Gargan sisters down their own drinks without a murmur. \"You are a fine warrior!\" declares XIV with approval.\n\nDespite their declarations of friendship, you are careful not to drink much. It's possible that, having realized you are too tough to beat in a straight fight, they hope to ply you with vodka until you are helpless. Turning to the innkeeper, you ask, \"I'll take food and a room for the night. How much do I owe you?\"\n\nEyes rolling in fright at the two Amazons, he flaps a hand limply in the air and replies in a strained voice: \"No charge, since you are a friend of these fine ladies.\"\n\nYou recover a bit after a night's rest.\n\nYou RECOVER 1 Life Point.\n\nIn the morning, the innkeeper provides you with a FOOD PACK for the journey.";

        Choices.clear();
        Choices.push_back(Choice::Base("Travel with the Gargan sisters", 294));
        Choices.push_back(Choice::Base("Delay setting out until they have moved on", 273));

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::GAIN_LIFE(player, 1);

        Character::GET_ITEMS(player, {Item::FOOD_PACK});
    }
};

class Story165 : public Story::Base
{
public:
    Story165()
    {
        ID = 165;

        Text = "Choose where to turn in for the night.";

        Choices.clear();
        Choices.push_back(Choice::Base("Bed down in a park", 209));
        Choices.push_back(Choice::Base("A quiet back alley", 231));
        Choices.push_back(Choice::Base("An open plaza", 253));

        Controls = Story::Controls::STANDARD;
    }

    int Background(Character::Base &player)
    {
        if (Character::VERIFY_SKILL(player, Skill::Type::STREETWISE))
        {
            return 187;
        }
        else
        {
            return -1;
        }
    }
};

class Story166 : public Story::Base
{
public:
    Story166()
    {
        ID = 166;

        Text = "You steady yourself as if you were suffering a giddy spell. \"I... can't remember,\" you tell him. \"My mind's a blank. The accident...\"\n\nHe glances at the doctor, who nods and says: \"That isn't uncommon in a case like this. You'll find your memory patchy at first, but it will slowly come back to you.\"\n\nThe pilot seems satisfied. His brooding look is suddenly swept away by an open smile. \"I have forgotten my manners. I am Riza Baihaqi; this is Dr. Anwar Mujam.\"\n\nYou shake hands. \"Be careful to use your flesh-and-blood hand for delicate tasks,\" warns the doctor. \"The grip strength of your cyborg arm is enough to crush bone.\"\n\n\"I'll be careful,\" you reply with a strained laugh.\n\nRiza gestures towards the viewport where the crescent Earth hangs like a luminous pearl. \"You would of course be welcome to remain here as our guest, but I expect that you will be anxious to return to Earth?\"";

        Choices.clear();
        Choices.push_back(Choice::Base("Go straight back to earth", 275));
        Choices.push_back(Choice::Base("Tell Riza about your quest", 188));

        Controls = Story::Controls::STANDARD;
    }
};

class Story167 : public Story::Base
{
public:
    Story167()
    {
        ID = 167;

        Text = "There is an alphanumeric keypad set in a recess beside the door. You tap in the sequence H-U-M-B-A-B-A and hold your breath, hardly daring to hope that the antique circuitry still works. Then, with a soft whir, the door slides up and you are able to step into the interior of the Great Pyramid.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 233; }
};

class Story168 : public Story::Base
{
public:
    std::string PreText = "";

    Story168()
    {
        ID = 168;

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Type = Story::Type::NORMAL;

        PreText = "After some squirming, you manage to wedge yourself into the shaft and begin a slow ascent. You are in total darkness. Minutes crawl by, and the air grows stifling as you climb. At last with the sweat pouring off your body, you are on the verge of giving up when you detect a nimbus of grey light from just above. You struggle towards it, cramming your body around a twist in the shaft. A heady aroma hangs in the air here, sweet as greenwood.\n\nYou emerge into a larger space at the junction of several ducts. As your eyes adjust to the faint light trickling down from above, you make out a human shape in the gloom. He is hanging like a puppet, entangled in a thick mass of fleshy creepers dangling from the top of the shaft. You approach and touch his shoulder. His head lolls back slowly, falls off, and strikes the floor with a hollow clatter.\n\nYou cannot stifle a gasp of horror. You step back, only to find a taut vine cable wound around your ankle. Another brushes your face, gropes with abrupt vitality, and seizes your throat in a firm grip. The creepers are alive and predatory. You struggle, grappling with the vine at your neck to no avail. It is tightening, squeezing your windpipe. Your pulse pounds inside your ears with a dull roar.";

        if (!Character::VERIFY_ITEMS(player, {Item::Type::VINE_KILLER}))
        {
            Type = Story::Type::DOOM;

            PreText += "\n\nThere is nothing you can do to free yourself and you will slowly strangle to death in the grip of the mutant plant.";
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 190; }
};

class Story169 : public Story::Base
{
public:
    Story169()
    {
        ID = 169;

        Text = "The air here is tolerable once filtered of volcanic fumes. You build a lean-to beside the bubbling pool in the shelter of the dwarf conifers. Although the water of the pool is undrinkable, you can easily collect snow from beyond the edge of the oasis and bring it back to camp to melt. For food, you catch insects and grubs and bake them on the hot rocks.\n\nYou RECOVER 4 Life Points.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::GAIN_LIFE(player, 4);
    }

    int Continue(Character::Base &player) { return 426; }
};

class Story170 : public Story::Base
{
public:
    Story170()
    {
        ID = 170;

        Text = "The baron is cloistered in his tent. A couple of servants are hunkered down in the snow outside next to a glimmering fire, but they pay you no heed. Glancing into the tent as you pass, you notice that the baron is intently studying a video screen which he has set up on a low table next to the cushions on which he is propped. He seems to be muttering something quietly to himself.";

        Choices.clear();
        Choices.push_back(Choice::Base("Eavesdrop", 364));
        Choices.push_back(Choice::Base("Boldly enter the tent uninvited", 386));
        Choices.push_back(Choice::Base("Go and talk to Golgoth", 126));
        Choices.push_back(Choice::Base("Talk to Boche", 104));
        Choices.push_back(Choice::Base("Talk to Gaunt", 148));

        Controls = Story::Controls::STANDARD;
    }
};

class Story171 : public Story::Base
{
public:
    Story171()
    {
        ID = 171;

        Text = "It is a beam phantom -- the victim of a teleporter accident which has left it eternally out of phase with the rest of the world. Such creatures suffer eternal torment because of the teleporter's mangling of their internal organs, but there is nothing that can be done for them, since they do not truly exist. Even in their ghostly state they are dangerous, however, as their desire to return to solid form means that they insatiably seek contact with the living. And their touch drains all life energy.\n\nWhile you still have strength to move, you shrug off your jacket and lunge forward holding it like a net, catching the beam phantom inside. The feel of it is sickening, like a broken body that has been put back together by a maniac surgeon. Even with the jacket insulating your grip, its negative force drains you like a well of coldness.\n\nYou LOSE 2 Life Points.";

        Bye = "You succeed in manhandling the squirming creature over to the chasm and hurling it down. Its thin bleating cry echoes up from the depths as it falls, trailing its gleaming wisps of vapour like a comet's tail.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::GAIN_LIFE(player, -2);
    }

    int Continue(Character::Base &player) { return 149; }
};

class Story172 : public Story::Base
{
public:
    Story172()
    {
        ID = 172;

        Text = "You hear a scuttling of many insectoid legs. In the sudden flare of light you catch a glimpse of a shape like a giant black centipede, mouthparts churning like oiled blades. Blinded by the unexpected light, it writhes and retreats in panic to the darkness of the cloisters. The three of you seize your chance to hurry on through the iron-bound door, slamming it behind you. A moment later a heavy form thuds against the door.\n\n\"That was a close call,\" breathes Boche, wiping a trickle of cold sweat out of his eyes.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 281; }
};

class Story173 : public Story::Base
{
public:
    Story173()
    {
        ID = 173;

        Image = "images/filler3.png";

        Text = "Misjudging the leap, you fall short by an arm's length and go plunging down through the fog to meet your doom on the hard flagstones twenty storeys below.";

        Type = Story::Type::DOOM;

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }
};

class Story174 : public Story::Base
{
public:
    Story174()
    {
        ID = 174;

        Text = "You kneel beside the huge gem and embrace it, feeling its power surge through you. Coruscating bands of energy blaze from the depths of the unearthly gem, swathing you in an aura of blinding violet light. The fabric of reality is ripped apart and you feel weightless. A vortex spins up through the dome, sweeping away rock and air, rising up into space out past the moon and planets. In what seems like seconds, all of creation has been swept away, replaced by a new universe of your own making.\n\nYou are immortal and all-powerful now. Your adventure has culminated in godhood.";

        Type = Story::Type::GODHOOD;

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }
};

class Story175 : public Story::Base
{
public:
    Story175()
    {
        ID = 175;

        Image = "images/laser-bots.png";

        Text = "There is a sound from somewhere ahead in the maze of tunnels. Boche cocks his hear. \"It's the old-fashioned laser fire,\" he says, adding significantly: \"But none of the others were carrying lasers.\"\n\nThe mystery is soon explained. Turning a corner, you find yourselves facing three hover-droids that resemble large silver eggs with a rotating gun-turret mounted underneath. They are gliding directly towards you, the aiming-lights of their lasers playing across your vision. Boche throws himself down a side tunnel. You are in motion an instant later, just as three laser beams lance out, burning a patch of molten stone on the floor where you had been standing.\n\nRacing headlong through the tunnels, you reach a circular room with a violet starburst set in mosaic on the floor. Several other tunnels lead into the chamber, and down one of them you see Vajra Singh, Thadra Bey and Chaim Golgoth running from another group of hover-droids. \"We stirred up a whole nest of them,\" shouts Golgoth. \"Too many to fight!\"\n\nAs he enters the room, Vajra Singh whirls and fires his mantramukta cannon over the heads of Bey and Golgoth. Four hover-droids explode in a blossom of plasma, but you can see others converging on the room from the other tunnels.\n\nAcross the sweep of the opposite wall are seven glass-fronted elevators, each large enough for one person. Thadra Bey lopes across to the nearest and the door slides shut behind her. In moments the room will be swarming with hover-droids. The rest of you have no choice but to follow her lead. As you step into an elevator, the door closes and an electronic voice says: \"Identify yourself, please.\"";

        Choices.clear();
        Choices.push_back(Choice::Base("Tell the truth", 280));
        Choices.push_back(Choice::Base("Pretend to be a follower of the Volentine cult", 301));

        Controls = Story::Controls::STANDARD;
    }
};

class Story176 : public Story::Base
{
public:
    Story176()
    {
        ID = 176;

        Text = "You return to the surface. Janus Gaunt and Thadra Bey are already back at the camp. They tell you they explored a warren of tunnels under the plaza but to no avail. \"All we found were vaults full of mildewed grain,\" growls Bey.\n\nThe long list of sunset is bathing the ruins by the time Vajra Singh and the baron reappear. You soon learn that they set out along different routes which converged at a long underground hall. They are obviously excited by what they discovered. \"We have identified the temple precincts,\" announces Vajra Singh, gesturing to a cluster of buildings across the square. \"The Heart of Volent lies somewhere below. Tomorrow may be the last day of the search.\"";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 38; }
};

class Story177 : public Story::Base
{
public:
    Story177()
    {
        ID = 177;

        Text = "Boche is not enthusiastic. \"The Lyonesse region is infested with malefactors and noctambules,\" he avers. \"We would be at great risk. Also, the Atlas Mountains are a daunting obstacle. As your partner in this venture, I strongly urge you to reconsider.\"\n\nHe is obviously not willing to accompany you if you insist on heading west.";

        Choices.clear();
        Choices.push_back(Choice::Base("Do so anyway", 221));
        Choices.push_back(Choice::Base("Change your mind and take the road to Venis", 200));

        Controls = Story::Controls::STANDARD;
    }
};

class Story178 : public Story::Base
{
public:
    Story178()
    {
        ID = 178;

        Text = "Strewn amid the mortal remains of the gorgon's victims, you discover a number of items: an ID CARD, a BATTERY UNIT for a BARYSAL GUN (good for six charges), a working FLASHLIGHT, and a set of POLARIZED GOGGLES.\n\nBoche holds up the goggles and jokes, \"I don't suppose our one-eyed chum had much use for these, eh?\" He insists on an equal division of the spoils, but gives you first choice.";

        Bye = "You spend a reasonably comfortable night in the cave and head on your way in the morning.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Take = {Item::ID_CARD, Item::BATTERY_UNIT, Item::FLASHLIGHT, Item::POLARIZED_GOGGLES};

        Limit = 2;
    }

    int Continue(Character::Base &player) { return 199; }
};

class Story179 : public Story::Base
{
public:
    Story179()
    {
        ID = 179;

        Text = "You find an answer to your enquiries in a drinking parlour under the Bridge of Sighs. Here a group of men, rendered affable and talkative by the vials of synthash liqueur they have imbibed, tell you all you need to know.\n\n\"Ky Boche?\" says one of them, scowling distractedly out into the drizzle beyond the eaves of the drinking parlour. \"I have heard of him. A wayfarer and sometimes trader.\"\n\n\"A trader?\" snorts another of the men, sluicing the hot liqueur around his mouth before adding, \"A parasite, rather. His sole instinct is treachery; his sole talent is self-preservation.\"\n\nYou turn to the man who has just spoken. \"I take it you've personally had dealings with Kyle Boche?\"\n\nHe nods slowly, narrowing his eyes as he peers inward at his memories through a haze of synthash fumes. \"We collaborated in a smuggling operation running furs into Daralbad. This was a few years back. The militia were alerted and Boche scarpered in the boat, leaving me to face the music alone. They're pigs in Daralbad, and that's how I lost these two fingers, see? Later I managed to escape, and I heard Boche had been strutting around telling everyone how I let him down. If I knew where to find him, I'd go this minute and put a knife through his weaselly heart!\"\n\nA third man joins the conversations. \"I do not wholly disagree with my friend here, except to add that Boche's character is more complex than he suggests. If he is treacherous or self-serving, he is not aware of being so, for he is a man of such prodigious vanity that he can admit no faults.\n\n\"The other man, refilling his glass, only hisses like an angry swan and says again, \"He is a mere parasite!\"\n\nIf you wish to reveal your knowledge of Boche's whereabouts to the man he betrayed, he will pay you 10 scads for the information.";

        Choices.clear();
        Choices.push_back(Choice::Base("Reveal Kyle Boche's whereabouts", -179, Choice::Type::GAIN_MONEY, 10));
        Choices.push_back(Choice::Base("Refuse the bribe", 414));

        Controls = Story::Controls::STANDARD;
    }
};

class Event179 : public Story::Base
{
public:
    Event179()
    {
        ID = -179;

        Choices.clear();

        Controls = Story::Controls::NONE;
    }

    int Background(Character::Base &player)
    {
        if (Character::VERIFY_CODEWORD(player, Codeword::Type::DIAMOND))
        {
            Character::REMOVE_CODEWORD(player, Codeword::Type::DIAMOND);
        }

        return 414;
    }
};

class Story180 : public Story::Base
{
public:
    std::string PreText = "";

    Story180()
    {
        ID = 180;

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::GAIN_LIFE(player, -2);

        PreText += "You LOSE 2 Life Points.";

        if (player.Life > 0)
        {
            PreText += "\n\nYou slump in the corner of the bench with barely strength to move. At last, thinking you helpless, the assassins open the steam room door. \"Let's get this carcass over to the baron -- prove we've done the job,\" one of them says.\n\n\"Rather a disappointment,\" says the other, grabbing a hunk of your hair and lifting your head up off the bench. \"I was hoping I'd get to use my knife.\"\n\nHearing this, you explode into action. You have no compunction about giving the nearest man a very hard kick in the groin. As he doubles up and his accomplice gapes in surprise, you lunge out of the door and race back towards the dormitory. Your nakedness draw a few choice looks from the inn's other residents, so you hastily pull on your clothes.\n\nA few minutes later, the assassins glance into the dormitory. With so many people around, they think better of pursuing you and instead duck out into the night. Just as well. After being shut up in the steam room, you feel too weak for a fight.";
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 92; }
};

class Story181 : public Story::Base
{
public:
    Story181()
    {
        ID = 181;

        Text = "Do you want to find Kyle Boche and tell him about acquiring the sky-car?";

        Choices.clear();
        Choices.push_back(Choice::Base("Yes", 203));
        Choices.push_back(Choice::Base("No", 159, Choice::Type::LOSE_CODEWORD, Codeword::Type::DIAMOND));

        Controls = Story::Controls::STANDARD;
    }
};

class Story182 : public Story::Base
{
public:
    Story182()
    {
        ID = 182;

        Text = "By luck you find the following reference:\n\nExtract form the interrogation of Eleazar Picard, leader and sole survivor of the Volentine Cult, July 16, 2113 AD; debriefing conducted by Colonel Mehmet Alishah of al-Lat:\n\nPicard: It was the light at the end of Time, and my sanity is blinded.\nAlishah: How is it you escaped? You alone and no one else?\nPicard: I remember the Truth.\nAlishah: What is the Truth?\nPicard [recites]: \"The Truth is a flame. What ignites the flame? The spark ignites the flame. What is the spark? The heart of Volent!\"\n\nYou flip through a few other books. Eleazar Picard was the founder of the cult that worshipped the Heart of Volent two hundred years go. He ruled Du-En with six others -- until the night of July 15, 2113, when the entire populace of the city went mad and fled into the Saharan Ice Wastes. Du-En has stood abandoned since then. You check one other detail. Picard died, still raving, only hours after being questioned by Colonel Alishah.\n\nYou gained the codeword LUNAR.";

        Choices.clear();
        Choices.push_back(Choice::Base("Try making contact with Gaia", 138));
        Choices.push_back(Choice::Base("You've finished in the library", 73));

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::GET_CODEWORDS(player, {Codeword::Type::LUNAR});

        if (Character::VERIFY_SKILL(player, Skill::Type::CYBERNETICS))
        {
            Choices[0].Destination = 116;
        }
        else
        {
            Choices[0].Destination = 138;
        }
    }
};

class Story183 : public Story::Base
{
public:
    Story183()
    {
        ID = 183;

        Text = "The signs you have noticed indicate a porphyr. These are parasitical creatures of ancient myth who roam the night in search of living victims, from whom they drain the body warmth (or, in some versions, the very lifeblood) in order to sustain their own existence. They can only be slain by decapitation, but are vulnerable to direct sunlight, running water, and certain extinct herbs.\n\nYou guess that this porphyr was overpowered and hurled into the glacier centuries ago as a way of getting rid of him. Even frozen running water has the power to hold him immobile. Of course, now that the ice has been chipped away, he might be able to escape. You glance up at the thin wedge of powder-blue sky beyond the crevasse, already streaked with dusky grey streamers of cloud. Less than two hours of daylight remain. You tell Shandor your suspicions.\n\n\"Porphyrs?\" he mocks. \"Fairy tales for kiddies, surely.\"\n\nYou aren't so sceptical. Fortunately you remember one other thing about these creatures. Their minds are quite simple, and they are particularly baffled by vertical or horizontal lines, which their eyes try to follow to infinity -- similar to the way a chicken can be hypnotized by drawing a chalk mark in front of its beak. You scratch two intersecting lines into the ice directly in front of the trapped porphyr's eyes, then step back with a satisfied nod. \"There it can't escape now.\"\n\nShandor chuckles and claps his hand on your shoulder. \"In case you hadn't noticed, it couldn't anyway: it's been dead two hundred years and it's trapped under thousands of tons of ice. Still, if it makes you feel safer.\" He turns to his bodyguards. \"Let's get going. We can do another six kilometres by nightfall.\"";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 249; }
};

class Story184 : public Story::Base
{
public:
    Story184()
    {
        ID = 184;

        Text = "At last you nestle down in the cleft of a dead tree and drift off to sleep. Strange feverish dreams flicker across the back of your eyelids. Moaning, bathed in torrents of sweat, you fling out an arm to brush away what you imagine to be a gnat. You are not aware of the long prow of reptilian head that hangs down out of the foliage and starts to sup on your blood. Its venom thunders through your bloodstream and keeps you drowsy wile it feasts. You are destined never to wake.";

        Type = Story::Type::DOOM;

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }
};

class Story185 : public Story::Base
{
public:
    std::string PreText = "";

    Story185()
    {
        ID = 185;

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        PreText = "They obviously mean to kill you, not out of viciousness but from the simple logic of survival. There probably is not enough game in the region to provide enough food for a fourth person. As they close in, you put on a crafty expression and glance over your shoulder, back the way you came.\n\n\"Got friends, have you?\" growls the man with the knife. He puts the tip to your throat. \"Answer.\"\n\n\"I'm alone.\"\n\n\"What, then? What were you looking at?\"\n\n\"Nothing.\" You compress your lips. The knife presses tighter, shedding a drop of your blood. \"All right! There's a storeroom down there -- food, medical supplies, that sort of thing. I was just regretting that I'll never get back to civilization and sell it.\"\n\nThey peer into the rubble-clogged tunnel, a sheen of greed in their eyes. \"You're lying,\" suggests the man with the gun.\n\nYou shrug. \"Yeah. I was just prowling the tunnels for the sake of sightseeing. Of course.\"\n\nThe third man scratches his chin thoughtfully. \"We ought to take a look. This joker didn't just spring out of thin air, after all.\" He looks at the man with the knife. \"Snarvo, you stay here and watch our... guest. Krench, you come with me.\"\n\nThey tie you up, then depart in search of non-existent booty. You wait until they must be out of earshot in the tunnels below. Snarvo is facing down the tunnel. He thinks you helpless, but in fact you have been burning the rope away from your wrists over the crackling fire. It is a painful business, but better than certain death.\n\nYou LOSE 1 Life Point.";

        Character::GAIN_LIFE(player, -1);

        if (player.Life > 0)
        {
            PreText += "\n\nSuddenly you cannon into Snarvo, winding him with a butt in the stomach. Wrestling for the knife, you twist it around and stab him. As he slumps gasping to the floor, you snatch up the knife and run over to the opening. Outside a blizzard is raging, but you cannot wait for the others to return. You push the rug aside and dash out into the storm.";
        }

        Text = PreText.c_str();

        Take = {Item::KNIFE};

        Limit = 1;
    }

    int Continue(Character::Base &player) { return 314; }
};

class Story186 : public Story::Base
{
public:
    std::string PreText = "";

    Story186()
    {
        ID = 186;

        Bye = "You finally manage to swing one of the twins off her feet and hurl her at the other with such force that both are knocked senseless. You are not in much better shape yourself.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        PreText = "Again the battle is joined. This time you and your two opponents have the measure of each other. The twins circle warily, trying to divide your attention. You plant your back solidly to the bar and wait with arms spread wide, daring them to attack. The extravagant acrobatics have been set aside now. All three of you are too breathless to waste effort on high flying kicks or fast barrages of punching.\n\nOne lunges forward, clutching at your neck. You counter, pushing her arm up as you drive stiffened fingers into the corded muscles of her throat. As she splutters, briefly limp, you swing her around, using her as a shield that her sister's sudden attack catches her in the back. But she recovers faster than you expected, and her knee drives up agonizingly into your groin.";

        auto DAMAGE = -3;

        DAMAGE = Character::COMBAT_DAMAGE(player, DAMAGE);

        PreText += "\n\nYou LOSE " + std::to_string(-DAMAGE) + " Life Points.";

        if (player.Life > 0)
        {
            Character::GET_CODEWORDS(player, {Codeword::Type::SCYTHE});

            PreText += "\n\nYou gained the codeword SCYTHE.";
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 314; }
};

class Story187 : public Story::Base
{
public:
    Story187()
    {
        ID = 187;

        Text = "You recognize the signs that mark some areas of the city as unsafe: the hard sidelong scrutiny of the locals, the ragged evidence of poverty, the nervous hurrying footsteps in the fog. You head for a well-lit plaza where people are likely to be working all night.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 253; }
};

class Story188 : public Story::Base
{
public:
    Story188()
    {
        ID = 188;

        Text = "You explain what you have learned regarding Du-En and the Heat of Volent. \"It seems far-fetched,\" says Riza when he has heard you out.\n\n\"You look down dourly at the fusion of metal and flesh that is now your body. \"Before my accident, it seemed a noble but distant goal. Now it has taken on a personal dimension. With the power of the Heart, I can restore myself.\"\n\n\"And much more, if Gaia is to be believed,\" says Riza. He takes you through to another room and sits you in front of a screen. \"Here you have access to the information banks. Perhaps you can find out something that will help in your venture.\"\n\nYou watch as he touches a few buttons. A list of files concerning the Heart of Volent appears on the screen. \"I think I can take it from here,\" you tell Riza.\n\nHe nods. \"Good. I'll come back in an hour or so and see how your work is going.\"";

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Choices.clear();

        if (!Character::VERIFY_SKILL(player, Skill::Type::LORE))
        {
            Choices.push_back(Choice::Base("Check through the files on the screen", 210));
            Choices.push_back(Choice::Base("Go for a stroll and see what else you can find out here", 254));
        }
    }

    int Continue(Character::Base &player) { return 232; }
};

class Story189 : public Story::Base
{
public:
    Story189()
    {
        ID = 189;

        Text = "Glancing around for a means of opening the door, you notice an alphanumeric keypad set in a recess to one side. As you are cleaning away the crust of snow, it strikes you that a hundred years might have passed since this door last opened. There are billions of possible combinations, but whoever used it then would have known the correct sequence to punch into the keypad.\n\nYou close your eyes, focusing with that inward mental sense which transcends ordinary reality. The veil of time draws aside. For a moment you glimpse a figure in the military dress of an earlier era, stabbing impatiently at this very keypad.\n\nThe image flickers, breaks apart like a projection on smoke. With bated breath you enter the same sequence you thought you saw the soldier use: H-U-M-B-A-B-A. There is a crackling of ice as the door slides up to admit you.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 233; }
};

class Story190 : public Story::Base
{
public:
    Story190()
    {
        ID = 190;

        Text = "A haze begins to close over your vision. You haul out the canister, point it up into the mass of squirming roots above your head, and press the nozzle. An acrid fluid sprays out, soaking the plant. Some of it covers your fingers, causing the canister to slip out of your grasp. Ignoring the slight stinging sensation, you jab your hand against the root holding your throat. With a hushed and eerie rustling sound, the creepers pull away and shrivel up the shaft. You steady yourself against the wall and gratefully suck in lungfuls of the stale vine-scented air.\n\nThe canister fell by your feet. As you retrieve it, you notice something glinting in the half-light. A BARYSAL GUN. You glance at the long-dead cadaver. The gun didn't do him much good, but you might find a use for it. It has just one charge remaining.\n\nYou can see now that there is no way you can hope to reach the surface form here. The shaft is blocked by the dead plant. You wriggle back down the shaft and take the subway carriage back to Marsay. Fax has long gone, of course.";

        Choices.clear();
        Choices.push_back(Choice::Base("Go up to the ruins and continue your journey on foot", 420));
        Choices.push_back(Choice::Base("Try one of the subway's other destinations: choose between Kahira", 50));
        Choices.push_back(Choice::Base("... Tarabul", 31));
        Choices.push_back(Choice::Base("... or Giza ", 74));

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Take = {Item::MAKE_BARYSAL_GUN(1)};

        Limit = 1;
    }
};

class Story191 : public Story::Base
{
public:
    Story191()
    {
        ID = 191;

        Text = "The gates of Du-En stand open to the elements, leaving the wide avenues piled under deep snow-dunes. The buildings are monoliths of dark stone, desolate remnants of a civilization long gone. Their vast scale and pitiless geometric decoration make them seem out of proportion to the human soul. You find them tyrannical and depressing.\n\n\"Impressive architecture, eh?\" says Boche, looking to a high black tower whose fretwork dome shows like a fleshless head against the bleak white sky.\n\nYou see a thin swirl of smoke rising from a campfire in the main plaza of the city. \"It seems we're not the only ones to heed Gaia's message,\" you say dourly. \"Come on, let's introduce ourselves.\"";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 243; }
};

class Story192 : public Story::Base
{
public:
    std::string PreText = "";

    Story192()
    {
        ID = 192;

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        PreText = "You return to your campsite in the shelter of a ruined building and settle down for the night. The flames of the fire throw long capering shadows across the walls. The ground is so cold that it seems to suck the vitality out of you.\n\nHow long has it been since you ate?\n\n";

        auto LIFE = 0;

        if (Character::CHECK_VEHICLE(player, Vehicle::Type::MANTA_SKY_CAR))
        {
            LIFE = 2;

            PreText += "[Vehicle: " + std::string(Vehicle::Descriptions[player.Vehicle]) + "] You found a MEDICAL KIT and several FOOD PACKs in the storage locker. ";
        }
        else
        {
            if (Character::VERIFY_ITEMS(player, {Item::Type::FOOD_PACK}))
            {
                LIFE++;

                PreText += "[FOOD PACK] ";
            }

            if (Character::VERIFY_ITEMS(player, {Item::Type::MEDICAL_KIT}))
            {
                LIFE++;

                PreText += "[MEDICAL KIT] ";
            }
        }

        if (Character::VERIFY_CODEWORD(player, Codeword::Type::HOURGLASS))
        {
            LIFE--;

            PreText += "[Codeword: HOURGLASS] ";
        }

        if (LIFE > 0)
        {
            PreText += "You RECOVER " + std::to_string(LIFE) + " Life Point(s).";
        }
        else if (LIFE < 0)
        {
            PreText += "You LOSE " + std::to_string(-LIFE) + " Life Point(s).";
        }
        else
        {
            PreText += "You managed to remain well enough not to lose any Life Points.";
        }

        Character::GAIN_LIFE(player, LIFE);

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 83; }
};

class Story193 : public Story::Base
{
public:
    Story193()
    {
        ID = 193;

        Text = "Your footing slips on an icy outcropping as you are making your way down and, having no wish to fall to your doom in the bowels of the earth, you scramble back up onto the bridge.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Background(Character::Base &player)
    {
        if (Character::VERIFY_SKILL_ALL(player, Skill::Type::AGILITY, {Item::Type::ROPE}))
        {
            return 237;
        }
        else
        {
            return -1;
        }
    }

    int Continue(Character::Base &player) { return 215; }
};

class Story194 : public Story::Base
{
public:
    Story194()
    {
        ID = 194;

        Text = "Your sixth sense, while not as sensitive as the baron's, warns you of something alien and predatory moving in for the kill. You hear its insectoid legs rattling on the marble floor. Diving to one side, you feel a stab of pain lance up your leg. Without the forewarning of your [ESP] you would have been torn in half by the monster's mandibles.\n\nYou LOSE 1 Life Point.";

        Bye = "You scramble to the doorway where you now see the flash of Boche's torch. As you throw yourself through, the baron slams the door shut and Boche drops the bolt. Just in time. A second later and the door gives a jolt as something massive slams against the other side.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::GAIN_LIFE(player, -1);
    }

    int Continue(Character::Base &player) { return 281; }
};

class Story195 : public Story::Base
{
public:
    Story195()
    {
        ID = 195;

        Text = "You retreat as far as the room where Novak was frozen in stasis. The baron's brain glides closer. The telepathic messages are getting scrambled and incoherent now, as the thing slowly uses up its remaining oxygen.\n\n(The soldier's dead. Can't risk using Boche... Not until I know... How did he hide grenade from me? Need new body... new life. Yours..)";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_ITEMS(player, {Item::Type::STUN_GRENADE}))
        {
            return 239;
        }
        else
        {
            return 85;
        }
    }
};

class Story196 : public Story::Base
{
public:
    Story196()
    {
        ID = 196;

        Text = "You manage to jump across catch hold of a girder that projects from the roof of the building opposite. You scramble up over the parapet and run off while the Fijian shouts out threats behind you.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 311; }
};

class Story197 : public Story::Base
{
public:
    Story197()
    {
        ID = 197;

        Text = "You fetch Vajra Singh's cannon and begin to power it up. A hum of colossal energy fills the air. Taking the STASIS BOMB from your pack, you station yourself beside the Heart. You will freeze yourself in time, so that if any others reach here in the years to come they will find you waiting, sealed along with the Heart inside a zone of stasis. Even if they have the means to break the stasis, they will have to get past you to claim the Heart.\n\nThe STASIS BOMB activates turning you into an undying sentinel for all time.";

        Type = Story::Type::SENTINEL;

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }
};

class Story198 : public Story::Base
{
public:
    Story198()
    {
        ID = 198;

        Text = "You had not expected Gilgamesh to be able to follow through the narrow ventilation ducts, but now there is a metallic clanking and he climbs out into the corridor. In order to fit in the shaft, he has detached his right arm and most of his armour segments, so that he now looks like a bizarre metal skeleton.\n\n\"This reconfiguration lacks a long-term power source,\" he tells you. \"I have enough power stored for three hours, then I must reintegrate with my other components.\"\n\n\"Aren't you more vulnerable without your armour?\"\"Affirmative. I shall avoid direct danger except when your own life is threatened.\"\n\n\"The automaton looks less impressive when it's undressed,\" you overhear Gargan XIII remark to her sister.\n\n\"Probably just like its owner,\" says Gargan XIV with a sneer.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 325; }
};

class Story199 : public Story::Base
{
public:
    Story199()
    {
        ID = 199;

        Text = "Dusk is falling on the tenth day after leaving the Etruscan Inn when you finally come in sight of Venis. It shimmers with a thousand lights under a sky like dull green bronze. Hungry and cold, you quicken your pace until you can make out individual buildings -- first the temporary shacks where hunters and traders dwell, then the slums of corrugated iron and plastic which fill the narrow streets that some say were once canals. Above them loom the blocks of ancient plazas, where the rich and powerful of the city reside in palatial buildings shored up with wooden scaffolding to support them from the ravages of time.\n\nYou soon learn that the ferry to Kahira is not due for a couple of days. Kyle Boche tells you that he has friends he must visit, and arranges to meet up with you when the ferry arrives. While waiting, you have a choice of where to take lodging. The lavish Marco Polo Hotel will charge 12 scads for two nights; the Hotel Paradise will charge 6 scads; the disreputable Doge's Inn will cost only 3 scads.";

        Choices.clear();
        Choices.push_back(Choice::Base("Go to the Marco Polo", 286, Choice::Type::LOSE_MONEY, 12));
        Choices.push_back(Choice::Base("Opt for Paradise", 244, Choice::Type::LOSE_MONEY, 6));
        Choices.push_back(Choice::Base("Check in at the Doge's Inn", 371, Choice::Type::LOSE_MONEY, 3));

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        if (Character::VERIFY_ITEMS(player, {Item::Type::ID_CARD}))
        {
            Choices[0].Destination = 222;
        }
        else
        {
            Choices[0].Destination = 286;
        }
    }
};

class Story200 : public Story::Base
{
public:
    Story200()
    {
        ID = 200;

        Image = "images/statues.png";

        Text = "Your journey takes you up into the mountains, where the days are dull under a leaden sky and the nights are filled with swirling snow. You subsist on a few rations brought with you from the inn, but these are quickly used up. Too quickly. You must reach an inhabited area soon or else starve.\n\nForcing your way bent-backed against a glacial wind, you are traversing a narrow pass when you catch sight of a human figure on a ledge up ahead. Your cries of greeting are ignored, and the figure is hidden for a moment behind a veil of snow. Hurrying forward, you discover several other figures, but none are glad to see you. They are beyond any emotion, in fact, being long dead and frozen into rigid statues by the cold.\n\nYou gained the codeword DIAMOND.";

        Choices.clear();
        Choices.push_back(Choice::Base("Go closer to investigate", 265));
        Choices.push_back(Choice::Base("Ignore the frozen corpses and continue along the pass", 285));

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::GET_CODEWORDS(player, {Codeword::Type::DIAMOND});
    }
};

class Story201 : public Story::Base
{
public:
    std::string PreText = "";

    Story201()
    {
        ID = 201;

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Type = Story::Type::NORMAL;

        PreText = "A sudden sense of danger warns you not to proceed and you slip out of the shop.";

        if (!Character::VERIFY_SKILL(player, Skill::Type::ESP))
        {
            Type = Story::Type::DOOM;

            PreText = "You blithely submit to the anaesthetic and your consciousness slides away, never to return. You should have been more wary than to put yourself at the mercy of such street scum.";
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 223; }
};

class Story202 : public Story::Base
{
public:
    Story202()
    {
        ID = 202;

        Text = "The queue shuffles along slowly and at last you reach the ticket office. A bored official in the livery of a Venisian merchant guild peers at you through a screen of wire-meshed glass. \"The cost of passage as far as Kahira is ten scads,\" he tells you.";

        Choices.clear();
        Choices.push_back(Choice::Base("Pay this sum (10 scads)", 246, Choice::Type::LOSE_MONEY, 10));
        Choices.push_back(Choice::Base("You refuse or cannot afford to pay", 288));

        Controls = Story::Controls::STANDARD;
    }
};

class Story203 : public Story::Base
{
public:
    Story203()
    {
        ID = 203;

        Text = "Boche has a broad grin on his face as he looks over the sky-car and then turns to you, saying, \"This is a startling and welcome piece of luck! Now that we have a Manta, success in our venture is virtually assured.\"\n\n\"Our venture?\" You raise your eyebrows. \"So you did overhear Gaia's message back at the Etruscan Inn.\"\n\nHe claps you heartily on the back. \"You know I did, but what of it? We are partners, and shall share the power of the Heart equally. Now that we have this vehicle, there is no need to wait for the ferry to Kahira. We can set out directly for Du-En.\"";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 159; }
};

class Story204 : public Story::Base
{
public:
    Story204()
    {
        ID = 204;

        Text = "You take the elevator to the floor indicated but find only the dim after-hours lights illuminating the corridor. As you are looking around, a janitor steps out of an office with pail and broom in hand and nods to you respectfully. \"If you are looking for Doctor Jaffe, she has gone home for the night. The surgery opens again at nine AM.\"";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 73; }
};

class Story205 : public Story::Base
{
public:
    Story205()
    {
        ID = 205;

        Text = "Shandor's two remaining bodyguards produce small picks with which they hack at the ice. They are strong men and throw themselves into the job tirelessly, but it is soon obvious that the ice is too hard-packed. They remove their fur jackets despite the cold and one of them, turning to Shandor and wiping beads of sweat out of his eyes, says, \"It's going to take us hours to get the body out, boss.\"\n\nShandor glances at you, then shrugs. \"Just chip the ice away around the hand, then. Get the sword.\"\n\nIt is almost an hour before they have achieved this. By now the wind has dropped, making just a low ghastly moan as it gusts along the gully leading down to the cave. Nightfall cannot be more than a couple of hours off. You go over with Shandor and peer at the ice wall. Where the picks have gouged down to the corpse's hand, the clear blue ice has become cloudy with white cracks. The hand hangs limply, the shortsword still clutched in its fingers. Shandor goes to take it, hesitates, turns to you with a grimace of distance. \"I'm not easily spooked, but... \" He gives an embarrassed chuckle.\n\nThe shortsword is of Japanese style, its blade still razor sharp in spite of a faint speckling of rust. You can take it if you wish; if not one of the bodyguards will claim it.\n\nYou notice something odd about the corpse's hand. The slight violet bloom to the skin could be attributed to its preservation in ice, but the long index finger and the hair on the palm are not so easy to explain.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Take = {Item::SHORT_SWORD};

        Limit = 1;
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_SKILL(player, Skill::Type::LORE))
        {
            return 183;
        }
        else
        {
            return 270;
        }
    }
};

class Story206 : public Story::Base
{
public:
    Story206()
    {
        ID = 206;

        Text = "The creature that attacked you is a sanguivore. A species of tree-dwelling lizard which is halfway to evolving flight, this is just one of the hallucinatory by-products of the Paradox War. From the way your wound is pouring blood, you suspect the sanguivore's saliva contained an anti-clotting agent. That alone would not explain your increasing dizziness, though, which suggests its bite also injected a drug. No wonder it's in no hurry to catch up with you -- easier by far to track you through the woods and wait until you collapse. Well, you are not as helpless as the wild animals on which the sanguivore normally preys. Tearing the lining of your jacket into strips, you bind the wound to prevent further blood loss.\n\nThe only thing you can do about the drug is stay on the move until you find shelter. If you were to lie down now you would never get up again.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 250; }
};

class Story207 : public Story::Base
{
public:
    Story207()
    {
        ID = 207;

        Text = "You start to move forwards, begin a lunge towards the man with the gun, the jump adroitly aside as he discharges a blast. The blue-white flash snaps through the air and explodes against the knife-wielder's arm. He drops with a whimper. The third hunter runs in, arms spread wide to grab you, but you duck under his clumsy tackle and dart towards the rug covering the door. Another shot crackles from the gun, and again you veer away, but this time it glances across your hip, burning the clothing away and inflicting a painful burn.\n\nYou LOSE 1 Life Point.";

        Bye = "You are lucky to be alive. Wasting no more time, you throw yourself through the opening and limp off into the blizzard. The hunters do not give chase.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::GAIN_LIFE(player, -1);
    }

    int Continue(Character::Base &player) { return 314; }
};

class Story208 : public Story::Base
{
public:
    Story208()
    {
        ID = 208;

        Text = "Placing your chair behind a heavy oaken post, you drape your coat across the back and prop your travelling-pack under it. If the sisters were to glance in your direction, they would assume you were still brooding at your table. In fact they seem entirely preoccupied, leaning on the bar and muttering to one another in terse whispers. You slide through the shadows to the far end of the room, slip unnoticed over the bar, then creep back on hands and knees until you are hidden just below where the sisters are talking. The innkeeper notices you, but luckily he is too terrified to make any comment.\n\n\"We'll need a BARYSAL GUN,\" one of the sisters is saying.\n\n\"We'll need two. Gaia said the Heart must be bombarded with two barysal beams at right angles. That will cause a critical resonance and destroy it.\"\n\n\"There's going to be others heading for the Heart. Vajra Singh's interest cannot be a coincidence; Janus Gaunt too. It'll be difficult to achieve the Heart's destruction with such as they are intent on preserving it.\"\n\nYou listen with keen interest. So others are bound for Du-En and it seems not all of them share the goal of ultimate power. This is worth thinking about. Meanwhile, you return stealthily to your seat.\n\nYou gained the codeword NEMESIS.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::GET_CODEWORDS(player, {Codeword::Type::NEMESIS});
    }

    int Continue(Character::Base &player) { return 252; }
};

class Story209 : public Story::Base
{
public:
    Story209()
    {
        ID = 209;

        Text = "Claustral Park is a large expanse in the upper level of the city which is entirely given over to wilderness. The fence is lined up with bright arc-lamps, and these are directed inwards across a grassy perimeter towards a thick unkempt woodland.\n\nScaling the fence, you scurry across the open area of grass and push into the bushes. Twigs and moss suffice to make a reasonably comfortable bed. As you drift off to sleep, you gaze back through the tree at the arc-lamps glaring hard in the mist. Why illuminate the park if no one comes here after dark? Perhaps the lamps were installed in bygone days and it is not known how to turn them off...\n\nYou wake with a stifled scream. You heave been seized by many hands and they are dragging you deep into the woods. You struggle, making contact with rough bodies that rasp your flesh like sandpaper. In the darkness you can see nothing of your assailants, but you can hear their snuffling grunts. They drag you to a clearing and break your limbs so that you cannot escape.\n\nThe claustrals cannot believe their good luck. Now they will feed on fresh meat for weeks to come.";

        Type = Story::Type::DOOM;

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }
};

class Story210 : public Story::Base
{
public:
    Story210()
    {
        ID = 210;

        Text = "Initiating a search of the files, you turn up a video almost two hundred years old. It shows an interview with Eleazar Picard, leader of the Volentine Cult, after he had fled form the unexplained destruction of Du-En. As you listen to Picard rambling about 'the madness' and 'the end of everything', you begin to piece together the facts. Apparently there was some kind of cataclysm or riot in the city, from which Picard barely escaped with his life.\n\nA hawk-faced man in a colonel's uniform appears on the screen. He listens as Picard becomes slightly more lucid under the effect of truth serum: \"It was the light at the end of Time, and my sanity is blinded!\"\n\n\"How is it you escaped?\" asks the colonel. \"You alone and no one else?\"\n\nPicard replies, \"I remembered the Truth.\"\n\nAt this point, Picard's eyes glaze and he starts to recite a catechism of his faith: \"The Truth is a flame. What ignites the flame? The spark ignites the flame. What is the spark? The Heart of Volent!\"\n\nYou scan forward through the rest of the interview, but Picard soon lapses into incoherence. A message at the end informs you that he died soon afterwards.\n\nYou gained the codeword LUNAR.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::GET_CODEWORDS(player, {Codeword::Type::LUNAR});
    }

    int Continue(Character::Base &player) { return 254; }
};

class Story211 : public Story::Base
{
public:
    Story211()
    {
        ID = 211;

        Text = "In a recess beside the door is a keypad comprising both letters and numerals. Putting your ear to the door, you begin to experiment with various combinations. Only an expert cracksman like yourself could recognize the distinctive clicks and whirs from the mechanism that show when you are on the right track.\n\nAfter an hour or so you have identified the first four letters of the code sequence. From that point it is a simple matter of trial and error to find the correct keys to press. Your efforts are rewarded with a hum of machinery as the door slides upwards, revealing the interior of the pyramid.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 233; }
};

class Story212 : public Story::Base
{
public:
    Story212()
    {
        ID = 212;

        Text = "The motilator registers your destination by flashing a light on the map. A chime sounds, warning that the doors are about to close. Fax steps back and raises his hand to wave. You watch his lean figure recede along the platform as the carriage gathers speed. The lights of the station dwindle into the distance of the tunnel.\n\nYou are on your way... where?";

        Choices.clear();
        Choices.push_back(Choice::Base("Go to Kahira", 50));
        Choices.push_back(Choice::Base("Go to Karthag", 124));
        Choices.push_back(Choice::Base("Go to Tarabul", 31));
        Choices.push_back(Choice::Base("Go to Giza", 74));

        Controls = Story::Controls::STANDARD;
    }
};

class Story213 : public Story::Base
{
public:
    Story213()
    {
        ID = 213;

        Text = "Up ahead you can now see a small figure trudging through the snow, dwarfed by the high walls of the city. Approaching, you attract his attention and he turns. As he unzips his hood you are struck by sudden recognition. \"Kyle Boche,\" you mutter wryly. \"What a small world.\"\n\n\"So,\" he says with a wide grin, \"you have also come to seek the ultimate power!\"";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 191; }
};

class Story214 : public Story::Base
{
public:
    Story214()
    {
        ID = 214;

        Text = "\"You might alter that list of priorities once you've heard what the Heart's 'ultimate power' actually consists of,\" you tell him.\n\nHe listens as you explain about Gaia's warnings. \"That does change things,\" he admits. \"It seems the Heart is a danger to the existence of the United States.\"\n\n\"The existence of the whole universe!\"\n\nGolgoth smiles. \"The universe doesn't sign my pay cheques. But I agree, we must see that the Heart is destroyed.\"\n\nYou gained the codeword BLUE.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::GET_CODEWORDS(player, {Codeword::Type::BLUE});
    }

    int Continue(Character::Base &player) { return 192; }
};

class Story215 : public Story::Base
{
public:
    Story215()
    {
        ID = 215;

        Text = "You reach the far end of the bridge and pass through into the tiled hall beyond. The architecture here is the same oppressive design as on the surface: the heavy harshly-chiselled lintels and monumental bulbous columns, the gigantic vaults and grotesque carvings. You have a choice of routes on from here: either of the two wide passages directly ahead, or a doorway to your left.";

        Choices.clear();
        Choices.push_back(Choice::Base("Go through the door", 259));
        Choices.push_back(Choice::Base("Take the left-hand passage", 3));
        Choices.push_back(Choice::Base("Take the right-hand passage", 128));

        Controls = Story::Controls::STANDARD;
    }
};

class Story216 : public Story::Base
{
public:
    Story216()
    {
        ID = 216;

        Text = "You are used to sneaking about in the dark, and you have learned to rely on your other sense almost as well as on your sight. Even as the huge predator comes darting out from the cloisters to attack, you are racing nimbly ahead to the feeble glimmer of light from the doorway.\n\nReaching it, you turn to see Boche and the baron fleeing in terror. In the light of Boche's flashlight you have a brief glimpse of a gigantic centipede with flanks like polished steel. As the others dive through, you slam the door shut and drop the bolt. A second later the door judders as the monster throws itself against it.\n\n\"Let's hope it holds,\" says Boche grimly. \"I wouldn't want to fight that beastie!\"";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 281; }
};

class Story217 : public Story::Base
{
public:
    Story217()
    {
        ID = 217;

        Text = "You retreat as far as the room where the soldier is frozen in stasis. The baron's brain glides in pursuit. The telepathic messages are getting scrambled and incoherent now, as the brain slowly uses up its remaining oxygen.\n\n(Stay... need your body. Getting dark.. Need new body...)\n\nSo that's its game! You duck around behind the stasis zone, watching the hovering brain as though through a lens. As soon as you are around the edge of the zone, you sidle to a point directly opposite your foe, then backtrack and run around to come up behind it. In his confusion, the baron had forgotten that light takes several seconds to cross the stasis zone. He is still watching your image when you club out of the air and crush him underfoot. The brain emits a dying wail -- a silent sound that you hear only inside your mind. It will haunt your nightmares for many years. If you live that long.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 261; }
};

class Story218 : public Story::Base
{
public:
    Story218()
    {
        ID = 218;

        Text = "Veering the sky-car in the air over Kahira, you glance down to see the Fijian shaking his fist up at you. He looks a tiny figure from here. You buzz him, swooping so low that he has to throw himself flat on the roof for fear of being hit. You fly off laughing into the night.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        player.Vehicle = Vehicle::Type::MANTA_SKY_CAR;
    }

    int Continue(Character::Base &player) { return 311; }
};

class Story219 : public Story::Base
{
public:
    Story219()
    {
        ID = 219;

        Text = "You station yourselves beside the Heart. Within its glittering facets, a universe is waiting to be born. Infinite possibilities flicker in the violet glare.\nGolgoth seems to share your mood. \"There's no other way,\" he says. \"Ready?\"\n\n\"What will happen?\" you ask him. \"I mean, will it just disappear, or will there be an explosion?\"\n\nHe shrugs. \"I don't know. Personally I doubt if I'll be around afterwards to collect this month's pay, but no one lives for ever.\"\n\nYou nod. \"I just never expected to be a martyr, that's all.\"\n\nThe beams from your guns converge at the centre of the Heart, splintering its crystal lattice. The light within grows until it blinds you. With a rush of energy, the Heart suddenly bursts apart, engulfing you and Golgoth in a cold flare of energy.\n\nIn this chamber deep below the ruins of Du-En, the two of you give your lives for the sake of the whole world.";

        Type = Story::Type::SACRIFICE;

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }
};

class Story220 : public Story::Base
{
public:
    Story220()
    {
        ID = 220;

        Text = "Gargan XIV was expecting that. She tosses a glass vial into Gilgamesh's unprotected ribcage. It breaks, releasing a green fluid which hisses as it gives off a cloud of acrid vapour. Gilgamesh staggers as the acid burns through his central power casing. He tries to aim his gun, but suddenly there is a crackle of sparks and his body locks rigid. You see the gleam of light fade from his eye-slit.\n\n\"That's your robot dealt with,\" says Gargan XIV, beckoning you to come closer. \"See if you can do any better.\"\n\nMeanwhile, Gargan XII has drawn a knife and is standing over Golgoth. She thinks he is beaten, when suddenly he looks up with a broad smile.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::REMOVE_CODEWORD(player, Codeword::Type::ENKIDU);
    }

    int Continue(Character::Base &player) { return 154; }
};

class Story221 : public Story::Base
{
public:
    Story221()
    {
        ID = 221;

        Text = "As you travel west, the bitter cold begins to ease. Instead of wild blizzards, you find yourself trudging through flurries of soft sleet. After several days you see a harsh light on the horizon. Ahead looms an arc of sunlight slanting through a wide gulf in the clouds. Even when night falls, the light keeps blazing down. An old weather satellite far out in space, misdirected by Gaia's freakish whims, battles the landscape in endless sun. These are the steaming swamps and jungles of Lyonesse.\n\nOn the northern fringes of the region, you have heard that men exploit the fertile farmland to support the old city of Lyon. To the south, the warm waters mean plentiful fish. But no one inhabits the interior of Lyonesse, which is the stalking ground of mutated beasts and carnivorous fungi.\n\nYou press on undaunted, pleased to be able to shrug off your thick furs as you leave the icy wasteland behind and enter the lush dank morass. Foliage like ships' sails blots out much of the sky, leaving you plunged in green gloom despite the ceaseless daylight. Extravagant blooms with jewel-like colours exude a mingle of musky scents. Creepers stretch in nets between the black trees. Ferns form high banks across your path. Off in the distance, the chittering and screeching of jungle animals seem unreal after so many days with only the wind's howl in your ears. Here there is no night or day. When tiredness becomes too much, you slump sweat-soaked beside a fallen log and roll our clothing up to make a pillow. The succulent jungle whispers lull you off to sleep.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_SKILL(player, Skill::Type::ESP))
        {
            return 30;
        }
        else if (Character::VERIFY_SKILL(player, Skill::Type::AGILITY))
        {
            return 53;
        }
        else
        {
            return 75;
        }
    }
};

class Story222 : public Story::Base
{
public:
    Story222()
    {
        ID = 222;

        Text = "You hurry along to the end of the street, where an old man stops you and asks, \"Is Sarco the Poisoner still open for business?\"\n\n\"How should I know?\" you reply. \"I am not acquainted with the man you mention.\"\n\nHe returns a look that suggests you are either deranged or a liar. \"I just now saw you leave his shop!\" he declares.\n\nYou rub your jaw ruefully. \"Sarco the Poisoner, indeed.\"\n\nIt seems you've had a narrow escape.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 286; }
};

class Story223 : public Story::Base
{
public:
    Story223()
    {
        ID = 223;

        Text = "You hurry along to the end of the street, where an old man stops you and asks, \"Is Sarco the Poisoner still open for business?\"\n\n\"How should I know?\" you reply. \"I am not acquainted with the man you mention.\"\n\nHe returns a look that suggests you are either deranged or a liar. \"I just now saw you leave his shop!\" he declares.\n\nYou rub your jaw ruefully. \"Sarco the Poisoner, indeed.\"\n\nIt seems you've had a narrow escape.";

        Choices.clear();
        Choices.push_back(Choice::Base("Keep trying to get the ID card altered", 245, {Item::ID_CARD}));
        Choices.push_back(Choice::Base("See to other matters before the ferry to Kahira arrives", 414));

        Controls = Story::Controls::STANDARD;
    }
};

class Story224 : public Story::Base
{
public:
    Story224()
    {
        ID = 224;

        Text = "As the queue moves along, Kyle Boche hurries to join you. Stamping his feet to fend off the morning chill, he tells you that he was delayed by some last-minute purchases in town. You notice a new BARYSAL GUN at his belt. \"We're on our way now,\" he says excitedly, grinning as he stares out into the mist over the sea. \"The adventure's begin!\"\n\nA problem arises when you reach the ticket office. The cost of passage to Kahira is 10 scads each. Boche peers woefully at his remaining funds and then turns to you with a shrug. \"You'll have to pay for me, I'm afraid. Still we're partners. I'm sure you'll agree I've never stinted from doing my bit up till now.\"";

        Choices.clear();
        Choices.push_back(Choice::Base("Pay for both (20 scads)", 246, Choice::Type::LOSE_MONEY, 20));
        Choices.push_back(Choice::Base("Pay for yourself but not for him (20 scads)", 267, Choice::Type::LOSE_MONEY, 10));
        Choices.push_back(Choice::Base("You do not have enough money even to buy your own ticket", 288));

        Controls = Story::Controls::STANDARD;
    }
};

class Story225 : public Story::Base
{
public:
    Story225()
    {
        ID = 225;

        Image = "images/vampire.png";

        Text = "The next morning you are overtaken by a blizzard that turns the sky dark as Doomsday. Stumbling on, you are barely able to see the others through the thick snowfall. Then a blue flare erupts in the gloom, drawing you to where Shandor stands with two of his bodyguards. He has a sputtering magnesium torch in his hand which drips hissing fragments of hot metal into the snow. \"Where's Goro?\" says one of the bodyguards.\n\nYou all gaze off into the blizzard. No one says a thing until Shandor grabs your sleeve. \"We can't do anything for him now,\" he calls over the shrieking wind. \"Come on, there's an ice cave we can shelter in.\"\n\nYou follow the others down a crevice in the ground. In the light of Shandor's flare, the walls around you gleam like old blue glass. \"This is frozen meltwater,\" you say, running your hand across the smooth ice. \"We must be on another glacier.\"\n\n\"Inside a glacier, you mean,\" mutters Shandor. \"Eerie, isn't it? We're safer here than out in that storm, though.\"\n\nYou walk along the gallery of ice, overwhelmed by a sense of awe. Perhaps this was a mighty river once, a raging torrent, before the summers died and the Ice Age took root in the world. Now it slides imperceptibly along the rate of a few centimetres a year.\n\nYou see a face in the ice and jump back with a startled cry. Shandor comes over and raises the torch. Frozen just inside the surface of the wall is a man in black military-style overalls, a shortsword held in one hand. Perhaps he has been there for centuries, but what makes you shudder is that fact that his eyes are wide open. \"I could almost fancy he's watching us,\" you say.\n\n\"Tell you what I fancy,\" says Shandor. \"That sword he's got.\"";

        Choices.clear();
        Choices.push_back(Choice::Base("Chip the body out of the ice", 205));
        Choices.push_back(Choice::Base("Wait until the blizzard is past and then press on", 249));

        Controls = Story::Controls::STANDARD;
    }
};

class Story226 : public Story::Base
{
public:
    Story226()
    {
        ID = 226;

        Text = "The relief crew arrive and take shelter from the cold inside the terminal building. Strolling along the wharf, you spy a cluster of wickerwork pots containing live cancretes. These are pale crustaceans with long spines: a noted local delicacy when baked in clay. With cautious dexterity, you open one of the pots and remove the cancrete inside. The cold has made it torpid and it makes only a half-hearted effort to sever your thumb with its claws. You wrap it up in your jacket. Returning with it to the terminal building, you sidle over to the fireside where some of the sailors are enjoying and early-morning nip of whiskey. None of them notices you slip the cancrete into the pile of jackets and pullovers they have strewn across a chair.\n\nYou stand by the window and look out to sea, pretending to watch for the ferry. At last a gong is sounded, announcing the ferry's imminent arrival, and the relief crew begin to gather their belongings. Suddenly there is a yelp of pain from one of the sailors and he throws off his jacket as though it were on fire. The cancrete emerges and crawls sullenly away across the floorboards. \"That cancrete has bitten me severely,\" says the sailor, displaying a read weal on his hand.\n\nA youngster whom you take to be the cabin boy watches in horror as the cancrete squeezes into hiding behind the stones of the hearth. You guess that this is his first voyage when he blurts out, \"Did you see that horrific creature? Are they common in the Inland Sea?\"\n\n\"Not of such a size,\" you tell him quietly. He looks relieved until you add, \"Most grow much bigger. Didn't your shipmates tell you about them?\"\n\nEyes widening in fright, the cabin boy rushes out without making any explanation. You step smartly into the breach. Introducing yourself to the captain, you portray yourself as a person with some prior nautical experience. \"Your cabin boy has withdraw from the voyage, but I am a willing and able worker,\" you tell him.\n\nA faint furrow of suspicion clouds his brow, but he admits that he is one hand short. \"Very well, I'll take you on. But since you are untried, I won't be offering full pay.\"\n\nYou shrug casually as you shoulder your pack. \"Pay? Passage to Kahira is enough.\"";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 246; }
};

class Story227 : public Story::Base
{
public:
    Story227()
    {
        ID = 227;

        Text = "\"They told me my boss was up there,\" says the Fijian, \"but all I see is you. Imposter!\" He starts to lunge towards you.\n\nYou turn in the doorway to face him, displaying your gun. \"Here are my credentials,\" you say coolly. \"Now, let's go and see what's in that closet over there.\"\n\nStaring down the barrel of your gun, he has no arguments. You guess that he is a professional bodyguard because he obviously expects what's coming next. With hands on the top of his head, he turns his back and steps into the closet. You club him across the neck with the butt of the gun and he drops. Closing the door of the closet, you leave quickly before he can recover and raise the alarm.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 311; }
};

class Story228 : public Story::Base
{
public:
    Story228()
    {
        ID = 228;

        Text = "Crashing through a thicket of glossy green fronds, you are brought up short by an astounding sight. A majestic ruined city spreads off into the jungle in front of you, seeming to shimmer in the haze of eternal tree-filtered sunlight. Some of the buildings have been choked by vegetation, grappled to destruction by cables of vine, walls uprooted by inexorable growth and carpeted with moss. But other towers still stand gleaming, bright glass and polished steel glorious in the dappled green-gold light. Those must be the buildings still cleaned and maintained by careteks, the diligent robot janitors left by the ancients.\n\nYou skirt the perimeter of the city, stumbling and staggering from sheer awe, like a blind man suddenly given the gift of vision. It can only be the remnants of old Marsay, the mythic place lost for two centuries. A bee as big as a child's fist goes careering cumbersomely past your ear, legs sprawling like a helicopter's skids, and disappears into a vast buzzing hive in the eaves of a crumbled house. Birds sit pecking at lichen-stained girders, sparing you only disdainful glance. You feel like an intruder here.\n\nA barren patch of ground lies ahead, about fifty paces across, forming an avenue towards the intact area of the city.";

        Choices.clear();
        Choices.push_back(Choice::Base("Follow it", 271));
        Choices.push_back(Choice::Base("Press on into the jungle", 292));

        Controls = Story::Controls::STANDARD;
    }
};

class Story229 : public Story::Base
{
public:
    Story229()
    {
        ID = 229;

        Image = "images/kahira.png";

        Text = "Kahira is shrouded in a perpetual mist that rises from the warm river waters that flow beneath the city. The city gate is a metal shutter opening into a wide cargo lift at the bottom of a concrete buttress. You hurry through just before the gate closes for the final time this evening. Standing in a crowd of people, donkeys and camels, you wait while the lift rattles up to the street level and opens to disgorge its passengers onto a fog-draped plaza. You step out under the dank glare of a neon lamp and gaze around the plaza, ignoring the stragglers barging past you with their packs. The babble of voices is muffled by the fog. The air is dankly cold, with a flat reek of mist and wet concrete.\n\nA man wearing an illuminated fez scurries up to you, brushing his fingertips together as though washing. A clutch of snaggled teeth gleam in the street-light as he bows. \"Greetings! I am Bador, a dracoman. For a single scad I will be pleased to assist you with the many queries you must have regarding this estimable city.\"";

        Choices.clear();
        Choices.push_back(Choice::Base("Pay Bador's fee (1 scad)", 33, Choice::Type::LOSE_MONEY, 1));
        Choices.push_back(Choice::Base("Tell him to be gone", 95));

        Controls = Story::Controls::STANDARD;
    }
};

class Story230 : public Story::Base
{
public:
    Story230()
    {
        ID = 230;

        Text = "You look into the surface thoughts of the twins as they converse. The memories are of a hostelry in Florida, a cracking video screen, a tantalizing message concerning the power of the Heart of Volent. They, like you, heard a transmission from Gaia. They travelled across the Atlantic to get here. No question about it -- they must be heading for Du-En.\n\nYou are about to end the mind probe when you glimpse another memory that amazes you. At an old computer complex in Madeira, the twins managed to make contact with Gaia again. This time she seems to have been more coherent, and told them that the Heart was dangerous. If a person were to claim its power, the universe as it currently exists would be swept away, remade in a new image.\n\nThe twins apparently concluded that the Heart should be destroyed. Gaia told them that this could be done by dual barysal beams striking the gemstones at right angles, which would set up an internal dissonance. The Heart would then crack apart like a glass shattered by a soprano's highest note.\n\nOne of the twin glances up. Guiltily you withdraw your mental scan, looking away before she notices you watching.\n\nYou gained the codeword NEMESIS.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::GET_CODEWORDS(player, {Codeword::Type::NEMESIS});
    }

    int Continue(Character::Base &player) { return 252; }
};

class Story231 : public Story::Base
{
public:
    Story231()
    {
        ID = 231;

        Text = "You find a deserted alley where you slump down behind a stack of cardboard boxes.\n\nYou have no sense of how long has passed when you feel a boot nudge you in the ribs. You open your eyes. Half a dozen figures stand over you in the dim mist-filtered moonlight. \"Get up,\" says a voice.\n\nYou start to rise, but the nearest figure sweeps out his leg and sends you sprawling back against the wall. You steady yourself against the damp brickwork and glare back at them.\n\nA woman steps forward and speaks in a brittle zealous voice. \"We are of the Church of Gaia. Give of your belongings that Gaia may bring salvation to the world.\"\n\nYou have heard of this cult -- just one of hundreds that have sprung up in the latter days of the world, as people turn in their desperation to strange beliefs. The Church is founded on a particularly deranged creed. Instead of thinking Gaia to be a sophisticated computer, they believe she is the creator goddess.";

        Choices.clear();
        Choices.push_back(Choice::Base("Give them your money", 274));
        Choices.push_back(Choice::Base("Use [CUNNING]", 295, Skill::Type::CUNNING));
        Choices.push_back(Choice::Base("Fight", 316));

        Controls = Story::Controls::STANDARD;
    }
};

class Story232 : public Story::Base
{
public:
    Story232()
    {
        ID = 232;

        Text = "You try a peripheral avenue of investigation, consulting all files relating to the Heart of Volent before it acquired that name. You discover that it was a large gemstone, apparently of interstellar origin, which fell to Earth in the late twenty-first century. Coming into the hands of the Volentine cult, it became the focus of cult worship and was conveyed to their citadel in the fastness of the Sahara.\n\nThe technical records are intriguing. Before the Heart was stolen by the Volentines, a preliminary scientific study was made to analyse its crystalline, structure and intrinsic radiation. The results, when fed into Gaia, yielded the startling conclusion that it was a by-product of the Big Bang which created the universe -- in essence a kind of 'twin' universe which had frozen in stasis before it could expand. It is eerily akin to the way that abortive twins are sometimes formed in human gestation, only to be reabsorbed by the developing foetus.\n\nGaia's remarks from the time ring ominously across the gulf of centuries: \"This object is unstable. Psionic disruption, as from direct contact with a human man, could reactivate its expansion inside our own universe, leading to the destruction of all that exists. A new universe would result. Destruction of the object is advised, and this can be simply accomplished by bombardment with coupled barysal beams of complementary phasing.\"\n\nGaia's advice was not acted upon. She was already considered unreliable. That was before the world experienced the Paradox War -- evidence of what a fraction of the Heart's untapped power could do.\n\nYou gained the codeword NEMESIS.";

        Choices.clear();
        Choices.push_back(Choice::Base("Look through the data on the Volentine cult", 210));
        Choices.push_back(Choice::Base("Take a look around the rest of al-Lat", 254));

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::GET_CODEWORDS(player, {Codeword::Type::NEMESIS});
    }
};

class Story233 : public Story::Base
{
public:
    Story233()
    {
        ID = 233;

        Text = "A tunnel beyond the doorway is lit by luminant strips close to the floor, which bathe you in a sinister blue glow as you advance towards the centre of the pyramid. You reach a room whose walls are covered with video screens. A partial projection of the world's surface stretches across the ceiling, reduced to a chequerboard of black patches where the scanning satellites have gone offline over the years.\n\nIn the centre of the room is a shaft that descends into the depths of the pyramid. A circular steel plate hovers at the top, beside a row of buttons. You guest it is a sort of elevator, although a much more sophisticated design than any in existence nowadays. Studying the buttons, you discount the levels given over to dormitories and recreation. That leaves the research and military levels, located furthest down in the building.";

        Choices.clear();
        Choices.push_back(Choice::Base("Risk taking the strange elevator going to the research level", 276));
        Choices.push_back(Choice::Base("The military level at the very bottom of the shaft", 255));
        Choices.push_back(Choice::Base("Leave the way you came", 361));

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::REMOVE_CODEWORD(player, Codeword::Type::HUMBABA);
    }
};

class Story234 : public Story::Base
{
public:
    std::string PreText = "";

    Story234()
    {
        ID = 234;

        Bye = "You eventually reach the River Isis and cross over an old iron bridge to the west bank. From here, a broad sweep of gleaming ivory dunes extends to the horizon under a sky darker than wet stone. These are the Saharan Ice Wastes, thousands of kilometres of uncharted wilderness, desolate snow fields pounded by incessant arctic gales. This is the terrible barrier that separates you from your goal.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        PreText = "The ground underfoot is hard-packed gravel under a sprinkling of snow. As you travel on, a biting wind sweeps down from the north bringing heavy snow. The pale blue sky is blotted out by glowering clouds through which enough daylight seeps to give the snow a canescent hue. You hunch into the wind and trudge on, fearing to step and rest in case you never get up again.";

        if (!Character::VERIFY_ITEMS_ANY(player, {Item::COLD_WEATHER_SUIT, Item::FUR_COAT}))
        {
            Character::GAIN_LIFE(player, -1);

            PreText += "\n\nYou LOSE 1 Life Point.";
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 393; }
};

class Story235 : public Story::Base
{
public:
    Story235()
    {
        ID = 235;

        Text = "You pass a restless night troubled by bouts of nausea. As the sky begins to show the dim silver burnish of predawn light, you rise and run trembling fingers through your sweat-matted hair.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_ITEMS(player, {Item::Type::ANTIDOTE_PILLS}))
        {
            return 320;
        }
        else
        {
            return 342;
        }
    }
};

class Story236 : public Story::Base
{
public:
    Story236()
    {
        ID = 236;

        Text = "He listens with interest as you explain how the Heart can be destroyed by two people working together. \"A fitting use for that fine barysal pistol of yours.\" you conclude with a grim smile.\n\nHe nods and then leans close, putting an arm across your shoulders. \"You're right of course. I'm glad you confided all this to me. We must keep it secret from the others. They are mad dogs, who care nothing for whether the world lives or dies.\"\n\nYou gained the codeword YELLOW.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::GET_CODEWORDS(player, {Codeword::Type::YELLOW});
    }

    int Continue(Character::Base &player) { return 192; }
};

class Story237 : public Story::Base
{
public:
    Story237()
    {
        ID = 237;

        Text = "Reaching the nearest grille, you find it rusted and easy to wrench out of the wall. Narrowing your shoulders, you squeeze through into a small tomb chamber. A body lies on a low carved slab, withered but preserved by the cold dry breeze gusting up from below. The robes suggest this was one of the priests of the Volentine cult. Hearing the others calling you, you make a hurried search of the tomb, finding nothing of interest except for a SPECULUM JACKET. This gives some protection from BARYSAL gunshots.\n\nSince the others are anxious to be off, you climb back up to rejoin them without inspecting the other grilles.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::GET_ITEMS(player, {Item::SPECULUM_JACKET});
    }

    int Continue(Character::Base &player) { return 215; }
};

class Story238 : public Story::Base
{
public:
    Story238()
    {
        ID = 238;

        Text = "With unerring precision, you blast a shot at the lintel above the doorway. The beam fractures the darkness and raises the lintel to white heat in an instant. \"Run towards the door!\" you shout to the others.\n\nYou race in the direction of the glowing stone lintel. The baron reaches it first, gliding through the doorway and turning to slam it after you and Boche have run through. A moment later, something slams into the other side with juddering impact, but the door holds.\n\n\"What was that?\" you ask.\n\n\"I caught a brief glimpse of it in the glare from your gunshot,\" says Boche. \"Believe me, you don't want to know.\"";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::FIRE_BARYSAL(player, 1);
    }

    int Continue(Character::Base &player) { return 281; }
};

class Story239 : public Story::Base
{
public:
    Story239()
    {
        ID = 239;

        Text = "You are almost hypnotized by terror, and it is only by the merest chance that your fingers brush the grenade hanging at your belt. Unclipping it, you send it rolling and bounding across the floor. The baron's brain comes rushing through the air just as the grenade detonates. There is a flash, and the brain is flung to the floor by the concussion. While it lies dazed, you crush it under your heel. If only you could shut out the dying shriek that echoes telepathically through your mind and will stay with you until your dying day.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::LOSE_ITEMS(player, {Item::Type::STUN_GRENADE});
    }

    int Continue(Character::Base &player) { return 261; }
};

class Story240 : public Story::Base
{
public:
    Story240()
    {
        ID = 240;

        Text = "You feel a jolting impact as though someone had punched you in the throat. Looking down, you are amazed to see a steel crossbow bolt has pierced your windpipe. You stagger back a couple of steps and then fall, dying in a pool of blood. You have failed with success almost in your grasp.";

        Type = Story::Type::DOOM;

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }
};

class Story241 : public Story::Base
{
public:
    Story241()
    {
        ID = 241;

        Text = "Golgoth checks Boche's gun, but the charge has been used up.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_ITEMS(player, {Item::Type::BATTERY_UNIT}))
        {
            return 219;
        }
        else
        {
            return 263;
        }
    }
};

class Story242 : public Story::Base
{
public:
    Story242()
    {
        ID = 242;

        Text = "You sift carefully through the debris, but find nothing more than torn clothes and broken ornaments. Boche points out a rusty brown stain on a broken tabletop. \"The rioters went in for human sacrifice,\" he says.\n\nThe baron hovers over the spot, peering back through the curtain of time. Finally he gives a curt nod. \"Yes. The populace broke in here and slaughtered the acolytes. But this is not where the high priest and his adepts dwelled. That lies deeper in the catacombs, and there we shall find the Shrine of the Heart.\" He swivels in the air and glides rapidly back to the antechamber, calling over his shoulder: \"Come.\"\n\nYou and Boche exchange a glance, then follow. The baron is inspecting the two passages, but his psychic senses cannot tell which is better. \"You choose,\" he says.";

        Choices.clear();
        Choices.push_back(Choice::Base("Go right", 128));
        Choices.push_back(Choice::Base("Go Left", 3));

        Controls = Story::Controls::STANDARD;
    }
};

class Story243 : public Story::Base
{
public:
    Story243()
    {
        ID = 243;

        Text = "Passing under a broken arch, you enter the central plaza. This is a broad area of snow-covered flagstones, roughly three hundred metres across, enclosed by ruined palaces that gleam like lead in the white haze.\n\nThere are several campfires built up against a fallen colonnade. As you get closer you see antique furniture and splintered doors crackling in the flames: plunder from the once great city of Du-En, list art treasures beyond price. In this desolate place, their only value is the warmth they give.\n\nA man steps forward from the fireside and pulls off his glove to shake hands. \"Greetings. I am Janus Gaunt.\"\n\nWhile Boche makes the introductions, you take stock of Gaunt. He is younger than his grey hair would suggest, with an open friendly manner. His servants stumble along behind him in the snow, but they wear only thin clothes and you guess they are past feeling the cold. Gaunt sees you looking at them and nods. \"These are my xoms -- reanimated cadavers, loyal and tireless.\"\n\nOthers are now emerging from their tents along the colonnade. A wizened old man with no legs who comes drifting through the air like a ghost is introduced as Baron Siriasis, a psionic from Bezant. Next comes a woman who walks with long feline strides, eyes glittering like jade in the wan afternoon light. She gives you a single guarded look and then turns away. \"That is Thadra Bey of al-Lat,\" says Gaunt. \"An here is Commander Chaim Golgoth, an agent of United States Intelligence. The two strapping bronzed ladies behind him are Gargan XIII and Gargan XIV, sole survivors of a clone superwarrior group.\"\n\nGolgoth smiles and shakes hands. The Gargan twins watch you with a glare like Medusa's sisters. \"There's also Vajra Singh,\" adds Golgoth, nodding towards a large scarlet-and-black pavilion across the plaza. \"You'll meet him soon enough, I'll warrant.\"";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_CODEWORD(player, Codeword::Type::SCYTHE))
        {
            Character::REMOVE_CODEWORD(player, Codeword::Type::SCYTHE);

            return 89;
        }
        else
        {
            return 111;
        }
    }
};

class Story244 : public Story::Base
{
public:
    Story244()
    {
        ID = 244;

        Text = "The Hotel Paradise proves to be a converted temple overlooked by a high bell-tower. You stand looking up at the inside of the splendid domes. Once, centuries ago, this vast hall must have rung with orisons to the forgotten deity of that age. Now it echoes instead to the grunt and clamour of people eating in the refectory or clattering up and down the wooden stairway to their rooms.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 329; }
};

class Story245 : public Story::Base
{
public:
    Story245()
    {
        ID = 245;

        Text = "You eventually find a man who can do what you want: a fat sweaty fellow with a profusion of ancient tools and devices strewn around his shop. Guiding his laser by hand, he makes a few deft changes to the image on the card until it could pass for your own likeness. \"Five scads,\" he says, holding it out to you.\n\n\"Five?\" You scowl.\n\n\"Membership of the prestigious Compass Society is usually much more expensive than that,\" he says with a shrug.\n\nYou consider snatching the card back, but the fat man cannily anticipates you and holds it close to the laser beam until you pay. If you refuse to pay, he destroys the ID card.";

        Choices.clear();
        Choices.push_back(Choice::Base("Agree to his terms (5 scads, gain codeword PROTEUS)", -245, Choice::Type::LOSE_MONEY, 5));
        Choices.push_back(Choice::Base("You refuse to or cannot pay him (ID CARD is destroyed)", 25, Choice::Type::LOSE_ITEMS, {Item::ID_CARD}));

        Controls = Story::Controls::STANDARD;
    }
};

class Event245 : public Story::Base
{
public:
    Event245()
    {
        ID = -245;

        Choices.clear();

        Controls = Story::Controls::NONE;
    }

    int Background(Character::Base &player)
    {
        Character::GET_CODEWORDS(player, {Codeword::Type::PROTEUS});

        return 25;
    }
};

class Story246 : public Story::Base
{
public:
    Story246()
    {
        ID = 246;

        Text = "After fifteen minutes, a pale green light flashes through the murk out to sea. The assembled travellers start to rise and gather their belongings as the ferry comes sweeping in towards the dock. A massive hovercraft of the tree tiered decks surmounted by a high conning-tower, the ferry glides up the frosty foreshore and settles on its metal skirt. Workers immediately rush out with planks to assemble a boardwalk, and you go aboard with the others.\n\nThere is a delay while supplies are loaded. You find a couch on the middle deck and gaze out to sea. A polarizing tint in the glass adds to the gloominess of the scene, with tall iron-black clouds pile high above a sea of grey swell and ice floes.\n\nEventually the craft raises itself and you are under way. Stewards come round and lunch is served at long curved tables in the central lounge. You chew at the stodgy gruel formulated from sea algae, washed down with spiced tea.\n\nYou take a promenade of outside deck, but the chill of the afternoon soon drives you back inside. Some of the other passengers started card games. As the daylight fades, a gap in the louring cloud reveals a handful of diamond-bright stars. The bar is opened and the atmosphere aboard gradually acquires a current of bonhomie, but you remain aloof and troubled. Most of these people have no further destination in mind than Kahira, no ambition beyond a small profit and a frisson of petty adventure. But your own goal is directly remote: the lost ruins of Du-En, in the far hinterland of the Saharan Ice Wastes. It seems impossible to believe, but here in Du-En you will either grasp the ultimate power -- or perish.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 10; }
};

class Story247 : public Story::Base
{
public:
    Story247()
    {
        ID = 247;

        Text = "You travel upriver until Kahira comes in sight through the veils of sparkling river-mist. It straddles the river on huge concrete buttresses, a city built on many levels, with gleaming towers stretching towards the darkening sky. Steering the sky-car towards the gate set into one of the colossal buttresses, you cast your eyes west across the Sahara. The pyramids of Giza are half-hidden by dusk and mist. Beyond lies the implacable desert of ice which you must cross to reach your goal.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_CODEWORD(player, Codeword::Type::DIAMOND))
        {
            return 251;
        }
        else
        {
            return 229;
        }
    }
};

class Story248 : public Story::Base
{
public:
    std::string PreText = "";

    Story248()
    {
        ID = 248;

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        PreText = "The Fijian is strong and well-trained, but you have the advantage of suppleness. His foot lashes out. You dodge to one side, catch the ankle, and twist him off balance. Rather than allow you to dislocate his knee, he falls back, braces his arms on the ground, and delivers a thrust with both legs that sends you slamming back against the wall.\n\n";

        auto DAMAGE = -2;

        DAMAGE = Character::COMBAT_DAMAGE(player, DAMAGE);

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Point(s).";

        if (player.Life > 0)
        {
            PreText += "\n\nThe impact leaves you stunned, giving your opponent time to get to his feet. He charges towards you bellowing. You steady yourself to meet the attack, and by luck a wall-mounted extinguisher comes to hand. You feel no compunction at using it; any weapon is fair in such a fight. A spray of foam to the eyes blinds the Fijian long enough for you to swing the canister in a solid clout to the side of his head. He falls like a sack of bricks.\n\nYou stoop and check his pulse, relieved to discover he is still alive. You had better get well away before he comes round.";
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 311; }
};

class Story249 : public Story::Base
{
public:
    std::string PreText = "";

    Story249()
    {
        ID = 249;

        Bye = "Outside Venis, Shandor tells you he is detouring north to meet some friends who live in an isolated manor house.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        PreText = "The third bodyguard, Goro, is found only fifty metres from the ice cave where you took shelter. He is frozen on his hands and knees, having died crawling in the merciless fury of the blizzard. Snow is piled around the body.\n\nThe other two, who may be his brothers for all you know, betray no emotion at the sight. Of course, they must have known he could not have survived. \"Shall we bury him, boss?\" says one.\n\nShandor looks at the sky. Dusk is already descending along the edge of the valley, shadows creeping like blots of soot across the crisp white snow. He considers, then gives you a curt nod back towards the crevasse. \"Toss the body down into the cave -- it's as good a tomb as any. I want to be off this glacier by nightfall.\"\n\nYour journey continues for another week. Hungry and beset by chilling winds, you feel drained of strength. The unbroken glower of grey cloud across the sky leaves you dispirited. By the time you reach the eastern foothills, your fingers are tingling with the first stages of frostbite.";

        auto DAMAGE = -2;

        if (Character::VERIFY_SKILL(player, Skill::Type::SURVIVAL))
        {
            DAMAGE = -1;
        }

        Character::GAIN_LIFE(player, DAMAGE);

        PreText += "\n\nYou LOSE " + std::to_string(-DAMAGE) + " Life Point(s).";

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player)
    {
        if (!Character::VERIFY_SKILL(player, Skill::Type::STREETWISE) && !Character::VERIFY_ITEMS(player, {Item::Type::VADE_MECUM}))
        {
            return 438;
        }
        else
        {
            return 117;
        }
    }
};

class Story250 : public Story::Base
{
public:
    Story250()
    {
        ID = 250;

        Image = "images/marsay.png";

        Text = "Crashing through a thicket of glossy green fronds, you are brought up short by an astounding sight. A majestic ruined city spreads off into the jungle in front of you, seeming to shimmer in the haze of eternal tree-filtered sunlight. Some of the buildings have been choked by vegetation, grappled to destruction by cables of vine, walls uprooted by inexorable growth and carpeted with moss. But other towers still stand gleaming, bright glass and polished steel glorious in the dappled green-gold light. Those must be the buildings still cleaned and maintained by careteks, the diligent robot janitors left by the ancients.\n\nYou skirt the perimeter of the city, stumbling and staggering as much from sheer awe as from your throbbing wound. It can only be the remnants of old Marsay, the mythic place lost to the sight of man for two centuries.\n\nA bee as big as a child's fist goes careering cumbersomely past your ear, legs sprawling like a helicopter's skids, to disappear into a vast buzzing hive in the eaves of a crumbled house. Birds sit pecking at lichen-stained girders, sparing you only a disdainful glance. You feel like an intruder here.\n\nA barren patch of ground lies ahead, about fifty paces across, forming an avenue towards the intact area of the city.";

        Choices.clear();
        Choices.push_back(Choice::Base("Follow it", 271));
        Choices.push_back(Choice::Base("Press on into the jungle", 184));

        Controls = Story::Controls::STANDARD;
    }
};

class Story251 : public Story::Base
{
public:
    Story251()
    {
        ID = 251;

        Text = "The city gate is a metal shutter that opens into a wide cargo lift at the bottom of a concrete abutment. Those entering are conveyed up inside the abutment to the lowest street level twenty metres above. You have arrived just before the gates are closed for the night, and as you step out into the street you draw the attention of a man in blue-&-grey combat dress.\n\nBoche recognizes him, throwing out his chest in a display of wary truculence. The man stops and nods, a relaxed smile on his slim features. \"Kyle Boche. And is this your accomplice, or your latest victim?\"\n\n\"Chaim Golgoth! Are you still a hired murderer?\" pipes up Boche in a tone of bravado. He turns to you. \"This man kills people for a living.\"\n\nYou notice the insignia on Golgoth's sleeve: the blazoned sliver eagle and blue stars of United States Intelligence. Even with the world close to collapse, the United States remains a force to be reckoned with. If Golgoth really is a USI assassin, Boche is being reckless in bantering with him like this. You size up what you can see.\n\nGolgoth has a holstered a BARYSAL GUN and, to your surprise, a steel crossbow slung behind his hip. At a glance he looks open and friendly, his voice mild with wry amusement. But there is something almost chilling about the eyes. Those are the eyes of a man who is always alert.";

        Choices.clear();
        Choices.push_back(Choice::Base("[ESP] Probe Golgoth's mind", 121, Skill::Type::ESP));
        Choices.push_back(Choice::Base("Query him about Giza", 337));
        Choices.push_back(Choice::Base("Ask about his reason for carrying a crossbow", 315));
        Choices.push_back(Choice::Base("Simply pass by him into the city", 358));

        Controls = Story::Controls::STANDARD;
    }
};

class Story252 : public Story::Base
{
public:
    Story252()
    {
        ID = 252;

        Text = "At last the fearsome twins begin to show signs of lassitude. They call for plates of stew, which they devour with gusto, then go crashing off into the dormitory where they hurl themselves down on cots and are soon snoring like lionesses.\n\n\"What fearsome furies,\" says the innkeeper in a disconsolate whisper. \"They drank a hundred scads' worth of vodka. I think there's little hope of getting my money, however.\"\n\nThe other customers of the inn set up bedding on the floor of the taproom. No one dares to share the dormitory for fear of the Gargan sisters awakening and setting on them in a drunken rage. You cannot blame them. You pass an uneasy night yourself, rising early to set out on your way.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 273; }
};

class Story253 : public Story::Base
{
public:
    Story253()
    {
        ID = 253;

        Text = "Fishermonger Plaza, on the lowest level of the city, is a wide paved area surrounding an area of gratings directly above the river. At night, the gratings are opened and nets lowered. Bright lamps are shone down into the river to attract the attention of the fish, which are then hauled up in huge writhing shoals. This continues throughout the night, and the promise of fresh fried fish brings all manner of night-time workers to the plaza. At stalls ranged around the gratings, a curious band rub shoulders: off-duty policemen, fishmongers, prostitutes, street cleaners, beggars, and burglars.\n\nYou huddle down beside a shuttered pastry stall. The bright light and bustle make it difficult to sleep here, but at least you are safe from muggers. You pass a restless night, awakening before the dawn with a gritty feeling in your eyes and cramp in your limbs. \"Get away from here,\" say the stall-holder as he opens up for the day. \"I will not have my wares contaminated by a beggar's lice.\"\n\nYou wait until he is not looking to steal a pastry for your breakfast, then make your way to the bazaar.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 333; }
};

class Story254 : public Story::Base
{
public:
    Story254()
    {
        ID = 254;

        Text = "A young technician sees you wandering around bewildered and takes you on a tour of the laboratories. \"The al-Lat station was originally intended as a base of pure scientific research,\" he explains. \"Luckily we were always designed to be self sufficient, so the degradation of Earth's climate and population has not affected us.\"\n\nYou pause beside a bank of supercooled chambers. \"What research is going on here?\"\n\n\"These are neural networks -- artificial brains into which we are loading the original weather control programs used when Gaia was first set up. They are works of genius, unsurpassed even today. Naturally we cannot tap into Gaia herself, by reason of the viruses that have invaded the system. But these original programs are uninfected.\"\n\n\"What is the purpose of the work?\" you ask.\n\n\"We hope to terraform the planet Venus -- that is, make it suitable for human habitation. Then we can colonize.\"\n\nYou give a snort of incredulous laughter. \"Why not use your skills on Earth? Although inhospitable, it is still a better place than Venus!\"\n\nHe smile. \"We thought of that, of course. Any attempt to alter the Earth's current climate might meet with Gaia's displeasure, and al-Lat would be targeted by nuclear missiles against which we have no defence.\"";

        Choices.clear();
        Choices.push_back(Choice::Base("Argue that the Earth should not be callously abandoned", 296));
        Choices.push_back(Choice::Base("Offer to join the people of al-Lat and help with the terraforming project", 317));

        Controls = Story::Controls::STANDARD;
    }
};

class Story255 : public Story::Base
{
public:
    Story255()
    {
        ID = 255;

        Text = "You step onto the hovering platform, surprised and relieved that it remains steady under your weight and does not go plummeting to the bottom of the shaft. Once again, as so often in the past, you have cause to marvel at the technology of your ancestors. Touching the lowest button, you activate the elevator and the platform glides gently down the shaft, depositing you in a circular chamber from which several doors lead off. All are closed. As you stand pondering which way to go, an automatic circuit engages and a synthesized voice speaks from a slot in the wall, enquiring your reason for being there. What will you answer?";

        Choices.clear();
        Choices.push_back(Choice::Base("Answer: Gaia", 297));
        Choices.push_back(Choice::Base("Answer: Gilgamesh", 318));
        Choices.push_back(Choice::Base("Answer: Du-En", 441));
        Choices.push_back(Choice::Base("Answer: The Heart of Volent", 383));

        Controls = Story::Controls::STANDARD;
    }
};

class Story256 : public Story::Base
{
public:
    Story256()
    {
        ID = 256;

        Text = "A hunter who makes a habit of tangling with bomeths will not live very long. Instead of grand heroics, you decide to track the beast to its lair. Crouching motionless until the moon rises, you see the bometh rouse itself and go loping away across the undulating hillocks of snow. You follow until you see it disappear into a snowdrift, whereupon you drop low and compose yourself for a long wait. Two or three hours go by. At last it emerges from the lair, sniffs the wind, and lumbers off in search of prey.\n\nOnce it is out of sight, you scramble over the snowdrift, pushing along a tunnel into a hollowed-out cavity where there are three small bomeths on a nest of moulted fur. Ignoring them, you turn your attention to the closely packed walls of the lair: the bometh's larder, where the beast has stored remains of previous kills. You dig out the carcass of a large fowl, which the icy cold has preserved well. Wrapping the flesh carefully, you make two FOOD PACKs.";

        Bye = "One of the young bomeths nips at your ankle. The teeth do not penetrate your boot, but it is a timely reminder that the parent might return any time. You squirm back to the open and hurry away.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Take = {Item::FOOD_PACK, Item::FOOD_PACK};

        Limit = 2;
    }

    int Continue(Character::Base &player) { return 298; }
};

class Story257 : public Story::Base
{
public:
    Story257()
    {
        ID = 257;

        Text = "Your fingers show signs of frostbite and you have dozen small bruises. You dose yourself with antibiotics and apply ointments and dressings from the MEDICAL KIT.\n\nYou RECOVER 1 Life Point.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::GAIN_LIFE(player, 1);
    }

    int Continue(Character::Base &player) { return 278; }
};

class Story258 : public Story::Base
{
public:
    Story258()
    {
        ID = 258;

        Text = "When he folds his hands across his chest and speaks in his mild whimsical way, Janus Gaunt puts you in mind of a priest. \"Vajra Singh is the lion among us,\" he says; \"proud, brave and noble -- but cross him, and you'll measure your remaining span in seconds. So, if Singh is a lion, Golgoth is a fox, agile and crafty. For Baron Siriasis's totem I would suggest the spider: a brooding, spiteful, sinister creature, always ready to cast its net.\"\n\n\"And Thadra Bey is evidently a tigress,\" you say, \"for I have never seen a woman so sleek and fierce. And Kyle Boche?\"\n\n\"Boche?\" He gives a snort of contempt. \"He is a jackal... no, an albatross around your neck, I think.\"\n\n\"And what about you, Janus Gaunt?\" breaks in a voice abruptly. You turn to see Kyle Boche sauntering out across the snow. He casts a hooded glance around the throng of silent xoms. \"Perhaps you're the worm of the group? The maggot that eats from within?\"\n\nGaunt turns on his heel and stalks away without reply, his xoms clustering along behind like sleepwalkers. Boche takes your arm and leads you back to the main square. \"You should be more careful,\" he chides. \"Gaunt might have murdered you out there and turned you into one of his undead. You're lucky I came along when I did.\"";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 104; }
};

class Story259 : public Story::Base
{
public:
    Story259()
    {
        ID = 259;

        Text = "You are in a dimly lit room with a number of smaller rooms leading off it. You guess that this was where the acolytes of the Volentine cult lived. Even after two hundred years, there are still signs of the rioting that led to the city's downfall. You clamber through a wreckage of broken furniture and toppled cupboards. Crumpled sheets of writing-plastic litter the floor.\n\nBoche picks up a book, ruffles the charred pages and drops it to the floor. \"There's nothing here to interest us,\" he says. \"Let's press on.\"";

        Choices.clear();
        Choices.push_back(Choice::Base("Insist on searching the wreckage", 242));
        Choices.push_back(Choice::Base("Go back to the antechamber and take either the passage to the left", 3));
        Choices.push_back(Choice::Base("... or the passage to the right", 128));

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        if (Character::VERIFY_SKILL(player, Skill::Type::ROGUERY))
        {
            Choices[0].Destination = 412;
        }
        else
        {
            Choices[0].Destination = 138;
        }
    }
};

class Story260 : public Story::Base
{
public:
    std::string PreText = "";

    Story260()
    {
        ID = 260;

        Bye = "You manage to break free with a sob of terror and stumble on to the doorway. Boche and the baron slam and bolt the door behind you, and you see it almost buckle off its hinges as the monster throws itself furiously against the other side. \"I hope that holds,\" you say to the others, \"or else we're insect food.\"";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        PreText = "There is the rattle of hard insectoid legs on the smooth floor, then something huge slams into you. It feels like a coiled monster with a body as hard as ebony. Striking out blindly, you feel a segmented eye squash under your fist, spurting out ichor. Then it seizes you in its mandibles and two scything blades are driven into your sides. You give a scream of agony. It feels as though the monster is trying to saw you in half.\n\n";

        auto DAMAGE = -6;

        if (Character::VERIFY_SKILL(player, Skill::Type::CLOSE_COMBAT))
        {
            PreText += "[CLOSE COMBAT] ";

            DAMAGE = -4;
        }

        if (Character::VERIFY_CODEWORD(player, Codeword::Type::TALOS))
        {
            PreText += "[Codeword: TALOS] ";

            DAMAGE++;
        }

        DAMAGE = Character::COMBAT_DAMAGE(player, DAMAGE);

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Points.";

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 281; }
};

class Story261 : public Story::Base
{
public:
    Story261()
    {
        ID = 261;

        Text = "You find Boche recovering from the blast. Despite a gash on his forehead, he is in good spirits. \"It worked!\" he says. \"I'd been barring that grenade all along, but the joke was that I didn't even know it myself. It was the only way to foil the baron's mind-reading you see.\"\n\n\"I don't understand.\"\n\nBoche spits out rock dust before explaining. \"I knew the baron was heading for Du-En and that he'd be the hardest foe I'd have to face, so I got myself hypnotized to forget that I was carrying a grenade. I had a post-hypnotic suggestion planted that I should use it at a key moment. He never knew what hit him, did he?\"\n\n\"A ruthlessly clever scheme.\"\n\nIf you intended any sarcasm, Boche fails to notice it. \"Thanks,\" he says. \"Now, let's get going and find the Heart.\"";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 175; }
};

class Story262 : public Story::Base
{
public:
    std::string PreText = "";

    Story262()
    {
        ID = 262;

        Bye = "As the smoke disperses you see that Golgoth had attached his gun to the wall magnetically and set it for remote-controlled fire. Retrieving it, he casts a wary glance at you. \"It's just up to us now,\" he says. \"We can make a shoot-out of it, and probably both die, or we can cooperate to get rid of that.\" And he jerks his thumb towards the ominously glittering Heart of Volent.\"";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        PreText = "A swishing in the air is followed by a noise like a knife being driven into an apple. You are startled to see a crossbow bolt protruding from Singh's eye. He is spun round by the impact, dead on his feet, raking the floor with a random blast from the mantramukta as he falls. Molten chunks of marble fly up, one of them striking you heavily across the left arm.\n\n";

        auto DAMAGE = -4;

        if (Character::VERIFY_CODEWORD(player, Codeword::Type::TALOS))
        {
            PreText += "[Codeword: TALOS] ";

            DAMAGE = -1;
        }

        Character::GAIN_LIFE(player, DAMAGE);

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Points.";

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 431; }
};

class Story263 : public Story::Base
{
public:
    Story263()
    {
        ID = 263;

        Text = "Golgoth gives you a bleak smile. His gun is aimed at your chest. \"So we can't destroy the damned thing after all,\" he says without relish. \"I certainly can't get it back to the States, so I guess that leaves one option.\"\n\n\"You can't...\" you say, shaking your head. \"It'll be the end of the world.\"\n\n\"The world is on its last legs anyway. No one thinks humanity will survive another century. Maybe I can make a better world.\"\n\nYou give him a withering look. \"You're just trying to justify yourself, Golgoth. You're like all the rest. It was power you wanted all along.\"\n\nHe shrugs. \"Just doing my job.\" And his gun spurts fire, killing you instantly.";

        Type = Story::Type::DOOM;

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }
};

class Story264 : public Story::Base
{
public:
    Story264()
    {
        ID = 264;

        Text = "Boche scrambles up to the ledge and goes through the pockets of one of the corpses. \"I don't like robbing the dead, but there might be some food,\" he mutters.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_SKILL(player, Skill::Type::ROGUERY))
        {
            return 328;
        }
        else if (Character::VERIFY_SKILL(player, Skill::Type::PARADOXING))
        {
            return 370;
        }
        else
        {
            return 306;
        }
    }
};

class Story265 : public Story::Base
{
public:
    Story265()
    {
        ID = 265;

        Text = "After reassuring the anxious scribe that you will do nothing to damage his hardware, you connect one of the laptops into the phone network. The antiquity of the modem will limit your ability to converse with Gaia, but it also means that the terminal you are using is safe from Gaia's resident hyperviruses, which are too complex to copy themselves across the primitive modem link.\n\nThe scribe watches in amazement as you type your questions. In this world of declining technology, what you are doing seems to him like magic.\n\n> HELLO, GAIA. PLEASE TELL ME ABOUT THE HEART OF VOLENT.\n> IT WAS FORMED AT THE BEGINNING OF TIME. THE PERSON WHO ATTUNES IT WILL HAVE POWER OVER EVERYTHING.\n> HOW CAN YOU HELP ME TO GET IT?\n> MY MIND IS NOT ALWAYS CLEAR. YOU WILL NEED AN ALLY. GO TO GIZA. SEEK GILGAMESH UNDER THE PYRAMID. HUMBABA WILL GIVE YOU ENTRY.\n> WHAT THEN?\n> UNKNOWN. THE FUTURE. A TABULA RASA.\n\nThe screen goes blank as the scribe reaches out and pulls the plug. \"Enough, please,\" he says firmly. \"I know you've assured me there is no risk, but bear in mind it is my livelihood that's at stake.\"\n\nResolving to establish a better link with Gaia when you have time, you leave the stall and head off along the street.\n\nYou gained the codeword HUMBABA.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::GET_CODEWORDS(player, {Codeword::Type::HUMBABA});
    }

    int Continue(Character::Base &player) { return 372; }
};

class Story266 : public Story::Base
{
public:
    Story266()
    {
        ID = 266;

        Text = "You identify each of the potions, describing its effects in detail. Malengin takes out a pad and eagerly scribbles down everything you can tell him. \"This will be invaluable,\" he says, so pleased that he agrees to sell to you at a discount.\n\nThe potion he calls the VIRID MYSTERY is easiest to identify. It is simply an antidote designed to reverse unwanted genetic changes. Luckily the other three potions are more useful.\n\nThe potion called the EXALTED ENHANCER costs 7 scads and will toughen your skin so that you gain 5 Life Points, even above your initial score. However it also slows your reflexes so that you must lose the [AGILITY] skill if you have it.\n\nThe MASK OF OCCULTATION costs 6 scads and gives the ability to alter your appearance and colouring.\n\nThe PEERLESS PERCEPTIVATE costs 4 scads and confers the ability to see in almost total darkness.";

        Choices.clear();

        Controls = Story::Controls::SHOP;
    }

    void Event(Character::Base &player)
    {
        Shop = {{Item::VIRID_MYSTERY, 3}, {Item::EXALTED_ENHANCER, 7}, {Item::MASK_OF_OCCULTATION, 6}, {Item::PEERLESS_PERCEPTIVATE, 4}};
    }

    int Continue(Character::Base &player)
    {
        if (!Character::VERIFY_SKILL(player, Skill::Type::STREETWISE) && !Character::VERIFY_ITEMS(player, {Item::Type::VADE_MECUM}))
        {
            return 25;
        }
        else
        {
            return 414;
        }
    }
};

class Story267 : public Story::Base
{
public:
    Story267()
    {
        ID = 267;

        Image = "images/filler3.png";

        Text = "Boche frowns in indignation and amazement. \"I cannot understand your attitude. I've worked very hard getting you this far, and now you propose to abandon me!\"\n\nYou thank the official for your ticket and then turn to Boche, saying, \"In the area of wilderness survival, you turned out to be long on opinion but short on expertise. Even so, I tolerated your company because you gave me to understand that your contacts would be useful once we reached civilization. However, I have not seen anything of you for the two days we've spent in Venis. Now you show up like a bad penny, having squandered your money on fancy gear such as that new BARYSAL GUN, and you have the audacity to expect me to pay for your passage. Boche, you are an idiot.\"\n\nThough taken aback, he is not at a loss for words. \"I expected a little more support from you. Have you forgotten that I started out with a gesture of comradeship by paying your bill at the Etruscan Inn before I knew a thing about you?\"\n\n\"That, more than anything else, should have warned me off associating with you. It's the typical con-man's opening gambit.\" You give a bored sigh. \"Farewell, Boche.\"\n\nYou walk off towards the seafront before he can say anything else.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::REMOVE_CODEWORD(player, Codeword::Type::DIAMOND);
    }

    int Continue(Character::Base &player) { return 246; }
};

class Story268 : public Story::Base
{
public:
    Story268()
    {
        ID = 268;

        Text = "You travel upriver. Flurries of sleet sweep out of the evening sky. Veering west, you steer towards the pyramids of Giza where they stand outlined against the feeble afterglow of sunset. The Sphinx lies huddled with banks of snow along its stone flanks, its face ravaged by frostbite. You set the sky-car down and trudge across the icy plain towards those ancient ruins.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 440; }
};

class Story269 : public Story::Base
{
public:
    Story269()
    {
        ID = 269;

        Image = "images/filler1.png";

        Text = "You catch on at once. The Fijian obviously works for the person who originally owned your ID card, and rushed up here in search of his boss, only to find you instead. \"There was another person here,\" you glibly tell him, \"who suffered a heart attack just minutes ago and was taken on a stretcher to the medical lounge. Could that have been your boss?\"\n\nHis jaw drops, then he races out and stabs anxiously at the elevator button. You join him and politely ask for the ground floor. Watching him fidget impatiently as the elevator descends, you say, \"Don't worry, the Society has excellent medical facilities -- the best in Kahira. I'm sure your boss will be all right.\"\n\nThe elevator stops at the floor for the medical lounge and the Fijian gets out. You continue down to the lobby and leave without delay, before he discovers he's been lied to.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 311; }
};

class Story270 : public Story::Base
{
public:
    Story270()
    {
        ID = 270;

        Text = "You find the missing bodyguard, Goro, lying only fifty metres from the ice cave where you took shelter. He froze to death on his hands and knees, crawling in the teeth of the blizzard. Snow is piled around the body. Ice cakes his features, locked rigid in an expression of stubborn intensity. The other two, who may be his brothers for all you know, betray no emotion at the sight. Of course, they must have realized he couldn't have survived. \"Shall we bury him, boss?\" says one.\n\nShandor looks at the sky. Dusk is already descending along the edge of the valley, shadows creeping like blots of soot across the crisp white snow. He considers, then gives a curt nod back towards the crevasse. \"Toss the body down into the cave -- it's as good a tomb as any. I want to be off this glacier by nightfall.\"\n\nAfter sliding Goro's body down into the crevasse, the four of you trudge on in silence. By the time you crest the shoulder of bare black rock bordering the glacier, stars begin to glimmer between the low banks of cloud. Something impels you to glance back. You see the last of the sunlight trickle off the world. In the same instant, a slurry of thick purple steam gushes out of the crack in the ice far behind. You rub your eyes and look again. Now there is nothing. It must have been a trick of the fading light.\n\nShandor's bodyguards have found a spar of rock to give cover from the night winds. They heat it with blasts from their BARYSAL GUNs, leaving the rock surface pleasantly warm when you hunker down beside it to eat the rations that Shandor shares out.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_ITEMS(player, {Item::Type::SHORT_SWORD}))
        {
            return 291;
        }
        else
        {
            return 312;
        }
    }
};

class Story271 : public Story::Base
{
public:
    Story271()
    {
        ID = 271;

        Image = "images/filler2.png";

        Text = "A man emerges from a low building and stands in a wary posture as he watches you approach. He licks his lips nervously and shifts his grip on the shovel he is holding, but your impression is that he is nervous rather than hostile. When you smile and hold out your hand, he relaxes with a shrug and tosses the shovel aside, introducing himself as Portrin Fax. He is a loose-limbed fellow, slender as a starved mantis, with fretfully pursed lips and wet blinking eyes.\n\nHis dwelling is paved with umber and grey tiles and show signs of having once been a transit terminal of some kind. The air is cool. Panels along the side wall shed a brisk white light. The only items of furniture are rickety frameworks of wood with ragged furs stretched over them. Fax waves you to what he describes as a chair and pours drinks. You take the mug he offers, wincing at the powerful fumes. \"My own liquor,\" he explains. \"I brew it by mixing herbs into a tank of cleaning fluid.\"\n\nYou pour the drink away when he isn't looking. \"You live alone here?\"\n\nHe sits, folds his arms, only to leap up and stride around the room. Company clearly makes him nervous. \"A hermit, that's old Fax,\" he says. \"In the outside world I was a misfit, but here I live like the Sun King. The city has generators which supply light and keep the air cooled.\" He giggles. \"Who would dream, in this time of the fimbulwinter, that a man might wish for a cool breath of air? But here in Lyonesse it is necessary.\"";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_SKILL(player, Skill::Type::LORE))
        {
            return 313;
        }
        else
        {
            return 356;
        }
    }
};

class Story272 : public Story::Base
{
public:
    Story272()
    {
        ID = 272;

        Text = "You are held at gunpoint while your arms are wrenched behind your back and securely tied. You test the bonds, but these are men who have learned not to be careless. One of them rummages through your belongings, then jerks his thumb contemptuously towards you. \"Are we keeping an extra mouth to feed, then, Snarvo?\"\n\nThe man with the knife sneers. \"Not unless you want yourself a pet, mate.\"\n\n\"Just get on with it, Snarvo,\" mutters the third man, tucking the gun back into his belt. \"And don't get blood on the rug.\"\n\nSnarvo steps forward, an unpleasant grin creasing his weather-hardened features, and his knife brings your life to a sticky end.";

        Type = Story::Type::DOOM;

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }
};

class Story273 : public Story::Base
{
public:
    std::string PreText = "";

    Story273()
    {
        ID = 273;

        Bye = "The only other people you spy are a group of Hamadan ascetics on their shaggy camels. They pay no attention to you, as their creed insists that they disdain all outsiders.\n\nAdjusting their white-and-tan turbans with cold contempt, they go bounding off across the lone and level snows.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        PreText = "The sun shows itself as a golden blur in the east, smoky and cheerless under an overhang of sullen cloud. The air grows colder as you walk, the warm geysers of the pass soon lost to view in the overcast.\n\nThe way rises into foothills, then the brooding hulks of the Atlas Mountains. Long shoulders of rock lie bare to the sky: the carcass of old Earth, gaunt under its pall of snow. Crags scrape up past you on all sides, stern and daunting, like pillars supporting a canopy of snow-bloated clouds.\n\nA frosty halitus hangs over the high hills. You find the landscape both magnificent and dispiriting and are glad when, after another two days, you begin to descend towards a broad windswept plain.\n\n";

        if (!Character::VERIFY_SKILL(player, Skill::Type::SURVIVAL) && !Character::HAS_FOOD(player, 0))
        {
            Character::GAIN_LIFE(player, -1);

            PreText += "You LOSE 1 Life Point.";
        }
        else
        {
            if (Character::VERIFY_SKILL(player, Skill::Type::SURVIVAL))
            {
                PreText += "[SURVIVAL] You manage to locate a few rodents in their burrows; once wind-dried, the meat is hard but acceptable.";
            }
            else
            {
                Character::CONSUME_FOOD(player, 1);

                PreText += "You consumed 1 FOOD PACK.";
            }
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 393; }
};

class Story274 : public Story::Base
{
public:
    Story274()
    {
        ID = 274;

        Text = "You must choose what to donate.";

        Bye = "With broad witless smiles, the priestess and her followers accept your donation. \"Thank you pilgrim,\" says the priestess. \"The goddess will watch over you.\"\n\n\"May she bring you as much good fortune as I believe you deserve,\" you reply ambiguously. \"Now, if you'll excuse me, I'd like to get some sleep.\"\n\nYou settle down among the litter, finding sleep hard to come by now that you are seething with fury at having been robbed. As daylight begins to show as a pearly gleam beyond the city spires, you rouse yourself and head towards the bazaar";

        Choices.clear();
        Choices.push_back(Choice::Base("Give them all your money", 333, Choice::Type::LOSE_MONEY, 0));
        Choices.push_back(Choice::Base("Give them two of your possessions", 333, Choice::Type::GIVE, 2));

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Choices[0].Value = player.Money;
    }
};

class Story275 : public Story::Base
{
public:
    Story275()
    {
        ID = 275;

        Image = "images/riza.png";

        Text = "Riza conducts you along a passage which leads out into what seems at first like open air. Seeing you look around in amazement, he laughs and points across a lawn towards a group of buildings. You now see that the horizon seems to rise upwards and the further you look, curving over into the sky until it is lost behind a haze of light directly overhead. To either side of you, only a few hundred metres away, unimaginably vast slabs of cliff reach off into the sky.\n\n\"Did you expect a cramped space-station in the ancient style?\" says Riza. \"We have enlarged al-Lat over the centuries until it is what you see today: a cylinder some two kilometres wide and half a kilometre long. Rotation provides us with gravity, the sun's rays with light and heat.\"\n\nYou have to admit that it is more comfortable than Earth is these days. Riza takes you to his flyer, and soon you are dropping towards Earth's atmosphere. You descend just ahead of the dawn, touching down on the shore of the Sea of Reeds at a fishing village called Sudan. You bid Riza farewell and stand watching as the flyer climbs back up into the early morning sky.";

        Choices.clear();
        Choices.push_back(Choice::Base("Do some shopping", 101));
        Choices.push_back(Choice::Base("Set out for Du-En without delay", 234));

        Controls = Story::Controls::STANDARD;
    }
};

class Story276 : public Story::Base
{
public:
    Story276()
    {
        ID = 276;

        Text = "The hovering platform conveys you to a circular landing marked \"Research Level.\" You step off and conduct a quick exploration of the rooms on this floor. In a long hall you find lockers containing COLD-WEATHER clothing, FOOD PACKs and MEDICAL KITs -- many more than any one person would ever need. You rip open one of the food packs. The contents are still fresh.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Take = {Item::COLD_WEATHER_SUIT, Item::FOOD_PACK, Item::MEDICAL_KIT, Item::COLD_WEATHER_SUIT, Item::FOOD_PACK, Item::MEDICAL_KIT, Item::COLD_WEATHER_SUIT, Item::FOOD_PACK, Item::MEDICAL_KIT, Item::COLD_WEATHER_SUIT, Item::FOOD_PACK, Item::MEDICAL_KIT, Item::COLD_WEATHER_SUIT, Item::FOOD_PACK, Item::MEDICAL_KIT, Item::COLD_WEATHER_SUIT, Item::FOOD_PACK, Item::MEDICAL_KIT, Item::COLD_WEATHER_SUIT, Item::FOOD_PACK, Item::MEDICAL_KIT, Item::COLD_WEATHER_SUIT, Item::FOOD_PACK, Item::MEDICAL_KIT};

        Limit = 24;
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_ITEMS(player, {Item::Type::LITTLE_GAIA}))
        {
            return 14;
        }
        else
        {
            return 36;
        }
    }
};

class Story277 : public Story::Base
{
public:
    std::string PreText = "";

    Story277()
    {
        ID = 277;

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Type = Story::Type::NORMAL;

        PreText = "The bometh sits like a sphinx atop the rise, head lifted to the star-filled sky. Wisps of breath escape its nostrils. It seems oblivious of your approach as you slowly work your way around and then stalk closer from downwind. But as you close the last few metres and leap from hiding, it rises with a languid growl and turns into long scything fangs to slash at you. Only now, as it rears to grapple, do you get a sense of the bometh's size and power. It is enough to make your knees go weak. The beast must weigh half a tonne!\n\nThe battle is ferocious and bloody.";

        if (Character::VERIFY_SKILL(player, Skill::Type::CLOSE_COMBAT))
        {
            auto DAMAGE = -5;

            DAMAGE = Character::COMBAT_DAMAGE(player, DAMAGE);

            PreText += "\n\n[CLOSE COMBAT] You LOSE " + std::to_string(-DAMAGE) + " Life Points.";
        }
        else
        {
            Type = Story::Type::DOOM;

            player.Life = 0;
        }

        if (player.Life <= 0)
        {
            PreText += "\n\nYou did not stand a chance: the bometh sinks its teeth deep into your chest and proceeds to rip out your lungs.";
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 341; }
};

class Story278 : public Story::Base
{
public:
    Story278()
    {
        ID = 278;

        Text = "The sun lurches into view above the city walls -- a disk as pale as watered milk. Out in the main square, the others are preparing to set off into the ruins. You see Golgoth clipping on his weapon-belt as he chats to the Gargan sisters, Apparently they have agreed to explore as a team. Surprisingly, Gaunt and Thadra Bey have also formed a group. Baron Siriasis and Vajra Singh both opt to search for the Heart alone.\n\nVajra Singh oversees the drawing of lots. \"We shall set off into the ruins at fifteen-minute intervals determined by the lots,\" he tells others. \"You can explore individually or in groups, as you prefer. Remember that the truce applies only here in the open. If groups encounter each other while in the catacombs below the city, they must negotiate or else do battle.\"";

        Choices.clear();
        Choices.push_back(Choice::Base("Descend into the catacombs", 385));
        Choices.push_back(Choice::Base("Rest for a day", 363));

        Controls = Story::Controls::STANDARD;
    }
};

class Story279 : public Story::Base
{
public:
    Story279()
    {
        ID = 279;

        Image = "images/filler1.png";

        Text = "\"You don't have what it takes, Gaunt,\" you as you gaze up at the constellations. \"You're too reflective, too prone to see all sides of an issue. You lack that heart of ice that makes a ruthless man successful.\"";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_CODEWORD(player, Codeword::Type::ENKIDU))
        {
            return 299;
        }
        else
        {
            return 321;
        }
    }
};

class Story280 : public Story::Base
{
public:
    std::string PreText = "";

    Story280()
    {
        ID = 280;

        Bye = "You stab desperately at the button. As the door closes, you see Thadra Bey die in a hail of laser-blasts. If you cannot figure out the answers that the computer wants to hear, you will be next.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        PreText = "The elevator door slides open. You are confronted by one of the hover-droids, which immediately open fire. Although you twist to one side, the blast rips through your arm, burning you to the bone.\n\n";

        auto DAMAGE = -6;

        if (Character::VERIFY_ITEMS(player, {Item::Type::SPECULUM_JACKET}))
        {
            PreText += "[Item: SPECULUM JACKET] ";

            DAMAGE = -4;
        }

        Character::GAIN_LIFE(player, DAMAGE);

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Points.";

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 301; }
};

class Story281 : public Story::Base
{
public:
    Story281()
    {
        ID = 281;

        Text = "You enter a lofty room plunged in gloom. Boche flicks his torchlight around and it falls on a curious sight. The light seems to spill slowly, like a puddle of oil, through a zone several metres across. In the the middle stands a man in old-fashioned military dress. Beside him on the floor lies a metal globe about the size of an egg, covered with glowing studs.\n\n\"It is a stasis bomb, I believe,\" announces the baron in his stern clipped tones. \"Watch.\"\n\nHe glides off, skirting the zone where the torch beam slowed down. You see him drifting around the far perimeter. But then suddenly he comes back into view around the edge of the zone, even though you can still see his image moving beyond it.\n\n\"The stasis bomb slows down time in a two-mere radius,\" explains the baron. \"Like takes several seconds to cross the zone, which is why you can still see my image floating on the other side.\"\n\nAs you watch, the image moves around the zone, disappearing like a ghost as it reaches the edge. \"How long has that man been frozen there?\" wonders Boche.\n\n\"Probably since the fall of Du-En. Almost two hundred years. That period will have seemed to him like only a few seconds.\"";

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Choices.clear();

        if (!Character::VERIFY_SKILL(player, Skill::Type::LORE))
        {
            Choices.push_back(Choice::Base("Try to free the man using either a charged BARYSAL GUN", 345, {Item::BARYSAL_GUN}));
            Choices.push_back(Choice::Base("[PARADOXING] Free him using a PSIONIC FOCUS", 366, Skill::Type::PARADOXING));
            Choices.push_back(Choice::Base("Leave him frozen and continue on your way", 388));
        }
    }

    int Continue(Character::Base &player) { return 323; }
};

class Story282 : public Story::Base
{
public:
    Story282()
    {
        ID = 282;

        Text = "You take a step back and shoot again as the baron's brain swoops closer. This time you use your own psionic power to break through the force field. The blast strikes the brain dead centre. There is a horrible psychic shriek as it falls to the floor and shrivels. You press your hands to your ears, but you cannot shut out those death-cries -- cries that will haunt your dreams forevermore.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::FIRE_BARYSAL(player, 1);
    }

    int Continue(Character::Base &player) { return 261; }
};

class Story283 : public Story::Base
{
public:
    Story283()
    {
        ID = 283;

        Text = "A merchant takes you to a vault tunnelled into the block of one of the great piazzas. Passing through a door guarded by two burly men with iron batons, you wait in a short corridor lit by a flickering light panel. At last a steel door opens at the far end and you walk through into the merchant's storeroom. \"I will also buy such items,\" the merchant tells you, \"at half the price I've quoted for sale.\"\n\n\"Why should I wish to sell?\"\n\nHis lips curls in an inscrutable half-smile. \"You have come to Venis for the ferry, I presume. You'll need money for your ticket.\"";

        Choices.clear();

        Controls = Story::Controls::BUY_AND_SELL;
    }

    void Event(Character::Base &player)
    {
        Shop = {{Item::MAKE_BARYSAL_GUN(6), 16}, {Item::PSIONIC_FOCUS, 18}, {Item::POLARIZED_GOGGLES, 8}, {Item::FLASHLIGHT, 10}, {Item::GAS_MASK, 10}, {Item::STUN_GRENADE, 8}, {Item::KNIFE, 4}};

        Sell = {{Item::MAKE_BARYSAL_GUN(6), 8}, {Item::PSIONIC_FOCUS, 9}, {Item::POLARIZED_GOGGLES, 4}, {Item::FLASHLIGHT, 5}, {Item::GAS_MASK, 5}, {Item::STUN_GRENADE, 4}, {Item::KNIFE, 2}};
    }

    int Continue(Character::Base &player) { return 261; }
};

class Story284 : public Story::Base
{
public:
    Story284()
    {
        ID = 284;

        Text = "You both draw your guns at the same time.";

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Choices.clear();

        if (!Character::VERIFY_SKILL(player, Skill::Type::SHOOTING))
        {
            Choices.push_back(Choice::Base("Aim at his chest", 348));
            Choices.push_back(Choice::Base("Aim at his throat", 369));
            Choices.push_back(Choice::Base("Aim between his eyes", 391));
        }
    }

    int Continue(Character::Base &player) { return 327; }
};

class Story285 : public Story::Base
{
public:
    std::string PreText = "";

    Story285()
    {
        ID = 285;

        Bye = "Morning comes as a sullen grey intrusion of light across the cloud draped-sky. Stamping the circulation back into your weary limbs, you set off towards Venis.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        PreText = "Night overtakes you on the slope of the mountain. You have to scoop snow to make a rudimentary shelter, and even so the wind numbs you to the core of your bones.\n\n";

        auto DAMAGE = -2;

        if (Character::VERIFY_SKILL(player, Skill::Type::SURVIVAL))
        {
            PreText += "[SURVIVAL] ";

            DAMAGE = -1;
        }

        Character::GAIN_LIFE(player, DAMAGE);

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Points.";

        if (player.Life > 0)
        {
            if (Character::VERIFY_SKILL(player, Skill::Type::SURVIVAL))
            {
                PreText += "\n\n[SURVIVAL] You were able to construct an effective shelter.";
            }
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 301; }
};

class Story286 : public Story::Base
{
public:
    Story286()
    {
        ID = 286;

        Text = "In the hotel bar, you strike up conversation with a tall sloe-eyed woman called Thadra Bey. Her accent tells you she is a native of al-Lat, the huge space colony in Earth-Moon orbit. \"I hear that conditions on al-Lat are vastly better than on Earth,\" you remark over a glass of synthash. \"But perhaps you are down on business?\"\n\nShe sips at her drink. \"What I seek is only available here. ON al-Lat, the science of genetic engineering cannot be practised for fear of infecting the colony with a deadly plague. Consequently, the only improvements that can be made to the human body are by mechanical means.\"\n\n\"Your own body appears not to need any improvement,\" you put in politely.\n\nShe continues as though you had not spoken. \"A friend of mine had an implant along her optic nerve so that she could tell the time merely by blinking. The device became stuck permanently on, so that she could not sleep because of seeing lighted numerals in front of her eyes. Worse, it was three minutes fast.\" She finishes her drink, waving away the waiter who tries to refill it. \"Here on Earth the body can be enhanced more efficiently using genetic retroviruses. Specifically, I seek a man called Malengin who is said to trade in such things.\"";

        Choices.clear();
        Choices.push_back(Choice::Base("[STREETWISE] Help her", 307, Skill::Type::STREETWISE));
        Choices.push_back(Choice::Base("Consult a VADE-MECUM", 307, {Item::VADE_MECUM}));
        Choices.push_back(Choice::Base("Leaver her alone", 329));

        Controls = Story::Controls::STANDARD;
    }
};

class Story287 : public Story::Base
{
public:
    Story287()
    {
        ID = 287;

        Text = "This retrovirus confers the ability to see in almost total darkness.\n\nYou gained the codeword SCOTOPIC.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::PEERLESS_PERCEPTIVATE(player, false);
    }

    int Continue(Character::Base &player) { return 434; }
};

class Story288 : public Story::Base
{
public:
    Story288()
    {
        ID = 288;

        Text = "Perhaps one of your skills could help you secure a place on the ferry.";

        Choices.clear();
        Choices.push_back(Choice::Base("Make use of [PILOTING]", 309, Skill::Type::PILOTING));
        Choices.push_back(Choice::Base("Rely on [ROGUERY]", 331, Skill::Type::ROGUERY));
        Choices.push_back(Choice::Base("Resort to [CUNNING]", 226, Skill::Type::CUNNING));
        Choices.push_back(Choice::Base("Otherwise", 352));

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        if (Character::VERIFY_CODEWORD(player, Codeword::Type::DIAMOND))
        {
            Character::REMOVE_CODEWORD(player, Codeword::Type::DIAMOND);
        }
    }
};

class Story289 : public Story::Base
{
public:
    Story289()
    {
        ID = 289;

        Image = "images/du-en.png";

        Text = "From a cruising height of thirty metres, the Ice Wastes resemble a sea of luminous snow broken by islands of exposed black rock. The wind shrieks across the land without respite, driving swathes of powdery snow that has carved strange shapes from the surrounding cliffs. You see few signs of life. This is one of the most desolate regions of Earth. The daylight is bleakly pale, the dusk as blue as smoke, and the night sky is filled with a thousand stars scintillating in the gaps between colossal crags of cloud.\n\nDays pass like a blur. And then at last, glowering on the horizon, you see the grim black walls of a ruined city, You have arrived at Du-En.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_CODEWORD(player, Codeword::Type::DIAMOND))
        {
            Character::REMOVE_CODEWORD(player, Codeword::Type::DIAMOND);

            return 191;
        }
        else
        {
            return 213;
        }
    }
};

class Story290 : public Story::Base
{
public:
    Story290()
    {
        ID = 290;

        Text = "You dive into the elevator and jab the button. The Fijian comes charging towards you. \"Stop! Imposter!\" he bellows. You are relieved that the doors close before he can get to you.\n\nJust as you are about to take the elevator down to the ground level, it occurs to you that he might call ahead to the lobby and have you intercepted by security guards.";

        Choices.clear();
        Choices.push_back(Choice::Base("Risk it", 453));
        Choices.push_back(Choice::Base("Go up to the roof instead", 376));

        Controls = Story::Controls::STANDARD;
    }
};

class Story291 : public Story::Base
{
public:
    Story291()
    {
        ID = 291;

        Text = "You are woken by the touch of fingers on your face. They feel like icicles. You look up, and in the silver lustre of the moonlight you see a man bending over you. You open your mouth to scream, but the only sound you can muster is a stifled croak of dread. It is the same man you saw frozen inside the glacier!\n\nYou press back against the rock and get your legs under you, slowly sliding to a standing position. The man watches you with an uncanny expression of capricious menace, like a cat confronted with an odd-looking canary.\n\nA severed arm lies by your feet in the snow. A head, half buried a short distance off, stares with glazed eyes. You look around in mounting horror. Shandor and his men have been brutally murdered while you slept. The strange man smiles, but not in a human way. He looks like a living shadow. Amazing that he doesn't freeze to death in those thin overalls. His hand comes up, a flicker of ivory in the the darkness. \"The sword,\" he murmurs. \"My sword. Return it to me.\"";

        Choices.clear();
        Choices.push_back(Choice::Base("[CLOSE COMBAT] Attack", 398, Skill::Type::CLOSE_COMBAT));
        Choices.push_back(Choice::Base("[PARADOXING] Use a PSIONIC FOCUS", 377, Skill::Type::PARADOXING));
        Choices.push_back(Choice::Base("[SHOOTING] Shoot", 355, Skill::Type::SHOOTING));
        Choices.push_back(Choice::Base("Otherwise", 419));

        Controls = Story::Controls::STANDARD;
    }
};

class Story292 : public Story::Base
{
public:
    Story292()
    {
        ID = 292;

        Text = "Day and night have no meaning in Lyonesse, forever drenched in its own block of humid sunshine. You find a place to rest, awaking to find the golden beams at a low slant. A tangible darkness hovers between the thick fleshy leaves. Insects scurry and swarm. You have been bitten by gnats while you slept, which causes you to scratch furiously as you make ready to seat out. The itching of insect-bites is an unfamiliar sensation in the latterday Ice Age. So is the lassitude brought on by the heat. You are now almost eager to leave the hot jungle region behind.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 420; }
};

class Story293 : public Story::Base
{
public:
    Story293()
    {
        ID = 293;

        Text = "The gondo meets you early the next day outside your hotel and leads you east out of town. Leaving the narrow understreets and the elegant crumbling mansions on their high stone platforms, you pass first through the warren of wooden buildings lining the outskirts of the town. The road becomes a hard slippery tract of frozen mud. Now you are surrounded by the ramshackle dwellings of Venice's slums: crude hovels with roofs of moss and scavenged fiberglass. The inhabitants are crippled beggars you peer out in fear as you pass.\n\nAs you leave the city behind, the temperature drops severely. A harsh wind howls into your face, scattering drifting veils of snow off the rolling terrain ahead. You trudge on, hood pulled tight across your face, following the gondo and wondering whether you would not have done better to stay by the fireside at your hotel.\n\nThe gondo stops and points at a white hump on the landscape. You see a wide door of black metal in the hillside. Advancing, you realize that the hillock is probably a building buried deep under snow and ice. The door intrigues you, since it must be heated internally to be clear of ice itself. You try pushing, soon discovering that it rolls up on smooth bearings. Dim green light shines out from inside.\n\nThe gondo refuses to come in with you. \"It's haunted,\" he tells you again. \"That's why even the wild animals don't shelter by the door, despite the warmth.\"\n\nScowling at him for his timidity, you step inside. You are in a corridor lit by green lights in the ceiling. The walls and floor are black.\n\nA metallic ringing noise comes out of the gloom. Your nerves are instantly on edge. Something is scuttling through the shadows at the far end of the corridor.";

        Choices.clear();
        Choices.push_back(Choice::Base("[SHOOTING] Fire at it", 373, Skill::Type::SHOOTING));
        Choices.push_back(Choice::Base("Retreat from here and return to town", 395));
        Choices.push_back(Choice::Base("Advance for a closer look", 416));

        Controls = Story::Controls::STANDARD;
    }
};

class Story294 : public Story::Base
{
public:
    std::string PreText = "";

    Story294()
    {
        ID = 294;

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        PreText = "You stand outside the inn in the grey predawn, waiting for the Gargan twins. They join you as the sun exudes a wash of white gold in the eastern sky. Clouds seep across the heavens like clots of iron filings. The warmth of the geysers attracts swarms of tiny midges which dance in the air as fine as siftings of dust.\n\nGargan XIII comes over. \"We are ready to set out. You, like us, are bound for the Sahara?\" As she speaks, she absent-mindedly plucks a scatter of midges out of the air and rolls them to death between her palms.\n\nThere is no point in pretending. These two are sharp enough to guess the truth. \"We're all bound for Du-En, I believe. Presumably for the same purpose.\"\n\n\"Our goals may not accord as closely as you imagine,\" says Gargan XIV as she joins you. \"Still, for the time being we are allies.\"\n\nYou set out. The way rises first into foothills, then through the brooding hulks of the Atlas Mountains. Long shoulders of rock lie bare to the sky. The sad carcass of old Earth is gaunt under its shroud of snow. Crags scrape up past you on all sides, stern and daunting, like pillars supporting a canopy of snow-dark cloud. As you walk, you ponder the uneasy alliance between yourself and the Gargan twins. They may be figuring that it is useful to have a third person in their party, since the Saharan Wastes are direly inhospitable. On the other hand, maybe they just intend to slay you when you are off guard.\n\nYour suspicions are settled on the morning of the third day, when you wake to discover them gone -- along with most of your supplies. They have left only your money and clothing.";

        Character::LOSE_POSSESSIONS(player);

        if (!Character::VERIFY_SKILL(player, Skill::Type::SURVIVAL))
        {
            Character::GAIN_LIFE(player, -1);

            PreText += "\n\nYou LOSE 1 Life Point.";
        }

        if (player.Life > 0)
        {
            if (!Character::VERIFY_SKILL(player, Skill::Type::SURVIVAL))
            {
                PreText += "\n\nIt will be hard to forage in this desolate wilderness.";
            }
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 393; }
};

class Story295 : public Story::Base
{
public:
    Story295()
    {
        ID = 295;

        Text = "\"The goddess Gaia!\" you cry in a voice of rapture. \"Blessed is she, exalted above all others. Give praise to Gaia!\"\n\n\"Praise be to Gaia!\" they reply like robots.\n\nThe priestess fixes you with a gimlet stare. \"What do you know of our goddess?\"\n\nYou return a beatific smile. \"She has given me a holy mission. That is why I have come to Kahira, and if she had not guided me on every step of the journey I would never have survived. It is a miracle indeed!\"\n\nOne of the others is indignant to hear you say this. \"Such lies!\" he fumes. \"It is blasphemy! You must be punished for your blasphemy, unbeliever --\"\n\nHe starts forward to kick you, but the priestess hold up her hand. She is watching you with a faraway look. \"No... I sense it is the truth.\"\n\nYou stifle a sigh or relief. Either it's your honest face, or she has some telepathic ability. You give them a broad grin. \"It's perfectly true. Leave me in peace, or Gaia will curse you. Better yet, give donations to help me in my holy cause.\"\n\nWith peevish ill-grace they hand over some cash, then shuffle off in search of someone else to victimize. At dawn you set off to the bazaar whistling a jaunty tune.\n\nYou RECEIVE 10 scads.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::GAIN_MONEY(player, 10);
    }

    int Continue(Character::Base &player) { return 333; }
};

class Story296 : public Story::Base
{
public:
    Story296()
    {
        ID = 296;

        Text = "You round on him in shock. \"And the Earth will be left to die?\"\n\n\"It is not our concern,\" he replies with a shrug. \"We are an independent state. The people of Earth must solve their own problems.\"\n\nYou give a hollow laugh. \"You are human! How can you abandon the planet that gave you life? Too look on uncaring as she expires under shrouds of ice is inexcusable. If there were a disaster which made al-Lat uninhabitable, would you go once the Earth is dead? Your indifference is short-sighted and stupid. You people are like survivors of a shipwreck, clinging to broken timbers in a stormy sea, gawping impassively while your ship sinks.\"\n\nHe thinks for a long moment, then slowly nods. \"Your arguments are passionate indeed.\" He goes off and returns with a metal box-shaped device the size of a book. \"This is my pet project. I call it Little Gaia. It is a miniature artificial brain programmed with Gaia's original personality, when she was still sane.\" He presses it into your hands. \"Take it. It is the only aid I can give you, except to wish you luck. Now I must go to find Captain Baihaqi, who should be ready to take you home.";

        Choices.clear();
        Choices.push_back(Choice::Base("Use the computer to contact Gaia", 360));
        Choices.push_back(Choice::Base("Wait until Riza al-Baihaqi comes for you", 382));

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        if (Character::VERIFY_SKILL(player, Skill::Type::CYBERNETICS))
        {
            Choices[0].Destination = 339;
        }
        else
        {
            Choices[0].Destination = 360;
        }
    }
};

class Story297 : public Story::Base
{
public:
    Story297()
    {
        ID = 297;

        Text = "\"The Global Artificial Intelligence Project is conducted on the research level directly above,\" replies the voice. \"Current status: on hold. Please step onto the stratum disk.\"\n\nIn awe of the science that could build a device capable of answering with such intelligence, you do as you are told. The platform ascends to the next level.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 276; }
};

class Story298 : public Story::Base
{
public:
    Story298()
    {
        ID = 298;

        Text = "The next day, with snow glittering like diamond dust under pale gold sunshine, you see a shimmering haze in the middle distance. Approaching, at first you think the oasis ahead is a trick of your imagination. Dwarf conifers surround a steaming pool fringed with moss-covered rocks. Long-beaked birds peck at the ground for grubs. Then you catch a sulphuric tang on the air and realize that the warm updraught here must be rising from fissures deep underground. It feels as unreal as a dream when you pass between the foliage and settle yourself on a slab of rock, pulling off your clothing to enjoy your first experience of warmth in many days.";

        Choices.clear();
        Choices.push_back(Choice::Base("Rest here to recover your strength", 405));
        Choices.push_back(Choice::Base("Press on without delay", 426));

        Controls = Story::Controls::STANDARD;
    }
};

class Story299 : public Story::Base
{
public:
    Story299()
    {
        ID = 299;

        Text = "Gaunt glances at your automaton bodyguard. \"Perhaps you're right,\" he says wistfully. Without another word, he turns and wanders back towards the camp, the xoms dutifully following like silent sleepwalkers.\n\nYou watch the constellations wheel imperceptibly overhead. Half an hour passes and you begin to notice the cold It is late. Trudging back to the main square, you see Golgoth checking his weaponry for tomorrow's adventure. Boche waves to you from beside the fire, where he has brewed up some tea.";

        Choices.clear();
        Choices.push_back(Choice::Base("Join Golgoth", 126));
        Choices.push_back(Choice::Base("Join Boche", 104));
        Choices.push_back(Choice::Base("Go off to bed", 192));

        Controls = Story::Controls::STANDARD;
    }
};

class Story300 : public Story::Base
{
public:
    Story300()
    {
        ID = 300;

        Image = "images/singh.png";

        Text = "Vajra Singh stands like a demon in the snow, a great slab of a man with a darkly saturnine face and eyes like a lion's. His silver anti-laser armour gleams in the weak sunshine. He watches you all for a moment, scanning you as he might size up a pack of hyenas. Then he raises his huge hand-cannon and touches the fire button. The air is riven by a sound like a volcano erupting as a torrent of blistering white plasma streams from the cannon, juddering across the walls above the and scattering chunks of smouldering masonry far out into the square. Everyone dives for cover. Still unleashing the barrage of crackling energy, Vajra Singh turns and directs it at the foundations of a building across the square. The heavy stone walls burst apart, and slowly the whole facade collapses to leave only a burnt-out shell.\n\nVajra Singh touches a button on the side of the cannon and the blast cuts out. The roar dies to a low hum. You notice a small red light start to glow as the cannon powers down. \"That weapon is the mantramukta,\" Janus Gaunt whispers in your ear.\n\n\"This squabbling must cease!\" snarls Vajra Singh as the others come out from hiding. \"We must work together in the short term or we stand little chance of penetrating as far as the Heart. Is that clear?\"\n\nOn the side of the mantramukta cannon, a green light winks on. Now you know that it takes a few seconds to rebuild its power after each blast. You see that Chaim Golgoth has noticed this too.\n\nSeeing that the only reply is sullen silence, Vajra Singh goes on, \"Here in the main square we shall remain in a state of truce. While exploring beneath the ruins, this truce is not in force. Also, no underlings or servants are to be taken on expeditions. This is a contest to see which of us deserves the ultimate power, not who is able to hire the most bodyguards.\"\n\nJanus Gaunt glances from face to face, then takes it on himself to answer. \"We agree to these terms.\"\n\nWith a swirl of his majestic robes, Vajra Singh turns and strides off to his tent, where his three servants stand waiting.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_CODEWORD(player, Codeword::Type::HOURGLASS))
        {
            return 235;
        }
        else if (!Character::CHECK_VEHICLE(player, Vehicle::Type::MANTA_SKY_CAR) && !Character::VERIFY_ITEMS(player, {Item::Type::MEDICAL_KIT}))
        {
            return 278;
        }
        else
        {
            return 257;
        }
    }
};

class Story301 : public Story::Base
{
public:
    Story301()
    {
        ID = 301;

        Text = "\"What is the Truth?\" asks the computer enigmatically.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_CODEWORD(player, Codeword::Type::LUNAR))
        {
            return 322;
        }
        else
        {
            return 61;
        }
    }
};

class Story302 : public Story::Base
{
public:
    Story302()
    {
        ID = 302;

        Text = "You hear a scuttling of many insectoid legs and the a giant black centipede bursts from the cloisters, mandibles churning like oiled blades. It was expecting you to stand helpless as it attacked, but you surprise it by evading its charge and racing off along the gallery, dragging Boche behind you. The baron is guided by his psychic senses, but you see it bite a bloody swathe in his flesh before he breaks away. The three of you hurtle through the iron-bound door, slamming it behind you. A moment later the monster slams against the door, buckling it inwards on its hinges, but the bolt holds.\n\n\"That was a close call,\" breathes Boche, wiping a trickle of cold sweat out of his eyes.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 281; }
};

class Story303 : public Story::Base
{
public:
    Story303()
    {
        ID = 303;

        Text = "Vajra Singh stabs at the trigger of his cannot and swings it round towards Golgoth. But the USI agent is not taken by surprise. He grabs Boche and throws him forward to take the brunt of the blast. Boche dies instantly, and at the same moment Golgoth drops a canister that releases a thick cloud of white smoke.\n\nSingh looms like a ghost in the spreading cloud. Holding the cannon ready, he peers through the smoke for any sign of movement. You've lost sight of Golgoth.";

        Choices.clear();
        Choices.push_back(Choice::Base("Attack Singh with a BARYSAL GUN", 20, Choice::Type::FIRE_WEAPON, {Item::BARYSAL_GUN}));
        Choices.push_back(Choice::Base("Or a STUN GRENADE", 20, Choice::Type::LOSE_ITEMS, {Item::STUN_GRENADE}));
        Choices.push_back(Choice::Base("Move in closer to give him back-up", 42));
        Choices.push_back(Choice::Base("Step away and wait to see what happens", 262));

        Controls = Story::Controls::STANDARD;
    }
};

class Story304 : public Story::Base
{
public:
    Story304()
    {
        ID = 304;

        Text = "Golgoth has no time to reload the crossbow. Drawing a knife, he charges forward, but he is to late to stop you from firing. The shot catches him in the chest and he falls dead at your feet.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::FIRE_BARYSAL(player, 1);
    }

    int Continue(Character::Base &player) { return 72; }
};

class Story305 : public Story::Base
{
public:
    std::string PreText = "";

    Story305()
    {
        ID = 305;

        Bye = "You finally manage to defeat this titanic foe.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        PreText = "Vajra Singh is a Sikh warlord, and a man of unyielding honour. Since you have no gun, he does not deign to use his own, but instead leaps forward to settle the matter with his bare hands. You see at once that he is a powerful fighter. His punch chops through the air, and even though you manage to twist aside you can almost feel the shockwave. A single blow from such a warrior could smash through stone.\n\nYou skip back on the balls of your feet, trying to make best use of your speed against his naked strength. A punch hammers into your stomach, and Singh follows it up with a great leonine roar as his foot crunches against your breastbone. You feel ribs crack, but you dive in with a gasp and launch a rapid series of strikes at his face.\n\n";

        auto DAMAGE = -9;

        if (Character::VERIFY_SKILL(player, Skill::Type::CLOSE_COMBAT))
        {
            PreText += "[CLOSE COMBAT] ";

            DAMAGE = -5;
        }

        DAMAGE = Character::COMBAT_DAMAGE(player, DAMAGE);

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Points.";

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 415; }
};

class Story306 : public Story::Base
{
public:
    Story306()
    {
        ID = 306;

        Text = "Boche shudders as he looks along the row of dead white faces. There are at least ten corpses here in the pass, some on this ledge and others are perched further along among the rocks. \"The must have climbed up onto the ledge to get away from wolves,\" says Boche.\n\nHe's wrong. There are no wolves up here in the mountains. And these people were not cowering from predators when they died. In every case they are frozen in postures that suggest curiosity: poised peering out from the ledge, lines of amazement stamped on their faces, icicles across their wide eyes. Death did not surround them with shivering jaws, but stole up softly like a thief in the night.\n\nThe sky is fading from grey to black. If you press on now, you will have to spend the night in open country, unprotected from the bitter wind.";

        Choices.clear();
        Choices.push_back(Choice::Base("Spend the night in open country", 285));
        Choices.push_back(Choice::Base("Shelter here in the pass", 349));

        Controls = Story::Controls::STANDARD;
    }
};

class Story307 : public Story::Base
{
public:
    Story307()
    {
        ID = 307;

        Text = "You give Thadra Bey a contact who should be able to put her in touch with Malengin. \"This will save me considerable effort,\" she murmurs, glancing at the name and address you have written out for her. As she carefully burns the paper over a candle flame, she adds: \"On al-Lat we adhere to a principle of equity in business. I must pay you for this information.\"\n\nYou wave your hand expansively. \"It's not necessary.\"\n\n\"But it is.\" She speaks firmly. \"I do not wish to be in your debt.\" She gives you a packet of antidote pills. \"We take these while on Earth to protect us from the toxins in the air. They are effective against most diseases and poisons. Keep them. I have more in my room.\"\n\nShe gets up and sweeps out of the bar.\n\nYou RECEIVED the ANTIDOTE PILLs.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::GET_ITEMS(player, {Item::ANTIDOTE_PILLS});
    }

    int Continue(Character::Base &player) { return 329; }
};

class Story308 : public Story::Base
{
public:
    Story308()
    {
        ID = 308;

        Text = "This retrovirus toughens your skin so that you gain 5 Life points -- even above your initial score. However, there is a drawback: it also slows your reflexes so that you must lose the [AGILITY] skill if you have it.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::EXALTED_ENHANCER(player, false);
    }

    int Continue(Character::Base &player) { return 434; }
};

class Story309 : public Story::Base
{
public:
    Story309()
    {
        ID = 309;

        Text = "You get talking to members of the relief crew who are waiting on the quayside for the ferry to arrive. After a brief discussion of general matters, you turn the conversation onto the specifics of your profession. \"I hear that the ferry is a Bauer Turbo-400 hovercraft,\" you say. \"Such a machine handles well, particularly in the turbulent waters around the Isis Delta, but you must take care to power down slowly and maintain it at a reasonable temperature. Cooling too quickly risks damage to the skirt and can cause cracks in the jet housings.\"\n\n\"You are a pilot or shipmaster?\" asks a thin-whiskered man who introduces himself as the captain.\n\n\"I have often navigated the Inland Sea -- usually skirting the coast in a mud-skimmer to avoid pirates, though I have handled a Bauer once or twice.\"\n\nThe captain points the stem of his pipe at you and nods thoughtfully. \"How would you be fixed for a trip to Kahira?\"\n\n\"As it happens, that is precisely where I wish to go,\" you say, shouldering your pack.\n\n\"Well, if you're not afraid of hard work then I think I can find a job for you aboard.\"\n\nHe goes on to discuss the details of employment. In addition to earning your passage to Kahira, you will be PAID a further twenty scads.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::GAIN_MONEY(player, 20);
    }

    int Continue(Character::Base &player) { return 246; }
};

class Story310 : public Story::Base
{
public:
    std::string PreText = "";

    Story310()
    {
        ID = 310;

        Bye = "Morning pushes crooked fingers of wan silver light through the thick clouds. Rising and rubbing the circulation back into your weary limbs, you gather your things together and set off towards Venis.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        PreText = "Sheltered from the freezing blast of the night wind by the craggy walls of the pass, you huddle down and snatch a few hours of fitful sleep. In this you take turns, one of you trudging up and down to stay warm while the other dozes. Even your best efforts cannot keep the deathly chill out of your bones.\n\n";

        if (Character::VERIFY_SKILL(player, Skill::Type::SURVIVAL))
        {
            PreText += "[SURVIVAL] You are hardy enough to endure the cold.";
        }
        else
        {
            Character::GAIN_LIFE(player, -1);

            PreText += "You LOSE 1 Life Point.";
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 199; }
};

class Story311 : public Story::Base
{
public:
    Story311()
    {
        ID = 311;

        Text = "The hour is late. It is time you found somewhere to pass the night. The Ossiman Hotel is still open, and charges 5 scads for a bed.";

        Choices.clear();
        Choices.push_back(Choice::Base("Take a room there (5 scads)", 333, Choice::Type::LOSE_MONEY, 5));
        Choices.push_back(Choice::Base("Save money by sleeping rough on the streets", 165));

        Controls = Story::Controls::STANDARD;
    }
};

class Story312 : public Story::Base
{
public:
    Story312()
    {
        ID = 312;

        Text = "You awaken and look around, then give a sob of horror and leap half to your feet, scrambling back against the rock. The others are dead, their bodies ripped apart and scattered far across the fresh white snow. When you see there is no immediate threat, your pounding heartbeat starts to return to normal and you venture over to a headless torso which you identify as Hal Shandor. Strange to see a man so vital and full of strength, now a broken shell as lifeless as clay.\n\nAnd as bloodless. You can the snow, puzzled. Despite the dreadful carnage, there is hardly a drop of blood to be seen.\n\nYou do not know what animal could have torn Shandor and his hulking bodyguards limb from limb in total silence. Nor do you want to find out. You make a cursory search of the shreds of clothing you can see, finding an ID CARD, a FLASHLIGHT, and a BARYSAL GUN with one remaining charge.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Take = {Item::ID_CARD, Item::FLASHLIGHT, Item::MAKE_BARYSAL_GUN(1)};

        Limit = 3;
    }

    int Continue(Character::Base &player) { return 161; }
};

class Story313 : public Story::Base
{
public:
    Story313()
    {
        ID = 313;

        Text = "Electric lighting is rare enough in this age, and is usually arranged by means of a coal- or oil-fuelled generator. For there still to be electricity here, when Marsay has been abandoned for nearly two centuries, there must be a nuclear power source. Presumably such a power source would have to be regulated by computers, which means the possibility of a link to Gaia.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_SKILL(player, Skill::Type::CYBERNETICS))
        {
            return 335;
        }
        else
        {
            return 356;
        }
    }
};

class Story314 : public Story::Base
{
public:
    std::string PreText = "";

    Story314()
    {
        ID = 314;

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        PreText = "You stumble on through the snow with a howling wind at your back. The air is so cold that each breath rasps in your throat, and your limbs are soon weary and numb. You are on the point of collapse when you find a rocky outcropping that gives partial shelter from the blizzard. You huddle down behind it and wait for daybreak.\n\n";

        auto DAMAGE = -2;

        if (Character::VERIFY_SKILL(player, Skill::Type::SURVIVAL))
        {
            PreText += "[SURVIVAL] ";

            DAMAGE = -1;
        }

        Character::GAIN_LIFE(player, DAMAGE);

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Points.";

        if (player.Life > 0)
        {
            if (Character::VERIFY_SKILL(player, Skill::Type::SURVIVAL))
            {
                PreText += "\n\n[SURVIVAL] You improvise a scooped shell of snow for better protection from the wind chill.";
            }
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 393; }
};

class Story315 : public Story::Base
{
public:
    Story315()
    {
        ID = 315;

        Text = "A look of keen enthusiasm crackles behind his tranquil gaze and for a moment you feel you've glimpsed the true Chaim Golgoth. \"The BARYSAL GUN is a potent weapon,\" he admits, \"but it is limited by charges and is prone to malfunction. This can lead to inconvenience. The crossbow also has its limitations, of course, but against the unarmoured human opponents I find it quite reliable.\"\n\n\"Golgoth is a stickler for efficiency,\" remarks Boche with a smile of undisguised dislike. \"Efficiency, in his case, is usually measured in terms of deaths per minute.\"\n\nGolgoth shrugs, apparently not bothered by Boche's views. \"By that criterion, the efficient agent just needs to equip himself with a few plutonium bombs. If only my superiors weren't so stingy with expenses.\"";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 358; }
};

class Story316 : public Story::Base
{
public:
    std::string PreText = "";

    Story316()
    {
        ID = 316;

        Bye = "You manage to fight free of them and run off, taking shelter in a boarded-up doorway as they clatter past in angry pursuit. You listen to them recede into the fog. Hobbling painfully from your wounds, you go in search of a safer place to spend the rest of the night.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        PreText = "The alley is too narrow for you to have space to use a gun, even if you are carrying one. Launching yourself at the fanatics with a roar of rage, you lash out with fists and feet. Somebody grabs your arm, but you twist free and shove him back into the priestess. A punch spins you round, but you keep your wits enough to drop low to avoid a follow-up, then straighten and drive your shoulder into your attacker's belly. He slumps with a groan.\n\n";

        auto DAMAGE = -4;

        if (Character::VERIFY_SKILL(player, Skill::Type::CLOSE_COMBAT))
        {
            PreText += "[CLOSE COMBAT] ";

            DAMAGE = -2;
        }

        DAMAGE = Character::COMBAT_DAMAGE(player, DAMAGE);

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Point(s).";

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 253; }
};

class Story317 : public Story::Base
{
public:
    Story317()
    {
        ID = 317;

        Text = "He gives you a dubious sidelong look. \"It is not so simple as all that. Our way of life here on al-Lat is governed by a complex creed, of which you know nothing. We are a society which is closed to outsiders.\"\n\nThe door of the laboratory slides open at this point and Riza Baihaqi comes in. \"I'm about to take a flyer back down to Earth, so I can drop you at Sudan,\" he says bluffly. \"Have you been learning about our work here? I hope it will prove useful in your venture.\"";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 275; }
};

class Story318 : public Story::Base
{
public:
    Story318()
    {
        ID = 318;

        Choices.clear();

        Controls = Story::Controls::NONE;
    }

    int Background(Character::Base &player)
    {
        if (Character::VERIFY_CODEWORD(player, Codeword::Type::ENKIDU))
        {
            return 340;
        }
        else
        {
            return 425;
        }
    }
};

class Story319 : public Story::Base
{
public:
    Story319()
    {
        ID = 319;

        Text = "The bometh moves off and the binoculars enable you to follow at a safe distance. You watch it slinking between the furrows of snow, a blot of shadow flowing in the moonlight. Seeing it disappear into a snowdrift, you move closer and compose yourself for a long wait. Two or three hours go by. At last it emerges, sniffs at the air, and lopes off in search of prey.\n\nOnce it is out of sight, you scramble over to the snowdrift, pushing along a tunnel into a hollowed-out cavity where there are three small bomeths on a nest of moulted fur. Ignoring them, you turn your attention to the closely packed walls of the lair -- the bometh's larder, where the beast has stored remains of previous kills. You dig out what looks like the carcass of a large fowl. The icy cold has preserved it well. Wrapping the flesh carefully, you make two food packs.";

        Bye = "One of the young bomeths nips at your ankle. The teeth do not penetrate your boot, but it is a timely reminder that the parent might return at any time. You squirm back to the open and hurry away.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Take = {Item::FOOD_PACK, Item::FOOD_PACK};

        Limit = 2;
    }

    int Continue(Character::Base &player) { return 298; }
};

class Story320 : public Story::Base
{
public:
    Story320()
    {
        ID = 320;

        Text = "You swallow the pills and soon begin to feel much better. Even so, the sleepless night has left you exhausted. Out in the main square, the others are preparing for another day's exploration of the ruins. With submerged tension forever threatening to boil over into open warfare, you know that you need all your wits about you. Your rivals are unlikely to find the Heart today -- better that you rest now, and gather your strength for what lies ahead.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        if (Character::CHECK_VEHICLE(player, Vehicle::Type::MANTA_SKY_CAR))
        {
            Character::GAIN_LIFE(player, 1);
        }
        else if (Character::VERIFY_ITEMS(player, {Item::Type::MEDICAL_KIT}))
        {
            Character::GAIN_LIFE(player, 1);
        }

        Character::REMOVE_CODEWORD(player, Codeword::Type::HOURGLASS);
    }

    int Continue(Character::Base &player) { return 363; }
};

class Story321 : public Story::Base
{
public:
    Story321()
    {
        ID = 321;

        Text = "\"I shall always think of you as having been a friend,\" says Gaunt after a long pause.\n\nSomething in the words makes them sound like an epitaph. You look round to see the xoms facing you with long crystal knives.\n\n\"What's going on?\"\n\nIn the dulled light of the glow-lamps, Gaunt himself looks like a pale phantom. You see the gleam of his teeth as he smiles. \"I must find the key to my own ruthless nature, or tomorrow I shall die,\" he murmurs. \"Do you know that the desire for power and change is the desire for one's own death? By expunging you, whom I admire, I expunge that weakness in myself. For you see, come what may, I shall have the Heart.\"\n\n\"You're completely mad.\"\n\nHe nods. \"I must be. It is not sane to covet the role of God.\" He gestures and the xoms shuffle forward, jabbing their knives towards your chest.\n\nA shot rings out and one of the xoms falls, flames spouting from a blast hole through its torso. Gaunt whirls in time to see a small man in combat fatigues who rushes out of the darkness and tackles him to the ground.\n\nVajra Singh steps into the light with his two other guards behind him. They are Brits -- small men with pallid pinched faces, thuggish but famed for their loyalty. The one standing over Gaunt presses a gun to his head and glances at Singh. \"Shall I kill 'im, sah?\" he barks. Singh nods. A blast of plasma scrambles Gaunt's brains into the snow. The xoms jerk back and their arms drop listlessly to their sides.\n\nYou breathe a sigh. \"A timely intervention. You saved my life.\"\n\nVajra Singh hardly looks at you. \"Gaunt broke the terms of the truce. He would have died tomorrow, in any case. He was a weak man.\" He turns and strides off with his guards following.";

        Choices.clear();
        Choices.push_back(Choice::Base("(Nemesis) Tell Singh what you know", 343, Codeword::Type::NEMESIS));
        Choices.push_back(Choice::Base("Return to the main square and either converse with Kyle Boche", 104));
        Choices.push_back(Choice::Base("Talk to Chaim Golgoth", 126));
        Choices.push_back(Choice::Base("Get some sleep", 192));

        Controls = Story::Controls::STANDARD;
    }
};

class Story322 : public Story::Base
{
public:
    Story322()
    {
        ID = 322;

        Text = "You remember the catechism that Eleazar Picard kept reciting after the fall of Du-En. What better code for the high priests to use than the most basic tenets of their faith? If only you can recall the precise wording...";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 18; }
};

class Story323 : public Story::Base
{
public:
    Story323()
    {
        ID = 323;

        Text = "He is wearing the red-and-violet uniform of the Monitor Corps, personal troops of Eleazar Picard, the Volentine high priest. Perhaps he was fighting to defend him when the populace of the city rebelled. If you want to free him, it might be possible.";

        Choices.clear();
        Choices.push_back(Choice::Base("(BARYSAL GUN) Destroy the STASIS BOMB", 345, {Item::BARYSAL_GUN}));
        Choices.push_back(Choice::Base("[PARADOXING] Cancel the time distortion effect", 366, Skill::Type::PARADOXING));
        Choices.push_back(Choice::Base("Continue deeper into the catacombs", 388));

        Controls = Story::Controls::STANDARD;
    }
};

class Story324 : public Story::Base
{
public:
    Story324()
    {
        ID = 324;

        Text = "Boche suddenly yells: \"Let's get him, Golgoth!\"\n\nVajra Singh, whirling, makes a split-second decision as to who is his most dangerous adversary. Pressing the power button on the mantramukta, he directs a blistering torrent of raw energy at Golgoth. Golgoth reacts by flinging himself into a sideways roll, firing a continuous barrage at Singh as he moves. Both blasts find their target at the same time. Singh falls with a pencil-thin barysal burn through his eye-socket. Golgoth is engulfed and blown to cinders.\n\nIt has all taken place in seconds. Now only you and Boche are left. He smiles and winks at you. You start to smile back, but it freezes on your face as he turns to show you the BARYSAL GUN he has trained on you.\n\n\"Well, Boche,\" you say, \"is this post-hypnotic treachery, or the regular kind?\"\n\n\"I knew you'd turn on me if I didn't act first,\" he replies with a shrug. \"Only one can have the Heart.\"\n\nIt is not pleasant to stare down the barrel of a gun. You had better decide what to do.";

        Choices.clear();
        Choices.push_back(Choice::Base("[SHOOTIG] Shoot him", 347, Skill::Type::SHOOTING));
        Choices.push_back(Choice::Base("[PARADOXING] Use a PSIONIC FOCUS", 390, Skill::Type::PARADOXING));
        Choices.push_back(Choice::Base("Throw a KNIFE at him", 368, {Item::KNIFE}));
        Choices.push_back(Choice::Base("Otherwise", 411));

        Controls = Story::Controls::STANDARD;
    }
};

class Story325 : public Story::Base
{
public:
    Story325()
    {
        ID = 325;

        Image = "images/puppets.png";

        Text = "You spend an hour or so exploring the area, which turns out to be a range of storerooms and living quarters. \"Obviously it's one of the bomb shelters built in the last days of Du-En.\" says Golgoth, shining his torch into a dusty hospital ward. He bangs the map box against the wall to encourage it to give a clearer picture. \"That being the case, I'm not so sure we'll find a route through to the temple area. The society's leaders would have had more luxurious shelters than this.\"\n\n\"You want luxury?\" calls Gargan XIV from down the corridor. \"We got your luxury right here.\"\n\nJoining the sisters, you enter an auditorium with banks of seats facing a curtain. Cold silver light flickers from plates of frosted glass set into the ceiling. The light gives a strobe effect, so that you all seem to be moving like figures in a jerky freeze-frame video. Gargan XIII pulls the curtain aside. In the flashes of light and dark, you see a stage where a dozen puppets stand in elegant postures.\n\nYou follow Golgoth up onto the sage. The puppets, about a metre tall, are suspended by thin copper wires from high in the grid above the stage. Each is robed like warrior of ancient times and has a scimitar in his hand. You reach out and feel one. It is sharp enough to prick your finger.\n\nThen, in the space between one flicker of light and the next, something changes. At first you cannot tell what, then it hits you: the puppets are moving. Gargan XIII gives a grunt of pain and you see the livid streak of a wound on her forearm. \"They're alive!\" she cries. \"Let's get out of here!\"";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_CODEWORD(player, Codeword::Type::ENKIDU))
        {
            return 132;
        }
        else if (Character::VERIFY_SKILL(player, Skill::Type::AGILITY))
        {
            return 22;
        }
        else
        {
            return 66;
        }
    }
};

class Story326 : public Story::Base
{
public:
    std::string PreText = "";

    Story326()
    {
        ID = 326;

        Bye = "You manage to twist the KNIFE around and impale him.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        PreText = "Golgoth wastes no time reloading the crossbow. Throwing it aside, he draws a knife and dives at you. You block his first thrust but take gash on your forearm, countering with a leg sweep which leaves him off balance.\n\nThe fight is short and brutal. Golgoth is a master of lethal killing techniques.\n\n";

        auto DAMAGE = -5;

        if (Character::VERIFY_SKILL(player, Skill::Type::CLOSE_COMBAT))
        {
            PreText += "[CLOSE COMBAT] ";

            DAMAGE = -5;
        }

        DAMAGE = Character::COMBAT_DAMAGE(player, DAMAGE);

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Point(s).";

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 72; }
};

class Story327 : public Story::Base
{
public:
    Story327()
    {
        ID = 327;

        Text = "Singh is more used to his cannon than the light pistol, and you are just a fraction faster. Your barysal beam splits the air and he falls without a sound. It is only as you go over to inspect the body that you realize he had a chance of hitting you with a dying shot. He chose not to take that shot. Why?\n\nBecause it would have been petty to deprive you of victory when you had beaten him fairly? You can think of no better explanation. Sing died as he had lived: a man of uncompromising honour.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 415; }
};

class Story328 : public Story::Base
{
public:
    Story328()
    {
        ID = 328;

        Text = "Boche is not only interested in food. While he extracts a parcel of rations from the corpse's pocket, his other hand adroitly filches a money token out of its wallet. He slips into his boot without telling you. A casual observer would never have noticed. Not to be outdone, you deftly remove the token while Boche is checking another of the bodies. Touching it to your own money token, you TRANSFER the sum of 60 scads and then replace the token before Boche is any wiser.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::GAIN_MONEY(player, 60);
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_SKILL(player, Skill::Type::PARADOXING))
        {
            return 370;
        }
        else
        {
            return 306;
        }
    }
};

class Story329 : public Story::Base
{
public:
    Story329()
    {
        ID = 329;

        Text = "With a day or two to wait until the ferry to Kahira arrives, you have time to make preparations for the adventure ahead. You take a stroll along the esplanade overlooking the gambling rooms of the notorious Hazard Strip. Below you, in the deep alley that was once the grandest of the canals glaring neon lights and raucous music intrude on the wistful grandeur of Venis by night.\n\nYou consider your options.";

        Choices.clear();
        Choices.push_back(Choice::Base("Try to communicate with Gaia", 351));
        Choices.push_back(Choice::Base("Discover more about the Heart of Volent", 91));
        Choices.push_back(Choice::Base("enquire after travellers who have gone missing on their way to Venis recently", 451));
        Choices.push_back(Choice::Base("Go look for gossip about Kyle Boche", 179));
        Choices.push_back(Choice::Base("Find some special purchases for the trip", 350));

        Controls = Story::Controls::STANDARD;
    }
};

class Story330 : public Story::Base
{
public:
    std::string PreText = "";

    Story330()
    {
        ID = 330;

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        auto reversed = 0;

        if (Character::IS_APPLIED(player, Item::Type::EXALTED_ENHANCER))
        {
            reversed++;

            PreText += "EXALTED ENHANCER";

            Character::EXALTED_ENHANCER(player, true);
        }

        if (Character::IS_APPLIED(player, Item::Type::MASK_OF_OCCULTATION))
        {
            if (reversed > 0)
            {
                PreText += ", ";
            }

            reversed++;

            PreText += "MASK OF OCCULTATION";

            Character::MASK_OF_OCCULTATION(player, true);
        }

        if (Character::IS_APPLIED(player, Item::Type::PEERLESS_PERCEPTIVATE))
        {
            if (reversed > 0)
            {
                PreText += ", ";
            }

            reversed++;

            PreText += "PEERLESS PERCEPTIVATE";

            Character::PEERLESS_PERCEPTIVATE(player, true);
        }

        Character::APPLY_VIRUS(player, Item::Type::VIRID_MYSTERY);

        if (reversed == 0)
        {
            PreText = "No retrovirus effects reversed!";
        }
        else
        {
            PreText += " effects reversed!";
        }

        Text = PreText.c_str();
    }

    int Continue(Character::Base &player) { return 434; }
};

class Story331 : public Story::Base
{
public:
    Story331()
    {
        ID = 331;

        Text = "Sidling up beside a man in a fur-trimmed cape of scarlet silk, you gesture out to sea. \"Is that the ferry?\"\n\nHe glances around, peers out through the icy haze swathing the coastline and then turns to regard you with a withering stare. \"It was a seagull. The ferry is not due for half an hour at least.\" With a flourish of his cape he stalks away, unaware that you have picked his pocket while he was distracted.\n\nYou now have a ticket for the ferry.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 246; }
};

class Story332 : public Story::Base
{
public:
    Story332()
    {
        ID = 332;

        Text = "There is nothing for it but to set out on foot, skirting the Inland Sea so as to reach Kahira by way of Bezant.\n\nYou are destined never to reach your goal. As you cross the hills south-east of Bezant that rise to form the Anatolian Plateau, there is a flare of light that spreads rapidly across the sky from the west. It seems as though titanic shockwaves are resounding over the world from a terrible explosion. You turn to run, but space and time flow around you like water. You fall and, looking up, you imagine a colossal face staring from the far horizon.\n\n\"I am Vajra Singh,\" says a voice inside your head. \"Now the Heart of Volent is mine, and I shall use it to remake all creation in my own image.\"\n\nThe world is swept away. Because you delayed too long, the power of the Heart fell into another's hand. The whole cosmos ends here.";

        Type = Story::Type::DOOM;

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }
};

class Story333 : public Story::Base
{
public:
    Story333()
    {
        ID = 333;

        Text = "Early morning sunlight drives away the wisps of mist, but there is still a nip in the air as you walk along the narrow streets. Even in this impoverished age, the bazaar of Kahira retains the quaint and colourful air for which it is famous. The alleyways are barely wide enough for one man to pass another without intimacy. Shutters of dark wood open onto shops which display artwork of ancient times: carpets and tapestries, silks, ivory carvings, gold and silver filigree, amulets, spices, wines, dyes and sultry perfumes. A thousand scents mingle in the hazy morning air, wafting from the food stalls where coffee bubbles in tall kettles and pancakes sizzle on the stoves. You pass a man with along clay pipe. No doubt his ancestors looked out from that very stall with the same brown high-cheekboned features. Mistaking your thoughtful expression, he beckons you over. \"See my fine goods. I have thick furs to keep out the cold.\"\n\nWhen the stall-holder learns you are undertaking a journey, he insists that you follow him to a courtyard wedged between the narrow buildings where his cousin sells animals and slaves.";

        Choices.clear();
        Choices.push_back(Choice::Base("Go with him", 338));
        Choices.push_back(Choice::Base("Look around the rest of the bazaar", 359));

        Controls = Story::Controls::SHOP;
    }

    void Event(Character::Base &player)
    {
        Shop = {{Item::FUR_COAT, 3}};
    }
};

class Story334 : public Story::Base
{
public:
    Story334()
    {
        ID = 334;

        Text = "Dusk is falling by the time you reach the ancient city of Venis. It shimmers with a thousand lights under a sky like dull green bronze. Hungry and cold, you quicken your pace through the outlying streets. You pass the temporary shacks where hunters and traders dwell, then the slums of corrugated iron and plastic filling the narrow, sunken streets that some say were once canals. Above them loom the blocks of ancient plazas, where the rich and powerful of the city reside in palatial buildings shored up with wooden scaffolding to support them from the ravages of time.\n\nYou soon learn that the ferry to Kahira is not due for a couple of days. While waiting, you have a choice of where to take lodging. The lavish Marco Polo Hotel will charge 12 scads for two nights; the Hotel Paradise will charge 6 scads; the disreputable Doge's Inn will cost only 3 scads.";

        Choices.clear();
        Choices.push_back(Choice::Base("Choose Marco Polo (12 scads)", 286, Choice::Type::LOSE_MONEY, 12));
        Choices.push_back(Choice::Base("Opt for Paradise (6 scads)", 244, Choice::Type::LOSE_MONEY, 6));
        Choices.push_back(Choice::Base("Check in at the Doge's Inn (3 scads)", 371, Choice::Type::LOSE_MONEY, 3));

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        if (Character::VERIFY_ITEMS(player, {Item::Type::ID_CARD}))
        {
            Choices[0].Destination = 222;
        }
        else
        {
            Choices[0].Destination = 286;
        }
    }
};

class Story335 : public Story::Base
{
public:
    Story335()
    {
        ID = 335;

        Text = "Although the computers monitoring and maintaining the power supply must still be functioning, it is obvious that they are not connected to Gaia. Gaia is too erratic to have kept up such a task for two centuries. However, if you can get access to the computers here in the city then you might be able to set up a link with Gaia.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 356; }
};

class Story336 : public Story::Base
{
public:
    Story336()
    {
        ID = 336;

        Text = "\"Possibly you overestimate Gaia's powers; she does not control the sun,\" you reply with a scornful laugh. The screen flickers and a blue globe logo appears announcing access to Gaia. Quickly you type: \"GAIA. ARE YOU THERE?\"\n\n\"WHERE IS THERE? THERE IS EVERYWHERE.\"\n\nUnfortunately Gaia is in one of her confused phases. You try again: \"HOW MUST I REACH DU-EN?\"\n\n\"GIZA FIRST. THE PYRAMID.\" You seem to sense in the juddering way the type appears on the screen, a great struggle going on inside Gaia's mind. \"YOU CAN FIND A FRIEND IN GILGAMESH. ENTER HUMBABA...\"\n\nThe message degenerates into gibberish. You break the link then, after pausing to reflect, you order the local computer to shut down. There is a chance it became afflicted with Gaia's viruses during the link, and it would not do to have a nuclear reactor go critical somewhere nearby.\n\n\"I'm afraid you'll lose electricity soon,\" you tell Fax. \"There's probably enough power in the storage cells for a day or two, then you'll have to make other arrangements for your lighting and so forth.\"\n\nHis jaw drops. \"What have you done? You have wreck my little paradise, all for the sake of a word with your precious Gaia!\"\n\nYou shrug. \"Those few words might mean the salvation of mankind -- if I can work out what they mean.\"\n\nYou gained the codeword HUMBABA.";

        Choices.clear();
        Choices.push_back(Choice::Base("Try one of the tunnels", 439));
        Choices.push_back(Choice::Base("Say goodbye to Fax and continue your journey west", 420));

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::GET_CODEWORDS(player, {Codeword::Type::HUMBABA});
    }
};

class Story337 : public Story::Base
{
public:
    Story337()
    {
        ID = 337;

        Text = "His gaze narrows just for an instant as he listens to the question. \"Giza was a military base a few klicks west. Long before that, I'm told it was an ancient burial ground. It's deserted now anyway. And off limits. I recommend you steer well clear of the place.\"\n\nBoche takes your arm. \"Excuse us,\" he says to Golgoth, \"but we have business in Kahira and I don't care to have my throat slit beforehand.\"\n\nGolgoth grins. \"Boche, you slit your throat every time you open your mouth.\"";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 358; }
};

class Story338 : public Story::Base
{
public:
    Story338()
    {
        ID = 338;

        Text = "You are shown a BURREK -- a hulking, thick-shouldered animal with shaggy white fur and a lugubrious snout. \"The nomads use such creatures when they wish to cross the Ice Wastes,\" the trader tells you. \"They huddle beside the beast in blizzards, and when hungry they tap its veins to make a blood pudding.\"\n\n\"What a sickening though.\"\n\nHe nods sagely. \"Indeed, it is probably only just preferable to dying of starvation. Still, if you intend to cross the Sahara you cannot do without a burrek. This stout animal is for sale at the generous price of thirty scads.\n\nAfter some haggling, the price drops to 10 scads.";

        Choices.clear();
        Choices.push_back(Choice::Base("Buy the BURREK for 10 scads", 359, Choice::Type::BUY_VEHICLE, Vehicle::Type::BURREK, 10));
        Choices.push_back(Choice::Base("Move on", 359));

        Controls = Story::Controls::STANDARD;
    }
};

class Story339 : public Story::Base
{
public:
    Story339()
    {
        ID = 339;

        Text = "Your fingers fly across the keyboard, bringing up a rapid sequence of access codes, file menus and options. Safety systems are in place to prevent accidental communication with Gaia. You override them, opening a channel via one of the al-Lat station's external radio dishes which connects you to an Earth weather satellite. In less than thirty seconds you are through to Gaia herself. You flip the link to \"Display Only\" so that no one overhears you. Glancing nervously at the door, you quickly bring Gaia up to date with what has been happening.\n\nThe screen fills with a torrent of glowing words: ACTIVATION OF THE HEART WILL CREATE A NEW UNIVERSE. THE PRESENT UNIVERSE WILL BE DESTROYED IN THE PROCESS. I RECOMMEND DESTRUCTION OF THE HEART. ACCOMPLISH THIS USING TWO BARYSAL BEAMS ALIGNED AT RIGHT ANGLES: THE CRYSTALLINE LATTICE OF THE HEART WILL BE DISRUPTED.\n\nStartling news. You are anxious to question Gaia further while she is in a communicative mood, but someone might come in and find you at any minute. You break the link, turning from the screen just in time to see the door open and Riza Baihaqi come back in.\n\nYou gained the codeword NEMESIS.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::GET_CODEWORDS(player, {Codeword::Type::NEMESIS});
    }

    int Continue(Character::Base &player) { return 382; }
};

class Story340 : public Story::Base
{
public:
    Story340()
    {
        ID = 340;

        Text = "You walk through to the adjacent chamber, Gilgamesh following with clanking strides. \"The podium is empty.\" Are there others like you?\" you ask him. \"Where can I find them?\"\n\nThere is the briefest of pauses as his artificial mind assimilates the question. \"I am the prototype,\" he responds. \"I was created to guard against danger from the Volentine sect. I know of no others.\"\n\nYou glance at him, a huge armoured automaton thrumming with power. The way that he stands so immobile, inexorably measuring each remark in the depths of his nonhuman brain, makes for an eerie and unnerving scene. You remind yourself that he is your servant -- dangerous to your foes, not to you. \"Come with me,\" you tell him. \"We are leaving.\"";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 361; }
};

class Story341 : public Story::Base
{
public:
    Story341()
    {
        ID = 341;

        Text = "You look down at the huge carcass stretched out in the pale moonlight.";

        Choices.clear();
        Choices.push_back(Choice::Base("Cut it up with a KNIFE", 362, {Item::KNIFE}));
        Choices.push_back(Choice::Base("Use a SHORT SWORD", 384, {Item::SHORT_SWORD}));
        Choices.push_back(Choice::Base("Otherwise", 298));

        Controls = Story::Controls::STANDARD;
    }
};

class Story342 : public Story::Base
{
public:
    Story342()
    {
        ID = 342;

        Text = "The truth dawns on you with chilling horror: you have a wasting disease. It will spread, gnawing away at you from inside. Now the race for the Heart takes on a vivid new importance, as it is only by obtaining its power that you can cure your body of the sickness.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player)
    {
        if (!Character::VERIFY_ITEMS(player, {Item::Type::MEDICAL_KIT}) && !Character::CHECK_VEHICLE(player, Vehicle::Type::MANTA_SKY_CAR))
        {
            return 278;
        }
        else
        {
            return 257;
        }
    }
};

class Story343 : public Story::Base
{
public:
    Story343()
    {
        ID = 343;

        Text = "You hurry to catch up with Singh and tell him about Gaia's warnings. At first he seems barely interested, but gradually your words get through. He comes to a halt and turns to stare into your face. \"You propose an alliance,\" he says. \"his is wise. If we are allies, we are virtually certain to prevail against all others and reach the Heart. Tomorrow, you go with Baron Siriasis and I shall team up with Golgoth. They are our most dangerous adversaries. If the opportunity arises, we must slay them.\"\n\n\"What if your group finds the Heart first? Or my group, for that matter?\"\n\n\"I swear I shall not take its power until only you and I are left. Then together wee can discuss the future of the cosmos.\"\n\nYou watch him march back to his tent. You feel sure Singh will keep his oath of alliance. If only you didn't have a sneaking suspicion that you've just tied yourself to the lion's tail.\n\nYou gained the codeword RED.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::GET_CODEWORDS(player, {Codeword::Type::RED});
    }

    int Continue(Character::Base &player) { return 192; }
};

class Story344 : public Story::Base
{
public:
    Story344()
    {
        ID = 344;

        Text = "Through the glass door you can see that Golgoth's elevator is starting to descend. Hastily you reach out and scan his mind to find out what he said. You learn that the code takes the form of a question-and-answer formula central to the Volentine faith. Golgoth found it when he watched the tapes showing the interrogation of Eleazar Picard, the high priest of the cult who escaped when Du-En fell.\n\nIt worked for Golgoth. Now you can see if it will work for you.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 18; }
};

class Story345 : public Story::Base
{
public:
    Story345()
    {
        ID = 345;

        Text = "It is strange to witness the barysal beam slowing down as it enters the time-distortion zone, like watching a white-hot needle pressing through ice. As it strikes the stasis bomb, there is a muffled explosion and the bomb splits into molten fragments. At the same instant, the man takes half a step forward and then jerks back in surprise as he sees the three of you standing around him.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::FIRE_BARYSAL(player, 1);
    }

    int Continue(Character::Base &player) { return 409; }
};

class Story346 : public Story::Base
{
public:
    Story346()
    {
        ID = 346;

        Text = "Golgoth suddenly explodes into action. Seizing Boche around the neck and pressing his gun to his temple, he orders him to shoot Singh unless he wants to die at once. Boche raises his own gun and fires at Singh is turning to act. As Singh staggers back, his armour breastplate charred by the blast, Golgoth coolly shoots Boche through the head, holding the body up as a shield.\n\nSingh is fumbling for the trigger of his mantramukta cannon. \"Get him now!\" Golgoth shouts to you. \"Before he recovers!\"";

        Choices.clear();
        Choices.push_back(Choice::Base("Attack with a BARYSAL GUN", 389, Choice::Type::FIRE_WEAPON, {Item::BARYSAL_GUN}));
        Choices.push_back(Choice::Base("Or a STUN GRENADE", 389, Choice::Type::LOSE_ITEMS, {Item::STUN_GRENADE}));
        Choices.push_back(Choice::Base("Decide not to act", 410));

        Controls = Story::Controls::STANDARD;
    }
};

class Story347 : public Story::Base
{
public:
    Story347()
    {
        ID = 347;

        Text = "Your gun is in your hand before Boche can react. The shot lashes through the air, catching him directly between the eyes. He dies without even realizing what has happened. Now nothing stands between you and the Heart of Volent.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Character::FIRE_BARYSAL(player, 1);
    }

    int Continue(Character::Base &player) { return 415; }
};

class Story348 : public Story::Base
{
public:
    Story348()
    {
        ID = 348;

        Text = "The barysal beam cleaves through Singh's armour, but his own shot slays you at once. You die knowing that victory was almost within your grasp.";

        Type = Story::Type::DOOM;

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }
};

class Story349 : public Story::Base
{
public:
    Story349()
    {
        ID = 349;

        Text = "Finding another ledge where there are no corpses, you sweep it clear of snow and settle down for the night. Boche casts a wry gaze along the rock face. \"A bit like sleeping in a graveyard,\" he remarks.\n\nYou can only shrug. \"Try not to think about it. At least here we're out of the wind.\"\n\n\"And safe from predators.\"\n\n\"Are we?\" You wonder.\n\nAs the feeble daylight gives way to night, snow comes in fast flurries on the wind coursing through the pass. The sound is like a banshee's wailing. Huddled in your jacket, you try to get to sleep. Then you hear the unmistakable tread of footsteps in the snow. Someone is coming.";

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Choices.clear();

        if (!Character::VERIFY_SKILL(player, Skill::Type::ESP) && !Character::VERIFY_SKILL(player, Skill::Type::LORE))
        {
            Choices.push_back(Choice::Base("Leap out and attack", 433));
            Choices.push_back(Choice::Base("Wait where you are", 2));
        }
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_SKILL(player, Skill::Type::ESP))
        {
            return 392;
        }
        else
        {
            return 413;
        }
    }
};

class Story350 : public Story::Base
{
public:
    Story350()
    {
        ID = 350;

        Image = "images/venis.png";

        Text = "The main market of Venis is located inside a lavish three-decked gallery perched incongruously amid the muddy lower streets. Here, on benches where rowers once plied gilded oars, merchants sit and call out their trade. When a customer shows interest, he is led off to the merchant's storehouse in one of the neighbouring side streets.";

        Choices.clear();
        Choices.push_back(Choice::Base("Look for ordinary goods", 394));
        Choices.push_back(Choice::Base("Weaponry and other devices", 283));
        Choices.push_back(Choice::Base("Undergo genetic enhancement", 434));

        Controls = Story::Controls::STANDARD;
    }
};

class Story351 : public Story::Base
{
public:
    Story351()
    {
        ID = 351;

        Text = "You pass a street-corner scribe who has two antique laptop computers. When you ask about using these to make contact with Gaia, he turns on a bland smile. Flicking one of the laptops to life, he gestures at the lines of dimly glowing green text that appear on the screen.\n\n\"My business is letters and contracts, not computers. I use the computer simply as a tools of the trade -- just as my forerunners used clay tablets, sheets of papyrus and quill pens.\"\n\nYou examine the equipment. \"There must be peripherals for linking these into the global computer net. Isn't this a modem socket?\"\n\nThe scribe whistles between his teeth. \"No doubt. However, I heard of a man who linked his computer to Gaia and forever afterward it would do nothing except iterate between prime numbers. I would as soon infect myself with the yellow pox!";

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_SKILL(player, Skill::Type::CYBERNETICS))
        {
            return 265;
        }
        else
        {
            return 372;
        }
    }
};

class Story352 : public Story::Base
{
public:
    Story352()
    {
        ID = 352;

        Text = "All you can do is sell some possessions in order to raise the money for a ticket. You reluctantly go looking for a street pedlar, knowing that the need to make a quick sale will lose you money. At least you find a buyer without delay -- or rather, he finds you. He is a gimlet-eyed man with a shock of pink-dyed hair, loitering at the quayside gate with the obvious intent of preying on people like yourself. \"A ferry ticket costs a bit more than you thought, doesn't it?\" he says patronizingly. \"Now then, let's see what you've got to sell.\"";

        Choices.clear();
        Choices.push_back(Choice::Base("Buy a ferry ticket", 246, Choice::Type::MONEY, 10));
        Choices.push_back(Choice::Base("You cannot afford a ticket", 332));

        Controls = Story::Controls::SELL;
    }

    void Event(Character::Base &player)
    {
        Sell = {{Item::FOOD_PACK, 1}, {Item::MAKE_BARYSAL_GUN(6), 5}, {Item::FLASHLIGHT, 5}, {Item::MEDICAL_KIT, 5}, {Item::POLARIZED_GOGGLES, 2}, {Item::ID_CARD, 10}};
    }
};

class Story353 : public Story::Base
{
public:
    Story353()
    {
        ID = 353;

        Text = "The card identifies its owner as a member of the Society of the Compass, a secretive and select organization with considerable resources worldwide. The society has a building here in Kahira. You stand on the other side of the street and look up at the tower of steel and glass rising off into the dank evening mist. In the colourless fluorescent glow of the lobby you can see a receptionist sitting at the front desk.";

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
        Choices.clear();

        if (!Character::VERIFY_SKILL(player, Skill::Type::ROGUERY) && !Character::VERIFY_SKILL(player, Skill::Type::PARADOXING) && !Character::VERIFY_CODEWORD(player, Codeword::Type::PROTEUS) && !Character::VERIFY_CODEWORD(player, Codeword::Type::CAMOUFLAGE))
        {
            Choices.push_back(Choice::Base("Enter and try to bluff your way past the receptionist", 436));
            Choices.push_back(Choice::Base("Give up any hope of entering the building", 311));
        }
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_CODEWORD(player, Codeword::Type::PROTEUS))
        {
            Character::REMOVE_CODEWORD(player, Codeword::Type::PROTEUS);

            return 374;
        }
        else if (Character::VERIFY_SKILL(player, Skill::Type::PARADOXING))
        {
            return 417;
        }
        else
        {
            return 396;
        }
    }
};

class Story354 : public Story::Base
{
public:
    Story354()
    {
        ID = 354;

        Text = "At the elevator door, you turn to look back at the others locked in stasis. They will outlive this frigid dying world. Perhaps even outlive the universe, forever just out of arm's reach of the power they coveted.\n\nIf the hover-droids are still waiting above then you will have a hard fight returning to the surface. And even if you survive the Ice Wastes, the Earth's time is running out. You don't know how long you have left.";

        Type = Story::Type::UNKNOWN;

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }
};

class Story355 : public Story::Base
{
public:
    Story355()
    {
        ID = 355;

        Text = "The scintillant beam of deadly radiation stabs out, striking the mysterious figure full in the chest. His smile does not waver. Although his clothing is burned away, his smooth pale skin is completely unscathed by the blast. Your arm drops limply to your side as you stare in disbelief. You have never seen anyone survive a direct hit from a BARYSAL GUN, unless protected by body armour.\n\nThe look in those silvery eyes confirms what you have guessed already. This is not a living man you are facing. It is a blood-drinking predator, one of the vampires of ancient myth.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 419; }
};

class Story356 : public Story::Base
{
public:
    Story356()
    {
        ID = 356;

        Text = "Portrin Fax provides you with a makeshift couch and you drift gratefully off to sleep, being weary after your long trek through the steaming swamplands.\n\nIt seems only a moment later that he is shaking awake. As you open your eyes, he jumps back and gives a jittery laugh. \"Morning.\" he says.\n\nYou yawn and stretch. The air inside the building feels almost chill in comparison to the sweltering heat outside. \"Morning?\" How can you tell?\"\n\nFax points to a clock on the wall, above an archway leading to a staircase that descends underground. \"There is one indicator. Also, although the sun never sets here, it does not move across the sky. Indeed, at times I have thought to see two suns.\"\n\n\"No doubt the 'second sun' is an orbiting mirror aligned so as to focus the sunlight on this region.\"";

        Choices.clear();
        Choices.push_back(Choice::Base("Ask Fax to show you where he gets his food", 378));
        Choices.push_back(Choice::Base("Ask first about the barren patch of ground you discovered leading to his dwelling", 399));
        Choices.push_back(Choice::Base("Leave now and press on westwards", 420));

        Controls = Story::Controls::STANDARD;
    }
};

class Story357 : public Story::Base
{
public:
    Story357()
    {
        ID = 357;

        Text = "The panel bends inward, then gives with a crack. You push through past a blanket which had been hanging against the wall. A fire crackles in the centre of the foyer, which is so badly dilapidated that it is little more than a cave. A rug hangs across the entrance, sealing out a keening blizzard. The chamber is warm, with a sweaty reek from its three occupants. You take them to be trappers, judging by the animal skins hung around the walls.\n\nThe three jump to their feet. Their surprise at your sudden appearance soon turns to open hostility. Two draw weapons.. One has a long knife, while the man nearest to the fire has a BARYSAL GUN.";

        Choices.clear();
        Choices.push_back(Choice::Base("Use [CLOSE COMBAT]", 97, Skill::Type::CLOSE_COMBAT));
        Choices.push_back(Choice::Base("[SHOOTING] a charged BARYSAL GUN", 119, Skill::Type::SHOOTING));
        Choices.push_back(Choice::Base("Fall back on [CUNNING]", 185, Skill::Type::CUNNING));
        Choices.push_back(Choice::Base("Try a spot of [ROGUERY]", 163, Skill::Type::ROGUERY));
        Choices.push_back(Choice::Base("Rely on [AGILITY]", 207, Skill::Type::AGILITY));
        Choices.push_back(Choice::Base("Otherwise", 272));

        Controls = Story::Controls::STANDARD;
    }
};

class Story358 : public Story::Base
{
public:
    Story358()
    {
        ID = 358;

        Text = "Boche pulls you away, expelling a hiss of breath between his teeth as he glances back at Golgoth through the fog. \"A stroke of bad luck, running into that one,\" he mutters.\n\n\"Why should a USI agent be interested in us?\"\n\nBoche scowls. \"Golgoth is a loose cannon. He wouldn't wait to ask what his superiors thought about something. Did you hear about the hijack of the warship Illustrious by the Seventh Seal Cult a couple of years ago? The Vice President was aboard and a ransom was being negotiated until USI stepped in.\"\n\nYou cast your mind back across the sketchy rumours and news reports that stretched across the Atlantic. \"I seem to remember that the Seventh Seal Cult was al but eradicated -- at least eighty people dead. Don't tell me Golgoth was in charge of that operation.\"\n\n\"In charge? He was the operation -- one-man death squad. He went in alone under disguise and variously stabbed, poisoned, shot or blew up every terrorist aboard.\"\n\n\"Well, it seems pretty impressive...\" you venture.\n\nBoche shakes his head grimly. \"He's a psychopath; he relishes killing. His USI badge just give him carte blanche to indulge his depravities. Such people are evil no matter how noble a mask they wear.\" Boche's mood brightens as he changes the subject: \"Now I have a few things to take care of. I'll meet you tomorrow morning at the Empty Quarter Cafe, next to the bazaar.\"\n\nBoche hurries off into the swirling fog. You are left alone on the street.";

        Choices.clear();

        Controls = Story::Controls::STANDARD;
    }

    int Continue(Character::Base &player) { return 95; }
};

class Story359 : public Story::Base
{
public:
    Story359()
    {
        ID = 359;

        Text = "Strolling through the bazaar, you check what items are on sale.";

        Choices.clear();

        Controls = Story::Controls::SHOP;
    }

    void Event(Character::Base &player)
    {
        Shop = {{Item::FOOD_PACK, 2}, {Item::GAS_MASK, 15}, {Item::FLASHLIGHT, 8}, {Item::MEDICAL_KIT, 8}, {Item::POLARIZED_GOGGLES, 6}, {Item::ROPE, 3}};
    }

    int Continue(Character::Base &player)
    {
        if (Character::VERIFY_CODEWORD(player, Codeword::Type::DIAMOND))
        {
            return 381;
        }
        else
        {
            return 402;
        }
    }
};

auto earth23rdCentury = Earth23rdCentury();
auto prologue = Prologue();
auto story001 = Story001();
auto story002 = Story002();
auto story003 = Story003();
auto story004 = Story004();
auto story005 = Story005();
auto story006 = Story006();
auto story007 = Story007();
auto story008 = Story008();
auto story009 = Story009();
auto story010 = Story010();
auto story011 = Story011();
auto story012 = Story012();
auto story013 = Story013();
auto story014 = Story014();
auto story015 = Story015();
auto story016 = Story016();
auto story017 = Story017();
auto story018 = Story018();
auto story019 = Story019();
auto story020 = Story020();
auto story021 = Story021();
auto story022 = Story022();
auto story023 = Story023();
auto story024 = Story024();
auto story025 = Story025();
auto story026 = Story026();
auto story027 = Story027();
auto story028 = Story028();
auto story029 = Story029();
auto story030 = Story030();
auto story031 = Story031();
auto story032 = Story032();
auto story033 = Story033();
auto story034 = Story034();
auto story035 = Story035();
auto story036 = Story036();
auto story037 = Story037();
auto story038 = Story038();
auto story039 = Story039();
auto story040 = Story040();
auto story041 = Story041();
auto story042 = Story042();
auto story043 = Story043();
auto story044 = Story044();
auto story045 = Story045();
auto story046 = Story046();
auto story047 = Story047();
auto story048 = Story048();
auto story049 = Story049();
auto story050 = Story050();
auto story051 = Story051();
auto story052 = Story052();
auto story053 = Story053();
auto story054 = Story054();
auto story055 = Story055();
auto story056 = Story056();
auto story057 = Story057();
auto story058 = Story058();
auto story059 = Story059();
auto story060 = Story060();
auto story061 = Story061();
auto story062 = Story062();
auto story063 = Story063();
auto story064 = Story064();
auto story065 = Story065();
auto story066 = Story066();
auto story067 = Story067();
auto story068 = Story068();
auto story069 = Story069();
auto story070 = Story070();
auto story071 = Story071();
auto story072 = Story072();
auto story073 = Story073();
auto story074 = Story074();
auto story075 = Story075();
auto story076 = Story076();
auto story077 = Story077();
auto story078 = Story078();
auto story079 = Story079();
auto story080 = Story080();
auto story081 = Story081();
auto story082 = Story082();
auto story083 = Story083();
auto story084 = Story084();
auto story085 = Story085();
auto story086 = Story086();
auto story087 = Story087();
auto story088 = Story088();
auto story089 = Story089();
auto story090 = Story090();
auto story091 = Story091();
auto story092 = Story092();
auto story093 = Story093();
auto story094 = Story094();
auto story095 = Story095();
auto story096 = Story096();
auto story097 = Story097();
auto story098 = Story098();
auto story099 = Story099();
auto story100 = Story100();
auto story101 = Story101();
auto story102 = Story102();
auto story103 = Story103();
auto story104 = Story104();
auto story105 = Story105();
auto story106 = Story106();
auto story107 = Story107();
auto story108 = Story108();
auto story109 = Story109();
auto story110 = Story110();
auto story111 = Story111();
auto story112 = Story112();
auto story113 = Story113();
auto story114 = Story114();
auto story115 = Story115();
auto story116 = Story116();
auto story117 = Story117();
auto story118 = Story118();
auto story119 = Story119();
auto story120 = Story120();
auto story121 = Story121();
auto story122 = Story122();
auto story123 = Story123();
auto story124 = Story124();
auto story125 = Story125();
auto story126 = Story126();
auto story127 = Story127();
auto story128 = Story128();
auto story129 = Story129();
auto story130 = Story130();
auto story131 = Story131();
auto story132 = Story132();
auto story133 = Story133();
auto story134 = Story134();
auto story135 = Story135();
auto story136 = Story136();
auto story137 = Story137();
auto story138 = Story138();
auto story139 = Story139();
auto story140 = Story140();
auto story141 = Story141();
auto story142 = Story142();
auto story143 = Story143();
auto story144 = Story144();
auto story145 = Story145();
auto story146 = Story146();
auto story147 = Story147();
auto story148 = Story148();
auto story149 = Story149();
auto story150 = Story150();
auto story151 = Story151();
auto story152 = Story152();
auto story153 = Story153();
auto story154 = Story154();
auto story155 = Story155();
auto story156 = Story156();
auto story157 = Story157();
auto story158 = Story158();
auto story159 = Story159();
auto story160 = Story160();
auto story161 = Story161();
auto story162 = Story162();
auto story163 = Story163();
auto story164 = Story164();
auto story165 = Story165();
auto story166 = Story166();
auto story167 = Story167();
auto story168 = Story168();
auto story169 = Story169();
auto story170 = Story170();
auto story171 = Story171();
auto story172 = Story172();
auto story173 = Story173();
auto story174 = Story174();
auto story175 = Story175();
auto story176 = Story176();
auto story177 = Story177();
auto story178 = Story178();
auto story179 = Story179();
auto event179 = Event179();
auto story180 = Story180();
auto story181 = Story181();
auto story182 = Story182();
auto story183 = Story183();
auto story184 = Story184();
auto story185 = Story185();
auto story186 = Story186();
auto story187 = Story187();
auto story188 = Story188();
auto story189 = Story189();
auto story190 = Story190();
auto story191 = Story191();
auto story192 = Story192();
auto story193 = Story193();
auto story194 = Story194();
auto story195 = Story195();
auto story196 = Story196();
auto story197 = Story197();
auto story198 = Story198();
auto story199 = Story199();
auto story200 = Story200();
auto story201 = Story201();
auto story202 = Story202();
auto story203 = Story203();
auto story204 = Story204();
auto story205 = Story205();
auto story206 = Story206();
auto story207 = Story207();
auto story208 = Story208();
auto story209 = Story209();
auto story210 = Story210();
auto story211 = Story211();
auto story212 = Story212();
auto story213 = Story213();
auto story214 = Story214();
auto story215 = Story215();
auto story216 = Story216();
auto story217 = Story217();
auto story218 = Story218();
auto story219 = Story219();
auto story220 = Story220();
auto story221 = Story221();
auto story222 = Story222();
auto story223 = Story223();
auto story224 = Story224();
auto story225 = Story225();
auto story226 = Story226();
auto story227 = Story227();
auto story228 = Story228();
auto story229 = Story229();
auto story230 = Story230();
auto story231 = Story231();
auto story232 = Story232();
auto story233 = Story233();
auto story234 = Story234();
auto story235 = Story235();
auto story236 = Story236();
auto story237 = Story237();
auto story238 = Story238();
auto story239 = Story239();
auto story240 = Story240();
auto story241 = Story241();
auto story242 = Story242();
auto story243 = Story243();
auto story244 = Story244();
auto story245 = Story245();
auto event245 = Event245();
auto story246 = Story246();
auto story247 = Story247();
auto story248 = Story248();
auto story249 = Story249();
auto story250 = Story250();
auto story251 = Story251();
auto story252 = Story252();
auto story253 = Story253();
auto story254 = Story254();
auto story255 = Story255();
auto story256 = Story256();
auto story257 = Story257();
auto story258 = Story258();
auto story259 = Story259();
auto story260 = Story260();
auto story261 = Story261();
auto story262 = Story262();
auto story263 = Story263();
auto story264 = Story264();
auto story265 = Story265();
auto story266 = Story266();
auto story267 = Story267();
auto story268 = Story268();
auto story269 = Story269();
auto story270 = Story270();
auto story271 = Story271();
auto story272 = Story272();
auto story273 = Story273();
auto story274 = Story274();
auto story275 = Story275();
auto story276 = Story276();
auto story277 = Story277();
auto story278 = Story278();
auto story279 = Story279();
auto story280 = Story280();
auto story281 = Story281();
auto story282 = Story282();
auto story283 = Story283();
auto story284 = Story284();
auto story285 = Story285();
auto story286 = Story286();
auto story287 = Story287();
auto story288 = Story288();
auto story289 = Story289();
auto story290 = Story290();
auto story291 = Story291();
auto story292 = Story292();
auto story293 = Story293();
auto story294 = Story294();
auto story295 = Story295();
auto story296 = Story296();
auto story297 = Story297();
auto story298 = Story298();
auto story299 = Story299();
auto story300 = Story300();
auto story301 = Story301();
auto story302 = Story302();
auto story303 = Story303();
auto story304 = Story304();
auto story305 = Story305();
auto story306 = Story306();
auto story307 = Story307();
auto story308 = Story308();
auto story309 = Story309();
auto story310 = Story310();
auto story311 = Story311();
auto story312 = Story312();
auto story313 = Story313();
auto story314 = Story314();
auto story315 = Story315();
auto story316 = Story316();
auto story317 = Story317();
auto story318 = Story318();
auto story319 = Story319();
auto story320 = Story320();
auto story321 = Story321();
auto story322 = Story322();
auto story323 = Story323();
auto story324 = Story324();
auto story325 = Story325();
auto story326 = Story326();
auto story327 = Story327();
auto story328 = Story328();
auto story329 = Story329();
auto story330 = Story330();
auto story331 = Story331();
auto story332 = Story332();
auto story333 = Story333();
auto story334 = Story334();
auto story335 = Story335();
auto story336 = Story336();
auto story337 = Story337();
auto story338 = Story338();
auto story339 = Story339();
auto story340 = Story340();
auto story341 = Story341();
auto story342 = Story342();
auto story343 = Story343();
auto story344 = Story344();
auto story345 = Story345();
auto story346 = Story346();
auto story347 = Story347();
auto story348 = Story348();
auto story349 = Story349();
auto story350 = Story350();
auto story351 = Story351();
auto story352 = Story352();
auto story353 = Story353();
auto story354 = Story354();
auto story355 = Story355();
auto story356 = Story356();
auto story357 = Story357();
auto story358 = Story358();
auto story359 = Story359();

void InitializeStories()
{
    Stories = {
        &earth23rdCentury, &event179, &event245,
        &prologue, &story001, &story002, &story003, &story004, &story005, &story006, &story007, &story008, &story009,
        &story010, &story011, &story012, &story013, &story014, &story015, &story016, &story017, &story018, &story019,
        &story020, &story021, &story022, &story023, &story024, &story025, &story026, &story027, &story028, &story029,
        &story030, &story031, &story032, &story033, &story034, &story035, &story036, &story037, &story038, &story039,
        &story040, &story041, &story042, &story043, &story044, &story045, &story046, &story047, &story048, &story049,
        &story050, &story051, &story052, &story053, &story054, &story055, &story056, &story057, &story058, &story059,
        &story060, &story061, &story062, &story063, &story064, &story065, &story066, &story067, &story068, &story069,
        &story070, &story071, &story072, &story073, &story074, &story075, &story076, &story077, &story078, &story079,
        &story080, &story081, &story082, &story083, &story084, &story085, &story086, &story087, &story088, &story089,
        &story090, &story091, &story092, &story093, &story094, &story095, &story096, &story097, &story098, &story099,
        &story100, &story101, &story102, &story103, &story104, &story105, &story106, &story107, &story108, &story109,
        &story110, &story111, &story112, &story113, &story114, &story115, &story116, &story117, &story118, &story119,
        &story120, &story121, &story122, &story123, &story124, &story125, &story126, &story127, &story128, &story129,
        &story130, &story131, &story132, &story133, &story134, &story135, &story136, &story137, &story138, &story139,
        &story140, &story141, &story142, &story143, &story144, &story145, &story146, &story147, &story148, &story149,
        &story150, &story151, &story152, &story153, &story154, &story155, &story156, &story157, &story158, &story159,
        &story160, &story161, &story162, &story163, &story164, &story165, &story166, &story167, &story168, &story169,
        &story170, &story171, &story172, &story173, &story174, &story175, &story176, &story177, &story178, &story179,
        &story180, &story181, &story182, &story183, &story184, &story185, &story186, &story187, &story188, &story189,
        &story190, &story191, &story192, &story193, &story194, &story195, &story196, &story197, &story198, &story199,
        &story200, &story201, &story202, &story203, &story204, &story205, &story206, &story207, &story208, &story209,
        &story210, &story211, &story212, &story213, &story214, &story215, &story216, &story217, &story218, &story219,
        &story220, &story221, &story222, &story223, &story224, &story225, &story226, &story227, &story228, &story229,
        &story230, &story231, &story232, &story233, &story234, &story235, &story236, &story237, &story238, &story239,
        &story240, &story241, &story242, &story243, &story244, &story245, &story246, &story247, &story248, &story249,
        &story250, &story251, &story252, &story253, &story254, &story255, &story256, &story257, &story258, &story259,
        &story260, &story261, &story262, &story263, &story264, &story265, &story266, &story267, &story268, &story269,
        &story270, &story271, &story272, &story273, &story274, &story275, &story276, &story277, &story278, &story279,
        &story280, &story281, &story282, &story283, &story284, &story285, &story286, &story287, &story288, &story289,
        &story290, &story291, &story292, &story293, &story294, &story295, &story296, &story297, &story298, &story299,
        &story300, &story301, &story302, &story303, &story304, &story305, &story306, &story307, &story308, &story309,
        &story310, &story311, &story312, &story313, &story314, &story315, &story316, &story317, &story318, &story319,
        &story320, &story321, &story322, &story323, &story324, &story325, &story326, &story327, &story328, &story329,
        &story330, &story331, &story332, &story333, &story334, &story335, &story336, &story337, &story338, &story339,
        &story340, &story341, &story342, &story343, &story344, &story345, &story346, &story347, &story348, &story349,
        &story350, &story351, &story352, &story353, &story354, &story355, &story356, &story357, &story358, &story359};
}

#endif