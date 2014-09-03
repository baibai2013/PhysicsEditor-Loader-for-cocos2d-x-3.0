//
//  MemoryCardsModel.h
//  MemoryCards_CPP
//
//  Created by lili on 14-8-26.
//
//

#ifndef __MemoryCards_CPP__MemoryCardsModel__
#define __MemoryCards_CPP__MemoryCardsModel__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;
/**
 * 游戏模型层 逻辑处理
 */
class MemoryCardsModel:public Ref
{
public:
    MemoryCardsModel();
    virtual ~MemoryCardsModel();
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
};
#endif /* defined(__MemoryCards_CPP__MemoryCardsModel__) */
