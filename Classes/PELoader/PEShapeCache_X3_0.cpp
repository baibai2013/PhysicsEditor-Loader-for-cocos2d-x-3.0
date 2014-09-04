//
//  PEShapeCache_X3_0.cpp
//  MemoryCards_CPP
//
//  Created by lili on 14-9-1.
//
//
#include "PEShapeCache_X3_0.h"
#include "chipmunk.h"
#include "chipmunk/CCPhysicsHelper_chipmunk.h"
using namespace cocos2d;
static PEShapeCache *_instance = nullptr;
static float area(Point *vertices, int numVertices)
{
    float area = 0.0f;
    int r = (numVertices - 1);
    area += vertices[0].x * vertices[r].y - vertices[r].x * vertices[0].y;
    for (int i = 0; i < numVertices - 1; ++i)
    {
        area += vertices[r - i].x * vertices[r - (i + 1)].y - vertices[r - (i + 1)].x * vertices[r - i].y;
    }
    area *= .5f;
    return area;
}
PEShapeCache::PEShapeCache()
{
}
bool PEShapeCache::init()
{
    return true;
}
PEShapeCache::~PEShapeCache()
{
    removeAllBodys();
}
PEShapeCache *PEShapeCache::getInstance()
{
    if (!_instance)
    {
        _instance = new PEShapeCache();
        _instance->init();
    }
    return _instance;
}
void PEShapeCache::destroyInstance()
{
    CC_SAFE_RELEASE_NULL(_instance);
}
void PEShapeCache::addBodysWithFile(const std::string &plist)
{
    ValueMap dict = FileUtils::getInstance()->getValueMapFromFile(plist);
    CCASSERT(!dict.empty(), "Shape-file not found");
    CCASSERT(dict.size() != 0, "plist file empty or not existing");
    ValueMap &metadata = dict["metadata"].asValueMap();
    int format = metadata["format"].asInt();
    CCASSERT(format == 1, "format not supported!");
    ValueMap &bodydict = dict.at("bodies").asValueMap();
    for (auto iter = bodydict.cbegin(); iter != bodydict.cend(); ++iter)
    {
        const ValueMap &bodyData = iter->second.asValueMap();
        std::string bodyName = iter->first;
        BodyDef *bodyDef = new BodyDef();
        bodyDefs.insert(bodyName, bodyDef);
        bodyDef->anchorPoint = PointFromString(bodyData.at("anchorpoint").asString());
        const ValueVector &fixtureList = bodyData.at("fixtures").asValueVector();
        float totalMass = 0.0f;
        float totalBodyMomentum = 0.0f;
        for (auto &fixtureitem : fixtureList)
        {
            FixtureData *fd = new FixtureData();
            bodyDef->fixtures.pushBack(fd);
            auto &fixturedata = fixtureitem.asValueMap();
            fd->friction = fixturedata.at("friction").asFloat();
            fd->elasticity = fixturedata.at("elasticity").asFloat();
            fd->mass = fixturedata.at("mass").asFloat();
            fd->surfaceVelocity = PointFromString(fixturedata.at("surface_velocity").asString());
            fd->layers = fixturedata.at("layers").asInt();
            fd->group = fixturedata.at("group").asInt();
            fd->collisionType = fixturedata.at("collision_type").asInt();
            fd->isSensor = fixturedata.at("isSensor").asBool();
            std::string fixtureType = fixturedata.at("fixture_type").asString();
            float totalArea = 0.0f;
            totalMass += fd->mass;
            if (strcmp("POLYGON", fixtureType.c_str()) == 0)
            {
                const ValueVector &polygonsArray = fixturedata.at("polygons").asValueVector();
                fd->fixtureType = SHAPE_POLYGON;
                for (auto &polygonitem : polygonsArray)
                {
                    Polygon *poly = new Polygon();
                    fd->polygons.pushBack(poly);
                    auto &polygonArray = polygonitem.asValueVector();
                    poly->numVertices = polygonArray.size();
                    Point *vertices = poly->vertices = new Point[poly->numVertices];
                    int vindex = 0;
                    for (auto &pointString : polygonArray)
                    {
                        Point offsex = PointFromString(pointString.asString());
                        vertices[vindex].x = offsex.x;
                        vertices[vindex].y = offsex.y;
                        vindex++;
                    }
                    poly->area = area(vertices, poly->numVertices);
                    totalArea += poly->area;
                }
            }
            else if (strcmp("CIRCLE", fixtureType.c_str()) == 0)
            {
                fd->fixtureType = SHAPE_CIRCLE;
                const ValueMap &circleData = fixturedata.at("circle").asValueMap();
                fd->radius = circleData.at("radius").asFloat();
                fd->center = PointFromString(circleData.at("position").asString());
                totalArea += 3.1415927 * fd->radius * fd->radius;
            }
            else
            {
                // unknown type
                assert(0);
            }
            fd->area = totalArea;
            // update sub polygon's masses and momentum
            cpFloat totalFixtureMomentum = 0.0f;
            if (totalArea)
            {
                if (fd->fixtureType == SHAPE_CIRCLE)
                {
                    totalFixtureMomentum += cpMomentForCircle(PhysicsHelper::float2cpfloat(fd->mass), PhysicsHelper::float2cpfloat(fd->radius), PhysicsHelper::float2cpfloat(fd->radius), PhysicsHelper::point2cpv(fd->center));
                }
                else
                {
                    for (auto *p : fd->polygons)
                    {
                        // update mass
                        p->mass = (p->area * fd->mass) / fd->area;
                        cpVect *cpvs = new cpVect[p->numVertices];
                        // calculate momentum
                        p->momentum = cpMomentForPoly(PhysicsHelper::float2cpfloat(p->mass), p->numVertices, PhysicsHelper::points2cpvs(p->vertices, cpvs, p->numVertices), PhysicsHelper::point2cpv(Point::ZERO));
                        delete[] cpvs;
                        // calculate total momentum
                        totalFixtureMomentum += p->momentum;
                    }
                }
            }
            fd->momentum = PhysicsHelper::cpfloat2float(totalFixtureMomentum);
            totalBodyMomentum = PhysicsHelper::cpfloat2float(totalFixtureMomentum);
        }
        // set bodies total mass
        bodyDef->mass = totalMass;
        bodyDef->momentum = totalBodyMomentum;
    }
}
PhysicsBody *PEShapeCache::getPhysicsBodyByName(const std::string name)
{
    BodyDef *bd = bodyDefs.at(name);
    CCASSERT(bd != nullptr, "Body not found");
    PhysicsBody *body = PhysicsBody::create(bd->mass, bd->momentum);
    body->setPositionOffset(bd->anchorPoint);
    for (auto fd : bd->fixtures)
    {
        if (fd->fixtureType == SHAPE_CIRCLE)
        {
            auto shape = PhysicsShapeCircle::create(fd->radius, PhysicsMaterial(0.0f, fd->elasticity, fd->friction), fd->center);
            shape->setGroup(fd->group);
            //            shape->setCategoryBitmask(fd->collisionType);
            body->addShape(shape);
        }
        else if (fd->fixtureType == SHAPE_POLYGON)
        {
            for (auto polygon : fd->polygons)
            {
                auto shape = PhysicsShapePolygon::create(polygon->vertices, polygon->numVertices, PhysicsMaterial(0.0f, fd->elasticity, fd->friction), fd->center);
                shape->setGroup(fd->group);
                //                shape->setCategoryBitmask(fd->collisionType);
                body->addShape(shape);
            }
        }
    }
    return body;
}
bool PEShapeCache::removeBodysWithFile(const std::string &plist)
{
    ValueMap dict = FileUtils::getInstance()->getValueMapFromFile(plist);
    CCASSERT(!dict.empty(), "Shape-file not found");
    CCASSERT(dict.size() != 0, "plist file empty or not existing");
    ValueMap &metadata = dict["metadata"].asValueMap();
    int format = metadata["format"].asInt();
    CCASSERT(format == 1, "format not supported!");
    ValueMap &bodydict = dict.at("bodies").asValueMap();
    for (auto iter = bodydict.cbegin(); iter != bodydict.cend(); ++iter)
    {
        std::string bodyName = iter->first;
        BodyDef *bd = bodyDefs.at(bodyName);
        if(bd != nullptr)
        {
            safeReleaseBodyDef(bd);
            bodyDefs.erase(bodyName);
        }
       
    }
    return true;
}
bool PEShapeCache::removeAllBodys()
{
    CCLOG("%s"," PEShapeCache removeAllbodys");
    for (auto iter = bodyDefs.cbegin(); iter != bodyDefs.cend(); ++iter)
    {
        safeReleaseBodyDef(iter->second);
    }
    bodyDefs.clear();
    return true;
}
void PEShapeCache::reset()
{
    removeAllBodys();
}
bool PEShapeCache::safeReleaseBodyDef(BodyDef *bodyDef)
{
    for (auto fixturedate : bodyDef->fixtures)
    {
        for (auto polygon : fixturedate->polygons)
        {
            CC_SAFE_DELETE_ARRAY(polygon->vertices);
//            CCLOG("%s"," PEShapeCache safeReleaseBodyDef-> polygons");
        }
//        CCLOG("%s%d","total delete array:",(int)fixturedate->polygons.size());
        fixturedate->polygons.clear();
    }
    bodyDef->fixtures.clear();
    return true;
}
