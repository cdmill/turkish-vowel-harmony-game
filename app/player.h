#ifndef PLAYER_H
#define PLAYER_H

#include <QPainter>
#include <Box2D/Box2D.h>
#include "Direction.h"
#include "state.h"
#include "wordbox.h"
#include <map>
#include <vector>
#include "model.h"

using std::map;
using std::vector;

class Player {

public:
    Player(b2World* w);
    void draw(QPainter* painter, QImage image);
    void update(map<int, WordBox> &boxes);
    void move(Direction d);
    State getState();
    void startAttack();
    void stopAttack();
    int xPos();
    int yPos();
    int maxHealth = 100;
    int score = 0;
    int health = maxHealth;
    int onLevel = 1;
    void recover();
    void saveModel(Model* model);

signals:
    void correctVowelHit(int vowel);

private:
    b2World* world;
    b2Body* body;
    Model* model;
    State playerState = State::running;
    bool collidePlayerWithBox(WordBox &box);
    bool collideSwordWithBox(WordBox &box);
    float animationIndex;
    float animationSpeed;
    vector<QImage> explosionAnimation;
    vector<int> boxesToErase;
    QPoint explosion;
    QImage explosionImage;
    QPoint magic;
    QImage magicImage;
    vector<QImage> magicAnimation;
    vector<QImage> loadAnimation(QString filepath, int frameCount);
    void animateExplosion();
    void hurtPlayer();
    void animateMagic();
};

#endif // PLAYER_H
