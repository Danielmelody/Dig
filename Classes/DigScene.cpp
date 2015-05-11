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

    listener->onTouchBegan = [=](Touch* touch,Event* event){
        virtualGrap->getSprite()->setVisible(true);
        virtualGrap->getSprite()->setOpacity(0);
        virtualGrap->getSprite()->setScale(grap->getSprite()->getScale());
      return true;
    };

    listener->onTouchMoved = [=](Touch* touch,Event* event){

        Point move = touch->getLocation()-touch->getStartLocation();

        if(move.getDistance(Point(0,0))>VISIZE.width/50) {


            grap->getSprite()->setRotation(-180 * (move.getAngle() + acos(-1) / 2) / acos(-1));

            virtualGrap->setPosition(grap->getPosition());
            virtualGrap->getSprite()->setOpacity(100);


            if (abs(move.x) > abs(move.y)) {
                virtualGrap->getSprite()->setRotation(move.x>0 ? -90 : 90);
                if (move.x > 0) {
                    virtualGrap->getSprite()->setPosition(
                            grap->getSprite()->getPosition() + Point(VISIZE.width / RECT_NUM_WIDTH, 0));

                }
                else {
                    virtualGrap->getSprite()->setPosition(
                            grap->getSprite()->getPosition() + Point(-VISIZE.width / RECT_NUM_WIDTH, 0));
                }
            }
            else {
                if (move.y < 0) {
                    virtualGrap->getSprite()->setPosition(
                            grap->getSprite()->getPosition() + Point(0, -VISIZE.width / RECT_NUM_WIDTH));
                    virtualGrap->getSprite()->setRotation(0);
                }

            }
        }
    };

    listener->onTouchEnded = [=](Touch* touch,Event* event){
        Point move = touch->getLocation()-touch->getStartLocation();
        if(move.getDistance(Point(0,0))>VISIZE.width/50) {
            log("end");
            if (abs(move.x) > abs(move.y)) {
                grap->getSprite()->runAction(RotateTo::create(ACTION_INTERVAL, move.x > 0 ? -90 : 90));
                turnHorizen(move.x > 0 ? RIGHT : LEFT);
            }
            else {
                grap->getSprite()->runAction(RotateTo::create(ACTION_INTERVAL, 0));

                if (move.y < 0)turnDown();
            }
            virtualGrap->getSprite()->setScale(VISIZE.width/RECT_NUM_WIDTH/virtualGrap->getSprite()->getContentSize().width);
            virtualGrap->getSprite()->runAction(
                    Spawn::create(ScaleBy::create(ACTION_INTERVAL * 2, 2), FadeTo::create(ACTION_INTERVAL * 2, 0),
                                  nullptr));
        }

    };

    listener->onTouchCancelled = [=](Touch* touch,Event* event){

    };


    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,layer);


}

