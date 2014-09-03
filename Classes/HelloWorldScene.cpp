#include "HelloWorldScene.h"
#include "VisibleRect.h"
#include "ResourcesPath.h"
#include "PEShapeCache_X3_0.h"
USING_NS_CC;
static const int DRAG_BODYS_TAG = 0x79;
static const int DRAG_BODYS_TAG_Y = 0x80;
static const int DRAG_BODYS_TAG1 = 0x81;
Scene *HelloWorld::createScene()
{
    auto scene = Scene::createWithPhysics();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    //    Device::setAccelerometerEnabled(true);
    return scene;
}
void HelloWorld::onEnter()
{
    Layer::onEnter();
    //初始化父节点
    _scene = dynamic_cast<Scene *>(this->getParent());
    //注册触摸监听器
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    //    auto accListener = EventListenerAcceleration::create(CC_CALLBACK_2(HelloWorld::onAcceleration, this));初始一个重力感应监听
    //    _eventDispatcher->addEventListenerWithSceneGraphPriority(accListener, this);注册重力感应监听
    //设置物理世界
    _scene->getPhysicsWorld()->setGravity(Vect(0, 0));                        //设置重力
    _scene->getPhysicsWorld()->setSpeed(1.0f);                                //物理世界速度 速度越快刚体越快
    _scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL); //绘制调试绘制
                                                                              //添加拼图模板图片
    auto bg = Sprite::create(shizi_di_png);
    bg->setPosition(VisibleRect::center());
    this->addChild(bg);
    
    PEShapeCache::getInstance()->addBodysWithFile(buttonsbodys_plist);
    
    
    this->addPhysicsSprite();
}
void HelloWorld::addPhysicsSprite()
{
    //    _scene->getPhysicsWorld()->setUpdateRate(5.0f);
    // wall 添加物理边境
    Size size = VisibleRect::getVisibleRect().size;
    auto wall = Node::create();
    //给节点添加静态矩形刚体（PhysicsBody）并带材质（PhysicsMaterial）
    wall->setPhysicsBody(PhysicsBody::createEdgeBox(Size(size.width - 5, size.height - 5), PhysicsMaterial(0.1f, 1.0f, 1.0f)));
    //    wall->getPhysicsBody()->setDynamic(false);//设置为静态刚体（Edge
    //    wall->getPhysicsBody()->setEnable(false);
    wall->getPhysicsBody()->setGroup(1); //组编号
    /**一个body的CategoryBitmask和另一个body的ContactTestBitmask的逻辑与的结果不等于0时，接触事件将被发出，否则不发送。
     *一个body的CategoryBitmask和另一个body的CollisionBitmask的逻辑与结果不等于0时，他们将碰撞，否则不碰撞
     */
    //    wall->getPhysicsBody()->setCategoryBitmask(0x03);//  0011  碰撞系数编号
    //    wall->getPhysicsBody()->setContactTestBitmask(0x03);// 0001        碰撞检测编号
    //    wall->getPhysicsBody()->setCollisionBitmask(0x01);// 0001          碰撞编号
    wall->setPosition(VisibleRect::center()); //位置可见区域中心
    this->addChild(wall);
    
        //多纳的头
    auto tou = Sprite::create(shizi_tou_png);
//    tou->setPhysicsBody(PhysicsBody::createCircle(271, PhysicsMaterial(1000.1f, 0.0f, 1.0f)));//添加半径为271动态圆形刚体 并赋予材质密度1000.1f 反弹力0.0f 摩擦力1.0f
    auto toubody = PEShapeCache::getInstance()->getPhysicsBodyByName("shizi_tou");
    tou->setPhysicsBody(toubody);
    tou->getPhysicsBody()->setTag(DRAG_BODYS_TAG);//给刚体设置标签
    tou->getPhysicsBody()->setMass(0.1);//刚体设置质量
    tou->getPhysicsBody()->setGroup(2);//刚体组编号
    tou->getPhysicsBody()->setCategoryBitmask(0x01);    //  0001
    tou->getPhysicsBody()->setContactTestBitmask(0x01); // 0001
    tou->getPhysicsBody()->setCollisionBitmask(0x01);   // 0001
    //    tou->getPhysicsBody()->setDynamic(false); 动态刚体是可以设置为静态
    tou->getPhysicsBody()->setRotationEnable(false); //设置不可旋转刚体 碰撞后也不会旋转刚体
        //    tou->getPhysicsBody()->setGravityEnable(false);//设置是否接受重力影响
    tou->getPhysicsBody()->setLinearDamping(3.0f); //设置线性阻尼系数 理论是0-1 但是可以大于1 值越大惯性越小
    this->addChild(tou);
    tou->setPosition(VisibleRect::center());
    auto touyingzi = Node::create();
    touyingzi->setPhysicsBody(PhysicsBody::createCircle(271, PhysicsMaterial(1000.1f, 0.0f, 1.0f)));
    touyingzi->getPhysicsBody()->setTag(DRAG_BODYS_TAG_Y);
    
        //多纳的身子
        //PhysicsShapePolygon 是通过点数组来构建不规则的凸多边形；用工具PhysicsEditor 编辑shap 让后导出Chipmunk 格式的plist 中的数据 注：PhysicsEditor Relative为锚点Anchor  设置为cocos默认值（0.5，0.5）才行 ，
    auto shengzi = Sprite::create(shizi_shenzi_png);
//    auto shengzibody = PhysicsBody::create();
//    Point vert1[3] = {Point(109.50000, 71.00000), Point(14.00000, 77.00000), Point(117.50000, 147.00000)};
//    shengzibody->addShape(PhysicsShapePolygon::create(vert1, 3, PhysicsMaterial(0.1f, 0.0f, 1.0f)));
//    Point vert2[6] = {Point(-130.50000, -154.00000), Point(-120.50000, 46.00000), Point(-67.50000, 102.00000), Point(14.00000, 77.00000), Point(-4.00000, -93.00000), Point(-63.00000, -178.50000)};
//    shengzibody->addShape(PhysicsShapePolygon::create(vert2, 6, PhysicsMaterial(0.1f, 0.0f, 1.0f)));
//    Point vert3[6] = {Point(138.50000, 18.00000), Point(110.50000, -177.00000), Point(51.50000, -175.00000), Point(-4.00000, -93.00000), Point(14.00000, 77.00000), Point(109.50000, 71.00000)};
//    shengzibody->addShape(PhysicsShapePolygon::create(vert3, 6, PhysicsMaterial(0.1f, 0.0f, 1.0f)));
//    Point vert4[4] = {Point(-67.50000, 102.00000), Point(-55.00000, 172.50000), Point(-54.00000, 172.50000), Point(14.00000, 77.00000)};
//    shengzibody->addShape(PhysicsShapePolygon::create(vert4, 4, PhysicsMaterial(0.1f, 0.0f, 1.0f)));
    
    auto shengzibody = PEShapeCache::getInstance()->getPhysicsBodyByName("shizi_shenzi");
    shengzi->setPhysicsBody(shengzibody);
    shengzibody->setTag(DRAG_BODYS_TAG1);
    shengzibody->setRotationEnable(false);
    shengzibody->setGroup(2);
    shengzibody->setMass(0.1);
    shengzibody->setCategoryBitmask(0x02);    //0010
    shengzibody->setContactTestBitmask(0x02); //0010
    shengzibody->setCollisionBitmask(0x02);   //0010
    shengzibody->setLinearDamping(3.0f);
    this->addChild(shengzi);
    shengzi->setPosition(VisibleRect::bottom() - Point(0, -300));
    //    auto contactListener = EventListenerPhysicsContactWithBodies::create(tou->getPhysicsBody(), shengzi->getPhysicsBody());
    //    contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegin, this);
    //    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
    
    
        //多纳的头
    auto b1 = Sprite::create(shizi_youshou_png);
    b1->setPhysicsBody(PEShapeCache::getInstance()->getPhysicsBodyByName("shizi_youshou"));
    b1->setPosition(VisibleRect::center()+Point(300,0));
    b1->getPhysicsBody()->setTag(1);
    b1->getPhysicsBody()->setRotationEnable(false);
    this->addChild(b1);
    auto b2 = Sprite::create(shizi_zuoshou_png);
    b2->setPhysicsBody(PEShapeCache::getInstance()->getPhysicsBodyByName("shizi_zuoshou"));
    b2->setPosition(VisibleRect::center()-Point(300,0));
    b2->getPhysicsBody()->setRotationEnable(false);
    b2->getPhysicsBody()->setTag(1);
    this->addChild(b2);
    
    
    
        // LabelTTF
    auto label2 = LabelTTF::create("多纳小狮子爱学习", "Arial", 64);
    label2->setPhysicsBody(PhysicsBody::createBox(label2->getBoundingBox().size, PhysicsMaterial(0.1f, 0.0f, 1.0f)));
    label2->getPhysicsBody()->setTag(1);
//    label2->getPhysicsBody()->setRotationEnable(false);
    label2->setPosition(VisibleRect::center()+Point(0,300));
    addChild(label2, 0);
    
    PEShapeCache::getInstance()->removeBodysWithWithFile(buttonsbodys_plist);
    
        //注册碰撞检测监听
    auto contactListener1 = EventListenerPhysicsContact::create();
    contactListener1->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegin, this);
    contactListener1->onContactPostSolve = CC_CALLBACK_2(HelloWorld::onContactPostSolve, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener1, this);
}

