#include "player.h"
#include <QDebug>
#include "wordbox.h"
#include <map>
#include <QDebug>

using std::map;

/**
 * @brief Player::Player - Constructor for player class.
 * @param w - pointer to the game world
 */
Player::Player(b2World* w) : world(w),
    animationIndex(0.0f), animationSpeed(0.1f),
    explosion(0, 0), explosionImage(":/Sprites/explosion/tile000.png"),
    magic(0, 0), magicImage(":/Sprites/magic/tile000.png") {

    // Define the dynamic body. We set its position and call the body factory.
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f, 10.0f);

    body = world->CreateBody(&bodyDef);

    // Define another box shape for our dynamic body.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);

    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;

    // Set the box density to be non-zero, so it will be dynamic.
    fixtureDef.density = 1.0f;

    // Override the default friction.
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.0f;

    // Add the shape to the body.
    body->CreateFixture(&fixtureDef);

    explosionAnimation = loadAnimation("explosion", 16);
    magicAnimation = loadAnimation("magic", 81);
}
/**
 * @brief Player::draw - allows the player to draw itself
 * @param painter - pointer to the painter in the scene
 * @param image - the current animation frame
 */
void Player::draw(QPainter *painter, QImage image) {
    b2Vec2 position = body->GetPosition();
    // paint crusader
    int xPosit = (int)(position.x*20);
    int yPosit = (int)(position.y*22);
    painter->drawImage(xPosit, yPosit, image.scaled(QSize(200, 200)));

//     draw player hitbox for debug
//    int playerTop = yPos() - 50;
//    int playerLeft = xPos() - 25;
//    painter->drawRect(playerLeft, playerTop, 50, 150);

    // draw explosion if it exists
    if(explosion.x() || explosion.y())
        painter->drawImage(explosion.x() - 50, explosion.y() - 50, explosionImage.scaled(QSize(150, 150)));
    // draw magic if it exists
    if(magic.x() || magic.y())
        painter->drawImage(magic.x() - 75, magic.y() - 75, magicImage.scaled(QSize(175, 175)));
}
/**
 * @brief Player::move - moves the player in a direction
 * @param d - direction the user wants the player to move
 */
void Player::move(Direction d) {
    // save players current velocity.
    b2Vec2 vel = body->GetLinearVelocity();
    b2Vec2 position = body->GetPosition();
    int xPos = (int)(position.x*20);
    float desiredXVel = 0;
    float desiredYVel = 0;
    // update the desired x, y velocity according to key inputs
    switch(d) {
      case Direction::left:
        if(xPos > -50) {
            desiredXVel = -5;
        }
        break;
      case Direction::right:
        if(xPos < 650) {
            desiredXVel = +5;
        }
        break;
      case Direction::up:  desiredYVel = -10; playerState = State::jumping; break;
      case Direction::down: /** TODO: add trigger for duck **/ break;
    }
    // update the impulse by multiplying dx/dy
    float velXChange = desiredXVel - vel.x;
    float velYChange = desiredYVel - vel.y;
    float xImpulse = body->GetMass() * velXChange;
    float yImpulse = body->GetMass() * velYChange;
    // apply new impulse to the player
    body->ApplyLinearImpulse(b2Vec2(xImpulse, yImpulse), body->GetWorldCenter(), true);
}

/**
 * @brief Player::startAttack - allows the player to
 * start attacking
 */
void Player::startAttack() {
    playerState = State::attacking;
}

/**
 * @brief Player::stopAttack - stops the attack state
 */
void Player::stopAttack() {
    playerState = State::running;
}

/**
 * @brief Player::recover - allows the player to stop being
 * hurt
 */
void Player::recover() {
    playerState = State::running;
}

/**
 * @brief Player::xPos - returns the players x position (in scene
 * widget not box2d position)
 * @return
 */
int Player::xPos() {
    return body->GetPosition().x*20 + 100;
}

/**
 * @brief Player::xPos - returns the players y position (in scene
 * widget not box2d position)
 * @return
 */
int Player::yPos() {
    return body->GetPosition().y*22 + 100;
}

/**
 * @brief Player::update - checks for player collisions, state, health
 * etc. Makes appropriate changes.
 */
void Player::update(map<int, WordBox> &boxes) {
    // if jumping check if player hit ground
    if(playerState == State::jumping &&
            body->GetLinearVelocity().y > 0 && body->GetPosition().y >= 15.0f)
        playerState = State::running;

    for(auto &[id, wordbox] : boxes) {
        // check for collision
        if(playerState == State::attacking) {
            if(collideSwordWithBox(wordbox)) {
                if(model->checkCorrectness(wordbox.vowelID)) {
                    // play magic
                    magic = QPoint(wordbox.xPos(), wordbox.yPos());
                    score++;
                }
                else {
                    // this creates blow back from the explosion
                    move(Direction::left);

                    // explode in player's face
                    explosion = QPoint(wordbox.xPos(), wordbox.yPos());
                    hurtPlayer();
                }
                animationIndex = 0;
                boxesToErase.push_back(id);
            }
        }
        if(collidePlayerWithBox(wordbox)) {
            // this creates blow back from the explosion
            move(Direction::left);

            explosion = QPoint(wordbox.xPos(), wordbox.yPos());
            animationIndex = 0;
            hurtPlayer();
            boxesToErase.push_back(id);
        }
    }
    for(int id : boxesToErase) {
        boxes.erase(id);
    }
    if(explosion.x() || explosion.y())
        animateExplosion();
    if(magic.x() || magic.y())
        animateMagic();
}

