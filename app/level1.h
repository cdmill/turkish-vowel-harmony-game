
#ifndef LEVEL1_H
#define LEVEL1_H

#include "level.h"
#include <QImage>
#include <QPainter>
#include <Box2D/Box2D.h>
#include <iostream>
#include <map>
#include "player.h"

using std::map;
using std::vector;



class Level1 : public Level
{
private:
    QImage bgLayer1;
    QImage bgLayer2;
    QImage bgLayer3;
    QImage bgLayer4;
    QImage bgLayer5;
    QImage bgLayer6;
    b2Body* bgLayer2Animation;
    b2Body* bgLayer3Animation;
    b2Body* bgLayer4Animation;
    b2Body* bgLayer5Animation;
    b2Body* bgLayer6Animation;



public:
    Level1(b2World* w, Player* p);
    void landscape() override;
    void paintLevelBackground(QPainter *painter) override;
    void paintLevelGround(QPainter* painter) override;
};

#endif // LEVEL1_H
