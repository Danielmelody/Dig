//
// Created by huyiming on 15-7-5.
//

#include "RuduceVolume.h"

void RuduceVolume::update(float dt) {

    if (_target&&(SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying() || SimpleAudioEngine::getInstance()->getBackgroundMusicVolume() > 0))
    {
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(
                SimpleAudioEngine::getInstance()->getInstance()->getBackgroundMusicVolume() - step
        );
    }
}

RuduceVolume *RuduceVolume::create(float duration) {
    RuduceVolume *ruduceVolume = new (std::nothrow) RuduceVolume();
    ruduceVolume->initWithDuration(duration);
    ruduceVolume->autorelease();
}

bool RuduceVolume::initWithDuration(float duration) {
    if (ActionInterval::initWithDuration(duration))
    {
        startVolume = SimpleAudioEngine::getInstance()->getBackgroundMusicVolume();
        step = startVolume/duration;
        return true;
    }
    return false;
}