void HelloWorld::onContactPostSolve(PhysicsContact &contact, const PhysicsContactPostSolve &solve)
{
    CCLOG("%s", "################## onContactPostSolve");
}
bool HelloWorld::onContactBegin(PhysicsContact &contact)
{
    CCLOG("%s", "################## onContactBegin");
    return true; // contact.getContactData()->normal.y < 0;
}
void HelloWorld::onAcceleration(Acceleration *acc, Event *event)
{
    static float prevX = 0, prevY = 0;
#define kFilterFactor 0.05f
    float accelX = (float)acc->x * kFilterFactor + (1 - kFilterFactor) * prevX;
    float accelY = (float)acc->y * kFilterFactor + (1 - kFilterFactor) * prevY;
    prevX = accelX;
    prevY = accelY;
    auto v = Point(accelX, accelY);
    v = v * 200;
    if (_scene != nullptr)
    {
        _scene->getPhysicsWorld()->setGravity(v);
    }
}
bool HelloWorld::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    CCLOG("%s", "touch");
    auto location = touch->getLocation();
    auto arr = _scene->getPhysicsWorld()->getShapes(location);//从物理世界得到多边形
    PhysicsBody *body = nullptr;
    for (auto &obj : arr)
    {
        if ((obj->getBody()->getTag() & DRAG_BODYS_TAG) != 0) //得到刚体
        {
            body = obj->getBody();
            break;
        }
    }
    if (body != nullptr)
    {
            //创建一个刚体
        Node *mouse = Node::create();
        mouse->setPhysicsBody(PhysicsBody::create(PHYSICS_INFINITY, PHYSICS_INFINITY));
        mouse->getPhysicsBody()->setDynamic(false);
        mouse->setPosition(location);
        this->addChild(mouse);
        body->setLinearDamping(0.0f);
            //用图钉关节与点中刚体绑定 赋予力 可以拖动
        PhysicsJointPin *joint = PhysicsJointPin::construct(mouse->getPhysicsBody(), body, location);
        joint->setMaxForce(5000.0f * body->getMass());
        _scene->getPhysicsWorld()->addJoint(joint);
        _mouses.insert(std::make_pair(touch->getID(), mouse));
        return true;
    }
    return false;
}
void HelloWorld::onTouchMoved(Touch *touch, Event *event)
{
    auto it = _mouses.find(touch->getID());
    if (it != _mouses.end())
    {
        it->second->setPosition(touch->getLocation());
    }
}
void HelloWorld::onTouchEnded(Touch *touch, Event *event)
{
    auto it = _mouses.find(touch->getID());
    if (it != _mouses.end())
    {
        this->removeChild(it->second);
        _mouses.erase(it);
    }
    
        //增加摩擦阻尼 减小惯性
    PhysicsBody *body = _scene->getPhysicsWorld()->getBody(DRAG_BODYS_TAG);
    if (body != nullptr)
    {
        body->setLinearDamping(2.5f);
    }
    PhysicsBody *body1 = _scene->getPhysicsWorld()->getBody(DRAG_BODYS_TAG1);
    if (body1 != nullptr)
    {
        body1->setLinearDamping(2.5f);
    }
}
void HelloWorld::menuCloseCallback(Ref *pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
    return;
#endif
    Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
