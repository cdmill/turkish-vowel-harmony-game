
#ifndef LEVEL2_H
#define LEVEL2_H

#include "level.h"



class Level2 : public Level
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

public:
    Level2(b2World* w, Player* p);
    void landscape() override;
    void paintLevelBackground(QPainter *painter) override;
    void paintLevelGround(QPainter* painter) override;
};

#endif // LEVEL2_H
