// Standard IO
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

// Using SDL
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

// JSON library
#include "nlohmann/json.hpp"

#include "constants.hpp"
#include "config.hpp"
#include "controls.hpp"
#include "input.hpp"
#include "items.hpp"
#include "skills.hpp"
#include "vehicle.hpp"
#include "character.hpp"
#include "story.hpp"

#if defined(_WIN32)

#include <windows.h>
#include <shlobj.h>

#else

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#endif

// forward declarations
bool aboutScreen(SDL_Window *window, SDL_Renderer *renderer);
bool characterScreen(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, Story::Base *story);
bool glossaryScreen(SDL_Window *window, SDL_Renderer *renderer, std::vector<Skill::Base> Skills);
bool inventoryScreen(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, Story::Base *story, std::vector<Item::Base> &Items, Control::Type mode, int limit);
bool loseSkills(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, int limit);
bool mainScreen(SDL_Window *window, SDL_Renderer *renderer, int storyID);
bool mapScreen(SDL_Window *window, SDL_Renderer *renderer);
bool processStory(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, Story::Base *story);
bool shopScreen(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, Story::Base *story, Control::Type mode);
bool storyScreen(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, int id);
bool takeScreen(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, std::vector<Item::Base> items, int limit);
bool tradeScreen(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, Item::Base mine, Item::Base theirs);

Character::Base selectCharacter(SDL_Window *window, SDL_Renderer *renderer);
Control::Type gameScreen(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, bool save_botton);

Story::Base *processChoices(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, Story::Base *story);
Story::Base *renderChoices(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, Story::Base *story);

void renderAdventurer(SDL_Window *window, SDL_Renderer *renderer, TTF_Font *font, Character::Base &player);

SDL_Surface *createImage(const char *image)
{
    // Load splash image
    auto surface = IMG_Load(image);

    if (surface == NULL)
    {
        std::cerr << "Unable to load image " << image << "! SDL Error: " << SDL_GetError() << std::endl;
    }

    return surface;
}

void createWindow(Uint32 flags, SDL_Window **window, SDL_Renderer **renderer, const char *title, const char *icon)
{
    // The window we'll be rendering to
    *window = NULL;
    *renderer = NULL;

    // Initialize SDL
    if (SDL_Init(flags) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
    }
    else
    {
        SDL_DisplayMode mode;

        SDL_GetCurrentDisplayMode(0, &mode);

        SCREEN_WIDTH = (mode.w) * 0.8;
        SCREEN_HEIGHT = (mode.h) * 0.8;

        Recompute();

        // Create window and renderer
        SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_RENDERER_ACCELERATED, window, renderer);

        SDL_SetRenderDrawBlendMode(*renderer, SDL_BLENDMODE_NONE);

        if (window == NULL || renderer == NULL)
        {
            std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        }
        else if (window)
        {
            SDL_SetWindowTitle(*window, title);
        }

        auto surface = createImage(icon);

        if (surface)
        {
            // The icon is attached to the window pointer
            SDL_SetWindowIcon(*window, surface);

            // ...and the surface containing the icon pixel data is no longer required.
            SDL_FreeSurface(surface);

            surface = NULL;
        }

        SDL_SetRenderDrawColor(*renderer, 0, 0, 0, 255);
        SDL_RenderClear(*renderer);
        SDL_RenderPresent(*renderer);

        SDL_SetRenderDrawColor(*renderer, 0, 0, 0, 255);
        SDL_RenderClear(*renderer);
        SDL_RenderPresent(*renderer);
    }
}

void renderImage(SDL_Renderer *renderer, SDL_Surface *image, int x, int y)
{
    if (image && renderer)
    {
        SDL_Rect position;

        position.w = image->w;
        position.h = image->h;
        position.x = x;
        position.y = y;

        auto texture = SDL_CreateTextureFromSurface(renderer, image);

        if (texture)
        {
            SDL_Rect src;

            src.w = image->w;
            src.h = image->h;
            src.x = 0;
            src.y = 0;

            SDL_RenderCopy(renderer, texture, &src, &position);

            SDL_DestroyTexture(texture);

            texture = NULL;
        }
    }
}

int fitImage(SDL_Renderer *renderer, SDL_Surface *image, int x, int y, int w, int h)
{
    int splash_h = image->h;
    int splash_w = w;

    if (image && renderer)
    {
        SDL_Rect position;

        if (w != image->w)
        {
            splash_h = (int)((double)w / image->w * image->h);
            splash_w = w;

            if (splash_h > h)
            {
                splash_h = h;
                splash_w = (int)((double)h / image->h * image->w);
            }
        }

        position.w = splash_w;
        position.h = splash_h;
        position.x = x;
        position.y = y;

        auto texture = SDL_CreateTextureFromSurface(renderer, image);

        if (texture)
        {
            SDL_Rect src;

            src.w = image->w;
            src.h = image->h;
            src.x = 0;
            src.y = 0;

            SDL_RenderCopy(renderer, texture, &src, &position);

            SDL_DestroyTexture(texture);

            texture = NULL;
        }
    }

    return splash_h;
}

void stretchImage(SDL_Renderer *renderer, SDL_Surface *image, int x, int y, int w, int h)
{
    int splash_h = image->h;
    int splash_w = w;

    if (image && renderer)
    {
        SDL_Rect position;

        position.w = w;
        position.h = h;
        position.x = x;
        position.y = y;

        auto texture = SDL_CreateTextureFromSurface(renderer, image);

        if (texture)
        {
            SDL_Rect src;

            src.w = image->w;
            src.h = image->h;
            src.x = 0;
            src.y = 0;

            SDL_RenderCopy(renderer, texture, &src, &position);

            SDL_DestroyTexture(texture);

            texture = NULL;
        }
    }
}

// Render a portion of the text (image) on bounded surface within the specified window
void renderText(SDL_Renderer *renderer, SDL_Surface *text, Uint32 bg, int x, int y, int bounds, int offset)
{
    if (renderer)
    {
        if (text && renderer)
        {
            SDL_Rect dst;
            SDL_Rect src;

            // select portion to render
            src.w = text->w;
            src.h = text->h < bounds ? text->h : bounds;
            src.y = offset;
            src.x = 0;

            // specify location within the window
            dst.w = text->w;
            dst.h = text->h < bounds ? text->h : bounds;
            dst.x = x;
            dst.y = y;

            if (bg != 0)
            {
                SDL_SetRenderDrawColor(renderer, R(bg), G(bg), B(bg), A(bg));
                SDL_RenderFillRect(renderer, &dst);
            }

            auto texture = SDL_CreateTextureFromSurface(renderer, text);

            if (texture)
            {
                SDL_RenderCopy(renderer, texture, &src, &dst);

                SDL_DestroyTexture(texture);

                texture = NULL;
            }
        }
    }
}

// create text image with line wrap limit
SDL_Surface *createText(const char *text, const char *ttf, int font_size, SDL_Color textColor, int wrap, int style = TTF_STYLE_NORMAL)
{
    SDL_Surface *surface = NULL;

    TTF_Init();

    auto font = TTF_OpenFont(ttf, font_size);

    if (font)
    {
        TTF_SetFontStyle(font, style);

        surface = TTF_RenderText_Blended_Wrapped(font, text, textColor, wrap);

        TTF_CloseFont(font);

        font = NULL;
    }

    TTF_Quit();

    return surface;
}

void fillRect(SDL_Renderer *renderer, int w, int h, int x, int y, int color)
{
    SDL_Rect rect;

    rect.w = w;
    rect.h = h;
    rect.x = x;
    rect.y = y;

    SDL_SetRenderDrawColor(renderer, R(color), G(color), B(color), A(color));
    SDL_RenderFillRect(renderer, &rect);
}

void drawRect(SDL_Renderer *renderer, int w, int h, int x, int y, int color)
{
    SDL_Rect rect;

    rect.w = w;
    rect.h = h;
    rect.x = x;
    rect.y = y;

    SDL_SetRenderDrawColor(renderer, R(color), G(color), B(color), A(color));
    SDL_RenderDrawRect(renderer, &rect);
}

void putText(SDL_Renderer *renderer, const char *text, TTF_Font *font, int space, SDL_Color fg, Uint32 bg, int style, int w, int h, int x, int y)
{
    if (renderer)
    {
        TTF_SetFontStyle(font, style);

        auto surface = TTF_RenderText_Blended_Wrapped(font, text, fg, w - 2 * space);

        if (surface)
        {
            fillRect(renderer, w, h, x, y, bg);

            renderText(renderer, surface, bg, x + space, y + space, h - 2 * space, 0);

            SDL_FreeSurface(surface);

            surface = NULL;
        }
    }
}

void fillWindow(SDL_Renderer *renderer, Uint32 color)
{
    SDL_SetRenderDrawColor(renderer, R(color), G(color), B(color), A(color));
    SDL_RenderClear(renderer);
}

void renderTextButtons(SDL_Renderer *renderer, std::vector<TextButton> controls, const char *ttf, int selected, SDL_Color fg, Uint32 bg, Uint32 bgSelected, int fontsize, int style = TTF_STYLE_NORMAL)
{
    if (controls.size() > 0)
    {
        for (auto i = 0; i < controls.size(); i++)
        {
            auto text = createText(controls[i].Text, ttf, fontsize, fg, controls[i].W, style);

            int x = controls[i].X + (controls[i].W - text->w) / 2;
            int y = controls[i].Y + (controls[i].H - text->h) / 2;

            SDL_Rect rect;

            rect.w = controls[i].W;
            rect.h = controls[i].H;
            rect.x = controls[i].X;
            rect.y = controls[i].Y;

            if (i == selected)
            {
                SDL_SetRenderDrawColor(renderer, R(bgSelected), G(bgSelected), B(bgSelected), A(bgSelected));
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, R(bg), G(bg), B(bg), A(bg));
            }

            SDL_RenderFillRect(renderer, &rect);

            renderText(renderer, text, bg, x, y, 2 * fontsize, 0);

            SDL_FreeSurface(text);

            text = NULL;
        }
    }
}

void renderButtons(SDL_Renderer *renderer, std::vector<Button> controls, int current, int fg, int space, int pts)
{
    if (controls.size() > 0)
    {
        for (auto i = 0; i < controls.size(); i++)
        {
            SDL_Rect rect;

            for (auto size = pts; size >= 0; size--)
            {
                rect.w = controls[i].W + 2 * (space - size);
                rect.h = controls[i].H + 2 * (space - size);
                rect.x = controls[i].X - space + size;
                rect.y = controls[i].Y - space + size;

                if (i == current)
                {
                    SDL_SetRenderDrawColor(renderer, R(fg), G(fg), B(fg), A(fg));
                    SDL_RenderDrawRect(renderer, &rect);
                }
            }

            renderImage(renderer, controls[i].Surface, controls[i].X, controls[i].Y);
        }
    }
}

std::vector<TextButton> createHTextButtons(const char **choices, int num, int text_buttonh, int text_x, int text_y)
{
    auto controls = std::vector<TextButton>();

    if (num > 0)
    {
        auto margin2 = (2.0 * Margin);
        auto marginleft = (1.0 - margin2);

        auto pixels = (int)(SCREEN_WIDTH * Margin);
        auto width = (int)(SCREEN_WIDTH * marginleft);

        auto text_spacew = width / num;
        auto text_buttonw = text_spacew - pixels;
        auto text_space = pixels / 2;

        for (auto i = 0; i < num; i++)
        {
            int left = i > 0 ? i - 1 : i;
            int right = i < num - 1 ? i + 1 : i;
            int up = i;
            int down = i;

            auto x = text_x + i * (text_buttonw + text_space * 2) + text_space;

            auto button = TextButton(i, choices[i], left, right, up, down, x, text_y, text_buttonw, text_buttonh);

            controls.push_back(button);
        }
    }

    return controls;
}

SDL_Surface *createHeaderButton(SDL_Window *window, const char *text, SDL_Color color, Uint32 bg, int w, int h, int x)
{
    auto button = SDL_CreateRGBSurface(0, w, h, arrow_size, 0, 0, 0, 0);
    auto text_surface = createText(text, FONT_FILE, 18, color, w, TTF_STYLE_NORMAL);

    if (button && text_surface)
    {
        SDL_Rect src;

        src.w = text_surface->w;
        src.h = text_surface->h;
        src.x = 0;
        src.y = 0;

        SDL_Rect dst;

        dst.w = button->w;
        dst.h = button->h;
        dst.x = 0;
        dst.y = 0;

        SDL_FillRect(button, &dst, bg);

        dst.x = x < 0 ? (button->w - text_surface->w) / 2 : x;
        dst.y = (button->h - text_surface->h) / 2;

        SDL_BlitSurface(text_surface, &src, button, &dst);
    }

    if (text_surface)
    {
        SDL_FreeSurface(text_surface);

        text_surface = NULL;
    }

    return button;
}

