//
//  PEShapeCache_X3_0.h
//  MemoryCards_CPP
//  描述：加载PhysicsEditor 编辑的chipmunk Shapes
//  Created by lili on 14-9-1.
//
//
#ifndef __MemoryCards_CPP__PEShapeCache_X3_0__
#define __MemoryCards_CPP__PEShapeCache_X3_0__
#include <iostream>
#include "cocos2d.h"
#include <unordered_map>

NS_CC_BEGIN
class PhysicsBody;
class BodyDef;

class PEShapeCache : public Ref
{
  public:
    bool init();
    PEShapeCache(void);
    ~PEShapeCache(void);
    /**
     * 得到实例
     */
    static PEShapeCache *getInstance();
    /**
     *
     * 删除实例
     */
    static void destroyInstance();
    /**
     *  通过plist 文件加载chipmunk  Shapes 到缓存中
     *
     *  @param plist 文件路径
     */
    void addBodysWithFile(const std::string &plist);
    /**
     *  通过名字得到 PhysicsBodyˆ
     *
     *  @param name 图片名字
     *
     *  @return 刚体
     */
    PhysicsBody *getPhysicsBodyByName(const std::string name);
    /**
     *  根据文件名移除刚体
     *
     *  @param plist 文件路径
     *
     *  @return 是否移除完成
     */
    bool removeBodysWithFile(const std::string &plist);
    /**
     *  移除所有刚体
     *
     *  @return 是否移除
     */
    bool removeAllBodys();
    /**
     *  重置缓存
     */
    void reset();

  private:
    bool safeReleaseBodyDef(BodyDef* bodyDef);
private:
    Map<std::string,BodyDef*> bodyDefs;
};
typedef enum
{
    SHAPE_POLYGON,
    SHAPE_CIRCLE
} PEShapeType;
class Polygon : public Ref
{
  public:
    Point* vertices; //顶点数组
    int numVertices;      //顶点数
    float area;           //面积
    float mass;           //质量
    float momentum;       //扭力
};
class FixtureData : public Ref
{
  public:
    PEShapeType fixtureType;//形状类型
    float mass;             //质量；
    float elasticity;       //弹力
    float friction;         //摩擦力
    Point surfaceVelocity; //初速度
    int collisionType;      //分类
    int group;              //组
    int layers;             //在的层
    float area;             //面积
    float momentum;         //扭力
    bool isSensor;          //是不是传感器
                            //for circles
    Point center;           //中心
    float radius;           //半径
                            //for polygons
    Vector<Polygon*> polygons;   //顶点数组
};



class BodyDef : public Ref
{
public:
    Point anchorPoint;    //锚点
    Vector<FixtureData*> fixtures; //配置表单
    float mass;           //质量
    float momentum;       //扭力
};
NS_CC_END
#endif /* defined(__MemoryCards_CPP__PEShapeCache_X3_0__) */
