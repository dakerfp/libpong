// The MIT License (MIT)

// Copyright (c) 2016 Daker Fernandes Pinheiro

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <pong.h>
#include <ncurses.h>
#include <unistd.h>

const char *NUMBERS[10][5] = {
    {
        "XXXX",
        "X  X",
        "X  X",
        "X  X",
        "XXXX"
    },
    {
        "   X",
        "   X",
        "   X",
        "   X",
        "   X"
    },
    {
        "XXXX",
        "   X",
        "XXXX",
        "X   ",
        "XXXX"
    },
    {
        "XXXX",
        "   X",
        "XXXX",
        "   X",
        "XXXX"
    },
    {
        "X  X",
        "X  X",
        "XXXX",
        "   X",
        "   X"
    },
    {
        "XXXX",
        "X   ",
        "XXXX",
        "   X",
        "XXXX"
    },
    {
        "XXXX",
        "X   ",
        "XXXX",
        "X  X",
        "XXXX"
    },
    {
        "XXXX",
        "   X",
        "   X",
        "   X",
        "   X"
    },
    {
        "XXXX",
        "X  X",
        "XXXX",
        "X  X",
        "XXXX"
    },
    {
        "XXXX",
        "X  X",
        "XXXX",
        "   X",
        "XXXX"
    }
};

void draw_vline(const struct pong_size size) {
    int x = (int) size.width / 2;
    int y;
    for (y = 0; y <= (int)size.height; ++y)
        mvaddch(y, x, ACS_VLINE);
}

void draw_score(unsigned score, int x, int y) {
    int i, j;
    for (i = 0; i < 5; ++i)
        for (j = 0; j < 5; ++j)
            if (NUMBERS[score][i][j] == 'X')
                mvaddch(y + i, x + j, ACS_CKBOARD);
}

void draw_player(const struct pong_player *p) {
    int x = p->pos.x;
    int to = p->pos.y + (int)p->size;
    int y;
    for (y = p->pos.y; y < to; ++y)
        mvaddch(y, x, ACS_CKBOARD);
};

void draw_ball(const struct pong_ball *b) {
    mvaddch(b->pos.y, b->pos.x, ACS_DIAMOND);
}

int main(void) {
    initscr();
    cbreak();
    raw();
    curs_set(0);
    timeout(1);

    struct pong_game g = {
        .grid = {.width = (unsigned)COLS, .height = (unsigned)LINES},
        .players = {
            {
                .score = 0,
                .size = 10,
                .pos = {.x = 0, .y = (LINES - 10) / 2}
            },
            {
                .score = 0,
                .size = 10,
                .pos = {.x = COLS - 1, .y = (LINES - 10) / 2}
            }
        },
        .ball = {
            .pos = {.x = 5, .y = 5},
            .vel = {.x = 1, .y = 1},
            .init = {.x = COLS/2, .y = LINES/2}
        },
        .target_score = 9
    };

    printf("st\n");
    int cmd[2] = {0, 0};
    do {
        // read command
        cmd[0] = 0;
        cmd[1] = 0;
        switch (getch()) {
        case 'w':
            cmd[0] = -1;
            break;
        case 's':
            cmd[0] = 1;
            break;
        case 'i':
            cmd[1] = -1;
            break;
        case 'k':
            cmd[1] = 1;
            break;
        case 'q':
            return endwin();
        }
        // draw
        clear();
        draw_vline(g.grid);
        draw_score(g.players[0].score, 20, 5);
        draw_score(g.players[1].score, COLS - 25, 5);
        draw_player(&g.players[0]);
        draw_player(&g.players[1]);
        draw_ball(&g.ball);
        refresh();
        // wait
        usleep(50000);
    } while (pong_game_tick(&g, cmd) == 0);

    return endwin();
}

