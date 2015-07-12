//
// Created by huyiming on 15-5-11.
//

#include "HomeScene.h"
#include "Audio.h"

void HomeScene::onEnter(){
    Node::onEnter();
    SimpleAudioEngine::getInstance()->playBackgroundMusic(Audio::S_BG_MUSIC.c_str(), true);
    MoveBy* c2Move = MoveBy::create(2,Point(VISIZE.width/10,0));
    log("action");
    c2->runAction(RepeatForever::create(Sequence::create(c2Move,c2Move->reverse(), nullptr)));
    dig_logo->setPosition(VISIZE.width/2,VISIZE.height*1.5);
    dig_logo->runAction(EaseBackOut::create(MoveTo::create(0.5,Point(VISIZE.width/2,VISIZE.height*0.6))));
}


bool HomeScene::init(){
    if (!Scene::init()){
        return false;
    }

    SimpleAudioEngine::getInstance()->preloadEffect(Audio::RESTART_AUDIO.c_str());
    SimpleAudioEngine::getInstance()->preloadEffect(Audio::DIG_AUDIO.c_str());
    SimpleAudioEngine::getInstance()->preloadEffect(Audio::MINE1_AUDIO.c_str());
    SimpleAudioEngine::getInstance()->preloadEffect(Audio::BUTTON.c_str());
    SimpleAudioEngine::getInstance()->preloadEffect(Audio::FAIL_AUDIO.c_str());
    SimpleAudioEngine::getInstance()->preloadEffect(Audio::CRASH_AUDIO.c_str());


    Sprite *BG = Sprite::create("background.jpg");
    BG->setPosition(VISIZE.width / 2, VISIZE.height / 2);
    BG->setScaleX(VISIZE.width / BG->getContentSize().width);
    BG->setScaleY(VISIZE.height / BG->getContentSize().height);
    this->addChild(BG,0);

    c2 = Sprite::create("cloud2.png");
    c2->setPosition(VISIZE.width * 0.2, VISIZE.height * 0.4);
    c2->setScale(VISIZE.width / c2->getContentSize().width);
    this->addChild(c2,2);

    Sprite *star = Sprite::create("Brick.jpg");
    star->setPosition(VISIZE.width / 2, VISIZE.height / 2);
    star->setScaleX(VISIZE.width / star->getContentSize().width);
    star->setScaleY(VISIZE.height / star->getContentSize().height);
    this->addChild(star,3);


    dig_logo = Sprite::create("dig_logo.png");

    dig_logo->setPosition(VISIZE.width/2,VISIZE.height*0.6);
    dig_logo->setScale(VISIZE.width/dig_logo->getBoundingBox().size.width);
    this->addChild(dig_logo,4);


    auto playMenu = Sprite::create("pl.png");
    playMenu->runAction(MoveBy::create(0.5,Point(0,-playMenu->getBoundingBox().size.height/2)));
    playMenu->setPosition(dig_logo->getContentSize().width/2,dig_logo->getContentSize().height/2);
    dig_logo->addChild(playMenu);

    auto listenner = EventListenerTouchOneByOne::create();

    listenner->onTouchBegan = [=](Touch* touch,Event* event){

        if(playMenu->getBoundingBox().containsPoint(playMenu->getParent()->convertToNodeSpace(touch->getLocation()))){
            playMenu->setScale(playMenu->getScale()*1.2);
            isOut = false;
            log("ok");
            return true;
        }
        return false;
    };

    listenner->onTouchMoved = [=](Touch* touch,Event* event){
        if(playMenu->getBoundingBox().containsPoint(playMenu->convertToNodeSpace(touch->getLocation()))){
            isOut = true;
        }
        else{
            isOut = false;
        }
    };

    listenner->onTouchEnded = [=](Touch* touch,Event* event){
        if(!isOut){
            int beforeReplpace = 0.5;
            playMenu->setScale(playMenu->getScale()/1.2);
            dig_logo->runAction(EaseBackOut::create(MoveTo::create(beforeReplpace,Point(VISIZE.width/2,VISIZE.height*1.5))));
            playMenu->runAction(Sequence::create(EaseBackOut::create(MoveBy::create(beforeReplpace,Point(0,0))),
            CallFunc::create([=](){

                //this->runAction(RuduceVolume::create(0.3));
                Director::getInstance()->replaceScene(TransitionSlideInB::create(0.3,DigScene::createScene()));

            }), nullptr));


        }
    };


    playMenu->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenner,dig_logo);



    return true;
}

void HomeScene::replaceScene(Ref *pRect) {
    this->runAction(RuduceVolume::create(0.5));
    Director::getInstance()->replaceScene(TransitionRotoZoom::create(0.5,DigScene::createScene()));
}

