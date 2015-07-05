//
// Created by huyiming on 15-7-5.
//

#ifndef MYGAME_RUDUCEVOLUME_H
#define MYGAME_RUDUCEVOLUME_H

#include "cocos2d.h"
#include "Audio.h"
#include "DataDefines.h"

USING_NS_CC;

class RuduceVolume:public ActionInterval {
public:
    static RuduceVolume* create(float duration);
    bool initWithDuration(float duration);
private:
    void update(float time) override;
    float startVolume,step;
};


#endif //MYGAME_RUDUCEVOLUME_H
