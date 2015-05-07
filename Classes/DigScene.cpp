#include "DigScene.h"

USING_NS_CC;

Scene* DigScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = DigScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void DigScene::setTouchLayer(Layer *layer) {

    listener = EventListenerTouchOneByOne::create();

    listener->onTouchBegan = [](Touch* touch,Event* event){
      return true;
    };

    listener->onTouchMoved = [](Touch* touch,Event* event){

    };

    listener->onTouchEnded = [=](Touch* touch,Event* event){
        Point move = touch->getLocation()-touch->getStartLocation();
        log("end");
        if(abs(move.x)>abs(move.y)){
            turnHorizen(move.x>0 ? RIGHT : LEFT);
        }
        else{
            if(move.y<0)turnDown();
        }
    };

    listener->onTouchCancelled = [=](Touch* touch,Event* event){

    };


    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,layer);


}

bool DigScene::Crash(list<Line*>::iterator nextLine,int nextverID) {
    if(nextverID >=0  && nextverID < 8) {
        if ((*nextLine)->getRectByID(nextverID)->getType() == BRICK){
            log("true");
            return false;
        }
        NotificationCenter::getInstance()->postNotification("shake");
    }
    return true;
}

void DigScene::turnDown() {

    if(Crash(++currentLine,verticalID)){
        currentLine--;
        return;
    }
    if ((vertical.size()-1) * VISIZE.width / RECT_NUM_WIDTH > VISIZE.height) {
        this->removeChild(vertical.front());
        vertical.pop_front();
    }


    Line *newLine = Line::create();
    this->addChild(newLine);
    newLine->initWithLastLine(vertical.back());
    vertical.push_back(newLine);
    for (auto line: vertical) {
        log("up");
        line->up();
    }
    log("lastPos:%f",vertical.back()->getPosition().y);


    grap->Dig(0);
    (*currentLine)->getRectByID(verticalID)->beDigged();
}

void DigScene::turnHorizen(int direction) {
    if(Crash(currentLine,verticalID + direction)){
        return;
    }
    grap->Dig(direction);
    verticalID += direction;
    (*currentLine)->getRectByID(verticalID)->beDigged();
}

void DigScene::Shake(Ref *pSender) {
    log("shake");
    this->runAction(CCShake::create(0.5,VISIZE.width/30));
}

// on "init" you need to initialize your instance
bool DigScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }


    
    /*init first line */
    Line* firstLine = Line::create();
    firstLine->setPosition(0,VISIZE.height);
    firstLine->firstLine();
    vertical.push_back(firstLine);
    this->addChild(firstLine);
    log("first line build done");



    /*init other line full screen*/
    while((vertical.size()-1)*VISIZE.width/RECT_NUM_WIDTH < VISIZE.height){
        Line* newLine = Line::create();
        this->addChild(newLine);
        newLine->initWithLastLine(vertical.back());
        vertical.push_back(newLine);
        log("loop");
        log("size:%d",vertical.size());
    }


    /*init Grap*/
    grap = Grap::create();
    currentLine = vertical.begin();
    advance(currentLine,3);
    verticalID = 2;


    RectMap* target = (*currentLine)->getRectByID(verticalID);

    grap->setPosition((target->getPosition())+(*currentLine)->getPosition());

    this->addChild(grap);

    (*currentLine)->getRectByID(verticalID)->clear();


    /*set touch enabled*/
    setTouchLayer(this);

    /*Shake call-func when run into Stone*/
    NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(DigScene::Shake),"shake", nullptr);

    
    return true;
}

