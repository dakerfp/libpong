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

#ifndef __LIBPONG_H__
#define __LIBPONG_H__
#if defined (__cplusplus)
extern "C" {
#endif

#define PONG_VERSION_MAJOR 1
#define PONG_VERSION_MINOR 0
#define PONG_VERSION ((PONG_VERSION_MAJOR << 16) | PONG_VERSION_MINOR)

#ifndef PONG_API
#  ifdef _WIN32
#     if defined(PONG_BUILD_SHARED) /* build dll */
#       define PONG_API __declspec(dllexport)
#     elif !defined(PONG_BUILD_STATIC) /* use dll */
#       define PONG_API __declspec(dllimport)
#     else /* static library */
#       define PONG_API
#     endif
#  else
#     if __GNUC__ >= 4
#       define PONG_API __attribute__((visibility("default")))
#     else
#       define PONG_API
#     endif
#  endif
#endif

unsigned int pong_get_version(void);
int pong_is_compatible_dll(void);

struct pong_point {
    int x, y;
};

struct pong_size {
    unsigned int width, height;
};

struct pong_player {
    struct pong_point pos;
    unsigned int size, score;
};

struct pong_ball {
    struct pong_point pos, vel, init;
};

struct pong_game {
    struct pong_player players[2];
    struct pong_size grid;
    struct pong_ball ball;
    unsigned int target_score;
};

PONG_API int pong_game_tick(struct pong_game *g, int cmd[2]);

#if defined (__cplusplus)
}
#endif
#endif
