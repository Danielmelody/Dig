#ifndef MYGAME_DATADEFINES_H
#define MYGAME_DATADEFINES_H

#include "cocos2d.h"

USING_NS_CC;
#define ACTION_INTERVAL 0.1
#define VOLUME_ACTION 0.01
#define PAUSE_TIME 0.5
#define RECT_NUM_WIDTH 6
#define VISIZE (Director::getInstance()->getVisibleSize())
#define LEFT -1
#define RIGHT 1
#define BRICK 1
#define STONE 2
#define DIAMOND 3
#define FIRE 4
#define LIGHT 5
#define HP_UNIT_UPDATE 0.2
#define HP_UPDATE_ADD 0.02
#define BRICK_ENERGY 0.8
#define STONE_ENERGY -10
#define DIAMOND_ENERGY 15
#define FIRE_ENERGY BRICK_ENERGY


#define GRAP_Z_ORDER 5

#define MAX_RANK_LIST 20


#endif //MYGAME_DATADEFINES_H
