#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include <QWidget>
#include <Box2D/Box2D.h>
#include <QTimer>
#include <vector>
#include "Direction.h"
#include "player.h"
#include "wordbox.h"
#include <map>
#include "level.h"
#include "level1.h"
#include "level2.h"
#include "level3.h"
#include "model.h"
#include <unordered_set>

using std::vector;
using std::map;
using std::unordered_set;

class SceneWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SceneWidget(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);
    vector<QImage> vowels;
    int searchVowel;
    void addBox();
    static vector<QImage> loadAnimation(QString playerColor, QString filepath, int frameCount);
    void resume();
    void pause();
    void saveModel(Model* model);

signals:
    void updateHealth(int health);
    void updateLevel(int level);
    void gameOver();
    void startNewLevel();

public slots:
    void updateWorld();
    void startUpdate(QString playerColor);
    void movePlayer(Direction d);
    void playerAttack();
    void saveCurrentParadigm(QStringList word);

private:
    b2World world;
    Player player;
    Model* model;
    Level* currentLevel;
    Level1 l1;
    Level2 l2;
    Level3 l3;
    State currentPlayerState;
    QStringList currentParadigm;
    QString playerColor = "whiteBlack";
    vector<b2Body*> animations;
    map<int, WordBox> boxes;
    int currentBoxId = 0;
    QTimer timer;
    QTimer boxTimer;
    QImage image;
    QImage boxSprite;
    float animationSpeed;
    float animationIndex;
    vector<QImage> jumpAnimation;
    vector<QImage> runAnimation;
    vector<QImage> attackAnimation;
    vector<QImage> hurtAnimation;
    vector<QImage> dieAnimation;
    vector<int> boxesToErase;
    unordered_set<int> usedVowels;
    void animate();
    void collision();
    int getUnusedVowel();
    bool noMoreVowelSlots();
    bool isWordComplete = false;
    bool incorrectVowelHit = false;
    int getBoxTime();

private slots:
    void fillVowelSlot(int vowel);
    void printIncorrectMessage();
    void turnOffIncorrectMessage();
    void getNewParadigm();
    void advanceLevel();

};

#endif // SCENEWIDGET_H
