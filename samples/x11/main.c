// The MIT License (MIT)

// Copyright (c) 2016 Patrick José Pereira

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

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pong.h>

#define WIDTH 3

#define ROWS 200
#define COLS 400

// https://en.wikibooks.org/wiki/C_Programming/C_Reference/stdlib.h/itoa
/* reverse:  reverse string s in place */
void reverse(char s[]) {
    unsigned int i, j;
    char c;

    for (i = 0, j = (unsigned int)strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

// https://en.wikibooks.org/wiki/C_Programming/C_Reference/stdlib.h/itoa
/* itoa:  convert positive n to characters in s */
void itoa(unsigned int n, char s[], unsigned int sn) {
    unsigned int i;

    i = 0;
    do {       /* generate digits in reverse order */
        s[i++] = (char)(n % 10 + '0');   /* get next digit */
    } while ((n /= 10) > 0 && i < sn);     /* delete it */
    s[i] = '\0';
    reverse(s);
}

struct pong_game g = {
    .grid = {.width = (unsigned)COLS, .height = (unsigned)ROWS},
    .players = {
        {
            .score = 0,
            .size = 60,
            .pos = {.x = 5, .y = (ROWS - 60) / 2}
        },
        {
            .score = 0,
            .size = 60,
            .pos = {.x = COLS - 5, .y = (ROWS - 60) / 2}
        }
    },
    .ball = {
        .pos = {.x = 5, .y = 5},
        .vel = {.x = 1, .y = 1},
        .init = {.x = COLS / 2, .y = ROWS / 2}
    },
    .target_score = 9
};

int main(void) {
    Display *d;
    Window w;
    XEvent e;
    int s;

    d = XOpenDisplay(NULL);
    if (d == NULL) {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }

    s = DefaultScreen(d);
    w = XCreateSimpleWindow(d, RootWindow(d, s), 0, 0, COLS + WIDTH, ROWS + WIDTH, 1,
                            BlackPixel(d, s), WhitePixel(d, s));
    XSelectInput(d, w, KeyPressMask);
    XMapWindow(d, w);

    int cmd[2];

    do {

        cmd[0] = 0;
        cmd[1] = 0;

        while(XPending(d) != 0) {
            XNextEvent(d, &e);
            if (e.type == KeyPress) {
                KeySym k = XLookupKeysym(&e.xkey, 0);
                switch (k) {
                case XK_w:
                    cmd[0] = -1;
                    break;
                case XK_s:
                    cmd[0] = 1;
                    break;
                case XK_i:
                    cmd[1] = -1;
                    break;
                case XK_k:
                    cmd[1] = 1;
                    break;
                case XK_q:
                    XCloseDisplay(d);
                    return 0;
                }
            }
        }

        /* Refresh screen */
        XClearWindow(d, w);

        // Score
        char msg[3];
        itoa(g.players[0].score, msg, sizeof(msg));
        XDrawString(d, w, DefaultGC(d, s), COLS / 4, 20, msg, (int)strlen(msg));
        itoa(g.players[1].score, msg, sizeof(msg));
        XDrawString(d, w, DefaultGC(d, s), 3 * COLS / 4, 20, msg, (int)strlen(msg));

        // Division
        for(int i = 0; i < ROWS / 10; i++)
            XFillRectangle(d, w, DefaultGC(d, s), COLS / 2, i*10 + 5, 1, 1);

        // Ball and Players
        XFillRectangle(d, w, DefaultGC(d, s), g.ball.pos.x, g.ball.pos.y, WIDTH, WIDTH);
        XFillRectangle(d, w, DefaultGC(d, s), g.players[0].pos.x-WIDTH, g.players[0].pos.y, WIDTH, g.players[0].size);
        XFillRectangle(d, w, DefaultGC(d, s), g.players[1].pos.x+WIDTH, g.players[1].pos.y, WIDTH, g.players[1].size);

        usleep(20000);
    } while (pong_game_tick(&g, cmd) == 0);

    XCloseDisplay(d);

    return 0;
}
