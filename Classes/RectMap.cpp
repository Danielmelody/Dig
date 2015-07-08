//
// Created by huyiming on 15-5-7.
//

#include "RectMap.h"



bool RectMap::init() {
    if(!Node::init()) {
        return false;
    }
    return true;
}

void RectMap::beDiggedInstantly() {
    __NotificationCenter::getInstance()->postNotification("changeEnergy",this);
    log("changeEnergy: type = %d ",this->getType());
    beDigged();
}

void RectMap::beDigged(){
    if(!empty){
        hp--;
    }
}

void RectMap::clear(){
    if(_sprite){
        this->removeChild(_sprite);
        hp = 0;
        empty = true;
    }
}



bool Brick::init() {
    if(!RectMap::init()){
        return false;
    }
    type = BRICK;
    hp = 1;
    _sprite = Sprite::create("Brick.jpg");
    _sprite->setScale(VISIZE.width/RECT_NUM_WIDTH/_sprite->getContentSize().width);
    this->addChild(_sprite);
    return true;
}

void Brick::beDiggedInstantly() {
    int beforeType = this->getType();
    RectMap::beDiggedInstantly();
    beDigged();
    type = beforeType;
}

void Brick::beDigged() {
    __NotificationCenter::getInstance()->postNotification("changeEnergy",this);
    RectMap::beDigged();
    if(hp == 0&&!empty){
        _sprite->setVisible(false);
        auto explo = ParticleExplosion::create();
        explo->setTexture(Director::getInstance()->getTextureCache()->addImage("Brick.jpg"));
        explo->setLife(ACTION_INTERVAL);
        explo->setLifeVar(ACTION_INTERVAL);
        explo->setTotalParticles(60);
        explo->setStartSize(VISIZE.width/RECT_NUM_WIDTH/5);
        explo->setStartSizeVar(VISIZE.width/RECT_NUM_WIDTH/2/5/2);
        explo->setSpeed(VISIZE.width/RECT_NUM_WIDTH*3);
        explo->setSpeedVar(explo->getSpeed());
        explo->setEndColor(Color4F(1,1,1,1));
        this->addChild(explo);
        explo->setGlobalZOrder(5);
        explo->setPosition(_sprite->getPosition());
        empty = true;
    }
    type = BRICK;
}

bool Stone::init(){
    if(!RectMap::init()){
        return false;
    }
    type = STONE;
    hp = 3;
    _sprite = Sprite::create("Stone.png");
    _sprite->setScale(VISIZE.width/RECT_NUM_WIDTH/_sprite->getContentSize().width);
    this->addChild(_sprite);
    return true;
}

void Stone::beDiggedInstantly() {
    RectMap::beDiggedInstantly();
    SimpleAudioEngine::getInstance()->playEffect(CRASH_AUDIO);
}

void Stone::beDigged() {
    RectMap::beDigged();
}

bool Ore::init() {
    if(!Brick::init()){
        return false;
    }
    return true;
}

void Ore::clear() {
    RectMap::clear();
    _extra->setOpacity(0);
    type = BRICK;
}

void Ore::beDigged() {
    switch (type){
        case DIAMOND:
            SimpleAudioEngine::getInstance()->playEffect(MINE1_AUDIO);
            break;
        case FIRE:
            BlendFunc dark = { GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA };


            auto fire = ParticleExplosion::create();

            fire->setStartColor(Color4F(1, 1, 0, 1));
            fire->setStartColorVar(Color4F(0, 0, 0, 0));
            fire->setEndColor(Color4F(1, 0, 0, 0.5));
            fire->setEndColorVar(Color4F(0, 0, 0, 0));
            fire->setEndSize(Director::getInstance()->getVisibleSize().height);
            fire->setBlendAdditive(true);
            fire->setLife(0.3);
            fire->setLifeVar( 0.3);
            fire->setSpeed(Director::getInstance()->getVisibleSize().width / 100);
            fire->setSpeedVar(Director::getInstance()->getVisibleSize().width / 100);
            fire->setEmissionRate(60);
            fire->setPosition(_sprite->getPosition());

            auto fireDark = ParticleExplosion::create();
            fireDark->setStartColor(Color4F(1, 0.3, 0, 0));
            fireDark->setStartColorVar(Color4F(0, 0, 0, 0));
            fireDark->setEndColor(Color4F(0, 0, 0,1));
            fireDark->setEndColorVar(Color4F(0, 0, 0, 0));
            fireDark->setEndSize(Director::getInstance()->getVisibleSize().height / 10);
            fireDark->setBlendAdditive(true);
            fireDark->setLife(0.3);
            fireDark->setLifeVar(0.3);
            fireDark->setSpeed(Director::getInstance()->getVisibleSize().width / 20);
            fireDark->setEmissionRate(1500);
            fireDark->setBlendFunc(dark);
            fireDark->setPosition(_sprite->getPosition());

            //this->addChild(fireDark);
            fireDark->setGlobalZOrder(50);
            this->addChild(fire);
            fire->setGlobalZOrder(51);

            fire->setPositionType(ParticleSystem::PositionType::GROUPED);
            fireDark->setPositionType(ParticleSystem::PositionType::GROUPED);

            SimpleAudioEngine::getInstance()->playEffect(MINE1_AUDIO);
            NotificationCenter::getInstance()->postNotification("equip",this);
            break;
    }
    Brick::beDigged();
    this->removeChild(_extra);

}

void Ore::setOreType(int type) {
    this->type = type;
    switch (type){
        case DIAMOND:
            _extra = Sprite::create("ex-rock.png");
            break;
        case FIRE:
            _extra = Sprite::create("f-rock.png");
            break;
        }
    _extra->setScale(VISIZE.width/RECT_NUM_WIDTH/_extra->getContentSize().width);
    _extra->setPosition(_sprite->getPosition());
    this->addChild(_extra);
}

