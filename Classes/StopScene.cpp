//
// Created by huyiming on 15-5-10.
//

#include "StopScene.h"

bool StopScene::init() {
    if (!Scene::init()) {
        return false;
    }
    Sprite *timeOut = Sprite::create("time-out.png");
    timeOut->setPosition(VISIZE.width / 2, VISIZE.height / 2);
    timeOut->setScale(VISIZE.width / timeOut->getContentSize().width / 2);
    this->addChild(timeOut);
    return true;
}
