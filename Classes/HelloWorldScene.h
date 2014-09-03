#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

#define CL(__className__) [](){ return __className__::create();}
#define CLN(__className__) [](){ auto obj = new __className__(); obj->autorelease(); return obj; }




class HelloWorld : public cocos2d::Layer
{
public:
    
    static Scene* createScene();
    
    CREATE_FUNC(HelloWorld);
    
    void menuCloseCallback(cocos2d::Ref* pSender);
    void onEnter() override;
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    void addPhysicsSprite();//添加物理精灵
    void onAcceleration(Acceleration* acc, Event* event);//重力感应处理
    bool onContactBegin(PhysicsContact& contact);//物理碰撞只执行一次
    void onContactPostSolve(PhysicsContact& contact,const PhysicsContactPostSolve& solve);//物理碰撞中回调函数
protected:
    Scene* _scene;//父类场景
    std::unordered_map<int, Node*> _mouses;//点击的添加的点
};

#endif // __HELLOWORLD_SCENE_H__
