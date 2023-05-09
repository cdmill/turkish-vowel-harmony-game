#include "wordbox.h"
#include <Box2D/Box2D.h>
#include <cstdlib>

/**
 * @brief WordBox::WordBox - constructor for the wordbox class
 * @param w - the game world
 * @param id - an identifier for the box
 * @param vowel - the vowel displayed on the box
 * @param vowelImage - the image of the vowel to display
 */
WordBox::WordBox(b2World* w, int id, int vowel, QImage vowelImage) : world(w),
    vowelID(vowel),
    vowelImage(vowelImage),
    id(id)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_kinematicBody;

    // get a random height for the box
    float offset = rand() % 3 * 4;
    bodyDef.position.Set(50.0f, 16.0f + offset);

    // build the box
    body = world->CreateBody(&bodyDef);
    b2Vec2 force(-8.0f, 0.0f);
    body->SetGravityScale(0.0f);
    body->SetLinearVelocity(force);
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 0.0f);

}
/**
 * @brief WordBox::WordBox - map needs a default constructor for
 * inserting this will never really be used.
 */
WordBox::WordBox() : id(-1) { }

/**
 * @brief WordBox::getBody - gets the box2d body of the box
 * @return
 */
b2Body *WordBox::getBody() {
    return body;
}

/**
 * @brief WordBox::draw - draws the box on the screen
 * @param painter
 * @param image - image of the box (not the vowel)
 */
void WordBox::draw(QPainter* painter, QImage image) {
    b2Vec2 boxPosition = body->GetPosition();
    painter->drawImage((int)(boxPosition.x*20), (int)(boxPosition.y*20), image.scaled(50, 50));
    painter->drawImage((int)(boxPosition.x*20+5), (int)(boxPosition.y*20+3), vowelImage.scaled(40,40));
}

/**
 * @brief WordBox::xPos - returns the x position relative to the scene
 *
 * @return
 */
int WordBox::xPos() {
    return body->GetPosition().x*20 + 25;
}

/**
 * @brief WordBox::xPos - returns the y position relative to the scene
 *
 * @return
 */
int WordBox::yPos() {
    return body->GetPosition().y*20 + 25;
}
