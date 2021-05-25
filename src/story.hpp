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
        GAIN_MONEY
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

        Base(const char *text, int destination, Choice::Type type, Codeword::Type codeword)
        {
            Text = text;
            Destination = destination;
            Type = type;
            Codeword = codeword;
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

        if (Character::CHECK_VEHICLE(player, Vehicle::Type::BURREK))
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

        Text = "Bador expresses dismay when you tell him you intend to cross the Ice Wastes. \"By your father's beard! Do you wish to become a corpse with hoarfrost in your veins? Put aside all thought of such a scheme, I pray you!\" You cannot help smiling. \"What?\" says Bador, starting to weep. \"Do you mock my concern?\"\n\nYou place a hand on his sleeve. \"Calm yourself. You and I are strangers, and you already have your fee. Do not allow thought of my death to upset you, but give me advice on how to avoid such a fate.\"\n\n\"Only the barbarian Ebor venture into the Sahara, and even they go no further that its fringes. It is a place of ghosts and devils, and the wind is like flint.\"\n\n\"The Ebor? A nomad tribe? How do they survive?\"\n\n\"They have burreks, shaggy thick-necked beasts that grow folds of fat. When the blizzard comes, the Ebor rider shelters by his burrek and bleeds the animal, frying up a blood pudding to sustain him.\" Bador grimaces to show what he thinks of such a custom.";

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
        Choices.push_back(Choice::Base("NEMESIS: Suggest an alliance", 236, Codeword::Type::NEMESIS));
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
        Choices.push_back(Choice::Base("ENKIDU: order your automaton to attack", 220, Codeword::Type::ENKIDU));
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

        Text = "The computer terminals are only intended to access the library catalogue, but you have no trouble routing into the building's administrative computer and then opening an outside line via the rooftop satellite dish. Like most organizations with the ability to connect into global communications, the Society protects its system from accidental linkage into Gaia by the use of a filter program. This is necessary to prevent infection by the same viruses that are resident in Gaia, as well as to stop Gaia from taking over the Society's whole system for her own use.\n\nYou set a standard filter-override program running. It should take a few minutes, and to kill time you run a check on other users who have logged into the system recently. Only one name is displayed: Janus Gaunt. He requested all the Society's records regarding the Heart of Volent. Intrigued, you call up his biofile. The screen shows a round-faced man with extremely white skin and hair like silver floss. Flicking idly through the data, you find he has a reputation for outstanding work in the fields of bioengineering and nanotechnology. The address of his mansion causes you a double-take; it is located in the Paris catacombs. You were not even sure Paris still existed.\n\nThe terminal bleeps, informing you the link with Gaia is ready. You switch over. When you type in your query about the Heart, Gaia's response is swift: THE HEART MUST BE DESTROYED. ACTIVATION OF ITS POWER WILL CRASH THE UNIVERSE, WIPING OUT ALL THAT EXISTS.\n\nYou reply: INCLUDING EARTH?\n\nEVERYTHING, Gaia tells you. BARYSAL BOMBARDMENT CAN CAUSE A CRITICAL RESONANCE. DESTROYING THE HEART'S CRYSTALLINE STRUCTURE. TWO SIMULTANEOUS BOMBARDMENTS MUST BE MADE, THE BEAMS PHASED AND CROSSING AT RIGHT ANGLES.\n\nThis is awkward. From what you have heard, the Heart is a gem several metres across. To destroy it as Gaia suggests, you'd need an accomplice. And two BARYSAL GUNs. You try to get further information, but the link is broken. Like a senile invalid, Gaia has lapsed back into her customary incoherence.";

        Choices.clear();
        Choices.push_back(Choice::Base("Study the records on Heart of Volent", 182));
        Choices.push_back(Choice::Base("You have finished in the library", 73));

        Controls = Story::Controls::STANDARD;
    }

    void Event(Character::Base &player)
    {
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
        Choices.push_back(Choice::Base("NEMESIS: Propose an alliance", 204, Codeword::Type::NEMESIS));
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
        PreText = "With the ghastly brain floating after you, you race out of the hall. The passage soon forks, but you have no time to pause and get your bearings. You blunder on, gasping for breath, looking back over your shoulder to see if your pursuer is still there. Losing sight of it around a bend in the tunnel, you begin to calm down and think. The baron is a powerful psychic .. ore powerful than you ever dreamed, to outlive his body like this -- but he cannot survive once the remaining oxygen in his brain tissue is used up. All you have to do is stay ahead of him that long.\n\nYour simple plan is ruined a moment later when, darting around a junction in the passage, you come face to face with a hover-droid. A quarter of a second is long enough for you to start tor turn, and for the hover-droid to identify you as an intruder in the catacombs. As you leap back, its laser flares up and you feel a sickening pain as the hot beam cuts through your gut.";

        auto DAMAGE = -4;

        if (Character::VERIFY_ITEMS(player, {Item::Type::SPECULUM_JACKET}))
        {
            DAMAGE = -3;
        }

        PreText += "\n\nYou LOSE " + std::to_string(-DAMAGE) + " Life Points.";

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
        PreText = "Your shot burns through Singh's armour and he staggers back, but although wounded he is far from beaten. He presses the fire button on his MANTRAMUKTA CANNON just as Boche goes for an opportunist shot at you. The beam carves through your shoulder.";

        auto DAMAGE = -2;

        if (Character::VERIFY_ITEMS(player, {Item::Type::SPECULUM_JACKET}))
        {
            DAMAGE = -1;
        }

        PreText += "\n\nYou LOSE " + std::to_string(-DAMAGE) + " Life Points.";

        Character::GAIN_LIFE(player, DAMAGE);

        if (player.Life > 0)
        {
            PreText += "\n\nA moment later Boche falls as Singh swings the cannon around, blasting him apart with a torrent of searing energy.\n\nThere is a moment of silence as the cannon's blast cuts out. It will take a few seconds to build up charge before it can fire again. Golgoth seizes the chance to take aim with his barysal gun. This is the showdown that will decide which of you lives to claim the power of the Heart.";
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
        &story060, &story061, &story062, &story063, &story064, &story065, &story066, &story067, &story068, &story069,
        &story070, &story071, &story072, &story073, &story074, &story075, &story076, &story077, &story078, &story079,
        &story080, &story081, &story082, &story083, &story084, &story085, &story086, &story087, &story088, &story089,
        &story090, &story091, &story092, &story093, &story094, &story095, &story096, &story097, &story098, &story099,
        &story100, &story101, &story102, &story103, &story104, &story105, &story106, &story107, &story108, &story109,
        &story110, &story111, &story112, &story113, &story114, &story115, &story116, &story117, &story118, &story119,
        &story120, &story121, &story122, &story123, &story124, &story125, &story126, &story127, &story128, &story129,
        &story130, &story131, &story132, &story133, &story134, &story135, &story136, &story137, &story138, &story139};
}

#endif