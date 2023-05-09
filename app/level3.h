
#ifndef LEVEL3_H
#define LEVEL3_H

#include "level.h"



class Level3 : public Level
{
private:
    QImage bgLayer1;
    QImage bgLayer2;
    QImage bgLayer3;
    QImage bgLayer4;
    QImage bgLayer5;
    b2Body* bgLayer2Animation;
    b2Body* bgLayer3Animation;
    b2Body* bgLayer4Animation;
    b2Body* bgLayer5Animation;

public:
    Level3(b2World* w, Player* p);
    void landscape() override;
    void paintLevelBackground(QPainter* painter) override;
    void paintLevelGround(QPainter* painter) override;
};

#endif // LEVEL3_H
