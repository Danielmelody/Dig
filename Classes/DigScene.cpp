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
        virtualGrap->setPosition(grap->getPosition());
      return true;
    };

    listener->onTouchMoved = [=](Touch* touch,Event* event){

        Point move = touch->getLocation()-touch->getStartLocation();

        if(move.getDistance(Point(0,0))>VISIZE.width/50) {

            virtualGrap->getSprite()->setScale(VISIZE.width/RECT_NUM_WIDTH/virtualGrap->getSprite()->getContentSize().width);


            //grap->getSprite()->setRotation(-180 * (move.getAngle() + acos(-1) / 2) / acos(-1));


             virtualGrap->getSprite()->setOpacity(100);


            if (abs(move.x) > abs(move.y)) {
                virtualGrap->getSprite()->setRotation(move.x>0 ? -90 : 90);
                grap->getSprite()->setRotation(move.x>0 ? -90 : 90);
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
                    grap->getSprite()->setRotation(0);
                }

            }
        }
    };

    listener->onTouchEnded = [=](Touch* touch,Event* event){
        Point move = touch->getLocation()-touch->getStartLocation();
        if(move.getDistance(Point(0,0))>VISIZE.width/50) {
            log("end");

            if(isFirstTouch){
                SimpleAudioEngine::getInstance()->playBackgroundMusic(BG_MUSIC,true);
                isFirstTouch = false;
            }

            if (abs(move.x) > abs(move.y)) {
                grap->getSprite()->runAction(RotateTo::create(ACTION_INTERVAL, move.x > 0 ? -90 : 90));
                turnHorizen(move.x > 0 ? RIGHT : LEFT);
            }
            else {
                grap->getSprite()->runAction(RotateTo::create(ACTION_INTERVAL, 0));

                if (move.y < 0)turnDown();
            }
            virtualGrap->getSprite()->runAction(
                    Spawn::create(ScaleBy::create(ACTION_INTERVAL * 2, 2), FadeTo::create(ACTION_INTERVAL * 2, 0),
                                  nullptr));
        }

    };

    listener->onTouchCancelled = [=](Touch* touch,Event* event){

    };


    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,layer);


}

void DigScene::setPauseMenu(Sprite* sprite) {
    auto pauseListener = EventListenerTouchOneByOne::create();

    pauseListener->onTouchBegan = [=](Touch* touch,Event* event){

        if(sprite->getBoundingBox().containsPoint(sprite->convertToNodeSpace(touch->getLocation()))){
            pauseFlag = true;
            return true;
        }
        return false;
    };

    pauseListener->onTouchMoved = [=](Touch* touch,Event* event){
        if(!sprite->getBoundingBox().containsPoint(sprite->convertToNodeSpace(touch->getLocation()))){
            pauseFlag = false;
        }

    };

    pauseListener->onTouchEnded = [=](Touch* touch,Event* event){
        if(pauseFlag){
            pause();
        }
    };

    pauseListener->onTouchCancelled = [=](Touch* touch,Event* event){

    };


    _eventDispatcher->addEventListenerWithSceneGraphPriority(pauseListener,sprite);


}



