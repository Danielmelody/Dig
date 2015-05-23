//
// Created by huyiming on 15-5-14.
//

#ifndef MYGAME_AUDIO_H
#define MYGAME_AUDIO_H

#include"cocos2d.h"
#include"audio/include/SimpleAudioEngine.h"

using namespace CocosDenshion;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
#define PLATFROM_ADDS ".ogg"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
#define PLATFROM_ADDS ".ogg"
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#define  PLATROM_ADDS ".aiff"
#endif

#define RESTART_AUDIO "audio/Restart"PLATFROM_ADDS
#define DIG_AUDIO "audio/dig"PLATFROM_ADDS
#define MINE1_AUDIO "audio/Mine1"PLATFROM_ADDS

#endif //MYGAME_AUDIO_H
