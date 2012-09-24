#ifndef GAME_H
#define GAME_H

#include <QGLWidget>
#include <QKeyEvent>
#include <QTimer>
#include <QList>
#include <QFont>
#include <math.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include "maptile.h"
#include "register.h"
#include "c3ds.h"
#include "player.h"
#include "cristal.h"

bool testCubePoint(Point3f p, Point3f min, Point3f max);

class Game : public QGLWidget
{
    Q_OBJECT

public:
    explicit Game(QGLWidget *parent = 0);
    ~Game();
    bool loadLevel(int level);
    void closeLevel();
    bool nextLevel();

protected:
    QList<MapTile*> mapTiles;
    QList<Cristal*> cristals;
    QList<C3DS> models;

    QTimer *mainTimer, *fpsTimer, *titleTextTimer;
    QFont titleFont, cristalFont;
    Player player;
    bool leftKey, rightKey, upKey;
    int rotateX;
    int frames, fps;
    int currentLevel;
    Point3f levelTranslate;
    float scaleWorld;
    int levelWidth, levelHeight;
    GLfloat angleX, angleY, angleZ;
    QColor levelBackground, hudColor, titleTextColor;
    void initializeGL();
    QString titleText;

    void showTitleText(QString text);

    void resizeGL(int w, int h);
    void paintGL();

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    SceneActor* actorAt(float x, float y, float z = 0);

    void calculateCollisions(SceneActor *target);
    void calculatePsychic(SceneActor *target);
public slots:
    void animate();
    void updatePsychics();
    void hideTitleText();
    void updateFps();
};

#endif // GAME_H
