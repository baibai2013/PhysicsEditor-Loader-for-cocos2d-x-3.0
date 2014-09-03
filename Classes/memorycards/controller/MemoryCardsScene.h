//
//  MemoryCardsScene.h
//  MemoryCards_CPP
//
//  Created by lili on 14-8-26.
//
//

#ifndef __MemoryCards_CPP__MemoryCardsScene__
#define __MemoryCards_CPP__MemoryCardsScene__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;
/**
 * 游戏控制器 控制处理
 */
class MemoryCardsScene:public Scene
{
public:
    static Scene *create();
    virtual bool init() override;
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
};
#endif /* defined(__MemoryCards_CPP__MemoryCardsScene__) */
