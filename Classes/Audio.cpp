//
//  Audio.cpp
//  Dig
//
//  Created by 胡一鸣 on 15/7/11.
//
//

#include "Audio.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
string Audio::PLATFROM_ADDS = ".ogg";
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
string Audio::PLATFROM_ADDS = ".ogg"
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
string Audio::PLATFROM_ADDS = ".aiff";
#endif



string Audio::RESTART_AUDIO = "audio/Restart" + Audio::PLATFROM_ADDS;
string Audio::DIG_AUDIO = "audio/dig" + Audio::PLATFROM_ADDS;
string Audio::MINE1_AUDIO = "audio/Mine1" + Audio::PLATFROM_ADDS;
string Audio::BUTTON = "audio/Buttun" + Audio::PLATFROM_ADDS;
string Audio::FAIL_AUDIO = "audio/Restart" + Audio::PLATFROM_ADDS;
string Audio::CRASH_AUDIO = "audio/Crash" + Audio::PLATFROM_ADDS;
string Audio::BG_MUSIC = "audio/BGM.mp3";
string Audio::S_BG_MUSIC = "audio/startBGM.mp3";