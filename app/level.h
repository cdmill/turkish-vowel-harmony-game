
#ifndef LEVEL_H
#define LEVEL_H
#include <QPainter>
#include <Box2D/Box2D.h>
#include <iostream>
#include <map>
#include "player.h"

using std::map;
using std::vector;


class Level
{
public:
    Level(b2World* w, Player* p);
    virtual void landscape(){};
    virtual void paintLevelBackground(QPainter* painter){};
    virtual void paintLevelGround(QPainter* painter){};

protected:
    b2World* world;
    b2Body* createBgAnimation(float speed);
    Player* player;
    void wrapping(b2Body *animation);

};

#endif // LEVEL_H
