#include "level1.h"
#include <QPaintEvent>
#include <iostream>
#include <map>
#include "player.h"

using std::vector;
using std::map;

/**
 * @brief Level1::Level1 - Constructor for Level 1 - initializes
 * the background and movement
 * @param w - game world
 * @param p - the player
 */
Level1::Level1(b2World* w, Player* p) : Level(w, p),
    bgLayer1(QImage(":/Sprites/background/Hills Layer 01.png").scaled(800, 600)),
    bgLayer2(QImage(":/Sprites/background/Hills Layer 02.png").scaled(800, 200)),
    bgLayer3(QImage(":/Sprites/background/Hills Layer 03.png").scaled(800, 200)),
    bgLayer4(QImage(":/Sprites/background/Hills Layer 04.png").scaled(800, 200)),
    bgLayer5(QImage(":/Sprites/background/Hills Layer 05.png").scaled(800, 400)),
    bgLayer6(QImage(":/Sprites/background/Hills Layer 06.png").scaled(800, 200)) {

    // set up all of the background movement
    bgLayer2Animation = createBgAnimation(1.5f);
    bgLayer3Animation = createBgAnimation(2.0f);
    bgLayer4Animation = createBgAnimation(2.5f);
    bgLayer5Animation = createBgAnimation(3.0f);
    bgLayer6Animation = createBgAnimation(3.5f);

}

/**
 * @brief Level1::landscape - animates the entire background
 */
void Level1::landscape() {
    wrapping(bgLayer2Animation);
    wrapping(bgLayer3Animation);
    wrapping(bgLayer4Animation);
    wrapping(bgLayer5Animation);
    wrapping(bgLayer6Animation);
}

/**
 * @brief Level1::paintLevelBackground - paints the bacground
 * behind the player
 * @param painter
 */
void Level1::paintLevelBackground(QPainter *painter)
{
    b2Vec2 l2Position = bgLayer2Animation->GetPosition();
    b2Vec2 l3Position = bgLayer3Animation->GetPosition();
    b2Vec2 l4Position = bgLayer4Animation->GetPosition();

    // paint background
    painter->drawImage(0, 0, bgLayer1);
    painter->drawImage((int)(l2Position.x*20), (int)(l2Position.y*35.5), bgLayer2);
    painter->drawImage((int)(l2Position.x*20) + 800, (int)(l2Position.y*35.5), bgLayer2);
    painter->drawImage((int)(l3Position.x*20), (int)(l3Position.y*35.5), bgLayer3);
    painter->drawImage((int)(l3Position.x*20) + 800, (int)(l3Position.y*35.5), bgLayer3);
    painter->drawImage((int)(l4Position.x*20), (int)(l4Position.y*35.5), bgLayer4);
    painter->drawImage((int)(l4Position.x*20) + 800, (int)(l4Position.y*35.5), bgLayer4);
}

/**
 * @brief Level1::paintLevelGround - paints the ground under
 *  the player
 * @param painter
 */
void Level1::paintLevelGround(QPainter* painter) {

    b2Vec2 l5Position = bgLayer5Animation->GetPosition();
    b2Vec2 l6Position = bgLayer6Animation->GetPosition();
    // paint ground
    painter->drawImage((int)(l5Position.x*20), (175), bgLayer5);
    painter->drawImage((int)(l5Position.x*20) + 800, (175), bgLayer5);
    painter->drawImage((int)(l6Position.x*20), (int)(l6Position.y*37.5), bgLayer6);
    painter->drawImage((int)(l6Position.x*20) + 800, (int)(l6Position.y*37.5), bgLayer6);
}
