#include "game.h"
#include <QImage>
#include <QLabel>
#include <QTextStream>
#include "maptile.h"
#include "cristal.h"
#include "c3ds.h"
#include "cristalmodel.h"
#include "tilemodel.h"

#define BEZ(a) (a>0)? a : -a

Game::Game(QGLWidget *parent) :
    QGLWidget(parent)
{
    this->mainTimer = new QTimer( this );
    connect( mainTimer, SIGNAL(timeout()), this, SLOT(animate()) );
    connect( mainTimer, SIGNAL(timeout()), this, SLOT(updatePsychics()) );
    mainTimer->start( 8 );

    this->fpsTimer = new QTimer( this );
    connect( fpsTimer, SIGNAL(timeout()), this, SLOT(updateFps()) );
    fpsTimer->start(1000);

    this->titleTextTimer = new QTimer( this );
    connect( titleTextTimer, SIGNAL(timeout()), this, SLOT(hideTitleText()) );

    titleFont.setBold(true);
    titleFont.setFamily("arial");
    titleFont.setPixelSize(30);
    titleText = "";

    cristalFont.setBold(true);
    cristalFont.setFamily("arial");
    cristalFont.setPixelSize(15);

    fps = frames = 0;
    currentLevel = 0;
}

Game::~Game()
{
}

