//
//  Maze.cpp
//  t08
//
//  Created by 野口一也 on 2014/06/25.
//
//

#include "Maze.h"

Maze::Maze()
{
    
}

Maze::~Maze()
{
}

bool Maze::init()
{
    return true;
}

void Maze::make()
{
    srand((unsigned)time(NULL));
    for (int sy = 0; sy < MAP_TILE_HEIGHT; sy++) {
        for (int sx = 0; sx < MAP_TILE_WIDTH; sx++) {
            map0[sy][sx] = 0;
            map1[sy][sx] = 1;
        }
    }
    for (int sy = 0; sy <= MAP_AREA_HEIGHT+1; sy++) {
        for (int sx = 0; sx <= MAP_AREA_WIDTH+1; sx++) {
            map2[sy][sx] = 0;
        }
    }
    
    startx = (int)(CCRANDOM_0_1() * MAP_AREA_WIDTH);
    starty = (int)(CCRANDOM_0_1() * MAP_AREA_HEIGHT);
    map2[starty][startx] = 1;
    
    float rs = CCRANDOM_0_1() * (MAP_AREA_WIDTH * MAP_AREA_HEIGHT / 4);
    int r = MAP_AREA_WIDTH * MAP_AREA_HEIGHT / 4;
    r += (int)rs;
    r -= 2;
    
    for (int i = 0; i < r; i++) {
        areainc();
    }
    
    for (int sy = 0; sy <= MAP_AREA_HEIGHT; sy++) {
        for (int sx = 0; sx <= MAP_AREA_WIDTH; sx++) {
            if (map2[sy][sx] == 1) {
                int t = 0;
//                int t = (int)CCRANDOM_0_1();
                int w = CCRANDOM_0_1() * (MAP_BLOCK_SIZE - 1);
                rw[sy][sx] = w;
                int h = CCRANDOM_0_1() * (MAP_BLOCK_SIZE - 1);
                rh[sy][sx] = h;
                int x = CCRANDOM_0_1() * (MAP_BLOCK_SIZE - 1 - w);
                rx[sy][sx] = x + sx * MAP_BLOCK_SIZE + 4;
                int y = CCRANDOM_0_1() * (MAP_BLOCK_SIZE - 1 - h);
                ry[sy][sx] = y + sy * MAP_BLOCK_SIZE + 4;
                if ((w < 3) || (h < 3)) {
                    t = 0;
                }
                for (int ssy = 0; ssy <= (y+h); ssy++) {
                    for (int ssx = 0; ssx <= (x+w); ssx++) {
                        int xx = (sx * MAP_BLOCK_SIZE) + ssx + 4;
                        int yy = (sy * MAP_BLOCK_SIZE) + ssy + 4;
                        if ((t == 1) && (((xx % 2) == 1) && (yy % 2) == 1)) {
                            map1[yy][xx] = 1;
                        } else {
                            map1[yy][xx] = 0;
                        }
                    }
                }
            }
        }
    }
    
    for (int sy = 0; sy <= MAP_AREA_HEIGHT; sy++) {
        for (int sx = 0; sx <= MAP_AREA_WIDTH; sx++) {
            if (map2[sy][sx] == 1) {
                int x1 = rx[sy][sx] + (int)(CCRANDOM_0_1() * rw[sy][sx]);
                int y1 = ry[sy][sx] + (int)(CCRANDOM_0_1() * rh[sy][sx]);
                if (map2[sy][sx + 1] == 1) {
                    int x2 = rx[sy][sx + 1] + (int)(CCRANDOM_0_1() * rw[sy][sx + 1]);
                    int y2 = ry[sy][sx + 1] + (int)(CCRANDOM_0_1() * rh[sy][sx + 1]);
                    makeroad(x1, y1, x2, y2);
                }
                if (map2[sy + 1][sx] == 1) {
                    int x2 = rx[sy + 1][sx] + (int)(CCRANDOM_0_1() * rw[sy + 1][sx]);
                    int y2 = ry[sy + 1][sx] + (int)(CCRANDOM_0_1() * rh[sy + 1][sx]);
                    makeroad(x1, y1, x2, y2);
                }
            }
        }
    }
}

