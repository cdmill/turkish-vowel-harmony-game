#include "level3.h"

/**
 * @brief Level3::Level3 - Constructor for level 3 class
 *
 * @param w - the game world
 * @param p - the player
 */
Level3::Level3(b2World* w, Player* p) : Level(w, p),
    bgLayer1(QImage(":/Sprites/background3/parallax-mountain-bg.png").scaled(800, 500)),
    bgLayer2(QImage(":/Sprites/background3/parallax-mountain-montain-far.png").scaled(800, 600)),
    bgLayer3(QImage(":/Sprites/background3/parallax-mountain-mountains.png").scaled(800, 600)),
    bgLayer4(QImage(":/Sprites/background3/parallax-mountain-trees.png").scaled(800, 600)),
    bgLayer5(QImage(":/Sprites/background3/parallax-mountain-foreground-trees.png").scaled(800, 600)) {

    // set up all of the background movement
    bgLayer2Animation = createBgAnimation(1.5f);
    bgLayer3Animation = createBgAnimation(2.0f);
    bgLayer4Animation = createBgAnimation(2.5f);
    bgLayer5Animation = createBgAnimation(3.0f);
}

/**
 * @brief Level3::landscape - animates the entire level 3 background
 */
void Level3::landscape() {
    wrapping(bgLayer2Animation);
    wrapping(bgLayer3Animation);
    wrapping(bgLayer4Animation);
    wrapping(bgLayer5Animation);
}

/**
 * @brief Level3::paintLevelBackground - paints the backgound behind
 * the player
 *
 * @param painter
 */
void Level3::paintLevelBackground(QPainter* painter) {
    b2Vec2 l2Position = bgLayer2Animation->GetPosition();
    b2Vec2 l3Position = bgLayer3Animation->GetPosition();
    b2Vec2 l4Position = bgLayer4Animation->GetPosition();
    b2Vec2 l5Position = bgLayer5Animation->GetPosition();

    // paint background
    painter->drawImage(0, 0, bgLayer1);
    painter->drawImage((int)(l2Position.x*20), 0, bgLayer2);
    painter->drawImage((int)(l2Position.x*20) + 800, 0, bgLayer2);
    painter->drawImage((int)(l3Position.x*20), 0, bgLayer3);
    painter->drawImage((int)(l3Position.x*20) + 800, 0, bgLayer3);
    painter->drawImage((int)(l4Position.x*20), 0, bgLayer4);
    painter->drawImage((int)(l4Position.x*20) + 800, 0, bgLayer4);
    painter->drawImage((int)(l5Position.x*20), (l5Position.y*37.5), bgLayer5);
    painter->drawImage((int)(l5Position.x*20) + 800, (l5Position.y*37.5), bgLayer5);
}

/**
 * @brief Level3::paintLevelGround - unused there is no ground in
 * front of the player in level 3
 * @param painter
 */
void Level3::paintLevelGround(QPainter* painter) { }



