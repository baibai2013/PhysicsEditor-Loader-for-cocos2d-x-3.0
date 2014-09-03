//
//  MemoryCardsLayer.h
//  MemoryCards_CPP
//
//  Created by lili on 14-8-26.
//
//

#ifndef __MemoryCards_CPP__MemoryCardsLayer__
#define __MemoryCards_CPP__MemoryCardsLayer__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;
class MemoryCardsLayer:public Layer
{
public:
    CREATE_FUNC(MemoryCardsLayer);
    
    virtual ~MemoryCardsLayer();
    MemoryCardsLayer();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
};
#endif /* defined(__MemoryCards_CPP__MemoryCardsLayer__) */
