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
        CHARGED_ITEMS
    };

    class Base
    {
    public:
        const char *Text = NULL;

        Choice::Type Type = Choice::Type::NORMAL;

        Skill::Type Skill = Skill::Type::NONE;

        std::vector<Item::Base> Items = std::vector<Item::Base>();

        Codeword::Type Codeword = Codeword::Type::NONE;

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

        std::pair<Item::Base, Item::Base> Trade;

        // Player selects items to take up to a certain limit
        std::vector<Item::Base> Take = std::vector<Item::Base>();

        // Player selects items to lose
        std::vector<Item::Base> ToLose = std::vector<Item::Base>();

        int Limit = 0;

        int LimitSkills = 0;

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

        Text = "The passage is lit by gleaming blue-white tubes along the ceiling. They cast a garish glare in which the sight of a dead body ahead seems like a glimpse of a nightmare. Boche and the baron watch while you stoop and inspect the corpse. You roll it over, surprised at how well the shrivelled flesh has kept in the cold air. \"His own mother could still recognize him,\" you remark with grim humour.\n\n\"Except she'll have been dead two centuries as well,\" says Boche. He gazes off along the corridor, then gives a start. \"There's another one!\"\n\nThe baron sweeps on ahead and hovers low over the next body. \"He died of a broken back.\"\n\"So did that first one,\" you say as you come hurrying up with Boche.\n\nA metallic scuttling sound resounds from the far end of the passage. Instantly your whole body is tensely alert, nerves jangling in fear of the unknown. Then you see it approaching along the passage like a giant robot spider: the body a glass bubble filled with blue fluid, surrounded by legs formed from long articulated steel pipes. Inside the glass bubble floats a lumpish embryonic figure pierced by many tubes. Its eyes are open and it is watching you.\n\nBoche gives a gasp of disgust and fires his barysal gun at the glass bubble. But the thing has already raised a row of its legs to form a shield, and the blast splashes away leaving hardly a mark.\n\n\"I think we'd better run,\" he says backing away.";

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

        Text = "You step through into what seems to be a recreation room, with padded couches set around low glass-topped tables. At the far end of the room, a row of couches is set facing a screen on the wall. An image flickers into sharp clarity, but it takes you a moment to identify the wary, baffled-looking figure in the picture. It is you.\n\nYou glance at the camera mounted on the wall, then back at the screen. The picture changes to show other views: the parked Manta sky-car, the outer door, the gondo trudging up and down in the snow outside.\n\n\"Who spoke?\" you say, feeling uneasy at calling out to an empty room.\n\n\"I did.\" The voice comes from the screen.\n\n\"Gaia?\"\n\n\"Yes. Attend, as there is little time before I fall again to the darkness. You must go to Giza.\" The screen flickers to show another scene, now of the pyramids against a backdrop of crystal night. \"The word 'humbaba' is the key to entry. Find Gilgamesh and activate him. He will be your servant in the race for the Heart.\"\n\n\"Race? So others are seeking the Heart?\"\n\n\"Yes. The broadcast you received was seen by man across the globe. The mightiest of this age will compete for the power. That is the way of mankind.\" Gaia gives a sound that might almost be a sigh, then speaks more quickly. \"I am working to secure an area of my mind that will be protected against the viruses that beset me. I will speak again to you when it is easier.\"\n\nThe screen suddenly goes blank. Gaia is no longer present.";

        Choices.clear();
        Choices.push_back(Choice::Base("Take a look at the sky car", 49));
        Choices.push_back(Choice::Base("Leave", 395));

        Controls = Story::Controls::STANDARD;
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
            Choices[0].Destination = 160;
        }
        else
        {
            Choices[0].Destination = 182;
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
        PreText = "Sunlight, hazed by a high overcast, is thrown up from the snow dunes in an unremitting glare as white and harsh as exposed bone. Squinting does no good. Your eyes feel gritty and tired. On the fourth evening, huddling behind the shelter of a crag of ice, you gaze across the landscape. It is like looking through a film of blood. The next day you find the sunrise burn so hard that you cannot stand to open your eyes.\n\nSnow-blinded, you can only sit and wait for the dazzle to clear. If you were to press on now, you would soon lose your bearings and die. As you wait, the chill crawls deeper into your bones.";

        auto DAMAGE = -3;

        if (Character::CHECK_VEHICLE(player, Character::Vehicle::BURREK))
        {
            DAMAGE = -2;
        }

        if (!Character::VERIFY_ITEMS(player, {Item::Type::FUR_COAT}) && !Character::VERIFY_ITEMS(player, {Item::Type::COLD_WEATHER_SUIT}))
        {
            DAMAGE--;
        }

        PreText += "\n\nYou LOSE " + std::to_string(-DAMAGE) + " Life Points.";

        Character::GAIN_LIFE(player, DAMAGE);

        if (player.Life > 0)
        {
            if (Character::CHECK_VEHICLE(player, Character::Vehicle::BURREK))
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
        PreText = "Captain Novak comes racing towards you out of the smoke. His uniform is torn and signed by the explosion and he has a wild look in his eyes. You are not sure whether to block his way or stand aside, when suddenly a barysal shot streams through the air, piercing his brain. A second shot hits him as he falls, but glances off his armour and ricochets into you.\n\nYou are badly burned.";

        auto DAMAGE = -6;

        if (Character::VERIFY_ITEMS(player, {Item::Type::SPECULUM_JACKET}))
        {
            DAMAGE = -4;
        }

        PreText += "\n\nYou LOSE " + std::to_string(-DAMAGE) + " Life Points.";

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

        if (Character::VERIFY_ITEMS(player, {Item::Type::SPECULUM_JACKET}))
        {
            PreText += "[CLOSE COMBAT]";

            DAMAGE = -1;
        }

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Points.";

        Character::GAIN_LIFE(player, DAMAGE);

        if (player.Life > 0)
        {
            if (Character::VERIFY_ITEMS(player, {Item::Type::SPECULUM_JACKET}))
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
        Choices.push_back(Choice::Base("... about the Saharan Ice Wastes", 77));
        Choices.push_back(Choice::Base("... the city of Kahira", 143));
        Choices.push_back(Choice::Base("... where you should take lodging", 99));

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
        Choices.push_back(Choice::Base("Return fire (BARYSAL GUN)", 64, Choice::Type::CHARGED_ITEMS, {Item::BARYSAL_GUN}));
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
        PreText = "Golgoth fires back, but he has no defence against the unstoppable power of the MANTRAMUKTA CANNON, which literally blasts him apart. His last shot strikes you glancingly, however, and you feel an agonizing pain coursing through your side.";

        auto DAMAGE = -3;

        if (Character::VERIFY_ITEMS(player, {Item::Type::SPECULUM_JACKET}))
        {
            DAMAGE = -2;
        }

        PreText += "\n\nYou LOSE " + std::to_string(-DAMAGE) + " Life Points.";

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

        PreText += "You LOSE " + std::to_string(-DAMAGE) + " Life Points.";

        Character::GAIN_LIFE(player, DAMAGE);

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

        if (Character::CHECK_VEHICLE(player, Character::Vehicle::BURREK))
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

            if (Character::CHECK_VEHICLE(player, Character::Vehicle::BURREK))
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

        Text = "You touch the red button. The box produces a prolonged hum which rises in tone and then ends with a bleep. \"Ready,\" says a synthetic feminine voice from the speaker.\n\nTaken aback for a moment, you lean closer and gingerly speak to the box. \"Er... ready for what?\"\n\n\"Explain,\" says the box crisply. \"Your query was unspecific.\"\n\n\"Is this a radio? Who am I talking to?\"\n\nLITTLE GAIA: \"I am a miniature facsimile of the Global Artificial Intelligence Array,\" replies the box.\n\n\"Gaia? But Gaia is crazy.\"\n\nLITTLE GAIA: \"I was loaded with Gaia's program prior to virus infection. I am able to model the thinking of the Gaia system at a reduced rate owing to my limited memory capacity, which now stands at 512 gigabytes.\"\n\n\"What can you tell me about the Heart of Volent?\"\n\nLITTLE GAIA: \"Nothing. No such information has been loaded into my memory.\" The device has an annoyingly smug little voice. All the same, if it has even a fraction of Gaia's intelligence then it may be useful.";

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

void InitializeStories()
{
    Stories = {
        &earth23rdCentury,
        &prologue, &story001, &story002, &story003, &story004, &story005, &story006, &story007, &story008, &story009,
        &story010, &story011, &story012, &story013, &story014, &story015, &story016, &story017, &story018, &story019,
        &story020, &story021, &story022, &story023, &story024, &story025, &story026, &story027, &story028, &story029,
        &story030, &story031, &story032, &story033, &story034, &story035, &story036, &story037, &story038, &story039,
        &story040, &story041, &story042, &story043, &story044, &story045, &story046, &story047, &story048, &story049,
        &story050, &story051, &story052, &story053, &story054, &story055, &story056, &story057, &story058, &story059,
        &story060};
}

#endif