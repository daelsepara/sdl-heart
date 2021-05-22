#ifndef __CONFIG__HPP__
#define __CONFIG__HPP__

// Future re-scalable, HD mode
class Config
{
public:
    int SCREEN_WIDTH = 1024;
    int SCREEN_HEIGHT = 768;

    double Margin = 0.05;

    int splashw = 0.30 * (SCREEN_WIDTH * (1.0 - 3.0 * Margin));
    int startx = (SCREEN_WIDTH * Margin);
    int starty = (SCREEN_HEIGHT * Margin);
    int textx = startx * 2 + splashw;
    int texty = starty;
    int buttonw = 64;
    int buttonh = 64;
    int buttony = (int)(SCREEN_HEIGHT * (1.0 - Margin) - buttonh);

    int button_space = 20;
    int gridsize = buttonw + button_space;
    int border_space = 8;
    int border_pts = 4;
    int arrow_size = 32;
    int text_bounds = SCREEN_HEIGHT * (1.0 - Margin * 2.0) - buttonh - button_space * 2;
    int textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space);

    int headerw;
    int profilew;
    
    void ComputeBounds()
    {
        startx = (SCREEN_WIDTH * Margin);
        starty = (SCREEN_HEIGHT * Margin);

        textx = startx * 2 + splashw;
        texty = starty;

        textwidth = ((1.0 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space);
        text_bounds = SCREEN_HEIGHT * (1.0 - Margin * 2.0) - buttonh - button_space * 2;

        buttony = (int)(SCREEN_HEIGHT * (1.0 - Margin) - buttonh);

        gridsize = buttonw + button_space;

        profilew = SCREEN_WIDTH * (1.0 - 2.0 * Margin);

        headerw = 0.6 * splashw;
    }

    Config()
    {
        SCREEN_WIDTH = 800;
        SCREEN_HEIGHT = 600;

        splashw = 250;

        ComputeBounds();
    }

    Config(int width, int height)
    {
        SCREEN_WIDTH = width;
        SCREEN_HEIGHT = height;

        splashw = 0.30 * (1.0 - 3.0 * Margin) * SCREEN_WIDTH;

        ComputeBounds();
    }
};

#endif
