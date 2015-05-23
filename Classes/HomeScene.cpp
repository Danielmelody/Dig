//
// Created by huyiming on 15-5-11.
//

#include "HomeScene.h"

bool HomeScene::init() {
    if (!Scene::init()) {
        return false;
    }
    Sprite *BG = Sprite::create("background.png");
    BG->setPosition(VISIZE.width / 2, VISIZE.height / 2);
    BG->setScaleX(VISIZE.width / BG->getContentSize().width);
    BG->setScaleY(VISIZE.height / BG->getContentSize().height);
    this->addChild(BG,0);
    return true;
}