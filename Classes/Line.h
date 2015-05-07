

#ifndef MYGAME_LINE_H
#define MYGAME_LINE_H

#include "cocos2d.h"
#include "vector"
#include "RectMap.h"
#include "DataDefines.h"
#include "Tools.h"

using namespace std;
USING_NS_CC;



class Line :public Node{
public:
    bool init();
    //create new line based on last line,ensuring there is no less than 1 way to go
    void initWithLastLine(Line* lastLine);
    //make sure there exist a road to go for players
    void convertToBrick(int ID);
    //init firrst line
    void firstLine();

    inline RectMap* getRectByID(int ID){ return rects[ID];}
    //when dig toward down all Bricks and Stones will run Action up
    void up();
    CREATE_FUNC(Line);
private:
    Point lastTargetPos;
    vector<RectMap*> rects;
};


#endif //MYGAME_LINE_H