bool DigScene::Crash(list<Line*>::iterator nextLine,int nextverID) {
    if (nextverID >= 0 && nextverID < RECT_NUM_WIDTH) {
        (*nextLine)->getRectByID(nextverID)->beDigged();
        if ((*nextLine)->getRectByID(nextverID)->getType() != STONE) {
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

    score++;
    char str[100] = {'0'};
    sprintf(str, "%d", score);
    scoreLabel->setString(str);
    scoreLabel->setPosition(Point(VISIZE.width/2,VISIZE.height*0.9));

    Line *newLine = Line::create();
    this->addChild(newLine);
    newLine->initWithLastLine(vertical.back());
    vertical.push_back(newLine);
    for (auto line: vertical) {
        line->up();
    }


    grap->Dig(0);
}

void DigScene::turnHorizen(int direction) {
    if(Crash(currentLine,verticalID + direction)){
        return;
    }
    grap->Dig(direction);
    verticalID += direction;
}

void DigScene::Shake(Ref *pSender) {
    log("shake");
    this->runAction(CCShake::create(0.5,VISIZE.width/30));
}


void DigScene::updateHp(float dt) {
    hp -= HP_UNIT_UPDATE;
    float nowhp = progressTimer->getPercentage();
    progressTimer->setPercentage(nowhp-HP_UNIT_UPDATE);
    if(nowhp>75){
        timer->setTexture(Director::getInstance()->getTextureCache()->addImage("energy_front_g.png"));
    }
    else if(nowhp>50){
        timer->setTexture(Director::getInstance()->getTextureCache()->addImage("energy_front_y.png"));
    }
    else if(nowhp>25){
        timer->setTexture(Director::getInstance()->getTextureCache()->addImage("energy_front_o.png"));
    }
    else if(nowhp>0){
        timer->setTexture(Director::getInstance()->getTextureCache()->addImage("energy_front_r.png"));
    }

    if(nowhp==0)fail();
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

    hp = 100;
    this->schedule(schedule_selector(DigScene::updateHp),1/60);
    timer = Sprite::create("energy_front.png");

    progressTimer = ProgressTimer::create(timer);
    progressTimer->setType(ProgressTimer::Type::BAR);
    progressTimer->setMidpoint(Point(-1,0));
    progressTimer->setBarChangeRate(Point(1,0));
    progressTimer->setPosition(VISIZE.width/2,VISIZE.width/RECT_NUM_WIDTH/2);
    progressTimer->setScaleX(VISIZE.width/timer->getContentSize().width*0.6);
    progressTimer->setScaleY(progressTimer->getScaleX()*2);
    progressTimer->setPercentage(100);

    Sprite* energy_back = Sprite::create("energy_back.png");

    energy_back->setScaleX(progressTimer->getScaleX());
    energy_back->setScaleY(progressTimer->getScaleY());
    energy_back->setPosition(progressTimer->getPosition());
    this->addChild(energy_back,10);

    this->addChild(progressTimer,10);

    
    /*init first line */
    Line* firstLine = Line::create();
    firstLine->setPosition(0,VISIZE.height);
    firstLine->firstLine();
    vertical.push_back(firstLine);
    this->addChild(firstLine);
    log("first line build done");

    score = 0;

    scoreLabel = Label::create();
    char str[100] = {'0'};
    sprintf(str, "%d", score);
    scoreLabel->setString(str);
    scoreLabel->setPosition(Point(VISIZE.width/2,VISIZE.height*0.92));
    scoreLabel->setSystemFontSize(VISIZE.height/15);

    this->addChild(scoreLabel,12);


    int lineCount = 0;

    /*init other line full screen*/
    while((vertical.size()-1)*VISIZE.width/RECT_NUM_WIDTH < VISIZE.height){
        lineCount++;
        Line* newLine = Line::create();
        this->addChild(newLine);
        if(lineCount == 3){
            vertical.back()->getRectByID(verticalID)->clear();
        }
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

    this->addChild(grap,GRAP_Z_ORDER);

    virtualGrap = Grap::create();
    virtualGrap->setOpacity(0);
    this->addChild(virtualGrap,GRAP_Z_ORDER);


    (*currentLine)->getRectByID(verticalID)->clear();

    auto pauseSprite = Sprite::create("settings.png");

    pauseSprite->setPosition(0,0);

    pauseSprite->setScale(VISIZE.width/pauseSprite->getContentSize().width/6);

    isPause = false;


    MenuItemSprite* pauseMenu = MenuItemSprite::create(pauseSprite, nullptr, [=](Ref* sender)
    {
        if(!isPause) {
            pauseSprite->setTexture(Director::getInstance()->getTextureCache()->addImage("settings-play.png"));
            pause();
            isPause = true;
        }
        else{
            pauseSprite->setTexture(Director::getInstance()->getTextureCache()->addImage("settings.png"));
            Myresume();
            isPause = false;
        }
    }
    );

    Menu* menu = Menu::create(pauseMenu, nullptr);

    menu->ignoreAnchorPointForPosition(true);

    ///pauseMenu->setPosition(VISIZE.width*0.8,VISIZE.height*0.9);

    menu->setPosition(VISIZE.width - pauseSprite->getBoundingBox().size.width,VISIZE.height-pauseSprite->getBoundingBox().size.height);



    this->addChild(menu,11);

    log("x:%f,y:%f",menu->getPosition().x,menu->getPosition().y);

    /*set touch enabled*/
    setTouchLayer(this);

    /*Shake call-func when run into Stone*/
    NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(DigScene::Shake),"shake", nullptr);
    NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(DigScene::addEnergy),"changeEnergy",nullptr);
    
    return true;
}

void DigScene::fail() {

    NotificationCenter::getInstance()->removeAllObservers(this);

    log("fail");
}

void DigScene::pause() {
    stop = Layer::create();

    stop->setPosition(Point(VISIZE.width,0));

    stop->runAction(MoveTo::create(0.3,Point(0,0)));

    this->addChild(stop,11);

    blackBG = Sprite::create("Black.jpg");
    blackBG->setPosition(VISIZE.width / 2, VISIZE.height / 2);
    blackBG->setScaleY(VISIZE.height/ blackBG->getContentSize().height);
    blackBG->setScaleX(VISIZE.width/ blackBG->getContentSize().width);
    blackBG->setOpacity(100);
    this->addChild(blackBG,10);

    Sprite* timeOut = Sprite::create("time-out.png");
    timeOut->setPosition(VISIZE.width / 2, VISIZE.height / 2);
    timeOut->setScale(VISIZE.width / timeOut->getContentSize().width/1.414);
    stop->addChild(timeOut,1);
    this->unschedule(schedule_selector(DigScene::updateHp));

    auto restart = MenuItemImage::create("menu_restart.png", "menu_restart.png", [=](Ref* sender)
    {
        log("restart");
        auto re = [=](){
            pause();

            listener->setEnabled(false);

            Director::getInstance()->replaceScene(DigScene::createScene());
        };
        stop->runAction(Sequence::create(MoveBy::create(0.3,Point(-VISIZE.width,0)),CallFunc::create(re), nullptr));
    }
    );

    restart->setPosition(Point(-VISIZE.width/1.414/2*0.6 + VISIZE.width/2,VISIZE.height/2 - timeOut->getBoundingBox().size.height/2+restart->getNormalImage()->getBoundingBox().size.height*0.7));
    restart->setScale(VISIZE.width/6/restart->getSelectedImage()->getContentSize().width);



    auto home = MenuItemImage::create("menu_home.png", "menu_home.png", [=](Ref* sender)
    {
        log("home");
        auto re = [=](){
            Director::getInstance()->replaceScene(DigScene::createScene());
        };
        stop->runAction(Sequence::create(MoveBy::create(0.3,Point(0,-VISIZE.height)),CallFunc::create(re), nullptr));
    }
    );

    home->setScale(VISIZE.width/6/home->getSelectedImage()->getContentSize().width);
    home->setPosition(VISIZE.width/2,VISIZE.height/2 - timeOut->getBoundingBox().size.height/2+home->getNormalImage()->getBoundingBox().size.height*0.7);


    auto sound = MenuItemImage::create("menu_voice.png", "menu_voice.png", [=](Ref* sender)
    {
        log("sound");
    }
    );

    sound->setPosition(Point(VISIZE.width*(1/1.414/2*0.6+0.5),VISIZE.height/2 - timeOut->getBoundingBox().size.height/2+sound->getNormalImage()->getBoundingBox().size.height*0.7));
    sound->setScale(VISIZE.width/6/sound->getSelectedImage()->getContentSize().width);

    auto menu = Menu::create(restart,home,sound, nullptr);
    menu->setPosition(0,0);
    stop->addChild(menu,12);
    stop->setTouchEnabled(true);
    menu->setEnabled(true);

}

void DigScene::Myresume() {

    auto re = [=](){
        this->removeChild(stop);
    };
    if(stop)
    stop->runAction(Sequence::create(MoveBy::create(0.3,Point(VISIZE.width,0)),CallFunc::create(re), nullptr));
    this->removeChild(blackBG);
    this->schedule(schedule_selector(DigScene::updateHp),1/60);
}

void DigScene::addEnergy(Ref *pSender) {
    RectMap* rect = (RectMap*)pSender;
    switch(rect->getType()){
        case BRICK:progressTimer->setPercentage(progressTimer->getPercentage()+BRICK_ENERGY);break;
        case STONE:progressTimer->setPercentage(progressTimer->getPercentage()+STONE_ENERGY);break;
        case DIAMOND:progressTimer->setPercentage(progressTimer->getPercentage()+DIAMOND_ENERGY);break;
    }
}