/**
 * @brief Player::animateExplosion - plays the explosion
 * animation
 */
void Player::animateExplosion() {
    animationIndex += animationSpeed;
    // clamp
    if ((uint)animationIndex >= explosionAnimation.size()) {
        explosion = QPoint(0,0);
        animationIndex = 0;
    }
    explosionImage = explosionAnimation[(int)animationIndex];
}

/**
 * @brief Player::animateMagic - plays the magic animation
 */
void Player::animateMagic() {
    animationIndex += 0.4f;
    // clamp
    if ((uint)animationIndex >= magicAnimation.size()) {
        magic = QPoint(0,0);
        animationIndex = 0;
    }
    magicImage = magicAnimation[(int)animationIndex];
}

/**
 * @brief Player::hurtPlayer - hurts the player and checks for
 * death
 */
void Player::hurtPlayer() {
    if(health == 10)
        health -= 10;
    else
        health -= 30;
    if(health > 0)
        playerState = State::hurt;
    else
        playerState = State::dying;
}

/**
 * @brief Player::collidedWithBox - helper method to check if the player has collided
 * with a box.
 * @param box - NOTE: this could be box objects with getters for x/y position
 * so that each box could have info about it's suffix.
 * @return - true if box collided with player
 */
bool Player::collidePlayerWithBox(WordBox &box) {
    // build player and box hitboxes
    int playerTop = yPos() - 50;
    int playerBottom = yPos() + 100;
    int playerLeft = xPos() - 25;
    int playerRight = xPos() + 25;

    int boxTop = box.yPos() - 25;
    int boxBottom = box.yPos() + 25;
    int boxLeft = box.xPos() - 25;
    int boxRight = box.xPos() + 25;


    // calculate if the x or y hitboxes overlap
    bool yOverlap = (playerTop >= boxTop && playerTop <= boxBottom)
                 || (playerBottom >= boxTop && playerBottom <= boxBottom)
                 || (playerBottom >= boxBottom && playerTop <= boxTop);
    bool xOverlap = (playerLeft >= boxLeft && playerLeft <= boxRight)
                 || (playerRight >= boxLeft && playerRight <= boxLeft)
                 || (playerLeft >= boxLeft && playerRight <= boxRight);


    if(xOverlap && yOverlap) {
        qDebug() << "collided";
        return true;
    }
    return false;
}

/**
 * @brief Player::collidedWithBox - helper method to check if the player's sword has collided
 * with a box.
 * @param box - NOTE: this could be box objects with getters for x/y position
 * so that each box could have info about it's suffix.
 * @return - true if box collided with player
 */
bool Player::collideSwordWithBox(WordBox &box)
{
    // build sword hitbox
    int swordTop = yPos() -75;
    int swordBottom = yPos() +90;
    int swordLeft = xPos() +50;
    int swordRight = xPos() +75;

    //build box hitbox
    int boxTop = box.yPos() - 25;
    int boxBottom = box.yPos() + 25;
    int boxLeft = box.xPos() - 25;
    int boxRight = box.xPos() + 25;

    //calculate whether the sword collides with the box
    bool yOverlap = (swordTop >= boxTop && swordTop <= boxBottom)
                 || (swordBottom >= boxTop && swordBottom <= boxBottom)
                 || (swordBottom >= boxBottom && swordTop <= boxTop);
    bool xOverlap = (swordLeft >= boxLeft && swordLeft <= boxRight)
                 || (swordRight >= boxLeft && swordRight <= boxLeft)
                 || (swordLeft >= boxLeft && swordRight <= boxRight);


    if(xOverlap && yOverlap) {
        qDebug() << "collided";
        return true;
    }
    return false;

}

/**
 * @brief Player::getState - getter for the player state
 * @return
 */
State Player::getState() {
    return playerState;
}

/**
 * @brief Player::loadAnimation - loads the frames of an animation
 * into a vector of QImages
 * @param filepath - the name of the resource folder in the Sprites
 * path
 * @param frameCount - the number of frames
 * @return - a vector of frames
 */
vector<QImage> Player::loadAnimation(QString filepath, int frameCount) {

    vector<QImage> frames;

    // Get each framepath, create an image, and add it to the vector
    for (int i = 0; i < frameCount; i++) {
        QString framePath("");
        // account for changes in path formats for different frame numbers
        // (max frame count is 999)
        if(i < 10)
           framePath = QString (":/Sprites/" + filepath +
                                "/tile00" + QString::number(i) + ".png");
        else if(i < 100)
            framePath = QString(":/Sprites/" + filepath +
                                "/tile0" + QString::number(i) + ".png");
        else
            framePath = QString(":/Sprites/" + filepath +
                                "/tile" + QString::number(i) + ".png");
        frames.push_back(QImage(framePath));
    }
    return frames;
}

/**
 * @brief Player::saveModel - stores a pointer to the model
 * @param model
 */
void Player::saveModel(Model* model) {
    this->model = model;
}