std::vector<Button> createItemList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Item::Base> list, int start, int last, int limit, bool confirm_button)
{
    auto text_space = 8;

    auto textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space);

    auto controls = std::vector<Button>();

    if (list.size() > 0)
    {
        for (int i = 0; i < last - start; i++)
        {
            auto index = start + i;

            std::string item_string = list[index].Name;

            if (list[index].Charge >= 0)
            {
                item_string += " (";

                if (list[index].Charge > 0)
                {
                    item_string += std::to_string(list[index].Charge);
                }

                item_string += ")";
            }

            auto text = createText(item_string.c_str(), FONT_FILE, 20, clrBK, textwidth - 4 * text_space, TTF_STYLE_NORMAL);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : texty + 2 * text_space);

            controls.push_back(Button(i, text, i, i, (i > 0 ? i - 1 : i), (i < (last - start) ? i + 1 : i), textx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = textwidth - 4 * text_space;

            controls[i].H = text->h;
        }
    }

    auto idx = controls.size();

    if (list.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + border_space, Control::Type::SCROLL_UP));

            idx++;
        }

        if (list.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + text_bounds - arrow_size - border_space, Control::Type::SCROLL_DOWN));

            idx++;
        }
    }

    if (confirm_button)
    {
        idx = controls.size();

        controls.push_back(Button(idx, "icons/yes.png", idx - 1, idx + 1, idx - 1, idx, startx, buttony, Control::Type::CONFIRM));
    }

    idx = controls.size();

    controls.push_back(Button(idx, "icons/back-button.png", idx - 1, idx, list.size() > 0 ? (last - start) : idx, idx, (1.0 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

    return controls;
}

bool characterScreen(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, Story::Base *story)
{
    std::string title = "Heart of Ice: Adventure Sheet";

    auto done = false;

    // Render the image
    if (window && renderer)
    {
        auto selected = false;
        auto current = -1;

        const int back_buttonh = 48;
        const int profilew = SCREEN_WIDTH * (1.0 - 2.0 * Margin);
        const int profileh = 0.12 * SCREEN_HEIGHT;

        auto controls = std::vector<Button>();

        auto marginw = Margin * SCREEN_WIDTH;
        auto marginh = Margin * SCREEN_HEIGHT / 2;

        auto headerw = 0.8 * splashw;
        auto headerh = 0.07 * SCREEN_HEIGHT;
        auto space = 8;
        auto font_size = 18;

        auto boxh = (profileh) / 2;

        auto skills = createHeaderButton(window, "Skills", clrWH, intDB, headerw, headerh, space);
        auto possessions = createHeaderButton(window, "Possessions", clrWH, intDB, headerw, headerh, space);

        controls.push_back(Button(0, skills, 0, 1, 0, 1, startx, starty + profileh + headerh + marginh, Control::Type::GLOSSARY));
        controls.push_back(Button(1, possessions, 0, 2, 0, 2, startx, starty + profileh + 3 * headerh + 3 * marginh + 2 * boxh, Control::Type::ACTION));
        controls.push_back(Button(2, "icons/back-button.png", 1, 2, 1, 2, (1 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

        std::string codewords;

        for (auto i = 0; i < player.Codewords.size(); i++)
        {
            if (i > 0)
            {
                codewords += ", ";
            }

            codewords += Codeword::Descriptions.at(player.Codewords[i]);
        }

        TTF_Init();

        auto font = TTF_OpenFont(FONT_FILE, font_size);

        if (font)
        {
            while (!done)
            {
                SDL_SetWindowTitle(window, title.c_str());

                // Fill the surface with background color
                fillWindow(renderer, intWH);

                renderAdventurer(window, renderer, font, player);

                if (player.Codewords.size() > 0)
                {
                    putText(renderer, "Codewords", font, space, clrWH, intDB, TTF_STYLE_NORMAL, headerw, headerh, startx, starty + 2 * profileh + 4 * headerh + 4 * marginh + 2 * boxh);
                    putText(renderer, codewords.c_str(), font, space, clrBK, intBE, TTF_STYLE_ITALIC, profilew - buttonw - 2 * space, profileh, startx, starty + 2 * profileh + 5 * headerh + 4 * marginh + 2 * boxh);
                }

                renderButtons(renderer, controls, current, intGR, space, space / 2);

                bool scrollUp = false;
                bool scrollDown = false;
                bool hold = false;

                done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

                if (selected && current >= 0 && current < controls.size())
                {
                    if (controls[current].Type == Control::Type::BACK)
                    {
                        break;
                    }
                    else if (controls[current].Type == Control::Type::GLOSSARY)
                    {
                        glossaryScreen(window, renderer, player.Skills);

                        current = -1;

                        selected = false;
                    }
                    else if (controls[current].Type == Control::Type::ACTION)
                    {
                        inventoryScreen(window, renderer, player, story, player.Items, Control::Type::USE, 0);

                        current = -1;

                        selected = false;
                    }
                }
            }
        }

        if (skills)
        {
            SDL_FreeSurface(skills);

            skills = NULL;
        }

        if (possessions)
        {
            SDL_FreeSurface(possessions);

            possessions = NULL;
        }

        if (font)
        {
            TTF_CloseFont(font);

            font = NULL;
        }

        TTF_Quit();
    }

    return false;
}

bool glossaryScreen(SDL_Window *window, SDL_Renderer *renderer, std::vector<Skill::Base> Skills)
{
    std::string title = "Heart of Ice: Skills Glossary";

    if (window && renderer)
    {
        auto space = 8;
        auto font_size = 20;

        const int glossary_width = SCREEN_WIDTH * (1.0 - 2.0 * Margin) - arrow_size - 2 * space;

        std::string text;

        for (auto i = 0; i < Skills.size(); i++)
        {
            if (i > 0)
            {
                text += "\n" + std::string(glossary_width / 10, '-') + "\n";
            }

            text += std::string(Skills[i].Name) + "\n\n";
            text += std::string(Skills[i].Description) + "\n";
        }

        auto glossary = createText(text.c_str(), FONT_FILE, font_size, clrBK, glossary_width - 2 * space, TTF_STYLE_NORMAL);

        auto quit = false;

        auto controls = std::vector<Button>();

        controls.push_back(Button(0, "icons/up-arrow.png", 0, 1, 0, 1, (1 - Margin) * SCREEN_WIDTH - arrow_size, texty + border_space, Control::Type::SCROLL_UP));
        controls.push_back(Button(1, "icons/down-arrow.png", 0, 2, 0, 2, (1 - Margin) * SCREEN_WIDTH - arrow_size, texty + text_bounds - arrow_size - border_space, Control::Type::SCROLL_DOWN));
        controls.push_back(Button(2, "icons/back-button.png", 1, 2, 1, 2, (1 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

        auto scrollSpeed = 20;
        auto hold = false;

        auto selected = false;
        auto current = -1;
        auto offset = 0;

        while (!quit)
        {
            SDL_SetWindowTitle(window, title.c_str());

            // Fill the surface with background color
            fillWindow(renderer, intWH);

            fillRect(renderer, glossary_width, text_bounds, startx, starty, intBE);
            renderText(renderer, glossary, intBE, startx + space, starty + space, text_bounds - 2 * space, offset);

            renderButtons(renderer, controls, current, intGR, border_space, border_pts);

            bool scrollUp = false;
            bool scrollDown = false;

            quit = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if ((selected && current >= 0 && current < controls.size()) || scrollUp || scrollDown || hold)
            {
                if (controls[current].Type == Control::Type::SCROLL_UP || (controls[current].Type == Control::Type::SCROLL_UP && hold) || scrollUp)
                {
                    if (offset > 0)
                    {
                        offset -= scrollSpeed;
                    }

                    if (offset < 0)
                    {
                        offset = 0;
                    }
                }
                else if (controls[current].Type == Control::Type::SCROLL_DOWN || ((controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown))
                {
                    if (glossary->h >= text_bounds - 2 * space)
                    {
                        if (offset < glossary->h - text_bounds + 2 * space)
                        {
                            offset += scrollSpeed;
                        }

                        if (offset > glossary->h - text_bounds + 2 * space)
                        {
                            offset = glossary->h - text_bounds + 2 * space;
                        }
                    }
                }
                else if (controls[current].Type == Control::Type::BACK && !hold)
                {
                    quit = true;

                    break;
                }
            }
        }

        if (glossary)
        {
            SDL_FreeSurface(glossary);

            glossary = NULL;
        }
    }

    return false;
}

bool inventoryScreen(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, Story::Base *story, std::vector<Item::Base> &Items, Control::Type mode, int limit)
{
    if (Items.size() > 0)
    {
        auto font_size = 20;
        auto text_space = 8;
        auto scrollSpeed = 1;
        auto display_limit = (text_bounds) / (font_size + 7 * text_space / 2);
        auto offset = 0;
        auto last = offset + display_limit;

        if (last > Items.size())
        {
            last = Items.size();
        }

        const char *message = NULL;

        std::string temp_message = "";

        auto flash_message = false;
        auto flash_color = intRD;

        Uint32 start_ticks = 0;
        Uint32 duration = 3000;

        auto done = false;

        auto textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space);

        auto controls = createItemList(window, renderer, Items, offset, last, display_limit, false);

        TTF_Init();

        auto font = TTF_OpenFont(FONT_FILE, font_size);

        auto selected = false;
        auto current = -1;
        auto quit = false;
        auto scrollUp = false;
        auto scrollDown = false;
        auto hold = false;

        auto infoh = 0.06 * SCREEN_HEIGHT;
        auto boxh = 0.150 * SCREEN_HEIGHT;
        auto box_space = 10;
        auto messageh = 0.25 * SCREEN_HEIGHT;

        while (!done)
        {
            last = offset + display_limit;

            if (last > Items.size())
            {
                last = Items.size();
            }

            SDL_SetWindowTitle(window, "Heart of Ice: Possessions");

            fillWindow(renderer, intWH);

            if (flash_message)
            {
                if ((SDL_GetTicks() - start_ticks) < duration)
                {
                    putText(renderer, message, font, text_space, clrWH, flash_color, TTF_STYLE_NORMAL, splashw, boxh * 2, startx, starty);
                }
                else
                {
                    flash_message = false;
                }
            }

            if (!flash_message)
            {
                if (mode == Control::Type::DROP)
                {
                    putText(renderer, "You are carrying too many items. Select item(s) to DROP.", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, boxh, startx, starty);
                }
                else if (mode == Control::Type::USE)
                {
                    putText(renderer, "Select an item to USE", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, boxh, startx, starty);
                }
                else if (mode == Control::Type::LOSE)
                {
                    std::string stolen_message = "DROP item(s) until only " + std::to_string(limit) + " item" + std::string(limit > 1 ? "s" : "") + " remains.";

                    putText(renderer, stolen_message.c_str(), font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, boxh, startx, starty);
                }
                else
                {
                    putText(renderer, "You are carrying these items", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, boxh, startx, starty);
                }
            }

            putText(renderer, "Life", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (boxh + infoh));
            putText(renderer, (std::to_string(player.Life)).c_str(), font, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - boxh);

            putText(renderer, "Money", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * (boxh + infoh) + box_space));
            putText(renderer, (std::to_string(player.Money) + std::string(" scads")).c_str(), font, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - (2 * boxh + infoh + box_space));

            fillRect(renderer, textwidth, text_bounds, textx, texty, intBE);

            for (auto idx = offset; idx < last; idx++)
            {
                auto i = idx - offset;

                drawRect(renderer, controls[i].W + 2 * text_space, controls[i].H + 2 * text_space, controls[i].X - text_space, controls[i].Y - text_space, intDB);
            }

            renderButtons(renderer, controls, current, intGR, text_space, text_space / 2);

            done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if ((selected && current >= 0 && current < controls.size()) || scrollUp || scrollDown || hold)
            {
                if (controls[current].Type == Control::Type::SCROLL_UP || (controls[current].Type == Control::Type::SCROLL_UP && hold) || scrollUp)
                {
                    if (offset > 0)
                    {
                        offset -= scrollSpeed;

                        if (offset < 0)
                        {
                            offset = 0;
                        }

                        last = offset + display_limit;

                        if (last > Items.size())
                        {
                            last = Items.size();
                        }

                        controls.clear();

                        controls = createItemList(window, renderer, Items, offset, last, display_limit, false);

                        SDL_Delay(50);
                    }

                    if (offset <= 0)
                    {
                        current = -1;

                        selected = false;
                    }
                }
                else if (controls[current].Type == Control::Type::SCROLL_DOWN || ((controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown))
                {
                    if (Items.size() - last > 0)
                    {
                        if (offset < Items.size() - display_limit)
                        {
                            offset += scrollSpeed;
                        }

                        if (offset > Items.size() - display_limit)
                        {
                            offset = Items.size() - display_limit;
                        }

                        last = offset + display_limit;

                        if (last > Items.size())
                        {
                            last = Items.size();
                        }

                        controls.clear();

                        controls = createItemList(window, renderer, Items, offset, last, display_limit, false);

                        SDL_Delay(50);

                        if (offset > 0)
                        {
                            if (controls[current].Type != Control::Type::SCROLL_DOWN)
                            {
                                current++;
                            }
                        }
                    }

                    if (Items.size() - last <= 0)
                    {
                        selected = false;

                        current = -1;
                    }
                }
                else if (controls[current].Type == Control::Type::ACTION && !hold)
                {
                    if ((current + offset >= 0) && (current + offset) < Items.size())
                    {
                        auto item = Items[current + offset];

                        if (mode == Control::Type::DROP)
                        {
                            Items.erase(Items.begin() + (current + offset));

                            if (offset > 0)
                            {
                                offset--;
                            }

                            last = offset + display_limit;

                            if (last > Items.size())
                            {
                                last = Items.size();
                            }

                            controls.clear();

                            controls = createItemList(window, renderer, Items, offset, last, display_limit, false);

                            std::string description = item.Name;

                            if (item.Charge >= 0)
                            {
                                description += " (";

                                if (item.Charge > 0)
                                {
                                    description += std::to_string(item.Charge) + " charges";
                                }
                                else
                                {
                                    description += "empty";
                                }

                                description += ")";
                            }

                            temp_message = description + " DROPPED!";

                            message = temp_message.c_str();

                            flash_color = intRD;

                            start_ticks = SDL_GetTicks();

                            flash_message = true;
                        }
                        else if (mode == Control::Type::LOSE)
                        {
                            if (Items.size() > limit)
                            {
                                Items.erase(Items.begin() + (current + offset));

                                Character::LOSE_ITEMS(player, {item.Type});

                                if (offset > 0)
                                {
                                    offset--;
                                }

                                last = offset + display_limit;

                                if (last > Items.size())
                                {
                                    last = Items.size();
                                }

                                controls.clear();

                                controls = createItemList(window, renderer, Items, offset, last, display_limit, false);

                                std::string description = item.Name;

                                if (item.Charge >= 0)
                                {
                                    description += " (";

                                    if (item.Charge > 0)
                                    {
                                        description += std::to_string(item.Charge) + " charges";
                                    }
                                    else
                                    {
                                        description += "empty";
                                    }

                                    description += ")";
                                }

                                temp_message = description + " DROPPED!";

                                message = temp_message.c_str();

                                flash_color = intRD;

                                start_ticks = SDL_GetTicks();

                                flash_message = true;
                            }
                        }
                        else if (mode == Control::Type::USE)
                        {
                        }
                    }

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::BACK && !hold)
                {
                    done = true;

                    break;
                }
            }
        }

        if (font)
        {
            TTF_CloseFont(font);

            font = NULL;
        }

        TTF_Quit();
    }

    return false;
}

bool takeScreen(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, std::vector<Item::Base> items, int TakeLimit)
{
    auto done = false;

    if (TakeLimit > 0)
    {
        auto text_space = 8;
        auto font_size = 20;
        auto scrollSpeed = 1;
        auto limit = (text_bounds) / (font_size + 7 * text_space / 2);
        auto offset = 0;
        auto last = offset + limit;

        if (last > items.size())
        {
            last = items.size();
        }

        const char *message = NULL;

        auto error = false;

        Uint32 start_ticks = 0;
        Uint32 duration = 3000;

        auto textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space);

        auto controls = createItemList(window, renderer, items, offset, last, limit, true);

        TTF_Init();

        auto font = TTF_OpenFont(FONT_FILE, font_size);

        auto selected = false;
        auto current = -1;
        auto quit = false;
        auto scrollUp = false;
        auto scrollDown = false;
        auto hold = false;

        auto infoh = 0.06 * SCREEN_HEIGHT;
        auto boxh = 0.125 * SCREEN_HEIGHT;

        auto selection = std::vector<int>();

        while (!done)
        {
            last = offset + limit;

            if (last > items.size())
            {
                last = items.size();
            }

            SDL_SetWindowTitle(window, "Heart of Ice");

            fillWindow(renderer, intWH);

            if (error)
            {
                if ((SDL_GetTicks() - start_ticks) < duration)
                {
                    putText(renderer, message, font, text_space, clrWH, intRD, TTF_STYLE_NORMAL, splashw, boxh * 2, startx, starty);
                }
                else
                {
                    error = false;
                }
            }

            if (!error)
            {
                std::string take_message = "";

                if (items.size() > 1)
                {
                    if (TakeLimit > 1)
                    {
                        if (TakeLimit == items.size())
                        {
                            take_message = "You can TAKE any number of items.";
                        }
                        else
                        {
                            take_message = "You can TAKE up to " + std::to_string(TakeLimit) + " items.";
                        }
                    }
                    else
                    {
                        take_message = "Choose an item to KEEP.";
                    }
                }
                else
                {
                    take_message = "KEEP this item?";
                }

                putText(renderer, take_message.c_str(), font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, boxh, startx, starty);
            }

            std::string take = "";

            if (selection.size() > 0)
            {
                for (auto i = 0; i < selection.size(); i++)
                {
                    if (i > 0)
                    {
                        take += ", ";
                    }

                    std::string description = items[selection[i]].Name;

                    if (items[selection[i]].Charge >= 0)
                    {
                        description += " (";

                        if (items[selection[i]].Charge > 0)
                        {
                            description += std::to_string(items[selection[i]].Charge) + " charges";
                        }
                        else
                        {
                            description += "empty";
                        }

                        description += ")";
                    }

                    take += description;
                }
            }

            putText(renderer, "SELECTED", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (3 * boxh + infoh));
            putText(renderer, selection.size() > 0 ? take.c_str() : "(None)", font, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 3 * boxh, startx, starty + text_bounds - 3 * boxh);

            fillRect(renderer, textwidth, text_bounds, textx, texty, intBE);

            renderButtons(renderer, controls, current, intGR, text_space, text_space / 2);

            if (last - offset > 0)
            {
                for (auto i = 0; i < last - offset; i++)
                {
                    if (Item::FIND(selection, offset + i) >= 0)
                    {
                        drawRect(renderer, controls[i].W + 2 * text_space, controls[i].H + 2 * text_space, controls[i].X - text_space, controls[i].Y - text_space, intDB);
                    }
                }
            }

            done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if ((selected && current >= 0 && current < controls.size()) || scrollUp || scrollDown || hold)
            {
                if (controls[current].Type == Control::Type::SCROLL_UP || (controls[current].Type == Control::Type::SCROLL_UP && hold) || scrollUp)
                {
                    if (offset > 0)
                    {
                        offset -= scrollSpeed;

                        if (offset < 0)
                        {
                            offset = 0;
                        }

                        last = offset + limit;

                        if (last > items.size())
                        {
                            last = items.size();
                        }

                        controls.clear();

                        controls = createItemList(window, renderer, items, offset, last, limit, true);

                        SDL_Delay(50);
                    }

                    if (offset <= 0)
                    {
                        current = -1;

                        selected = false;
                    }
                }
                else if (controls[current].Type == Control::Type::SCROLL_DOWN || ((controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown))
                {
                    if (items.size() - last > 0)
                    {
                        if (offset < items.size() - limit)
                        {
                            offset += scrollSpeed;
                        }

                        if (offset > items.size() - limit)
                        {
                            offset = items.size() - limit;
                        }

                        last = offset + limit;

                        if (last > items.size())
                        {
                            last = items.size();
                        }

                        controls.clear();
                        controls = createItemList(window, renderer, items, offset, last, limit, true);

                        SDL_Delay(50);

                        if (offset > 0)
                        {
                            if (controls[current].Type != Control::Type::SCROLL_DOWN)
                            {
                                current++;
                            }
                        }
                    }

                    if (items.size() - last <= 0)
                    {
                        selected = false;

                        current = -1;
                    }
                }
                else if (controls[current].Type == Control::Type::ACTION && !hold)
                {
                    if (current >= 0 && current < controls.size())
                    {
                        auto result = Item::FIND(selection, offset + current);

                        if (result >= 0)
                        {
                            selection.erase(selection.begin() + result);
                        }
                        else
                        {
                            if (selection.size() < TakeLimit)
                            {
                                selection.push_back(offset + current);
                            }
                        }
                    }

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::CONFIRM && !hold)
                {
                    auto take = std::vector<Item::Base>();

                    for (auto i = 0; i < selection.size(); i++)
                    {
                        take.push_back(items[selection[i]]);
                    }

                    Character::GET_ITEMS(player, take);

                    current = -1;

                    selected = false;

                    done = true;

                    break;
                }
                else if (controls[current].Type == Control::Type::BACK && !hold)
                {
                    done = false;

                    break;
                }
            }
        }

        if (font)
        {
            TTF_CloseFont(font);

            font = NULL;
        }

        TTF_Quit();
    }

    return done;
}

void renderAdventurer(SDL_Window *window, SDL_Renderer *renderer, TTF_Font *font, Character::Base &player)
{
    const int profilew = SCREEN_WIDTH * (1.0 - 2.0 * Margin);
    const int profileh = 0.12 * SCREEN_HEIGHT;

    auto headerw = 0.8 * splashw;
    auto headerh = 0.07 * SCREEN_HEIGHT;
    auto space = 8;

    auto marginw = Margin * SCREEN_WIDTH;
    auto marginh = Margin * SCREEN_HEIGHT / 2;

    std::string skills;

    for (auto i = 0; i < player.Skills.size(); i++)
    {
        if (i > 0)
        {
            skills += ", ";
        }

        skills += player.Skills[i].Name;
    }

    auto boxw = (profilew - marginw) / 2;
    auto boxh = (profileh) / 2;

    std::string possessions;

    for (auto i = 0; i < player.Items.size(); i++)
    {
        if (i > 0)
        {
            possessions += ", ";
        }

        possessions += player.Items[i].Name;

        if (player.Items[i].Charge >= 0)
        {
            possessions += " (";

            if (player.Items[i].Charge > 0)
            {
                possessions += std::to_string(player.Items[i].Charge) + " charges";
            }
            else
            {
                possessions += "empty";
            }

            possessions += ")";
        }
    }

    auto name_string = player.Name;

    if (player.Vehicle != Vehicle::Type::NONE)
    {
        name_string += " (Vehicle: " + std::string(Vehicle::Descriptions[player.Vehicle]) + ")";
    }

    // Fill the surface with background color
    fillWindow(renderer, intWH);

    putText(renderer, name_string.c_str(), font, space, clrWH, intDB, TTF_STYLE_NORMAL, player.Vehicle != Vehicle::Type::NONE ? headerw * 3 : headerw, headerh, startx, starty);
    putText(renderer, player.Description.c_str(), font, space, clrBK, intBE, TTF_STYLE_NORMAL, profilew, profileh, startx, starty + headerh);

    putText(renderer, "Skills", font, space, clrWH, intDB, TTF_STYLE_NORMAL, headerw, headerh, startx, starty + profileh + headerh + marginh);
    putText(renderer, skills.c_str(), font, space, clrBK, intBE, TTF_STYLE_NORMAL, profilew, boxh, startx, starty + profileh + 2 * headerh + marginh);

    putText(renderer, "Life", font, space, clrWH, intDB, TTF_STYLE_NORMAL, headerw, headerh, startx, starty + profileh + 2 * headerh + 2 * marginh + boxh);
    putText(renderer, std::to_string(player.Life).c_str(), font, space, clrBK, intBE, TTF_STYLE_NORMAL, boxw, boxh, startx, starty + profileh + 3 * headerh + 2 * marginh + boxh);

    putText(renderer, "Money", font, space, clrWH, intDB, TTF_STYLE_NORMAL, headerw, headerh, startx + boxw + marginw, starty + profileh + 2 * headerh + 2 * marginh + boxh);
    putText(renderer, (std::to_string(player.Money) + " scads").c_str(), font, space, clrBK, intBE, TTF_STYLE_NORMAL, boxw, boxh, startx + boxw + marginw, starty + profileh + 3 * headerh + 2 * marginh + boxh);

    putText(renderer, "Possessions", font, space, clrWH, intDB, TTF_STYLE_NORMAL, headerw, headerh, startx, starty + profileh + 3 * headerh + 3 * marginh + 2 * boxh);
    putText(renderer, player.Items.size() > 0 ? possessions.c_str() : "(None)", font, space, clrBK, intBE, TTF_STYLE_NORMAL, profilew, profileh, startx, starty + profileh + 4 * headerh + 3 * marginh + 2 * boxh);
}

std::vector<Button> skillsList(SDL_Window *window, SDL_Renderer *renderer, int start, int last, int limit)
{
    auto text_space = 8;

    auto textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space);

    auto controls = std::vector<Button>();

    for (int i = 0; i < last - start; i++)
    {
        auto index = start + i;

        std::string item_string = Skill::ALL[index].Name;

        auto text = createText(item_string.c_str(), FONT_FILE, 20, clrBK, textwidth - 4 * text_space, TTF_STYLE_NORMAL);

        auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : texty + 2 * text_space);

        controls.push_back(Button(i, text, i, i, (i > 0 ? i - 1 : i), (i < (last - start) ? i + 1 : i), textx + 2 * text_space, y, Control::Type::ACTION));

        controls[i].W = textwidth - 4 * text_space;

        controls[i].H = text->h;
    }

    auto idx = controls.size();

    if (Skill::ALL.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", 0, idx, idx, idx + 1, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + border_space, Control::Type::SCROLL_UP));

            idx++;
        }

        if (Skill::ALL.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", 0, idx, start > 0 ? idx - 1 : idx, idx + 1, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, (texty + 2 * text_bounds / 3 - arrow_size - border_space), Control::Type::SCROLL_DOWN));

            idx++;
        }
    }

    idx = controls.size();

    auto button_width = 100;
    auto button_space = 25;
    auto button_height = 48;

    controls.push_back(Button(idx, createHeaderButton(window, "Glossary", clrWH, intBK, button_width, button_height, -1), idx - 1, idx + 1, idx - 1, idx, startx, buttony, Control::Type::GLOSSARY));
    controls.push_back(Button(idx + 1, createHeaderButton(window, "Start", clrWH, intBK, button_width, button_height, -1), idx, idx + 2, idx - 1, idx + 1, startx + button_width + button_space, buttony, Control::Type::NEW));
    controls.push_back(Button(idx + 2, createHeaderButton(window, "Back", clrWH, intBK, button_width, button_height, -1), idx + 1, idx + 2, idx - 1, idx + 2, startx + 2 * (button_width + button_space), buttony, Control::Type::BACK));

    return controls;
}

Character::Base customCharacter(SDL_Window *window, SDL_Renderer *renderer)
{
    std::string title = "Down Among the Dead Men: Create Character";

    auto done = false;

    Character::Base player = Character::Base();

    // Render the image
    if (window && renderer)
    {
        auto flash_message = false;
        auto flash_color = intRD;

        std::string message = "";

        Uint32 start_ticks = 0;
        Uint32 duration = 5000;

        auto selected = false;
        auto current = -1;
        auto character = 0;

        auto font_size = 20;

        auto text_space = 8;

        auto textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space);

        auto Limit = (2 * text_bounds / 3) / (font_size + 7 * text_space / 2);
        auto offset = 0;
        auto last = offset + Limit;

        if (last > Skill::ALL.size())
        {
            last = Skill::ALL.size();
        }

        auto controls = skillsList(window, renderer, offset, last, Limit);

        TTF_Init();

        auto font = TTF_OpenFont(FONT_FILE, font_size);

        bool scrollUp = false;
        bool scrollDown = false;
        bool hold = false;
        auto scrollSpeed = 1;

        auto selection = std::vector<int>();

        auto infoh = 0.07 * SCREEN_HEIGHT;
        auto boxh = 0.150 * SCREEN_HEIGHT;
        auto box_space = 10;
        auto messageh = 0.25 * SCREEN_HEIGHT;

        while (!done)
        {
            last = offset + Limit;

            if (last > Skill::ALL.size())
            {
                last = Skill::ALL.size();
            }

            SDL_SetWindowTitle(window, title.c_str());

            fillWindow(renderer, intWH);

            std::string selection_string = "";

            if (selection.size() > 0)
            {
                for (auto i = 0; i < selection.size(); i++)
                {
                    if (i > 0)
                    {
                        selection_string += ", ";
                    }

                    std::string description = Skill::ALL[selection[i]].Name;

                    selection_string += description;
                }
            }

            putText(renderer, "SELECTED", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh));

            putText(renderer, selection.size() > 0 ? selection_string.c_str() : "(None)", font, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);

            fillRect(renderer, textwidth, 2 * text_bounds / 3, textx, texty, intBE);

            fillRect(renderer, textwidth, text_bounds / 3 - texty, textx, 2 * (texty + text_bounds / 3), intLB);

            if (current >= 0 && current < Limit)
            {
                auto text = createText(Skill::ALL[current + offset].Description, FONT_FILE, font_size, clrWH, textwidth - 2 * text_space, TTF_STYLE_NORMAL);

                renderText(renderer, text, intLB, textx + text_space, 2 * (texty + text_bounds / 3) + text_space, text_bounds / 3 - texty, 0);

                SDL_FreeSurface(text);

                text = NULL;
            }

            renderButtons(renderer, controls, current, intGR, 8, 4);

            if (last - offset > 0)
            {
                for (auto i = 0; i < last - offset; i++)
                {
                    if (Skill::FIND_LIST(selection, offset + i) >= 0)
                    {
                        drawRect(renderer, controls[i].W + 2 * text_space, controls[i].H + 2 * text_space, controls[i].X - text_space, controls[i].Y - text_space, intDB);
                    }
                }
            }

            if (flash_message)
            {
                if ((SDL_GetTicks() - start_ticks) < duration)
                {
                    putText(renderer, message.c_str(), font, text_space, clrWH, flash_color, TTF_STYLE_NORMAL, splashw, boxh * 2, startx, starty);
                }
                else
                {
                    flash_message = false;
                }
            }

            if (!flash_message)
            {
                putText(renderer, "SELECT 4 Skills for your character.", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, boxh, startx, starty);
            }

            Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if (selected && current >= 0 && current < controls.size() || scrollUp || scrollDown || hold)
            {
                if (controls[current].Type == Control::Type::SCROLL_UP || (controls[current].Type == Control::Type::SCROLL_UP && hold) || scrollUp)
                {
                    if (offset > 0)
                    {
                        offset -= scrollSpeed;

                        if (offset < 0)
                        {
                            offset = 0;
                        }

                        last = offset + Limit;

                        if (last > Skill::ALL.size())
                        {
                            last = Skill::ALL.size();
                        }

                        controls.clear();

                        controls = skillsList(window, renderer, offset, last, Limit);

                        SDL_Delay(50);
                    }

                    if (offset <= 0)
                    {
                        current = -1;

                        selected = false;
                    }
                }
                else if (controls[current].Type == Control::Type::SCROLL_DOWN || ((controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown))
                {
                    if (Skill::ALL.size() - last > 0)
                    {
                        if (offset < Skill::ALL.size() - Limit)
                        {
                            offset += scrollSpeed;
                        }

                        if (offset > Skill::ALL.size() - Limit)
                        {
                            offset = Skill::ALL.size() - Limit;
                        }

                        last = offset + Limit;

                        if (last > Skill::ALL.size())
                        {
                            last = Skill::ALL.size();
                        }

                        controls.clear();

                        controls = skillsList(window, renderer, offset, last, Limit);

                        SDL_Delay(50);

                        if (offset > 0)
                        {
                            if (controls[current].Type != Control::Type::SCROLL_DOWN)
                            {
                                current++;
                            }
                        }
                    }

                    if (Skill::ALL.size() - last <= 0)
                    {
                        selected = false;

                        current = -1;
                    }
                }
                else if (controls[current].Type == Control::Type::ACTION && !hold)
                {
                    if ((current + offset >= 0) && (current + offset) < Skill::ALL.size())
                    {
                        auto result = Skill::FIND_LIST(selection, current + offset);

                        if (result >= 0)
                        {
                            selection.erase(selection.begin() + result);
                        }
                        else
                        {
                            if (selection.size() < player.SKILLS_LIMIT)
                            {
                                selection.push_back(offset + current);
                            }
                        }
                    }

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::NEW)
                {
                    if (selection.size() == player.SKILLS_LIMIT)
                    {
                        Character::CUSTOM.Skills.clear();
                        Character::CUSTOM.Items.clear();

                        for (auto i = 0; i < selection.size(); i++)
                        {
                            Character::CUSTOM.Skills.push_back(Skill::ALL[selection[i]]);

                            if (Skill::ALL[selection[i]].Type == Skill::Type::SHOOTING)
                            {
                                Character::CUSTOM.Items.push_back(Item::BARYSAL_GUN);
                            }
                            else if (Skill::ALL[selection[i]].Type == Skill::Type::ESP || Skill::ALL[selection[i]].Type == Skill::Type::PARADOXING)
                            {
                                if (!Character::VERIFY_ITEMS(Character::CUSTOM, {Item::Type::PSIONIC_FOCUS}))
                                {
                                    Character::CUSTOM.Items.push_back(Item::PSIONIC_FOCUS);
                                }
                            }
                        }

                        Character::CUSTOM.Money = 10;
                        Character::CUSTOM.Life = 10;

                        player = Character::CUSTOM;

                        current = -1;

                        selected = false;

                        done = true;

                        break;
                    }
                    else
                    {
                        flash_message = true;

                        flash_color = intRD;

                        start_ticks = SDL_GetTicks();

                        message = "Please select " + std::to_string(player.SKILLS_LIMIT) + " skills.";

                        selected = false;

                        current = -1;
                    }
                }
                else if (controls[current].Type == Control::Type::GLOSSARY)
                {
                    glossaryScreen(window, renderer, Skill::ALL);

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::BACK)
                {
                    player = Character::Base();

                    current = -1;

                    selected = false;

                    done = true;

                    break;
                }
            }
        }

        if (font)
        {
            TTF_CloseFont(font);

            font = NULL;
        }

        TTF_Quit();
    }

    return player;
}

Character::Base selectCharacter(SDL_Window *window, SDL_Renderer *renderer)
{
    std::string title = "Heart of Ice: Select Character";

    auto done = false;

    Character::Base player = Character::Classes[0];

    // Render the image
    if (window && renderer)
    {
        auto selected = false;
        auto current = -1;
        auto character = 0;

        auto main_buttonh = 48;

        auto font_size = 18;

        const char *choices[6] = {"Previous", "Next", "Glossary", "Custom", "Start", "Back"};

        auto controls = createHTextButtons(choices, 6, main_buttonh, startx, SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh);

        controls[0].Type = Control::Type::BACK;
        controls[1].Type = Control::Type::NEXT;
        controls[2].Type = Control::Type::GLOSSARY;
        controls[3].Type = Control::Type::CUSTOM;
        controls[4].Type = Control::Type::NEW;
        controls[5].Type = Control::Type::QUIT;

        TTF_Init();

        auto font = TTF_OpenFont(FONT_FILE, font_size);

        if (font)
        {
            while (!done)
            {
                SDL_SetWindowTitle(window, title.c_str());

                renderAdventurer(window, renderer, font, Character::Classes[character]);

                renderTextButtons(renderer, controls, FONT_FILE, current, clrWH, intBK, intRD, 20, TTF_STYLE_NORMAL);

                bool scrollUp = false;
                bool scrollDown = false;
                bool hold = false;

                Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

                if (selected && current >= 0 && current < controls.size())
                {
                    if (controls[current].Type == Control::Type::NEW)
                    {
                        player = Character::Classes[character];

                        current = -1;

                        selected = false;

                        done = true;

                        break;
                    }
                    else if (controls[current].Type == Control::Type::BACK)
                    {
                        if (character > 0)
                        {
                            character--;
                        }
                    }
                    else if (controls[current].Type == Control::Type::NEXT)
                    {
                        if (character < Character::Classes.size() - 1)
                        {
                            character++;
                        }
                    }
                    else if (controls[current].Type == Control::Type::GLOSSARY)
                    {
                        glossaryScreen(window, renderer, Skill::ALL);

                        current = -1;
                    }
                    else if (controls[current].Type == Control::Type::CUSTOM)
                    {
                        player = customCharacter(window, renderer);

                        if (player.Skills.size() == player.SKILLS_LIMIT)
                        {
                            current = -1;

                            selected = false;

                            done = true;

                            break;
                        }
                        else
                        {
                            current = -1;
                        }
                    }
                    else if (controls[current].Type == Control::Type::QUIT)
                    {
                        player = Character::Base();

                        player.StoryID = -1;

                        current = -1;

                        selected = false;

                        done = true;

                        break;
                    }

                    selected = false;
                }
            }
        }

        if (font)
        {
            TTF_CloseFont(font);

            font = NULL;
        }

        TTF_Quit();
    }

    return player;
}

bool aboutScreen(SDL_Window *window, SDL_Renderer *renderer)
{
    auto done = false;

    auto *about = "Critical IF are gamebooks with a difference. The outcomes are not random. Whether you live or die is a matter not of luck, but of judgement.\n\nTo start your adventure simply choose your character. Each character has a unique selection of four skills; these will decide which options are available to you. Also note your Life Points and your possessions.\n\nLife Points are lost each time you are wounded. If you are ever reduced to zero Life Points, you have been killed and the adventure ends. Sometimes you can recover Life Points during your adventure, but you can never have more Life Points than you started with.\n\nYou can carry up to eight possessions at a time. If you are at this limit and find something else you want, drop one of your other possessions to make room for the new item.\n\nConsider your selection of skills. They establish your special strengths, and will help you to role-play your choices during the adventrue. If you arrive at an entry which lists options for more than one of your skills, you can choose which skill to use in that situation.\n\nThat's all you need to know. Now choose your character.";

    auto splash = createImage("images/virtual-reality.png");

    auto text = createText(about, FONT_FILE, 18, clrWH, SCREEN_WIDTH * (1.0 - 3 * Margin) - splashw);

    // Render the image
    if (window && renderer && splash && text)
    {
        SDL_SetWindowTitle(window, "About the game");

        auto selected = false;
        auto current = -1;

        auto about_buttonw = 150;
        auto about_buttonh = 48;

        auto about_buttony = (int)(SCREEN_HEIGHT * (1 - Margin) - buttonh);

        std::vector<TextButton> controls = {TextButton(0, "Back", 0, 0, 0, 0, startx, about_buttony, about_buttonw, about_buttonh, Control::Type::BACK)};

        while (!done)
        {
            // Fill the surface with background color
            fillWindow(renderer, intLB);

            fitImage(renderer, splash, startx, starty, splashw, text_bounds);
            renderText(renderer, text, intLB, startx * 2 + splashw, starty, SCREEN_HEIGHT * (1.0 - 2 * Margin), 0);
            renderTextButtons(renderer, controls, FONT_FILE, current, clrWH, intBK, intRD, 20, TTF_STYLE_NORMAL);

            bool scrollUp = false;
            bool scrollDown = false;
            bool hold = false;

            done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if (selected && current >= 0 && current < controls.size() && controls[current].Type == Control::Type::BACK)
            {
                break;
            }
        }

        SDL_FreeSurface(splash);
        SDL_FreeSurface(text);

        text = NULL;
        splash = NULL;
    }

    return done;
}

#if defined(_WIN32) || defined(__arm__)
std::string time_string(long long deserialised)
{
    auto epoch = std::chrono::time_point<std::chrono::system_clock>();
    auto since_epoch = std::chrono::milliseconds(deserialised);
    std::chrono::system_clock::time_point timestamp = epoch + since_epoch;

    auto in_time_t = std::chrono::system_clock::to_time_t(timestamp);

    std::stringstream ss;

    if (in_time_t >= 0)
    {
        ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    }

    return ss.str();
}
#else
std::string time_string(long deserialised)
{
    auto epoch = std::chrono::time_point<std::chrono::high_resolution_clock>();
    auto since_epoch = std::chrono::milliseconds(deserialised);
    auto timestamp = epoch + since_epoch;

    auto in_time_t = std::chrono::system_clock::to_time_t(timestamp);

    std::stringstream ss;

    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");

    return ss.str();
}
#endif

bool saveGame(Character::Base &player, const char *overwrite)
{
    auto seed = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);

    std::ostringstream buffer;

#if defined(_WIN32)

    PWSTR path_str;

    SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_DEFAULT, NULL, &path_str);

    std::wstring wpath(path_str);

    CoTaskMemFree(path_str);

    std::string save(wpath.length(), ' ');

    std::copy(wpath.begin(), wpath.end(), save.begin());

    save += "/Saved Games/Heart of Ice";

    std::string path = save + "/";

#else

    const char *homedir;

    if ((homedir = getenv("HOME")) == NULL)
    {
        homedir = getpwuid(getuid())->pw_dir;
    }

    std::string save = std::string(homedir) + "/Documents/Saved Games/Heart Of Ice";
    std::string path = save + "/";

#endif

    fs::create_directories(save);

    if (overwrite != NULL)
    {
        buffer << std::string(overwrite);
    }
    else
    {
        buffer << path << std::to_string(seed) << ".save";
    }

    nlohmann::json data;

    player.Epoch = seed;

    data["name"] = player.Name;
    data["description"] = player.Description;
    data["type"] = player.Type;
    data["life"] = player.Life;
    data["money"] = player.Money;
    data["itemLimit"] = player.ITEM_LIMIT;
    data["lifeLimit"] = player.MAX_LIFE_LIMIT;
    data["skillsLimit"] = player.SKILLS_LIMIT;
    data["codewords"] = player.Codewords;
    data["vehicle"] = player.Vehicle;
    data["epoch"] = player.Epoch;

    auto skills = std::vector<Skill::Type>();
    auto lostSkills = std::vector<Skill::Type>();

    for (auto i = 0; i < player.Skills.size(); i++)
    {
        skills.push_back(player.Skills[i].Type);
    }

    for (auto i = 0; i < player.LostSkills.size(); i++)
    {
        lostSkills.push_back(player.LostSkills[i].Type);
    }

    data["skills"] = skills;
    data["lostSkills"] = lostSkills;
    data["lostMoney"] = player.LostMoney;
    data["storyID"] = player.StoryID;

    auto items = std::vector<nlohmann::json>();
    auto lostItems = std::vector<nlohmann::json>();

    for (auto i = 0; i < player.Items.size(); i++)
    {
        nlohmann::json item;

        item.emplace("name", player.Items[i].Name);
        item.emplace("description", player.Items[i].Description);
        item.emplace("type", player.Items[i].Type);
        item.emplace("charge", player.Items[i].Charge);

        items.push_back(item);
    }

    for (auto i = 0; i < player.LostItems.size(); i++)
    {
        nlohmann::json item;

        item.emplace("name", player.LostItems[i].Name);
        item.emplace("description", player.LostItems[i].Description);
        item.emplace("type", player.LostItems[i].Type);
        item.emplace("charge", player.LostItems[i].Charge);

        lostItems.push_back(item);
    }

    data["lostItems"] = lostItems;
    data["items"] = items;

    auto potionsApplied = std::vector<int>();

    for (auto i = 0; i < player.PotionsApplied.size(); i++)
    {
        potionsApplied.push_back(static_cast<int>(player.PotionsApplied[i]));
    }

    data["potionsApplied"] = potionsApplied;

    std::string filename = buffer.str();

    std::ofstream file(filename);

    file << data.dump();

    file.close();

    return true;
}

Character::Base loadGame(std::string file_name)
{
    std::ifstream ifs(file_name);

    auto character = Character::Base();

    if (ifs.good())
    {
        auto data = nlohmann::json::parse(ifs);

        ifs.close();

        std::string name = std::string(data["name"]);

        std::string description = data["description"];

        auto type = static_cast<Character::Type>((int)data["type"]);

        auto skills = std::vector<Skill::Base>();
        auto items = std::vector<Item::Base>();
        auto codewords = std::vector<Codeword::Type>();

        auto lostSkills = std::vector<Skill::Base>();
        auto lostItems = std::vector<Item::Base>();

        for (auto i = 0; i < (int)data["skills"].size(); i++)
        {
            auto skill = static_cast<Skill::Type>((int)data["skills"][i]);
            auto found = Skill::FIND(Skill::ALL, skill);

            if (found >= 0)
            {
                skills.push_back(Skill::ALL[found]);
            }
        }

        for (auto i = 0; i < (int)data["lostSkills"].size(); i++)
        {
            auto skill = static_cast<Skill::Type>((int)data["lostSkills"][i]);
            auto found = Skill::FIND(Skill::ALL, skill);

            if (found >= 0)
            {
                lostSkills.push_back(Skill::ALL[found]);
            }
        }

        for (auto i = 0; i < (int)data["items"].size(); i++)
        {
            auto item_name = std::string(data["items"][i]["name"]);
            auto item_description = std::string(data["items"][i]["description"]);
            auto item_type = static_cast<Item::Type>((int)data["items"][i]["type"]);
            auto item_charge = (int)data["items"][i]["charge"];

            items.push_back(Item::Base(item_name.c_str(), item_description.c_str(), item_type, item_charge));
        }

        for (auto i = 0; i < (int)data["lostItems"].size(); i++)
        {
            auto item_name = std::string(data["lostItems"][i]["name"]);
            auto item_description = std::string(data["lostItems"][i]["description"]);
            auto item_type = static_cast<Item::Type>((int)data["lostItems"][i]["type"]);
            auto item_charge = (int)data["lostItems"][i]["charge"];

            lostItems.push_back(Item::Base(item_name.c_str(), item_description.c_str(), item_type, item_charge));
        }

        for (auto i = 0; i < (int)data["codewords"].size(); i++)
        {
            auto codeword = static_cast<Codeword::Type>((int)data["codewords"][i]);

            codewords.push_back(codeword);
        }

        auto money = (int)data["money"];
        auto life = (int)data["life"];

        character = Character::Base(name.c_str(), type, description.c_str(), skills, items, codewords, life, money);

        character.LostSkills = lostSkills;
        character.LostItems = lostItems;
        character.LostMoney = (int)data["lostMoney"];

        character.Vehicle = static_cast<Vehicle::Type>((int)data["vehicle"]);

        character.ITEM_LIMIT = (int)data["itemLimit"];
        character.MAX_LIFE_LIMIT = (int)data["lifeLimit"];
        character.SKILLS_LIMIT = (int)data["skillsLimit"];
        character.StoryID = (int)data["storyID"];

        try
        {

#if defined(_WIN32) || defined(__arm__)
            character.Epoch = (long long)(data["epoch"]);
#else
            character.Epoch = (long)(data["epoch"]);
#endif
        }
        catch (std::exception &ex)
        {
            character.Epoch = 0;
        }

        auto potionsApplied = std::vector<Item::Type>();

        try
        {
            if (data["potionsApplied"].size() > 0)
            {
                for (auto i = 0; i < (int)data["potionsApplied"].size(); i++)
                {
                    potionsApplied.push_back(static_cast<Item::Type>((int)data["potionsApplied"][i]));
                }
            }
        }
        catch (const std::exception &e)
        {
        }

        character.PotionsApplied = potionsApplied;
    }
    else
    {
        character.StoryID = -1;
    }

    return character;
}

std::vector<Button> createFilesList(SDL_Window *window, SDL_Renderer *renderer, std::vector<std::string> list, int start, int last, int limit, bool save_button)
{
    auto controls = std::vector<Button>();

    auto text_space = 8;

    if (list.size() > 0)
    {
        for (int i = 0; i < last - start; i++)
        {
            std::string game_string = "";

            auto index = start + i;

            auto character = loadGame(list[index]);

#if defined(_WIN32) || defined(__arm__)
            long long epoch_long;
#else
            long epoch_long;
#endif

            if (character.Epoch == 0)
            {
                auto epoch = list[index].substr(list[index].find_last_of("/") + 1, list[index].find_last_of(".") - list[index].find_last_of("/") - 1);

#if defined(_WIN32) || defined(__arm__)
                epoch_long = std::stoull(epoch);
#else
                epoch_long = std::stol(epoch);
#endif
            }
            else
            {
                epoch_long = character.Epoch;
            }

            if (character.StoryID != -1)
            {
                auto storyID = std::to_string(character.StoryID);

                game_string += std::string(4 - std::to_string(index + 1).length(), '0') + std::to_string(index + 1) + ". " + character.Name + "\n";
                game_string += "Date: " + time_string(epoch_long) + "\n";
                game_string += "Section " + std::string(4 - storyID.length(), '0') + storyID + ": ";
                game_string += "Life: " + std::to_string(character.Life);
                game_string += ", Money: " + std::to_string(character.Money);
                game_string += ", Items: " + std::to_string(character.Items.size());
                game_string += ", Codewords: " + std::to_string(character.Codewords.size());
            }

            auto button = createHeaderButton(window, game_string.c_str(), clrWH, intLB, textwidth - 3 * button_space / 2, 0.125 * SCREEN_HEIGHT, text_space);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : texty + 2 * text_space);

            controls.push_back(Button(i, button, i, i, (i > 0 ? i - 1 : i), (i < (last - start) ? i + 1 : i), textx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = button->w;

            controls[i].H = button->h;
        }
    }

    auto idx = controls.size();

    if (list.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + border_space, Control::Type::SCROLL_UP));

            idx++;
        }

        if (list.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + text_bounds - arrow_size - border_space, Control::Type::SCROLL_DOWN));

            idx++;
        }
    }

    controls.push_back(Button(idx, "icons/open.png", idx, idx + 1, idx > 0 ? idx - 1 : idx, idx, startx, buttony, Control::Type::LOAD));

    if (save_button)
    {
        controls.push_back(Button(idx + 1, "icons/disk.png", idx, idx + 2, idx > 0 ? idx - 1 : idx + 1, idx + 1, startx + gridsize, buttony, Control::Type::SAVE));
    }

    idx = controls.size();

    controls.push_back(Button(idx, "icons/back-button.png", idx - 1, idx, list.size() > 0 ? (last - start) : idx, idx, (1.0 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

    return controls;
}

Control::Type gameScreen(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, bool save_botton)
{
    auto result = Control::Type::BACK;
    auto done = false;

    if (window && renderer)
    {
#if defined(_WIN32)
        PWSTR path_str;

        SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_DEFAULT, NULL, &path_str);

        std::wstring wpath(path_str);

        CoTaskMemFree(path_str);

        std::string save(wpath.length(), ' ');

        std::copy(wpath.begin(), wpath.end(), save.begin());

        save += "/Saved Games/Heart of Ice";

        std::string path = save + "/";
#else
        const char *homedir;

        if ((homedir = getenv("HOME")) == NULL)
        {
            homedir = getpwuid(getuid())->pw_dir;
        }

        std::string save = std::string(homedir) + "/Documents/Saved Games/Heart Of Ice";
        std::string path = save + "/";
#endif

        fs::create_directories(save);

        std::vector<std::string> entries;

        auto splash = createImage("images/filler1.png");

        auto saved_games = std::multimap<std::filesystem::file_time_type, std::string, std::greater<std::filesystem::file_time_type>>();

        try
        {
            for (const auto &entry : fs::directory_iterator(path))
            {
                auto time_stamp = entry.last_write_time();

#if defined(_WIN32) || defined(__arm__)
                std::string file_name = entry.path().string();
#else
                std::string file_name = entry.path();
#endif

                if (file_name.substr(file_name.find_last_of(".") + 1) == "save")
                {
                    saved_games.insert(std::make_pair(time_stamp, file_name));
                }
            }
        }
        catch (std::exception &ex)
        {
            std::cerr << "Unable to read save directory!" << std::endl;
        }

        for (auto const &entry : saved_games)
        {
            entries.push_back(entry.second);
        }

        auto font_size = 20;
        auto text_space = 8;
        auto infoh = 0.06 * SCREEN_HEIGHT;
        auto boxh = 0.125 * SCREEN_HEIGHT;
        auto box_space = 10;

        auto offset = 0;
        auto last = 0;
        int limit = (text_bounds) / (boxh + 3 * text_space);

        last = offset + limit;

        if (last > entries.size())
        {
            last = entries.size();
        }

        auto controls = createFilesList(window, renderer, entries, offset, last, limit, save_botton);

        auto current = -1;
        auto selected = false;
        auto scrollUp = false;
        auto scrollDown = false;
        auto hold = false;
        auto scrollSpeed = 1;
        auto space = 8;

        auto selected_file = -1;

        TTF_Init();

        auto font = TTF_OpenFont(FONT_FILE, font_size);

        while (!done)
        {
            last = offset + limit;

            if (last > entries.size())
            {
                last = entries.size();
            }

            SDL_SetWindowTitle(window, "Heart of Ice: LOAD/SAVE game");

            // Fill the surface with background color
            fillWindow(renderer, intWH);

            fitImage(renderer, splash, startx, starty, splashw, text_bounds);

            fillRect(renderer, textwidth, text_bounds, textx, texty, intBE);

            renderButtons(renderer, controls, current, intGR, border_space, border_pts);

            putText(renderer, "Selected", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh));

            if (selected_file >= 0 && selected_file < entries.size())
            {
                std::string game_string = "";

                auto character = loadGame(entries[selected_file]);

#if defined(_WIN32) || defined(__arm__)
                long long epoch_long;
#else
                long epoch_long;
#endif

                if (character.Epoch > 0)
                {
                    epoch_long = character.Epoch;
                }
                else
                {
                    auto epoch = entries[selected_file].substr(entries[selected_file].find_last_of("/") + 1, entries[selected_file].find_last_of(".") - entries[selected_file].find_last_of("/") - 1);

#if defined(_WIN32) || defined(__arm__)
                    epoch_long = std::stoull(epoch);
#else
                    epoch_long = std::stol(epoch);
#endif
                }

                if (character.StoryID != -1)
                {
                    auto storyID = std::to_string(character.StoryID);

                    game_string = "Date: " + time_string(epoch_long) + "\n";
                    game_string += std::string(4 - storyID.length(), '0') + storyID + ": " + character.Name;
                    game_string += "\nLife: " + std::to_string(character.Life);
                    game_string += " Money: " + std::to_string(character.Money);
                }

                putText(renderer, game_string.c_str(), font, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
            }
            else
            {
                fillRect(renderer, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh, intBE);
            }

            if (last - offset > 0)
            {
                for (auto i = 0; i < last - offset; i++)
                {
                    if (offset + i == selected_file)
                    {
                        for (auto size = 4; size >= 0; size--)
                        {
                            drawRect(renderer, controls[i].W + 2 * text_space - 2 * size, controls[i].H + 2 * text_space - 2 * size, controls[i].X - text_space + size, controls[i].Y - text_space + size, intDB);
                        }
                    }
                }
            }

            done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if ((selected && current >= 0 && current < controls.size()) || scrollUp || scrollDown || hold)
            {
                if (controls[current].Type == Control::Type::SCROLL_UP || (controls[current].Type == Control::Type::SCROLL_UP && hold) || scrollUp)
                {
                    if (offset > 0)
                    {
                        offset -= scrollSpeed;

                        if (offset < 0)
                        {
                            offset = 0;
                        }

                        last = offset + limit;

                        if (last > entries.size())
                        {
                            last = entries.size();
                        }

                        controls = createFilesList(window, renderer, entries, offset, last, limit, save_botton);

                        SDL_Delay(50);
                    }

                    if (offset <= 0)
                    {
                        current = -1;

                        selected = false;
                    }
                }
                else if (controls[current].Type == Control::Type::SCROLL_DOWN || ((controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown))
                {
                    if (entries.size() - last > 0)
                    {
                        if (offset < entries.size() - limit)
                        {
                            offset += scrollSpeed;
                        }

                        if (offset > entries.size() - limit)
                        {
                            offset = entries.size() - limit;
                        }

                        last = offset + limit;

                        if (last > entries.size())
                        {
                            last = entries.size();
                        }

                        controls = createFilesList(window, renderer, entries, offset, last, limit, save_botton);

                        SDL_Delay(50);

                        if (offset > 0)
                        {
                            if (controls[current].Type != Control::Type::SCROLL_DOWN)
                            {
                                current++;
                            }
                        }
                    }

                    if (entries.size() - last <= 0)
                    {
                        selected = false;

                        current = -1;
                    }
                }
                else if (controls[current].Type == Control::Type::ACTION && !hold)
                {
                    if (offset + current == selected_file)
                    {
                        selected_file = -1;
                    }
                    else
                    {
                        selected_file = offset + current;
                    }
                }
                else if (controls[current].Type == Control::Type::LOAD && !hold)
                {
                    if (selected_file >= 0 && selected_file < entries.size())
                    {
                        player = loadGame(entries[selected_file]);

                        result = Control::Type::LOAD;

                        done = true;

                        break;
                    }
                }
                else if (controls[current].Type == Control::Type::SAVE && !hold)
                {
                    if (selected_file != -1)
                    {
                        saveGame(player, entries[selected_file].c_str());
                    }
                    else
                    {
                        saveGame(player, NULL);
                    }

                    result = Control::Type::SAVE;

                    done = true;

                    break;
                }
                else if (controls[current].Type == Control::Type::BACK && !hold)
                {
                    result = Control::Type::BACK;

                    done = true;

                    break;
                }
            }
        }

        if (font)
        {
            TTF_CloseFont(font);

            font = NULL;
        }

        if (splash)
        {
            SDL_FreeSurface(splash);

            splash = NULL;
        }

        TTF_Quit();
    }

    return result;
}

bool tradeScreen(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, Item::Base mine, Item::Base theirs)
{
    auto done = false;

    if (Character::VERIFY_ITEMS(player, {mine.Type}))
    {
        const char *message = NULL;

        auto error = false;

        Uint32 start_ticks = 0;
        Uint32 duration = 3000;

        auto text_space = 8;

        auto box_space = 10;

        auto textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space) - 2 * text_space;

        auto controls = std::vector<Button>();

        auto idx = 0;

        controls.push_back(Button(idx, "icons/yes.png", idx, idx + 1, idx, idx, startx, buttony, Control::Type::ACTION));
        controls.push_back(Button(idx + 1, "icons/back-button.png", idx, idx + 1, idx, idx + 1, (1 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

        TTF_Init();

        auto font = TTF_OpenFont(FONT_FILE, 20);

        auto selected = false;
        auto current = -1;
        auto quit = false;
        auto scrollUp = false;
        auto scrollDown = false;
        auto hold = false;

        auto infoh = 0.06 * SCREEN_HEIGHT;
        auto boxh = 0.125 * SCREEN_HEIGHT;

        auto donation = 1;

        while (!done)
        {
            SDL_SetWindowTitle(window, "Make a Donation");

            fillWindow(renderer, intWH);

            if (error)
            {
                if ((SDL_GetTicks() - start_ticks) < duration)
                {
                    putText(renderer, message, font, text_space, clrWH, intRD, TTF_STYLE_NORMAL, splashw, boxh * 2, startx, starty);
                }
                else
                {
                    error = false;
                }
            }

            if (!error)
            {
                putText(renderer, "TRADE", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, boxh, startx, starty);
            }

            putText(renderer, "Life", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (boxh + infoh));
            putText(renderer, (std::to_string(player.Life)).c_str(), font, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - boxh);

            putText(renderer, "Money", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * (boxh + infoh) + box_space));
            putText(renderer, (std::to_string(player.Money) + std::string(" scads")).c_str(), font, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - (2 * boxh + infoh + box_space));

            fillRect(renderer, textwidth + arrow_size + button_space, text_bounds, textx, texty, intBE);

            std::string trade_text = "Trade " + std::string(mine.Name) + " for " + std::string(theirs.Name) + "?";
            putText(renderer, trade_text.c_str(), font, text_space, clrBK, intBE, TTF_STYLE_NORMAL, textwidth, boxh, textx + text_space, texty + text_space);

            renderButtons(renderer, controls, current, intGR, text_space, text_space / 2);

            done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if (selected)
            {
                if (controls[current].Type == Control::Type::ACTION && !hold)
                {
                    Character::LOSE_ITEMS(player, {mine.Type});
                    Character::GET_ITEMS(player, {theirs});

                    done = true;

                    current = -1;

                    selected = false;

                    break;
                }
                else if (controls[current].Type == Control::Type::BACK && !hold)
                {
                    done = false;

                    break;
                }
            }
        }

        if (font)
        {
            TTF_CloseFont(font);

            font = NULL;
        }

        TTF_Quit();
    }

    return done;
}

bool shopScreen(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, Story::Base *story, Control::Type mode)
{
    auto shop = mode == Control::Type::BUY ? story->Shop : story->Sell;

    if (shop.size() > 0)
    {
        std::string message;

        auto error = false;
        auto purchased = false;

        Uint32 start_ticks = 0;
        Uint32 duration = 3000;

        auto done = false;

        auto controls = std::vector<Button>();

        auto text_space = 8;

        auto textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space) - 2 * text_space;

        auto idx = 0;

        for (auto i = shop.begin(); i != shop.end(); i++)
        {
            auto item = i->first;
            auto price = i->second;

            std::string choice = item.Name;

            if (mode == Control::Type::BUY)
            {
                if (item.Charge >= 0)
                {
                    choice += " (";

                    if (item.Charge > 0)
                    {
                        choice += std::to_string(item.Charge) + " charges";
                    }
                    else
                    {
                        choice += "empty";
                    }

                    choice += ")";
                }
            }

            choice += " (" + std::to_string(price) + " scads)";

            auto text = createText(choice.c_str(), FONT_FILE, 16, clrBK, textwidth + button_space, TTF_STYLE_NORMAL);

            auto y = (idx > 0 ? controls[idx - 1].Y + controls[idx - 1].H + 3 * text_space : texty + 2 * text_space);

            controls.push_back(Button(idx, text, idx, idx, (idx > 0 ? idx - 1 : idx), (idx < shop.size() ? idx + 1 : idx), textx + 2 * text_space, y, Control::Type::ACTION));
            controls[idx].W = textwidth + button_space;
            controls[idx].H = text->h;

            idx++;
        }

        controls.push_back(Button(idx, "icons/items.png", idx - 1, idx + 1, idx - 1, idx + 1, startx, buttony, Control::Type::USE));
        controls.push_back(Button(idx + 1, "icons/back-button.png", idx, idx + 1, idx - 1, idx + 1, (1 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

        TTF_Init();

        auto font = TTF_OpenFont(FONT_FILE, 20);

        auto selected = false;
        auto current = -1;
        auto quit = false;
        auto scrollUp = false;
        auto scrollDown = false;
        auto hold = false;

        auto messageh = 0.25 * SCREEN_HEIGHT;
        auto boxh = 0.125 * SCREEN_HEIGHT;
        auto infoh = 0.06 * SCREEN_HEIGHT;
        auto box_space = 10;

        while (!done)
        {
            if (story->Title)
            {
                SDL_SetWindowTitle(window, story->Title);
            }
            else
            {
                SDL_SetWindowTitle(window, "Heart of Ice: Shop");
            }

            fillWindow(renderer, intWH);

            if (error)
            {
                if ((SDL_GetTicks() - start_ticks) < duration)
                {
                    putText(renderer, message.c_str(), font, text_space, clrWH, intRD, TTF_STYLE_NORMAL, splashw, messageh, startx, starty);
                }
                else
                {
                    error = false;
                }
            }
            else if (purchased)
            {
                if ((SDL_GetTicks() - start_ticks) < duration)
                {
                    putText(renderer, message.c_str(), font, text_space, clrWH, intLB, TTF_STYLE_NORMAL, splashw, messageh, startx, starty);
                }
                else
                {
                    purchased = false;
                }
            }

            if (!error && !purchased)
            {
                if (mode == Control::Type::BUY)
                {
                    putText(renderer, "Select an item to buy", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, messageh, startx, starty);
                }
                else
                {
                    putText(renderer, "You may sell your items at prices indicated here", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, messageh, startx, starty);
                }
            }

            putText(renderer, "Possessions", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (boxh + infoh));
            putText(renderer, (std::to_string(player.Items.size()) + std::string(" item(s)")).c_str(), font, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - boxh);

            putText(renderer, "Money", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * (boxh + infoh) + box_space));
            putText(renderer, (std::to_string(player.Money) + std::string(" scads")).c_str(), font, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - (2 * boxh + infoh + box_space));

            fillRect(renderer, textwidth + arrow_size + button_space, text_bounds, textx, texty, intBE);

            renderButtons(renderer, controls, current, intGR, text_space, text_space / 2);

            for (auto i = 0; i < shop.size(); i++)
            {
                if (i != current)
                {
                    drawRect(renderer, controls[i].W + 2 * text_space, controls[i].H + 2 * text_space, controls[i].X - text_space, controls[i].Y - text_space, intDB);
                }
            }

            done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if (selected)
            {
                if (controls[current].Type == Control::Type::ACTION && !hold)
                {
                    if (current >= 0 && current < shop.size())
                    {
                        auto element = shop.at(current);
                        auto item = element.first;
                        auto price = element.second;

                        if (mode == Control::Type::BUY)
                        {
                            if (player.Money >= price)
                            {
                                if (item.Type == Item::Type::EXALTED_ENHANCER || item.Type == Item::Type::VIRID_MYSTERY || item.Type == Item::Type::PEERLESS_PERCEPTIVATE || item.Type == Item::Type::MASK_OF_OCCULTATION)
                                {
                                    if (Character::IS_APPLIED(player, item.Type))
                                    {
                                        message = "Malengin no longer has this retrovirus in stock!";

                                        start_ticks = SDL_GetTicks();

                                        purchased = false;

                                        error = true;
                                    }
                                    else
                                    {
                                        player.Money -= price;

                                        if (item.Type == Item::Type::EXALTED_ENHANCER)
                                        {
                                            auto agility_lost = Character::VERIFY_SKILL(player, Skill::Type::AGILITY);

                                            Character::EXALTED_ENHANCER(player, false);

                                            message = "You gain 5 Maximum Life Points.";

                                            if (agility_lost)
                                            {
                                                message += " However, your [AGILITY] skill is LOST.";
                                            }

                                            start_ticks = SDL_GetTicks();

                                            purchased = true;

                                            error = false;
                                        }
                                        else if (item.Type == Item::Type::MASK_OF_OCCULTATION)
                                        {
                                            Character::MASK_OF_OCCULTATION(player, false);

                                            message = "You now have the ability to alter your appearance and colouring. You gain the codeword CAMOUFLAGE.";

                                            start_ticks = SDL_GetTicks();

                                            purchased = true;

                                            error = false;
                                        }
                                        else if (item.Type == Item::Type::PEERLESS_PERCEPTIVATE)
                                        {
                                            Character::PEERLESS_PERCEPTIVATE(player, false);

                                            message = "You now have the ability to see in almost total darkness. You gain the codeword SCOTOPIC.";

                                            start_ticks = SDL_GetTicks();

                                            error = false;

                                            purchased = true;
                                        }
                                        else if (item.Type == Item::Type::VIRID_MYSTERY)
                                        {
                                            auto reversed = 0;

                                            message = "";

                                            if (Character::IS_APPLIED(player, Item::Type::EXALTED_ENHANCER))
                                            {
                                                reversed++;

                                                message += "EXALTED ENHANCER effects reversed.";

                                                Character::EXALTED_ENHANCER(player, true);
                                            }

                                            if (Character::IS_APPLIED(player, Item::Type::MASK_OF_OCCULTATION))
                                            {
                                                if (reversed > 0)
                                                {
                                                    message += " ";
                                                }

                                                reversed++;

                                                message += "MASK OF OCCULTATION effects reversed.";

                                                Character::MASK_OF_OCCULTATION(player, true);
                                            }

                                            if (Character::IS_APPLIED(player, Item::Type::PEERLESS_PERCEPTIVATE))
                                            {
                                                if (reversed > 0)
                                                {
                                                    message += " ";
                                                }

                                                reversed++;

                                                message += "PEERLESS PERCEPTIVATE effects reversed.";

                                                Character::PEERLESS_PERCEPTIVATE(player, true);
                                            }

                                            Character::APPLY_VIRUS(player, Item::Type::VIRID_MYSTERY);

                                            if (reversed == 0)
                                            {
                                                message = "No retrovirus effects reversed!";

                                                error = true;

                                                purchased = false;
                                            }
                                            else
                                            {
                                                error = false;

                                                purchased = true;
                                            }

                                            start_ticks = SDL_GetTicks();
                                        }
                                    }
                                }
                                else
                                {
                                    if (Item::IsUnique(item.Type) && Character::VERIFY_ITEMS(player, {item.Type}))
                                    {
                                        message = "You already have this item!";

                                        start_ticks = SDL_GetTicks();

                                        purchased = false;

                                        error = true;
                                    }
                                    else
                                    {
                                        Character::GET_ITEMS(player, {item});

                                        player.Money -= price;

                                        while (!Character::VERIFY_POSSESSIONS(player))
                                        {
                                            inventoryScreen(window, renderer, player, story, player.Items, Control::Type::DROP, 0);
                                        }

                                        std::string description = item.Name;

                                        if (item.Charge >= 0)
                                        {
                                            description += " (";

                                            if (item.Charge > 0)
                                            {
                                                description += std::to_string(item.Charge) + " charges";
                                            }
                                            else
                                            {
                                                description += "empty";
                                            }

                                            description += ")";
                                        }

                                        message = description + " purchased.";

                                        start_ticks = SDL_GetTicks();

                                        purchased = true;

                                        error = false;
                                    }
                                }
                            }
                            else
                            {
                                message = "You do not have enough scads to buy that!";

                                start_ticks = SDL_GetTicks();

                                purchased = false;

                                error = true;
                            }
                        }
                        else if (mode == Control::Type::SELL)
                        {
                            auto result = Item::FIND_TYPE(player.Items, item.Type);

                            if (result >= 0)
                            {
                                message = std::string(item.Name) + " SOLD.";

                                start_ticks = SDL_GetTicks();

                                purchased = true;

                                error = false;

                                Character::GAIN_MONEY(player, price);

                                if (Item::COUNT_TYPES(player.Items, item.Type) > 1)
                                {
                                    auto least = Item::FIND_LEAST(player.Items, item.Type);

                                    if (least >= 0)
                                    {
                                        player.Items.erase(player.Items.begin() + least);
                                    }
                                }
                                else
                                {
                                    Character::LOSE_ITEMS(player, {item.Type});
                                }
                            }
                            else
                            {
                                message = "You do not have that item!";

                                start_ticks = SDL_GetTicks();

                                purchased = false;

                                error = true;
                            }
                        }
                    }
                }
                else if (controls[current].Type == Control::Type::CHARACTER && !hold)
                {
                    characterScreen(window, renderer, player, story);

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::USE && !hold)
                {
                    inventoryScreen(window, renderer, player, story, player.Items, Control::Type::USE, 0);

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::BACK && !hold)
                {
                    done = true;

                    break;
                }
            }
        }

        if (font)
        {
            TTF_CloseFont(font);

            font = NULL;
        }

        TTF_Quit();
    }

    return false;
}

bool mapScreen(SDL_Window *window, SDL_Renderer *renderer)
{
    auto done = false;

    auto splash = createImage("images/map-earth-23rd.png");
    auto background = createImage("images/background.png");

    // Render the image
    if (window && renderer && splash && background)
    {
        SDL_SetWindowTitle(window, "Map: Earth in the 23rd Century");

        auto selected = false;
        auto current = -1;

        auto marginw = (1.0 - 2.0 * Margin) * SCREEN_WIDTH;

        std::vector<Button> controls = {Button(0, "icons/back-button.png", 0, 0, 0, 0, (1 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK)};

        int offset_x = (marginw - (double)text_bounds / splash->h * splash->w) / 2;

        while (!done)
        {
            // Fill the surface with background color
            stretchImage(renderer, background, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

            fitImage(renderer, splash, startx + offset_x, starty, marginw, text_bounds);

            renderButtons(renderer, controls, current, intWH, 8, 4);

            bool scrollUp = false;
            bool scrollDown = false;
            bool hold = false;

            done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if (selected && current >= 0 && current < controls.size() && controls[current].Type == Control::Type::BACK)
            {
                break;
            }
        }

        SDL_FreeSurface(splash);
        SDL_FreeSurface(background);

        splash = NULL;
        background = NULL;
    }

    return done;
}

Story::Base *processChoices(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, Story::Base *story)
{
    Story::Base *next = &notImplemented;

    auto error = false;
    const char *message = NULL;

    Uint32 start_ticks = 0;
    Uint32 duration = 5000;

    auto background = createImage("images/background.png");

    if (renderer && story->Choices.size() > 0)
    {
        SDL_Surface *splash = NULL;

        if (story->Image)
        {
            splash = createImage(story->Image);
        }

        auto choices = story->Choices;

        auto selected = false;
        auto current = -1;
        auto done = false;
        auto scrollUp = false;
        auto scrollDown = false;
        auto hold = false;

        auto controls = std::vector<Button>();

        auto text_space = 8;
        auto textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space) - 2 * text_space;
        auto messageh = 0.25 * SCREEN_HEIGHT;
        auto boxh = 0.125 * SCREEN_HEIGHT;
        auto infoh = 0.06 * SCREEN_HEIGHT;
        auto box_space = 10;

        for (int i = 0; i < choices.size(); i++)
        {
            auto text = createText(choices[i].Text, FONT_FILE, 20, clrBK, textwidth + button_space, TTF_STYLE_NORMAL);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : texty + 2 * text_space);

            controls.push_back(Button(i, text, i, i, (i > 0 ? i - 1 : i), (i < choices.size() ? i + 1 : i), textx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = textwidth + button_space;

            controls[i].H = text->h;
        }

        auto idx = choices.size();

        controls.push_back(Button(idx, "icons/map.png", idx - 1, idx + 1, idx - 1, idx, startx, buttony, Control::Type::MAP));
        controls.push_back(Button(idx + 1, "icons/user.png", idx, idx + 2, idx - 1, idx + 1, startx + gridsize, buttony, Control::Type::CHARACTER));
        controls.push_back(Button(idx + 2, "icons/items.png", idx + 1, idx + 3, idx - 1, idx + 2, startx + 2 * gridsize, buttony, Control::Type::USE));
        controls.push_back(Button(idx + 3, "icons/back-button.png", idx + 2, idx + 3, idx - 1, idx + 3, (1 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

        TTF_Init();

        auto font = TTF_OpenFont(FONT_FILE, 20);

        int splash_h = splashw;

        if (splash)
        {
            if (splash->w != splashw)
            {
                splash_h = (int)((double)splashw / splash->w * splash->h);
            }
        }

        while (!done)
        {
            if (story->Title)
            {
                SDL_SetWindowTitle(window, story->Title);
            }

            fillWindow(renderer, intWH);

            if (background)
            {
                stretchImage(renderer, background, 0, 0, SCREEN_WIDTH, buttony - button_space);
            }

            if (splash)
            {
                splash_h = fitImage(renderer, splash, startx, starty, splashw, text_bounds);
            }

            if (!splash || (splash && splash_h < (text_bounds - (boxh + infoh))))
            {
                putText(renderer, "Life", font, text_space, clrWH, (player.Life > 0 && story->Type != Story::Type::DOOM) ? intDB : intRD, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (boxh + infoh));
                putText(renderer, (std::to_string(player.Life)).c_str(), font, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - boxh);
            }

            if (!splash || (splash && splash_h < text_bounds - (2 * (boxh + infoh) + box_space)))
            {
                putText(renderer, "Money", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * (boxh + infoh) + box_space));
                putText(renderer, (std::to_string(player.Money) + std::string(" scads")).c_str(), font, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - (2 * boxh + infoh + box_space));
            }

            if (error)
            {
                if ((SDL_GetTicks() - start_ticks) < duration)
                {
                    putText(renderer, message, font, text_space, clrWH, intRD, TTF_STYLE_NORMAL, splashw, messageh, startx, starty);
                }
                else
                {
                    error = false;
                }
            }

            fillRect(renderer, textwidth + arrow_size + button_space, text_bounds, textx, texty, intBE);

            renderButtons(renderer, controls, current, intGR, text_space, text_space / 2);

            for (auto i = 0; i < story->Choices.size(); i++)
            {
                if (i != current)
                {
                    drawRect(renderer, controls[i].W + 2 * text_space, controls[i].H + 2 * text_space, controls[i].X - text_space, controls[i].Y - text_space, intDB);
                }
            }

            done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if (selected && current >= 0 && current < controls.size())
            {
                if (controls[current].Type == Control::Type::ACTION && !hold)
                {
                    if (current >= 0 && current < story->Choices.size())
                    {
                        if (story->Choices[current].Type == Choice::Type::NORMAL)
                        {
                            next = (Story::Base *)findStory(story->Choices[current].Destination);

                            done = true;

                            break;
                        }
                        else if (story->Choices[current].Type == Choice::Type::ITEMS)
                        {
                            auto items = std::vector<Item::Type>();

                            for (auto i = 0; i < story->Choices[current].Items.size(); i++)
                            {
                                items.push_back(story->Choices[current].Items[i].Type);
                            }

                            if (Character::VERIFY_ITEMS(player, items))
                            {
                                next = (Story::Base *)findStory(story->Choices[current].Destination);

                                done = true;

                                break;
                            }
                            else
                            {
                                if (Item::VERIFY(player.Items, story->Choices[current].Items[0]))
                                {
                                    message = "The item you are carrying is not charged!";
                                }
                                else
                                {
                                    message = "You do not have the required item!";
                                }

                                start_ticks = SDL_GetTicks();

                                error = true;
                            }
                        }
                        else if (story->Choices[current].Type == Choice::Type::ANY_ITEM)
                        {
                            if (Character::VERIFY_ITEMS_ANY(player, story->Choices[current].Items))
                            {
                                next = (Story::Base *)findStory(story->Choices[current].Destination);

                                done = true;

                                break;
                            }
                            else
                            {
                                message = "You do not have any of the required items that can be used.";

                                start_ticks = SDL_GetTicks();

                                error = true;
                            }
                        }
                        else if (story->Choices[current].Type == Choice::Type::CODEWORD)
                        {
                            if (Character::VERIFY_CODEWORD(player, story->Choices[current].Codeword))
                            {
                                next = (Story::Base *)findStory(story->Choices[current].Destination);

                                done = true;

                                break;
                            }
                            else
                            {
                                message = "You do not have the required codeword!";

                                start_ticks = SDL_GetTicks();

                                error = true;
                            }
                        }
                        else if (story->Choices[current].Type == Choice::Type::GET_ITEMS)
                        {
                            Character::GET_ITEMS(player, {story->Choices[current].Items});

                            while (!Character::VERIFY_POSSESSIONS(player))
                            {
                                inventoryScreen(window, renderer, player, story, player.Items, Control::Type::DROP, 0);
                            }

                            next = (Story::Base *)findStory(story->Choices[current].Destination);

                            done = true;

                            break;
                        }
                        else if (story->Choices[current].Type == Choice::Type::LOSE_ITEMS)
                        {
                            auto items = std::vector<Item::Type>();

                            for (auto i = 0; i < story->Choices[current].Items.size(); i++)
                            {
                                items.push_back(story->Choices[current].Items[i].Type);
                            }

                            if (Character::VERIFY_ITEMS(player, items))
                            {
                                Character::LOSE_ITEMS(player, items);

                                next = (Story::Base *)findStory(story->Choices[current].Destination);

                                done = true;

                                break;
                            }
                            else
                            {
                                message = "You do not have the required item!";

                                start_ticks = SDL_GetTicks();

                                error = true;
                            }
                        }
                        else if (story->Choices[current].Type == Choice::Type::GIVE)
                        {
                            if (player.Items.size() >= story->Choices[current].Value)
                            {
                                auto limit = player.Items.size() - story->Choices[current].Value;

                                while (player.Items.size() > limit)
                                {
                                    inventoryScreen(window, renderer, player, story, player.Items, Control::Type::LOSE, limit);
                                }

                                next = (Story::Base *)findStory(story->Choices[current].Destination);

                                done = true;

                                break;
                            }
                            else if (player.Items.size() > 0)
                            {
                                Character::LOSE_POSSESSIONS(player);

                                next = (Story::Base *)findStory(story->Choices[current].Destination);

                                done = true;

                                break;
                            }
                            else if (player.Items.size() == 0)
                            {
                                message = "You do not have anything to give!";

                                start_ticks = SDL_GetTicks();

                                error = true;
                            }
                        }
                        else if (story->Choices[current].Type == Choice::Type::GET_CODEWORD)
                        {
                            Character::GET_CODEWORDS(player, {story->Choices[current].Codeword});

                            next = (Story::Base *)findStory(story->Choices[current].Destination);

                            done = true;

                            break;
                        }
                        else if (story->Choices[current].Type == Choice::Type::LOSE_CODEWORD)
                        {
                            Character::REMOVE_CODEWORD(player, story->Choices[current].Codeword);

                            next = (Story::Base *)findStory(story->Choices[current].Destination);

                            done = true;

                            break;
                        }
                        else if (story->Choices[current].Type == Choice::Type::LOSE_ALL)
                        {
                            Character::LOSE_ALL(player);

                            next = (Story::Base *)findStory(story->Choices[current].Destination);

                            done = true;

                            break;
                        }
                        else if (story->Choices[current].Type == Choice::Type::LOSE_MONEY)
                        {
                            if (player.Money >= story->Choices[current].Value)
                            {
                                player.Money -= story->Choices[current].Value;

                                next = (Story::Base *)findStory(story->Choices[current].Destination);

                                done = true;

                                break;
                            }
                            else
                            {
                                message = "You do not have enough money!";

                                start_ticks = SDL_GetTicks();

                                error = true;
                            }
                        }
                        else if (story->Choices[current].Type == Choice::Type::BUY_VEHICLE && story->Choices[current].Vehicle != Vehicle::Type::NONE)
                        {
                            if (player.Money >= story->Choices[current].Value)
                            {
                                if (Character::CHECK_VEHICLE(player, story->Choices[current].Vehicle))
                                {
                                    message = "You already have this!";

                                    start_ticks = SDL_GetTicks();

                                    error = true;
                                }
                                else if (story->Choices[current].Vehicle == Vehicle::Type::BURREK && Character::CHECK_VEHICLE(player, Vehicle::Type::MANTA_SKY_CAR))
                                {
                                    message = "There is no point in buying a BURREK, as it would not fit inside the MANTA SKY-CAR";

                                    start_ticks = SDL_GetTicks();

                                    error = true;
                                }
                                else
                                {
                                    if (Character::CHECK_VEHICLE(player, Vehicle::Type::BURREK))
                                    {
                                        message = "You have abandonned your BURREK.";

                                        start_ticks = SDL_GetTicks();

                                        error = true;
                                    }

                                    player.Vehicle = story->Choices[current].Vehicle;

                                    player.Money -= story->Choices[current].Value;

                                    next = (Story::Base *)findStory(story->Choices[current].Destination);

                                    done = true;

                                    break;
                                }
                            }
                            else
                            {
                                message = "You do not have enough money!";

                                start_ticks = SDL_GetTicks();

                                error = true;
                            }
                        }
                        else if (story->Choices[current].Type == Choice::Type::GAIN_MONEY)
                        {
                            player.Money += story->Choices[current].Value;

                            next = (Story::Base *)findStory(story->Choices[current].Destination);

                            done = true;

                            break;
                        }
                        else if (story->Choices[current].Type == Choice::Type::MONEY)
                        {
                            if (player.Money >= story->Choices[current].Value)
                            {
                                next = (Story::Base *)findStory(story->Choices[current].Destination);

                                done = true;

                                break;
                            }
                            else
                            {
                                message = "You do not have enough money!";

                                start_ticks = SDL_GetTicks();

                                error = true;
                            }
                        }
                        else if (story->Choices[current].Type == Choice::Type::LIFE)
                        {
                            Character::GAIN_LIFE(player, story->Choices[current].Value);

                            if (player.Life > 0)
                            {
                                next = (Story::Base *)findStory(story->Choices[current].Destination);

                                done = true;
                            }
                            else
                            {
                                message = "That decision has cost you your life!";

                                start_ticks = SDL_GetTicks();

                                error = true;
                            }
                        }
                        else if (story->Choices[current].Type == Choice::Type::SKILL_ANY)
                        {
                            auto items = std::vector<Item::Type>();

                            for (auto i = 0; i < story->Choices[current].Items.size(); i++)
                            {
                                items.push_back(story->Choices[current].Items[i].Type);
                            }

                            if (Character::VERIFY_SKILL_ANY(player, story->Choices[current].Skill, items))
                            {
                                next = (Story::Base *)findStory(story->Choices[current].Destination);

                                done = true;

                                break;
                            }
                            else
                            {
                                if (Character::HAS_SKILL(player, story->Choices[current].Skill))
                                {
                                    message = "You do not have any of the required item to use with this skill!";
                                }
                                else
                                {
                                    message = "You do not possess the required skill!";
                                }

                                start_ticks = SDL_GetTicks();

                                error = true;
                            }
                        }
                        else if (story->Choices[current].Type == Choice::Type::SKILL)
                        {
                            if (Character::VERIFY_SKILL(player, story->Choices[current].Skill))
                            {
                                next = (Story::Base *)findStory(story->Choices[current].Destination);

                                done = true;

                                break;
                            }
                            else
                            {
                                if (Character::HAS_SKILL(player, story->Choices[current].Skill))
                                {
                                    auto result = Character::FIND_SKILL(player, story->Choices[current].Skill);

                                    auto item = player.Skills[result].Requirement;

                                    if (Item::FIND_TYPE(player.Items, item) >= 0)
                                    {
                                        message = "The item you are carrying is not charged!";
                                    }
                                    else
                                    {
                                        message = "You do not have the required item to use with this skill!";
                                    }
                                }
                                else
                                {
                                    message = "You do not possess the required skill!";
                                }

                                start_ticks = SDL_GetTicks();

                                error = true;
                            }
                        }
                        else if (story->Choices[current].Type == Choice::Type::SKILL_ANY)
                        {
                            auto items = std::vector<Item::Type>();

                            for (auto i = 0; i < story->Choices[current].Items.size(); i++)
                            {
                                items.push_back(story->Choices[current].Items[i].Type);
                            }

                            if (Character::VERIFY_SKILL_ANY(player, story->Choices[current].Skill, items))
                            {
                                next = (Story::Base *)findStory(story->Choices[current].Destination);

                                done = true;

                                break;
                            }
                            else
                            {
                                if (Character::HAS_SKILL(player, story->Choices[current].Skill))
                                {
                                    message = "You do not have any of the required item to use with this skill!";
                                }
                                else
                                {
                                    message = "You do not possess the required skill!";
                                }

                                start_ticks = SDL_GetTicks();

                                error = true;
                            }
                        }
                        else if (story->Choices[current].Type == Choice::Type::SKILL_ITEM)
                        {
                            auto items = std::vector<Item::Type>();

                            for (auto i = 0; i < story->Choices[current].Items.size(); i++)
                            {
                                items.push_back(story->Choices[current].Items[i].Type);
                            }

                            if (Character::HAS_SKILL(player, story->Choices[current].Skill) && Character::VERIFY_ITEMS(player, items))
                            {
                                next = (Story::Base *)findStory(story->Choices[current].Destination);

                                done = true;

                                break;
                            }
                            else
                            {
                                if (Character::HAS_SKILL(player, story->Choices[current].Skill))
                                {
                                    message = "You do not have the required item!";
                                }
                                else
                                {
                                    message = "You do not possess the required skill!";
                                }

                                start_ticks = SDL_GetTicks();

                                error = true;
                            }
                        }
                        else if (story->Choices[current].Type == Choice::Type::LOSE_SKILLS)
                        {
                            int limit = story->Choices[current].Value;

                            auto done = loseSkills(window, renderer, player, limit);

                            if (!done)
                            {
                                continue;
                            }
                            else
                            {
                                if (player.Skills.size() <= limit)
                                {
                                    auto nextID = story->Choices[current].Destination;

                                    if (nextID != story->ID)
                                    {
                                        next = (Story::Base *)findStory(nextID);
                                    }
                                    else
                                    {
                                        next = story;
                                    }

                                    done = true;

                                    break;
                                }
                            }
                        }
                        else if (story->Choices[current].Type == Choice::Type::FIRE_WEAPON)
                        {
                            auto weapons = std::vector<Item::Type>();

                            for (auto i = 0; i < story->Choices[current].Items.size(); i++)
                            {
                                weapons.push_back(story->Choices[current].Items[i].Type);
                            }

                            if (Character::VERIFY_ITEMS(player, weapons))
                            {
                                Character::FIRE_BARYSAL(player, 1);

                                next = (Story::Base *)findStory(story->Choices[current].Destination);

                                done = true;

                                break;
                            }
                            else
                            {
                                if (Item::FIND_TYPE(player.Items, Item::Type::BARYSAL_GUN) >= 0)
                                {
                                    message = "The BARYSAL GUN you are carrying is not charged!";
                                }
                                else
                                {
                                    message = "You do not have a charged BARYSAL GUN!";
                                }

                                start_ticks = SDL_GetTicks();

                                error = true;
                            }
                        }
                    }
                }
                else if (controls[current].Type == Control::Type::CHARACTER && !hold)
                {
                    characterScreen(window, renderer, player, story);

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::USE && !hold)
                {
                    inventoryScreen(window, renderer, player, story, player.Items, Control::Type::USE, 0);

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::MAP && !hold)
                {
                    mapScreen(window, renderer);

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::BACK && !hold)
                {
                    next = story;

                    done = true;

                    break;
                }
            }
        }

        if (font)
        {
            TTF_CloseFont(font);

            font = NULL;
        }

        TTF_Quit();

        if (splash)
        {
            SDL_FreeSurface(splash);

            splash = NULL;
        }
    }

    if (background)
    {
        SDL_FreeSurface(background);

        background = NULL;
    }

    return next;
}

std::vector<Button> createSkillControls(std::vector<Skill::Base> Skills)
{
    auto text_space = 8;
    auto textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space) - 2 * text_space;

    auto controls = std::vector<Button>();

    controls.clear();

    for (auto idx = 0; idx < Skills.size(); idx++)
    {
        auto text = createText(Skills[idx].Name, FONT_FILE, 20, clrBK, textwidth + button_space, TTF_STYLE_NORMAL);

        auto y = (idx > 0 ? controls[idx - 1].Y + controls[idx - 1].H + 3 * text_space : texty + 2 * text_space);

        controls.push_back(Button(idx, text, idx, idx, (idx > 0 ? idx - 1 : idx), (idx < Skills.size() ? idx + 1 : idx), textx + 2 * text_space, y, Control::Type::ACTION));

        controls[idx].W = textwidth + button_space;

        controls[idx].H = text->h;
    }

    return controls;
}

bool loseSkills(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, int limit)
{
    auto done = false;

    if (player.Skills.size() > limit)
    {
        const char *message = NULL;

        auto error = false;

        Uint32 start_ticks = 0;
        Uint32 duration = 3000;

        auto text_space = 8;

        auto textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space) - 2 * text_space;

        auto controls = createSkillControls(player.Skills);

        auto idx = player.Skills.size();

        controls.push_back(Button(idx, "icons/yes.png", idx - 1, idx + 1, idx - 1, idx, startx, buttony, Control::Type::CONFIRM));
        controls.push_back(Button(idx + 1, "icons/back-button.png", idx, idx + 1, idx - 1, idx + 1, (1 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

        TTF_Init();

        auto font = TTF_OpenFont(FONT_FILE, 20);

        auto selected = false;
        auto current = -1;
        auto quit = false;
        auto scrollUp = false;
        auto scrollDown = false;
        auto hold = false;

        auto infoh = 0.06 * SCREEN_HEIGHT;
        auto boxh = 0.125 * SCREEN_HEIGHT;

        auto selection = std::vector<Skill::Base>();

        while (!done)
        {
            SDL_SetWindowTitle(window, "Heart of Ice");

            fillWindow(renderer, intWH);

            if (error)
            {
                if ((SDL_GetTicks() - start_ticks) < duration)
                {
                    putText(renderer, message, font, text_space, clrWH, intRD, TTF_STYLE_NORMAL, splashw, boxh * 2, startx, starty);
                }
                else
                {
                    error = false;
                }
            }

            if (!error)
            {
                std::string lose_message = "Select " + std::string((player.SKILLS_LIMIT - limit) > 1 ? std::string(std::to_string(player.SKILLS_LIMIT - limit) + " skills") : "a skill") + " to LOSE.";

                putText(renderer, lose_message.c_str(), font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, boxh, startx, starty);
            }

            std::string to_lose = "";

            if (selection.size() > 0)
            {
                for (auto i = 0; i < selection.size(); i++)
                {
                    if (i > 0)
                    {
                        to_lose += ", ";
                    }

                    to_lose += std::string(selection[i].Name);
                }
            }

            putText(renderer, "SKILLS", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (boxh + infoh));
            putText(renderer, selection.size() > 0 ? to_lose.c_str() : "(None)", font, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - boxh);

            fillRect(renderer, textwidth + arrow_size + button_space, text_bounds, textx, texty, intBE);

            renderButtons(renderer, controls, current, intGR, text_space, text_space / 2);

            for (auto i = 0; i < player.Skills.size(); i++)
            {
                if (Skill::VERIFY(selection, player.Skills[i]))
                {
                    drawRect(renderer, controls[i].W + 2 * text_space, controls[i].H + 2 * text_space, controls[i].X - text_space, controls[i].Y - text_space, intDB);
                }
            }

            done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if (selected)
            {
                if (controls[current].Type == Control::Type::ACTION && !hold)
                {
                    if (current >= 0 && current < player.Skills.size())
                    {
                        if (Skill::VERIFY(selection, player.Skills[current]))
                        {
                            Skill::REMOVE(selection, player.Skills[current]);
                        }
                        else
                        {
                            if (selection.size() < (player.SKILLS_LIMIT - limit))
                            {
                                Skill::ADD(selection, player.Skills[current]);
                            }
                        }
                    }

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::CONFIRM && !hold)
                {
                    if (selection.size() > 0 && ((player.SKILLS_LIMIT - selection.size()) <= limit))
                    {
                        auto skills = std::vector<Skill::Type>();

                        for (auto i = 0; i < selection.size(); i++)
                        {
                            skills.push_back(selection[i].Type);
                        }

                        Character::LOSE_SKILLS(player, skills);

                        done = true;

                        break;
                    }
                    else
                    {
                        message = "Please complete your selection";

                        start_ticks = SDL_GetTicks();

                        error = true;
                    }

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::BACK && !hold)
                {
                    done = false;

                    break;
                }
            }
        }

        if (font)
        {
            TTF_CloseFont(font);

            font = NULL;
        }

        TTF_Quit();
    }

    return done;
}

Story::Base *renderChoices(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, Story::Base *story)
{
    Story::Base *next = &notImplemented;

    if (story->Choices.size() > 0)
    {
        next = processChoices(window, renderer, player, story);
    }
    else
    {
        next = (Story::Base *)findStory(story->Continue(player));
    }

    return next;
}

void clipValue(int &val, int min, int max)
{
    if (val < min)
    {
        val = min;
    }
    if (val > max)
    {
        val = max;
    }
}

bool processStory(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, Story::Base *story)
{
    auto quit = false;

    auto space = 8;

    auto font_size = 20;

    auto flash_message = false;
    auto flash_color = intRD;

    const char *message = NULL;

    Uint32 start_ticks = 0;
    Uint32 duration = 5000;

    TTF_Init();

    auto font = TTF_OpenFont(FONT_FILE, font_size);
    auto text_space = 8;
    auto messageh = 0.25 * SCREEN_HEIGHT;

    auto infoh = 0.06 * SCREEN_HEIGHT;
    auto boxh = 0.125 * SCREEN_HEIGHT;
    auto box_space = 10;

    auto background = createImage("images/background.png");

    Character::Base saveCharacter;

    std::vector<Button> controls = std::vector<Button>();

    while (!quit)
    {
        player.StoryID = story->ID;

        // capture player state before running the story
        saveCharacter = player;

        auto run_once = true;

        SDL_Surface *splash = NULL;
        SDL_Texture *splashTexture = NULL;
        SDL_Surface *text = NULL;

        if (run_once)
        {
            run_once = false;

            auto jump = story->Background(player);

            if (jump >= 0)
            {
                story = (Story::Base *)findStory(jump);

                continue;
            }

            story->Event(player);
        }

        int splash_h = 250;

        if (story->Image)
        {
            splash = createImage(story->Image);

            if (splash)
            {
                if (splash->w != splashw)
                {
                    splash_h = (int)((double)splashw / splash->w * splash->h);
                }

                splashTexture = SDL_CreateTextureFromSurface(renderer, splash);
            }
        }

        if (story->Text)
        {
            auto textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space) - 2 * space;

            text = createText(story->Text, FONT_FILE, font_size, clrDB, textwidth, TTF_STYLE_NORMAL);
        }

        auto compact = text && text->h <= text_bounds - 2 * text_space;

        if (story->Controls == Story::Controls::STANDARD)
        {
            controls = Story::StandardControls(compact);
        }
        else if (story->Controls == Story::Controls::SHOP)
        {
            controls = Story::ShopControls(compact);
        }
        else if (story->Controls == Story::Controls::SELL)
        {
            controls = Story::SellControls(compact);
        }
        else if (story->Controls == Story::Controls::BUY_AND_SELL)
        {
            controls = Story::BuyAndSellControls(compact);
        }
        else if (story->Controls == Story::Controls::TRADE)
        {
            controls = Story::TradeControls(compact);
        }
        else
        {
            controls = Story::ExitControls();
        }

        std::string vehicle_message = "There is a " + std::string(Vehicle::Descriptions[story->Vehicle]) + " here. Do you want to take it?";
        auto vehicle_text = createText(vehicle_message.c_str(), FONT_FILE, font_size, clrWH, textwidth - 2 * text_space, TTF_STYLE_NORMAL);
        auto vehicle_trigger = false;
        auto vehicle_done = false;
        auto message_x = (SCREEN_WIDTH - textwidth) / 2;
        auto message_y = (SCREEN_HEIGHT - messageh) / 2;

        auto vehicle_controls = std::vector<Button>();
        vehicle_controls.push_back(Button(0, "icons/yes.png", 0, 1, 0, 0, message_x + button_space, message_y + messageh - buttonh - button_space, Control::Type::YES));
        vehicle_controls.push_back(Button(1, "icons/no.png", 0, 1, 1, 1, message_x + textwidth - button_space - buttonw, message_y + messageh - buttonh - button_space, Control::Type::NO));

        // Render the image
        if (window && renderer)
        {
            auto scrollSpeed = 20;
            auto hold = false;

            auto selected = false;
            auto current = -1;
            auto offset = 0;

            while (!quit)
            {
                if (story->Title)
                {
                    SDL_SetWindowTitle(window, story->Title);
                }
                else
                {
                    SDL_SetWindowTitle(window, (std::string("Heart of Ice: ") + std::to_string(story->ID)).c_str());
                }

                fillWindow(renderer, intWH);

                //Fill the surface with background
                stretchImage(renderer, background, 0, 0, SCREEN_WIDTH, buttony - button_space);

                if (splash)
                {
                    splash_h = fitImage(renderer, splash, startx, texty, splashw, text_bounds);
                }

                if (!splash || (splash && splash_h < (text_bounds - (boxh + infoh))))
                {
                    putText(renderer, "Life", font, text_space, clrWH, (player.Life > 0 && story->Type != Story::Type::DOOM) ? intDB : intRD, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (boxh + infoh));
                    putText(renderer, (std::to_string(player.Life)).c_str(), font, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - boxh);
                }

                if (!splash || (splash && splash_h < text_bounds - (2 * (boxh + infoh) + box_space)))
                {
                    putText(renderer, "Money", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * (boxh + infoh) + box_space));
                    putText(renderer, (std::to_string(player.Money) + std::string(" scads")).c_str(), font, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - (2 * boxh + infoh + box_space));
                }

                fillRect(renderer, textwidth, text_bounds, textx, texty, intBE);

                if (story->Text)
                {
                    renderText(renderer, text, intBE, textx + space, texty + space, text_bounds - 2 * space, offset);
                }

                if (flash_message)
                {
                    if ((SDL_GetTicks() - start_ticks) < duration)
                    {
                        putText(renderer, message, font, text_space, clrWH, flash_color, TTF_STYLE_NORMAL, splashw, messageh, startx, starty);
                    }
                    else
                    {
                        flash_message = false;
                    }
                }

                if (!compact)
                {
                    fillRect(renderer, controls[0].W + 2 * border_space, controls[0].H + 2 * border_space, controls[0].X - border_space, controls[0].Y - border_space, intWH);
                    fillRect(renderer, controls[1].W + 2 * border_space, controls[1].H + 2 * border_space, controls[1].X - border_space, controls[1].Y - border_space, intWH);
                }

                bool scrollUp = false;
                bool scrollDown = false;

                renderButtons(renderer, controls, vehicle_trigger ? -1 : current, intGR, border_space, border_pts);

                if (vehicle_trigger)
                {
                    fillRect(renderer, textwidth, messageh, message_x, message_y, intLB);

                    renderImage(renderer, vehicle_text, (SCREEN_WIDTH - vehicle_text->w) / 2, message_y + text_space);

                    renderButtons(renderer, vehicle_controls, current, intWH, border_space, border_pts);
                }

                if (splash)
                {
                    auto mousex = 0;
                    auto mousey = 0;

                    auto state = SDL_GetMouseState(&mousex, &mousey);

                    auto zoomw = (int)(0.80 * (double)textwidth);
                    auto zoomh = (int)(0.80 * (double)text_bounds);

                    clipValue(zoomw, 0, splash->w);
                    clipValue(zoomh, 0, splash->h);

                    auto boundx = splashw;

                    if (splash_h == text_bounds)
                    {
                        boundx = (int)((double)splash_h / splash->h * (double)splash->w);
                    }

                    if (mousex >= startx && mousex <= (startx + boundx) && mousey >= starty && mousey <= (starty + splash_h))
                    {
                        auto scalex = (double)(mousex - startx) / boundx;
                        auto scaley = (double)(mousey - starty) / splash_h;

                        int centerx = (int)(scalex * (double)splash->w);
                        int centery = (int)(scaley * (double)splash->h);

                        clipValue(centerx, zoomw / 2, splash->w - zoomw / 2);
                        clipValue(centery, zoomh / 2, splash->h - zoomh / 2);

                        if (splashTexture)
                        {
                            SDL_Rect src;

                            src.w = zoomw;
                            src.h = zoomh;
                            src.x = centerx - zoomw / 2;
                            src.y = centery - zoomh / 2;

                            SDL_Rect dst;

                            dst.w = zoomw;
                            dst.h = zoomh;
                            dst.x = (textx + (textwidth - zoomw) / 2);
                            dst.y = (texty + (text_bounds - zoomh) / 2);

                            SDL_RenderCopy(renderer, splashTexture, &src, &dst);
                            drawRect(renderer, dst.w, dst.h, dst.x, dst.y, intBK);
                        }
                    }
                }

                if (vehicle_trigger)
                {
                    quit = Input::GetInput(renderer, vehicle_controls, current, selected, scrollUp, scrollDown, hold);
                }
                else
                {
                    quit = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);
                }

                if (!vehicle_trigger && ((selected && current >= 0 && current < controls.size()) || scrollUp || scrollDown || hold))
                {
                    if (controls[current].Type == Control::Type::SCROLL_UP || (controls[current].Type == Control::Type::SCROLL_UP && hold) || scrollUp)
                    {
                        if (offset > 0)
                        {
                            offset -= scrollSpeed;
                        }

                        if (offset < 0)
                        {
                            offset = 0;
                        }
                    }
                    else if (controls[current].Type == Control::Type::SCROLL_DOWN || ((controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown))
                    {
                        if (text->h >= text_bounds - 2 * space)
                        {
                            if (offset < text->h - text_bounds + 2 * space)
                            {
                                offset += scrollSpeed;
                            }

                            if (offset > text->h - text_bounds + 2 * space)
                            {
                                offset = text->h - text_bounds + 2 * space;
                            }
                        }
                    }
                    else if (controls[current].Type == Control::Type::CHARACTER && !hold)
                    {
                        if (story->Type == Story::Type::NORMAL && player.Life > 0)
                        {
                            characterScreen(window, renderer, player, story);
                        }
                        else
                        {
                            message = "This adventure is over.";

                            start_ticks = SDL_GetTicks();

                            flash_message = true;

                            flash_color = intRD;
                        }

                        current = -1;

                        selected = false;
                    }
                    else if (controls[current].Type == Control::Type::MAP && !hold)
                    {
                        mapScreen(window, renderer);

                        current = -1;

                        selected = false;
                    }
                    else if (controls[current].Type == Control::Type::USE && !hold)
                    {
                        if (story->Type == Story::Type::NORMAL && player.Life > 0)
                        {
                            inventoryScreen(window, renderer, player, story, player.Items, Control::Type::USE, 0);
                        }
                        else
                        {
                            message = "This adventure is over.";

                            start_ticks = SDL_GetTicks();

                            flash_message = true;

                            flash_color = intRD;
                        }

                        current = -1;

                        selected = false;
                    }
                    else if ((controls[current].Type == Control::Type::SHOP || controls[current].Type == Control::Type::SELL) && !hold)
                    {
                        if (story->Type == Story::Type::NORMAL && player.Life > 0)
                        {
                            if (controls[current].Type == Control::Type::SHOP)
                            {
                                shopScreen(window, renderer, player, story, Control::Type::BUY);
                            }
                            else
                            {
                                shopScreen(window, renderer, player, story, Control::Type::SELL);
                            }
                        }
                        else
                        {
                            message = "This adventure is over.";

                            start_ticks = SDL_GetTicks();

                            flash_message = true;

                            flash_color = intRD;
                        }

                        current = -1;

                        selected = false;
                    }
                    else if (controls[current].Type == Control::Type::TRADE && !hold)
                    {
                        if (story->Type == Story::Type::NORMAL && player.Life > 0)
                        {
                            tradeScreen(window, renderer, player, story->Trade.first, story->Trade.second);
                        }
                        else
                        {
                            message = "This adventure is over.";

                            start_ticks = SDL_GetTicks();

                            flash_message = true;

                            flash_color = intRD;
                        }

                        current = -1;

                        selected = false;
                    }
                    else if (controls[current].Type == Control::Type::GAME && !hold)
                    {
                        if (story->Type == Story::Type::NORMAL && player.Life > 0)
                        {
                            auto result = gameScreen(window, renderer, saveCharacter, true);

                            if (result == Control::Type::SAVE)
                            {
                                message = "Game saved!";

                                start_ticks = SDL_GetTicks();

                                flash_message = true;

                                flash_color = intLB;
                            }
                            else if (result == Control::Type::LOAD)
                            {
                                if (saveCharacter.StoryID >= 0 && saveCharacter.Life > 0)
                                {
                                    player = saveCharacter;

                                    story = (Story::Base *)findStory(saveCharacter.StoryID);

                                    message = "Game loaded!";

                                    start_ticks = SDL_GetTicks();

                                    flash_message = true;

                                    flash_color = intLB;

                                    break;
                                }
                            }
                        }
                        else
                        {
                            message = "You cannot LOAD or SAVE your game at this time.";

                            start_ticks = SDL_GetTicks();

                            flash_message = true;

                            flash_color = intRD;
                        }

                        current = -1;

                        selected = false;
                    }
                    else if (controls[current].Type == Control::Type::NEXT && !hold)
                    {
                        if (story->Type == Story::Type::NORMAL)
                        {
                            if (story->Vehicle != Vehicle::Type::NONE && player.Vehicle != story->Vehicle)
                            {
                                if (!vehicle_done)
                                {
                                    vehicle_trigger = true;

                                    continue;
                                }
                            }

                            if (player.Life > 0)
                            {
                                if (story->LimitSkills > 0)
                                {
                                    auto done = loseSkills(window, renderer, player, story->LimitSkills);

                                    if (!done)
                                    {
                                        continue;
                                    }
                                    else
                                    {
                                        story->LimitSkills = 0;
                                    }
                                }

                                if (story->Take.size() > 0 && story->Limit > 0)
                                {
                                    auto done = takeScreen(window, renderer, player, story->Take, story->Limit);

                                    if (!done)
                                    {
                                        continue;
                                    }
                                    else
                                    {
                                        story->Limit = 0;
                                    }
                                }

                                if (story->Limit > 0 && story->ToLose.size() > story->Limit)
                                {
                                    while (story->ToLose.size() > story->Limit)
                                    {
                                        inventoryScreen(window, renderer, player, story, story->ToLose, Control::Type::LOSE, story->Limit);
                                    }
                                }

                                while (!Character::VERIFY_POSSESSIONS(player))
                                {
                                    inventoryScreen(window, renderer, player, story, player.Items, Control::Type::DROP, 0);
                                }

                                current = -1;

                                selected = false;

                                auto next = renderChoices(window, renderer, player, story);

                                if (next->ID != story->ID)
                                {
                                    if (story->Bye)
                                    {
                                        auto bye = createText(story->Bye, FONT_FILE, font_size + 4, clrBK, (SCREEN_WIDTH * (1.0 - 2.0 * Margin)) - 2 * text_space, TTF_STYLE_NORMAL);
                                        auto forward = createImage("icons/next.png");

                                        if (bye && forward)
                                        {
                                            fillWindow(renderer, intWH);

                                            fillRect(renderer, (1.0 - 2.0 * Margin) * SCREEN_WIDTH, bye->h + 2 * text_space, startx, (SCREEN_HEIGHT - (bye->h + 2 * text_space)) / 2, intBE);
                                            renderText(renderer, bye, intBE, (SCREEN_WIDTH - bye->w) / 2, (SCREEN_HEIGHT - bye->h) / 2, SCREEN_HEIGHT, 0);

                                            renderImage(renderer, forward, SCREEN_WIDTH * (1.0 - Margin) - buttonw - button_space, buttony);

                                            SDL_RenderPresent(renderer);

                                            Input::WaitForNext(renderer);

                                            SDL_FreeSurface(bye);

                                            bye = NULL;

                                            SDL_FreeSurface(forward);

                                            forward = NULL;
                                        }
                                    }

                                    story = next;

                                    break;
                                }
                            }
                            else
                            {
                                message = "This adventure is over. You have died.";

                                start_ticks = SDL_GetTicks();

                                flash_message = true;

                                flash_color = intRD;
                            }
                        }
                        else if (story->Type == Story::Type::DOOM)
                        {
                            message = "This adventure is over. You have failed.";

                            start_ticks = SDL_GetTicks();

                            flash_message = true;

                            flash_color = intRD;
                        }
                        else if (story->Type == Story::Type::GODHOOD)
                        {
                            message = "You are immortal and all-powerful now. Your adventure has culminated in godhood.";

                            start_ticks = SDL_GetTicks();

                            flash_message = true;

                            flash_color = intLB;
                        }
                        else if (story->Type == Story::Type::SACRIFICE)
                        {
                            message = "This adventure is over. You have sacrificed your lives for the sake of the whole world.";

                            start_ticks = SDL_GetTicks();

                            flash_message = true;

                            flash_color = intLB;
                        }
                        else if (story->Type == Story::Type::SENTINEL)
                        {
                            message = "This adventure is over. You have become an undying sentinel for all time.";

                            start_ticks = SDL_GetTicks();

                            flash_message = true;

                            flash_color = intLB;
                        }
                        else if (story->Type == Story::Type::UNKNOWN)
                        {
                            message = "You have rejected the chance for power. This adventure is over now. You don't know how long you'll live. But then, who does?";

                            start_ticks = SDL_GetTicks();

                            flash_message = true;

                            flash_color = intRD;
                        }
                        else if (story->Type == Story::Type::GOOD)
                        {
                            message = "You found the Heart of Volent and rejected the chance for power. Perhaps your destiny now is to save the world. But for now, this adventure is over.";

                            start_ticks = SDL_GetTicks();

                            flash_message = true;

                            flash_color = intLB;
                        }
                    }
                    else if (controls[current].Type == Control::Type::BACK && !hold)
                    {
                        quit = true;

                        break;
                    }
                }
                else if (vehicle_trigger && (selected && current >= 0 && current < vehicle_controls.size()))
                {
                    if (vehicle_controls[current].Type == Control::Type::YES && !hold)
                    {
                        player.Vehicle = story->Vehicle;

                        vehicle_trigger = false;

                        vehicle_message = "You commandeered the " + std::string(Vehicle::Descriptions[story->Vehicle]);

                        message = vehicle_message.c_str();

                        start_ticks = SDL_GetTicks();

                        flash_message = true;

                        flash_color = intLB;

                        vehicle_done = true;
                    }
                    else
                    {
                        vehicle_trigger = false;

                        vehicle_done = true;
                    }
                }
            }
        }

        if (splash)
        {
            SDL_FreeSurface(splash);

            splash = NULL;
        }

        if (splashTexture)
        {
            SDL_DestroyTexture(splashTexture);

            splashTexture = NULL;
        }

        if (text)
        {
            SDL_FreeSurface(text);

            text = NULL;
        }
    }

    if (background)
    {
        SDL_FreeSurface(background);

        background = NULL;
    }

    if (font)
    {
        TTF_CloseFont(font);

        font = NULL;
    }

    TTF_Quit();

    return quit;
}

bool storyScreen(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, int id)
{
    auto story = (Story::Base *)findStory(id);

    return processStory(window, renderer, player, story);
}

bool mainScreen(SDL_Window *window, SDL_Renderer *renderer, int storyID)
{
    auto font_size = 20;

    auto *introduction = "It is the dawn of the 24th century and the Earth is dying. Gaia, a satellite array designed to repair the effects of global warming, is out of control. A new Ice Age tightens its inexorable grip on the planet. Humanity is on the edge of extinction.\n\nMankind's last hope is the Heart of Volent, a strange meteorite which has already brought mutation and chaos to the world. Legend says that the one who finds it shall wield the power to reshape the universe.\n\nAs the ice sheets spread and the world slides towards the brink, you and a handful of bitter rivals compete in the race for the Heart. Only one can win. Are you ruthless and resourceful enough to claim the ultimate price?";

    auto splash = createImage("images/heart-cover.png");

    auto text = createText(introduction, FONT_FILE, font_size, clrWH, SCREEN_WIDTH * (1.0 - 3.0 * Margin) - splashw);

    auto title = "Heart of Ice";

    InitializeStories();

    Character::Base Player;

    // Render window
    if (window && renderer && splash && text)
    {
        SDL_SetWindowTitle(window, title);

        const char *choices[4] = {"New Game", "Load Game", "About", "Exit"};

        auto current = -1;

        auto selected = false;

        auto main_buttonh = 48;

        auto controls = createHTextButtons(choices, 4, main_buttonh, startx, SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh);

        controls[0].Type = Control::Type::NEW;
        controls[1].Type = Control::Type::LOAD;
        controls[2].Type = Control::Type::ABOUT;
        controls[3].Type = Control::Type::QUIT;

        auto done = false;

        while (!done)
        {
            // Fill the surface with background
            fillWindow(renderer, intLB);

            fitImage(renderer, splash, startx, starty, splashw, text_bounds);
            renderText(renderer, text, intLB, startx * 2 + splashw, starty, SCREEN_HEIGHT * (1.0 - 2 * Margin), 0);
            renderTextButtons(renderer, controls, FONT_FILE, current, clrWH, intBK, intRD, font_size, TTF_STYLE_NORMAL);

            bool scrollUp = false;
            bool scrollDown = false;
            bool hold = false;

            Control::Type result;

            done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if (selected && current >= 0 && current < controls.size())
            {
                switch (controls[current].Type)
                {
                case Control::Type::NEW:

                    Player = selectCharacter(window, renderer);

                    if (Player.StoryID != -1)
                    {
                        storyScreen(window, renderer, Player, storyID);
                    }

                    current = -1;

                    selected = false;

                    storyID = 2300;

                    break;

                case Control::Type::ABOUT:

                    aboutScreen(window, renderer);

                    current = -1;

                    selected = false;

                    break;

                case Control::Type::LOAD:

                    result = gameScreen(window, renderer, Player, false);

                    if (result == Control::Type::LOAD)
                    {
                        // TODO: add sanity check to the loaded player
                        if (Player.StoryID >= 0 && Player.Life > 0)
                        {
                            storyScreen(window, renderer, Player, Player.StoryID);
                        }
                    }

                    current = -1;

                    selected = false;

                    break;

                case Control::Type::QUIT:

                    done = true;

                    break;

                default:

                    selected = false;

                    done = false;

                    break;
                }
            }

            SDL_SetWindowTitle(window, title);
        }

        SDL_FreeSurface(splash);
        SDL_FreeSurface(text);

        splash = NULL;
        text = NULL;
    }

    return false;
}

int main(int argc, char **argv)
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    auto title = "Heart of Ice";

    createWindow(SDL_INIT_VIDEO, &window, &renderer, title, "icons/snowflakes.png");

    auto numGamePads = Input::InitializeGamePads();

    auto quit = false;

    auto storyID = 2300;

    if (argc > 1)
    {
        storyID = std::atoi(argv[1]);
    }

    if (window)
    {
        quit = mainScreen(window, renderer, storyID);

        // Destroy window and renderer
        SDL_DestroyRenderer(renderer);

        renderer = NULL;

        SDL_DestroyWindow(window);

        window = NULL;
    }

    // Quit SDL subsystems
    IMG_Quit();

    SDL_Quit();

    return 0;
}