void Game::initializeGL()
{
    qglClearColor(this->levelBackground);
    glEnable(GL_DEPTH_TEST);
    glShadeModel (GL_SMOOTH);

    glEnable(GL_LIGHTING);
    GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat light_ambient[] = { 0.6, 0.6, 0.6, 1 };
    GLfloat light_position[] = { 0.0, 10.0, 0.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial (GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glCullFace(GL_FRONT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(150, this->height()/this->width(), 0.1, 10000);
    angleY = angleZ = 0;
    angleX = -0.2;
    this->loadLevel(currentLevel++);
    leftKey = rightKey = upKey = 0;
    scaleWorld = 0.002;
    rotateX = 0;
}

bool testCubePoint(Point3f p, Point3f min, Point3f max){

    if(min.x < p.x && max.x > p.x
            && min.y < p.y && max.y > p.y){
        return true;
    }
    return false;
}

bool Game::loadLevel(int level){
    QImage image;
    QFile data(QString("data/levels/") + QString::number(level) + ".txt");
    if(!image.load(QString("data/levels/") + QString::number(level) + ".gif") ||
            !data.open(QIODevice::ReadOnly | QIODevice::Text)) return false;

    QTextStream in( & data );

    QStringList tmpColor = in.readLine().split(","); //odczyt linii z skladowymi koloru
    this->levelBackground.setRgb(tmpColor[0].toInt(), tmpColor[1].toInt(), tmpColor[2].toInt()); //ustawienie tla poziomu
    qglClearColor(this->levelBackground);

    tmpColor = in.readLine().split(","); //odczyt linii z skladowymi koloru
    this->hudColor.setRgb(tmpColor[0].toInt(), tmpColor[1].toInt(), tmpColor[2].toInt()); //ustawienie tla poziomu

    tmpColor = in.readLine().split(","); //odczyt linii z skladowymi koloru
    this->titleTextColor.setRgb(tmpColor[0].toInt(), tmpColor[1].toInt(), tmpColor[2].toInt()); //ustawienie tla poziomu


    levelWidth = image.width()*20;
    levelHeight = image.height()*20;

    CristalModel cristalModel;
    TileModel tileModel;
    tileModel.loadTexture(in.readLine()); //wczytanie tekstury na kafle

    C3DS *m = (C3DS*) &tileModel;
    C3DS m2((char*)"./data/models/kolo.3DS");
    C3DS *m3 = (C3DS*) &cristalModel;

    this->models.clear();
    this->models.append(*m);
    this->models.append(m2);
    this->models.append(*m3);

    QColor color;

    for(int x=0; x < image.width(); x++){
        for(int y=0; y < image.height(); y++){
            color.setRgb(image.pixel(x, y));

            if(color.red() == 0 && color.green() == 255){
                Cristal *power = new Cristal();
                power->setModelId(2);
                power->setPosition(new Point3f(x*20, (image.height()-y)*20, 0));
                this->cristals.append(power);
            }

            if(color.red() == 0 && color.blue() == 0 && color.green() == 0){
                MapTile *tile = new MapTile();
                tile->setModelId(0);
                tile->setPosition(new Point3f(x*20, (image.height()-y)*20, 0));
                this->mapTiles.append(tile);
            }

            if(color.red() == 0 && color.green()== 0 && color.blue() == 255){
                this->player.reset();
                this->player.setModelId(1);
                this->player.setPosition(new Point3f(x*20, (image.height()-y)*20, 0));

                levelTranslate.x = (levelWidth > x)? 490 : (levelWidth*0.5f)-10;
                levelTranslate.y = (levelHeight*0.5f)+10;
            }
        }
    }

    return true;
}

void Game::closeLevel(){
    for(int i=0; i < this->mapTiles.length(); i++){
        delete this->mapTiles[i];
    }
    this->mapTiles.clear();

    for(int i=0; i < this->cristals.length(); i++){
        delete this->cristals[i];
    }
    this->cristals.clear();
}

bool Game::nextLevel(){
    closeLevel();

    if(loadLevel(currentLevel++)){ //jesli znaleziono kolejny poziom
        showTitleText(QString("Gratulacje, przeszedles poziom ") +  QString::number(currentLevel-1));
        return true;
    }else{ //w przeciwnym razie ustawia poziom na zerowy
        loadLevel((this->currentLevel = 0)++);
        showTitleText(QString("Gratulacje, przeszedles cala gre !"));
    }
    return false;
}

void Game::showTitleText(QString text){
    this->titleText = text;
    this->titleTextTimer->start(2000);
    this->mainTimer->stop();
    this->animate(); //wyswietlam jedna klatke, juz z nowym poziomem oraz napisem tytulowym
}

void Game::hideTitleText(){
    this->titleText = "";
    this->mainTimer->start(8);
    this->titleTextTimer->stop();
}

void Game::calculateCollisions(SceneActor *target){
    SceneActor *a = NULL;
    Point3f *p = target->getPosition();

    //z prawej
    if(((a=this->actorAt(p->x+10, p->y-5)) || (a=this->actorAt(p->x+10, p->y+5))) && a->calculateCollisions()){
        if(a->getActorType() == ACTOR_MAP_TILE){
            MapTile *b = (MapTile*)a;
            b->onCollisionLeft(target);
        }
    }else
    //z lewej
    if(((a=this->actorAt(p->x-10, p->y-5)) || (a=this->actorAt(p->x-10, p->y+5))) && a->calculateCollisions()){
        if(a->getActorType() == ACTOR_MAP_TILE){
            MapTile *b = (MapTile*)a;
            b->onCollisionRight(target);
        }
    }

    //z gory
    if(((a=this->actorAt(p->x-3, p->y+10)) || (a=this->actorAt(p->x+3, p->y+10))) && a->calculateCollisions()){
        if(a->getActorType() == ACTOR_MAP_TILE){
            MapTile *b = (MapTile*)a;
            b->onCollisionDown(target);
        }
    }else
    //z dolu
    if(((a=this->actorAt(p->x-3, p->y-10)) || (a=this->actorAt(p->x+3, p->y-10))) && a->calculateCollisions()){

        if(a->getActorType() == ACTOR_MAP_TILE){
            MapTile *b = (MapTile*)a;
            b->onCollisionUp(target, upKey);
        } else if(a->getActorType() == ACTOR_POWERUP){
            Cristal *d = (Cristal*)a;
            d->onCollisionUp(target);
        }
    }else {
        target->gameState.force.y -= 0.03; //grawitacja
    }
}

void Game::calculatePsychic(SceneActor *target){

    calculateCollisions(target);

    Point3f *p = target->getPosition();
    Point3f f = target->getForce();
    Point3f newF;

    //kalkulacja nowych sil
    newF.x = (f.x + target->gameState.force.x) * target->getFriction();
    newF.y = (f.y + target->gameState.force.y) * target->getFriction();

    //zdarzenie gdy sila na x przestala oddzialywac
    if(newF.x < 0.001f && newF.x > -0.001f){
        target->gameState.zeroForce = true;
        newF.x = 0;
    }

    //gdy sila na osi y przestala oddzialywac
    if(newF.y < 0.001f && newF.y > -0.001){
        newF.y = 0;
    }

    //maksymalna oddzialywana sila
    if(newF.x > 10){
        newF.x = 10;
    } else if(newF.x < -10) {
        newF.x = -10;
    }

    target->setPosition(new Point3f(p->x+newF.x, p->y+newF.y, 0));
    target->setForce(&newF);
    target->update();
    target->gameState = GameState();
}

void Game::resizeGL(int w, int h)
{
    glViewport(0, 0, (GLint)w, (GLint)h);
}

void Game::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    qglColor(this->hudColor);
    this->renderText(-0.95,0.9,-.5, QString("FPS: ") + QString::number(fps));
    this->renderText(-0.95,0.8,-.5, QString("Poziom: ") + QString::number(currentLevel));
    this->renderText(-0.95,0.7,-.5, QString("Krysztaly: ") + QString::number(this->cristals.length()), this->cristalFont);
    /*this->renderText(-1,0.7,-.5,
                     QString("Player pos: (") + QString::number(this->player.getPosition()->x) +
                     QString(", ") + QString::number(this->player.getPosition()->y)
                     + QString(", ") + QString::number(this->player.getPosition()->z)
                     + QString(")"));
*/
    qglColor(this->titleTextColor);
    this->renderText(-0.8,0,-.5, this->titleText, this->titleFont);

    glRotatef(this->angleX, 1, 0, 0);
    glRotatef(this->angleY, 0, 1, 0);
    glRotatef(this->angleZ, 0, 0, 1);

    Point3f *p = player.getPosition();
    glScalef(scaleWorld, scaleWorld, scaleWorld);

    //pilka ma srodek w p(x,y) -10 +10 za gabaryty
    if(p->x > 490 && levelWidth-510 > p->x) {
        levelTranslate.x = p->x;
    }

    //pilka ma na spodzie y=0
    if(p->y > 500 && levelHeight-510 > p->y) {
        levelTranslate.y = p->y;
    }
    glTranslatef(-levelTranslate.x, -levelTranslate.y, 0);

    //map
    for(int i=0; i < mapTiles.length(); i++){
        glPushMatrix();
        Point3f *p = mapTiles[i]->getPosition();

        glTranslatef(p->x, p->y, p->z);
        models[mapTiles[i]->getModelId()].render();
        glPopMatrix();
    }

    //cristals
    for(int i=0; i < cristals.length(); i++){
        glPushMatrix();
        Point3f *p = cristals[i]->getPosition();

        glTranslatef(p->x, p->y, p->z);
        glRotatef(cristals[i]->rotateZ, 1,1,1);
        cristals[i]->rotateZ += 0.6;
        models[cristals[i]->getModelId()].render();
        glPopMatrix();
    }

    //player
    glTranslatef(p->x, p->y, p->z);
    //glPushMatrix();
    this->rotateX += this->player.getForce().x * 1.5;
    glRotatef(this->rotateX * -1, 0,0,1 );
    //glPopMatrix();
    models[player.getModelId()].render();

    frames++;
}

void Game::keyPressEvent(QKeyEvent *event){
    switch(event->key()){
    case Qt::Key_Right:
        rightKey = true;
        break;
    case Qt::Key_Left:
        leftKey = true;
        break;
    case Qt::Key_Up:
        upKey = true;
        break;
    }
}

void Game::keyReleaseEvent(QKeyEvent *event){
    switch(event->key()){
    case Qt::Key_Right:
        rightKey = false;
        break;
    case Qt::Key_Left:
        leftKey = false;
        break;
    case Qt::Key_Up:
        upKey = false;
        break;
    case Qt::Key_Y:
        angleY += 1;
        break;
    case Qt::Key_Z:
        angleZ += 1;
        break;
    case Qt::Key_F9:
        scaleWorld -= 0.0001;
        break;
    case Qt::Key_F10:
        scaleWorld += 0.0001;
        break;
    }
}

SceneActor* Game::actorAt(float x, float y, float z){
    /* Map tiles */
    for(int i=0; i < mapTiles.length(); i++){
        Point3f *translation = mapTiles[i]->getPosition();
        Point3f min = models[mapTiles[i]->getModelId()].getMinPoint();
        Point3f max = models[mapTiles[i]->getModelId()].getMaxPoint();

        if(testCubePoint(Point3f(x,y,z), min + *translation, max + *translation)){
            return (SceneActor*) mapTiles[i];
        }
    }

    /* Cristals */
    if(cristals.length()){
        Point3f min = models[cristals[0]->getModelId()].getMinPoint();
        Point3f max = models[cristals[0]->getModelId()].getMaxPoint();

        for(int i=0; i < cristals.length(); i++){
            Point3f *translation = cristals[i]->getPosition();

            if(testCubePoint(Point3f(x,y,z), min + *translation, max + *translation)){ //sprawdzam czy punkt (x,y,z) znajduje sie wewnatrz prostopadloscianu krysztalu
                SceneActor *c = (SceneActor*) cristals[i]; //pobieram wskaznik na krysztal
                this->cristals.removeAt(i); //usuwam krysztal z listy, aby go nie wyswietlac wiecej, ani nie liczyc kolizji
                return c; //zwracam krysztal
            }
        }
    }else {
        nextLevel();
    }

    return NULL;
}

void Game::animate(){
    this->updateGL();
}

void Game::updatePsychics(){
    this->calculatePsychic(&this->player);

    if(rightKey){
        player.gameState.force.x += 0.1;
    }

    if(leftKey){
        player.gameState.force.x -= 0.1;
    }
}

void Game::updateFps(){
    fps = frames;
    frames = 0;
}