bool DigScene::Crash(list<Line*>::iterator nextLine,int nextverID) {
    if (nextverID >= 0 && nextverID < RECT_NUM_WIDTH) {
        if ((*nextLine)->getRectByID(nextverID)->getType() != STONE) {
            log("true");
            return false;
        }
        NotificationCenter::getInstance()->postNotification("shake");
        (*nextLine)->getRectByID(nextverID)->beDiggedInstantly();
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
    effect(0);
}

void DigScene::turnHorizen(int direction) {
    if(Crash(currentLine,verticalID + direction)){
        return;
    }
    verticalID += direction;
    effect(direction);
}

void DigScene::effect(int direction) {
    int nextLine = GRAP_START_LINE;
    if(direction == 0){
        //nextLine++;
    }

    grap->Dig(direction);

    (*currentLine)->getRectByID(verticalID)->beDiggedInstantly();

    exploInSeq(nextLine,verticalID);

}

void DigScene::exploInSeq(int line, int ID) {
    log("%d,%d",line,ID);
    if(line >= vertical.size() || line <0 ){
        return;
    }
    if(ID < 0 || ID >= RECT_NUM_WIDTH){
        return;
    }
    auto lineIt = vertical.begin();
    advance(lineIt,line);
    if((*lineIt)->getRectByID(ID)->getType() == FIRE) {
        for (auto point: grap->getBrickEffect(line, ID,FIRE)) {
            if(line != point[0] || ID != point[1]) {
                exploInSeq(point[0], point[1]);
            }
            clearBricks(point[0], point[1]);
            //log("explo: %d,%d",point[0],point[1]);
        }
    }
    else {
        clearBricks(line,ID);
    }

}

void DigScene::Shake(Ref *pSender) {
    log("shake");
    this->runAction(Sequence::create(CCShake::create(0.5,VISIZE.width/30),CallFunc::create([=](){
        this->setRotation(0);
    }), nullptr));
}


void DigScene::updateHp(float dt) {
    hp = MIN(hp,100);
    hp -= hpSpeed;
    hpSpeed += HP_UPDATE_ADD;
    hpSpeed = MIN(hpSpeed,3);
    float nowhp = progressTimer->getPercentage();
    progressTimer->setPercentage(hp);
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


void DigScene::onEnter(){
    Layer::onEnter();
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic(BG_MUSIC);
    isFirstTouch = true;
}

// on "init" you need to initialize your instance
bool DigScene::init(){

    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    extraDig = [&](int direction){};

    hp = 100;
    hpSpeed = HP_UNIT_UPDATE;
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

    verticalID = 2;

    int lineCount = 0;

    /*init other line full screen*/
    while((vertical.size()-1)*VISIZE.width/RECT_NUM_WIDTH < VISIZE.height){
        lineCount++;
        Line* newLine = Line::create();
        this->addChild(newLine);
        if(lineCount == 4){
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

    RectMap* target = (*currentLine)->getRectByID(verticalID);

    grap->setPosition((target->getPosition())+(*currentLine)->getPosition());

    this->addChild(grap,GRAP_Z_ORDER);

    virtualGrap = Grap::create();
    virtualGrap->setOpacity(0);
    this->addChild(virtualGrap,GRAP_Z_ORDER);


    (*currentLine)->getRectByID(verticalID)->clear();

    pauseSprite = Sprite::create("settings.png");
    pauseSprite->setPosition(VISIZE.width - VISIZE.width/6,VISIZE.height-VISIZE.width/6);
    pauseSprite->setScale(VISIZE.width/pauseSprite->getContentSize().width/6);
    this->addChild(pauseSprite,10);

    /*set touch enabled*/
    setTouchLayer(this);

    /*set Menu Enabled*/
    setPauseMenu(pauseSprite);

    /*Shake call-func when run into Stone*/
    NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(DigScene::Shake),"shake", nullptr);
    NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(DigScene::addEnergy),"changeEnergy",nullptr);
    //NotificationCenter::getInstance()->addObserver(this,callfuncO_selector())


    return true;
}




void DigScene::fail() {


    SimpleAudioEngine::getInstance()->playEffect(FAIL_AUDIO);

    pause();
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    log("bg is ok");
    pauseSprite->setVisible(false);
    //pauseSprite->setEnabled(false);

}

void DigScene::pause(){

    stop = Layer::create();
    stop->setPosition(Point(VISIZE.width,0));
    stop->runAction(EaseBackOut::create(MoveTo::create(PAUSE_TIME,Point(0,0))));

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

        SimpleAudioEngine::getInstance()->playEffect(RESTART_AUDIO);

        auto re = [=](){

            listener->setEnabled(false);

            NotificationCenter::getInstance()->removeAllObservers(this);

            Director::getInstance()->replaceScene(DigScene::createScene());

        };
        stop->runAction(Sequence::create(EaseBackIn::create(MoveBy::create(PAUSE_TIME,Point(-VISIZE.width,0))),CallFunc::create(re), nullptr));
    }
    );

    restart->setPosition(Point(-VISIZE.width/1.414/2*0.6 + VISIZE.width/2,VISIZE.height/2 - timeOut->getBoundingBox().size.height/2+restart->getNormalImage()->getBoundingBox().size.height));
    restart->setScale(VISIZE.width/6/restart->getSelectedImage()->getContentSize().width);



    auto home = MenuItemImage::create("menu_home.png", "menu_home.png", [=](Ref* sender)
    {
        log("home");
        auto re = [=](){
            Director::getInstance()->replaceScene(TransitionSlideInT::create(0.3,HomeScene::create()));
        };
        stop->runAction(Spawn::create(MoveBy::create(0.3,Point(0,-VISIZE.height)),CallFunc::create(re), nullptr));
    }
    );

    home->setScale(VISIZE.width/6/home->getSelectedImage()->getContentSize().width);
    home->setPosition(VISIZE.width/2,VISIZE.height/2 - timeOut->getBoundingBox().size.height/2+home->getNormalImage()->getBoundingBox().size.height);


    auto sound = MenuItemImage::create("menu_voice.png", "menu_voice.png", [=](Ref* sender)
    {
        log("sound");
    }
    );

    sound->setPosition(Point(VISIZE.width*(1/1.414/2*0.6+0.5),VISIZE.height/2 - timeOut->getBoundingBox().size.height/2+sound->getNormalImage()->getBoundingBox().size.height));
    sound->setScale(VISIZE.width/6/sound->getSelectedImage()->getContentSize().width);

    auto menu = Menu::create(restart,home,sound, nullptr);
    menu->setPosition(0,0);
    stop->addChild(menu,12);
    stop->setTouchEnabled(true);
    menu->setEnabled(true);
    listener->setEnabled(false);

    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();

    log("end of pause");

}

void DigScene::Myresume() {

    auto re = [=](){
        this->removeChild(stop);
    };
    if(stop)
    stop->runAction(Sequence::create(MoveBy::create(0.3,Point(VISIZE.width,0)),CallFunc::create(re), nullptr));
    this->removeChild(blackBG);
    this->schedule(schedule_selector(DigScene::updateHp),1/60);
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

void DigScene::addEnergy(Ref *pSender) {

    RectMap* rect = (RectMap*)pSender;
    log("real change: type = %d",rect->getType());

    switch(rect->getType()){
        case BRICK:hp += BRICK_ENERGY;break;
        case STONE:hp += STONE_ENERGY;break;
        case DIAMOND:hp += DIAMOND_ENERGY;break;
        case FIRE:hp += FIRE_ENERGY;break;
    }
}

bool DigScene::clearBricks(int line, int id){
    if(line >= vertical.size() || id < 0 || id >= RECT_NUM_WIDTH){
        return false;
    }
    list<Line*>::iterator targetLine = vertical.begin();
    advance(targetLine,line);
    (*targetLine)->getRectByID(id)->beDigged();
}



