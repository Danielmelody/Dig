//
// Created by huyiming on 15-5-8.
//

#include "CicleAction.h"


bool CircleAlltime::init(){
    if (!Node::init())return false;
    this->schedule(schedule_selector(CircleAlltime::updatePos), 1 / 60);
    unitAngle = 2 * PI * 1 / 60 / _timeACircle;
    dis = (_node->getPosition()).getDistance(_center);
    alpha = acos((_node->getPosition().x - _center.x) / dis);
    if (_node->getPosition().y < _center.y){
        alpha = -alpha;
    }
    return true;
}
void CircleAlltime::updatePos(float dt){
    totalTime += dt;
    if(totalTime >_timeACircle){
        _node->removeChild(this);
    }
    if (alpha < -PI)alpha = PI;
    else{
        if (alpha> PI)alpha = -PI;
    }
    alpha += _clockwise*unitAngle;
    _node->setPosition(_center+Point(dis*cos(alpha), dis*sin(alpha)));
}