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

#include "pong.h"

unsigned int pong_get_version(void) {
    return PONG_VERSION;
}

int pong_is_compatible_dll(void) {
    unsigned int major = pong_get_version() >> 16;
    return major == PONG_VERSION_MAJOR;
}

static void ball_hits_player(struct pong_ball *b, const struct pong_player *p) {
    if (b->pos.x != p->pos.x)
        return;
    if (b->pos.y < p->pos.y)
        return;
    if (b->pos.y > p->pos.y + (int)(p->size))
        return;
    b->vel.x = -b->vel.x;
    b->pos.x += b->vel.x;
    b->pos.y += b->vel.y;
}

static int ball_hits_wall(struct pong_ball *b, const struct pong_size sz) {
    if (b->pos.y < 0) {
        b->pos.y = 0;
        b->vel.y = -b->vel.y;
    } else if (b->pos.y >= (int)(sz.height)) {
        b->pos.y = (int)sz.height;
        b->vel.y = -b->vel.y;
    }
    if (b->pos.x <= 0) {
        b->pos.x = 0;
        b->vel.x = -b->vel.x;
        return -1;
    } else if (b->pos.x >= (int)(sz.width)) {
        b->pos.x = (int)sz.width;
        b->vel.x = -b->vel.x;
        return 1;
    }
    return 0;
}

static void player_move(struct pong_player *p, int dy, unsigned int height) {
    p->pos.y += dy;
    if (p->pos.y < 0)
        p->pos.y = 0;
    else if (p->pos.y + (int)p->size > (int)height)
        p->pos.y = (int)height - (int)p->size;
}

static void ball_move(struct pong_ball *b) {
    b->pos.x += b->vel.x;
    b->pos.y += b->vel.y;
}

int pong_game_tick(struct pong_game *g, int cmd[2]) {
    player_move(&g->players[0], cmd[0], g->grid.height);
    player_move(&g->players[1], cmd[1], g->grid.height);
    ball_move(&g->ball);
    ball_hits_player(&g->ball, &g->players[0]);
    ball_hits_player(&g->ball, &g->players[1]);
    switch (ball_hits_wall(&g->ball, g->grid)) {
    case -1:
        g->players[1].score++;
        g->ball.pos = g->ball.init;
        break;
    case 1:
        g->players[0].score++;
        g->ball.pos = g->ball.init;
        break;
    default:
        return 0;
    }
    if (g->players[0].score >= g->target_score)
        return -1;
    if (g->players[1].score >= g->target_score)
        return 1;
    return 0;
}