void Maze::areainc()
{
    num0 = 0;
    for (int sy = 0; sy <= MAP_AREA_HEIGHT; sy++) {
        for (int sx = 0; sx <= MAP_AREA_WIDTH; sx++) {
            if (map2[sy][sx] == 1) {
                areaincsub(sx - 1, sy);
                areaincsub(sx, sy - 1);
                areaincsub(sx + 1, sy);
                areaincsub(sx, sy + 1);
            }
        }
    }
    int num1 = (int)(CCRANDOM_0_1() * num0);
    num0 = 0;
    for (int y = 0; y <= MAP_AREA_HEIGHT; y++) {
        for (int x = 0; x <= MAP_AREA_WIDTH; x++) {
            if (map2[y][x] == 2) {
                if (num0 == num1) {
                    map2[y][x] = 1;
                    areaincskip();
                    return;
                }
                num0 += 1;
            }
        }
    }
    areaincskip();
}

void Maze::areaincskip()
{
    for (int y = 0; y <= MAP_AREA_HEIGHT; y++) {
        for (int x = 0; x <= MAP_AREA_WIDTH; x++) {
            if (map2[y][x] == 2) {
                map2[y][x] = 0;
            }
        }
    }
}

void Maze::areaincsub(int x, int y)
{
    if ((x < 0) || (y < 0) || (x > MAP_AREA_WIDTH) || (y > MAP_AREA_HEIGHT)) {
        return;
    }
    if (map2[y][x] == 0) {
        map2[y][x] = 2;
        num0 += 1;
    }
}

int Maze::sign(int signval)
{
    if (signval == 0) {
        return 0;
    } else if (signval > 0) {
        return 1;
    } else {
        return -1;
    }
}

void Maze::makeroad(int x1, int y1, int x2, int y2)
{
    int x = x1;
    int y = y1;
    int dx = sign(x2 - x1);
    int dy = sign(y2 - y1);
    if (((int)(CCRANDOM_0_1() * 10)) < 5) {
        do {
            map1[y][x] = 0;
            x += dx;
        } while(x != x2);
        do {
            map1[y][x] = 0;
            y += dy;
        } while(y != y2);
    } else {
        do {
            map1[y][x] = 0;
            y += dy;
        } while(y != y2);
        do {
            map1[y][x] = 0;
            x += dx;
        } while(x != x2);
    }
    map1[y1][x1] = 0;
    map1[y2][x2] = 0;
}

void Maze::getroom()
{
    num0 = 0;
    for (int sy = 0; sy <= MAP_AREA_HEIGHT; sy++) {
        for (int sx = 0; sx <= MAP_AREA_WIDTH; sx++) {
            if (map2[sy][sx] == 1) {
                num0 += 1;
            }
        }
    }
    int num1 = (int)(CCRANDOM_0_1() * num0);
    num0 = 0;
    for (int sy = 0; sy <= MAP_AREA_HEIGHT; sy++) {
        for (int sx = 0; sx <= MAP_AREA_WIDTH; sx++) {
            if (map2[sy][sx] == 1) {
                if (num0 == num1) {
                    rmx = sx;
                    rmy = sy;
                    return;
                }
                num0 += 1;
            }
        }
    }
}

Vec2 Maze::getpos()
{
    num0 = 0;
    int x = rx[rmy][rmx];
    int y = ry[rmy][rmx];
    for (int sy = 0; sy <= rh[rmy][rmx]; sy++) {
        for (int sx = 0; sx <= rw[rmy][rmx]; sx++) {
            if ((map1[sy + y][sx + x] == 0) && (map0[sy + y][sx + x] == 0)) {
                num0 += 1;
            }
        }
    }
    int num1 = (int)(CCRANDOM_0_1() * num0);
    num0 = 0;
    for (int sy = 0; sy <= rh[rmy][rmx]; sy++) {
        for (int sx = 0; sx <= rw[rmy][rmx]; sx++) {
            if ((map1[sy + y][sx + x] == 0) && map0[sy + y][sx + x] == 0) {
                if (num0 == num1) {
                    x += sx;
                    y += sy;
                    return Vec2(x, y);
                }
                num0 += 1;
            }
        }
    }
}