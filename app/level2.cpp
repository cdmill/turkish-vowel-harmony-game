#include "level2.h"

/**
 * @brief Level2::Level2 - constructor for level 2 loads the background
 * images and animations.
 * @param w - the game world
 * @param p - the player
 */
Level2::Level2(b2World* w, Player* p) : Level(w, p),
    bgLayer1(QImage(":/Sprites/background2/L1.png").scaled(800, 600)),
    bgLayer2(QImage(":/Sprites/background2/L2.png").scaled(800, 600)),
    bgLayer3(QImage(":/Sprites/background2/L3.png").scaled(800, 600)),
    bgLayer4(QImage(":/Sprites/background2/L4.png").scaled(800, 600)),
    bgLayer5(QImage(":/Sprites/background2/L5.png").scaled(800, 600)) {

    // set up all of the background movement
    bgLayer2Animation = createBgAnimation(1.0f);
    bgLayer3Animation = createBgAnimation(2.0f);
    bgLayer4Animation = createBgAnimation(2.5f);
    bgLayer5Animation = createBgAnimation(4.5f);

}

/**
 * @brief Level2::landscape - animates the entire backgound
 */
void Level2::landscape() {
    wrapping(bgLayer2Animation);
    wrapping(bgLayer3Animation);
    wrapping(bgLayer4Animation);
    wrapping(bgLayer5Animation);
}

/**
 * @brief Level2::paintLevelBackground - paints the background
 * behind the player
 *
 * @param painter
 */
void Level2::paintLevelBackground(QPainter *painter)
{
    b2Vec2 l2Position = bgLayer2Animation->GetPosition();
    b2Vec2 l3Position = bgLayer3Animation->GetPosition();
    b2Vec2 l4Position = bgLayer4Animation->GetPosition();

    // paint background
    painter->drawImage(0, 0, bgLayer1.scaled(QSize(800, 600)));
    painter->drawImage((int)(l2Position.x*20), 0, bgLayer2.scaled(800, 600));
    painter->drawImage((int)(l2Position.x*20) + 800, 0, bgLayer2.scaled(800, 600));
    painter->drawImage((int)(l3Position.x*20), 0, bgLayer3.scaled(800, 600));
    painter->drawImage((int)(l3Position.x*20) + 800, 0, bgLayer3.scaled(800, 600));
    painter->drawImage((int)(l4Position.x*20), 0, bgLayer4.scaled(800, 600));
    painter->drawImage((int)(l4Position.x*20) + 800, 0, bgLayer4.scaled(800, 600));

}

/**
 * @brief Level2::paintLevelGround - paints the ground below the player
 * @param painter
 */
void Level2::paintLevelGround(QPainter* painter) {
    b2Vec2 l5Position = bgLayer5Animation->GetPosition();

    painter->drawImage((int)(l5Position.x*20), 30, bgLayer5.scaled(800, 600));
    painter->drawImage((int)(l5Position.x*20) + 800, 30, bgLayer5.scaled(800, 600));
}


