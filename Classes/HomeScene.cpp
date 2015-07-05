//
// Created by huyiming on 15-5-11.
//

#include "HomeScene.h"
#include "Audio.h"

void HomeScene::onEnter(){
    Node::onEnter();
    SimpleAudioEngine::getInstance()->playBackgroundMusic(S_BG_MUSIC, true);
}


bool HomeScene::init(){
    if (!Scene::init()){
        return false;
    }

    SimpleAudioEngine::getInstance()->preloadEffect(RESTART_AUDIO);
    SimpleAudioEngine::getInstance()->preloadEffect(DIG_AUDIO);
    SimpleAudioEngine::getInstance()->preloadEffect(MINE1_AUDIO);
    SimpleAudioEngine::getInstance()->preloadEffect(BUTTON);
    SimpleAudioEngine::getInstance()->preloadEffect(FAIL_AUDIO);
    SimpleAudioEngine::getInstance()->preloadEffect(CRASH_AUDIO);


    Sprite *BG = Sprite::create("background.png");
    BG->setPosition(VISIZE.width / 2, VISIZE.height / 2);
    BG->setScaleX(VISIZE.width / BG->getContentSize().width);
    BG->setScaleY(VISIZE.height / BG->getContentSize().height);

    this->addChild(BG,0);


    Sprite* dig_logo = Sprite::create("dig_logo.png");

    dig_logo->setPosition(VISIZE.width/2,VISIZE.height*0.7);
    dig_logo->setScale(VISIZE.width/dig_logo->getBoundingBox().size.width);
    this->addChild(dig_logo);


    auto playMenu = Sprite::create("pl.png");
    playMenu->runAction(MoveBy::create(0.5,Point(0,-playMenu->getBoundingBox().size.height/2)));
    playMenu->setPosition(dig_logo->getContentSize().width/2,dig_logo->getContentSize().height/2);
    dig_logo->addChild(playMenu);
    playMenu->setGlobalZOrder(3);

    auto listenner = EventListenerTouchOneByOne::create();

    listenner->onTouchBegan = [=](Touch* touch,Event* event){

        if(playMenu->getBoundingBox().containsPoint(playMenu->getParent()->convertToNodeSpace(touch->getLocation()))){
            playMenu->setOpacity(0);
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

            playMenu->setOpacity(255);


            playMenu->runAction(Sequence::create(Spawn::create(MoveBy::create(0.5,Point(0,playMenu->getBoundingBox().size.height/2)),FadeTo::create(0.5,0),nullptr),
            CallFunc::create([=](){

                Director::getInstance()->replaceScene(TransitionSlideInB::create(0.5,DigScene::createScene()));

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

