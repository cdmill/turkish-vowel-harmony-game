#include "level.h"

/**
 * @brief Level::Level - constructor for levels
 * @param w - the game world
 * @param p - the player
 */
Level::Level(b2World* w, Player* p) : world(w), player(p) { }

/**
 * @brief Level::wrapping - allows the level background to
 * scroll left continuously
 * @param animation - the animation image for background
 */
void Level::wrapping(b2Body* animation) {
    float l2Position = animation->GetPosition().x * 20.0f;
    if(l2Position < -800)
        animation->SetTransform(b2Vec2(0.0f, 10.0f), 0.0);
}

/**
 * @brief Level::createBgAnimation - sets up background
 * animation object that moves accross the screen
 * @param speed - the speed that it will move to the left
 * @return - b2Body to animate
 */
b2Body* Level::createBgAnimation(float speed) {
    // Define the dynamic body. We set its position and call the body factory.
    b2BodyDef bgDef;
    bgDef.type = b2_kinematicBody;
    bgDef.position.Set(0.0f, 10.0f);

    // Creating background objects
    b2Body* bgAnimation = world->CreateBody(&bgDef);
    // add fixture to bg animation
    // Define another box shape for our dynamic body.
    b2PolygonShape bgBox;
    bgBox.SetAsBox(2.0f, 2.0f);
    // Define the dynamic body fixture.
    b2FixtureDef bgfixtureDef;
    bgfixtureDef.shape = &bgBox;
    // Set the box density to be non-zero, so it will be dynamic.
    bgfixtureDef.density = 1.0f;
    // Setting background movement
    bgAnimation->CreateFixture(&bgfixtureDef);
    bgAnimation->SetLinearVelocity(b2Vec2(-speed, 0.0f));
    return bgAnimation;
}

