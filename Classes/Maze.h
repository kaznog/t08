//
//  Maze.h
//  t08
//
//  Created by 野口一也 on 2014/06/25.
//
//

#ifndef __t08__Maze__
#define __t08__Maze__

#include <iostream>
#include <vector>
#include "cocos2d.h"
#include "Constants.h"

USING_NS_CC;

class Maze : public Ref {
public:
    Maze();
    ~Maze();
    
    bool init();
    void make();
    void getroom();
    Vec2 getpos();
    int map0[MAP_TILE_HEIGHT][MAP_TILE_WIDTH];
    int map1[MAP_TILE_HEIGHT][MAP_TILE_WIDTH];
    int map2[MAP_AREA_HEIGHT+2][MAP_AREA_WIDTH+2];
    int rw[MAP_AREA_HEIGHT+1][MAP_AREA_WIDTH+1];
    int rh[MAP_AREA_HEIGHT+1][MAP_AREA_WIDTH+1];
    int rx[MAP_AREA_HEIGHT+1][MAP_AREA_WIDTH+1];
    int ry[MAP_AREA_HEIGHT+1][MAP_AREA_WIDTH+1];
    int rmx;
    int rmy;
    int startx;
    int starty;
private:
    int num0;
    void areainc();
    void areaincsub(int x, int y);
    void areaincskip();
    int sign(int signval);
    void makeroad(int x1, int y1, int x2, int y2);
};

#endif /* defined(__t08__Maze__) */
