#ifndef WORDBOX_H
#define WORDBOX_H

#include <Box2D/Box2D.h>
#include <QPainter>
#include <QImage>

class WordBox
{
public:
    WordBox(b2World* w, int id, int character, QImage image);
    WordBox();
    b2Body* getBody();
    void draw(QPainter *painter, QImage image);
    int xPos();
    int yPos();
    int vowelID;
    QImage vowelImage;
private:
    b2World* world = nullptr;
    int id;
    b2Body* body;

};

#endif // WORDBOX_H
