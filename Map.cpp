/*
** Map.cpp for Heex
*/

#include <cmath>
#include "Map.h"
#include "Object.h"

Map::Map(float w, float h) : _objectList(), _width(w), _height(h)
{
#ifdef DEBUG
    std::cout << "Map: new map, width : " << w << " height : " << h << std::endl;
#endif
}

Map::~Map()
{
    for (ObjectList::iterator itr = _objectList.begin(); itr != _objectList.end(); ++itr)
        delete *itr;
}

void Map::AddObject(Object* obj)
{
#ifdef DEBUG
    float x, y, z, o;
    obj->GetPosition(x, y, z, o);
    std::cout << "Map: add an object modelid : " << obj->GetModelId() << " pos x : " << x << " pos y : " << y << " pos z : " << z << " orientation : " << o << std::endl;
#endif

    obj->SetMap(this);
    _objectList.push_back(obj);
}

void Map::GetWidthAndHeight(float& w, float& h)
{
    w = _width;
    h = _height;
}

float Map::GetDistance2d(Object const* obj1, Object const* obj2)
{
    Position pos1;
    Position pos2;
    obj1->GetPosition(pos1);
    obj2->GetPosition(pos2);
    return GetDistance2d(pos1, pos2);
}

float Map::GetDistance2d(Position const& pos1, Position const& pos2)
{
    return GetDistance2d(pos1.posX, pos1.posY, pos2.posX, pos2.posY);
}

float Map::GetDistance2d(float x1, float y1, float x2, float y2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    return std::sqrt(dx * dx + dy * dy);
}

void Map::GetObjectListInRange(Position const& pos, float range, ObjectList& list)
{
    GetObjectListInRange(pos.posX, pos.posY, range, list);
}

void Map::GetObjectListInRange(float x, float y, float range, ObjectList& list)
{
    for(ObjectList::iterator itr = _objectList.begin(); itr != _objectList.end(); ++itr)
    {
        if (GetDistance2d(x, y, (*itr)->GetPositionX(), (*itr)->GetPositionY()) <= range)
            list.push_back(*itr);
    }
}

bool Map::IsValidePosition(Position const& pos) const
{
    return IsValidePosition(pos.posX, pos.posY, pos.posZ);
}

bool Map::IsValidePosition(float x, float y, float z) const
{
    (void)x;
    (void)y;
    (void)z;
    return true; // TODO
}

bool Map::IntersectObjectAt(Position const& pos, Object const* obj) const
{
    return IntersectObjectAt(pos.posX, pos.posY, pos.posZ, obj);
}

bool Map::IntersectObjectAt(float x, float y, float z, Object const* obj) const
{
    ModelBox other = sModelMgr->GetModelBoxAtPos(x, y, z, obj->GetModelId());

    for(ObjectList::const_iterator itr = _objectList.begin(); itr != _objectList.end(); ++itr)
    {
        if ((*itr) != obj && obj->GetDistance2d(*itr) <= obj->GetVisibilityRange())
            if ((*itr)->IntersectObject(other))
                return true;
    }

    return false;
}

