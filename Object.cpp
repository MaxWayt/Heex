/*
** Object.cpp
*/

#include <iostream>
#include <cmath>
#include "Object.h"
#include "Map.h"

Object::Object(uint32 modelId, Position const& pos) :
    _modelId(modelId), _pos(pos), _movementFlags(MOVE_NONE), _map(NULL), _speed(1.0f)
{
#ifdef DEBUG
    std::cout << "Object: create an object modelid : " << modelId << " pos x : " << pos.posX << " pos y : " << pos.posY << " pos z : " << pos.posZ << " orientation : " << pos.orientation << std::endl;
#endif
}

Object::Object(uint32 modelId, float x, float y, float z, float o) :
    _modelId(modelId), _pos(x, y, z, o), _movementFlags(MOVE_NONE), _map(NULL), _speed(1.0f)
{
#ifdef DEBUG
    std::cout << "Object: create an object modelid : " << modelId << " pos x : " << x << " pos y : " << y << " pos z : " << z << " orientation : " << o << std::endl;
#endif
}

void Object::GetPosition(Position& pos) const
{
    pos = _pos;
}

void Object::GetPosition(float& x, float &y) const
{
    x = _pos.posX;
    y = _pos.posY;
}

void Object::GetPosition(float& x, float& y, float& z) const
{
    x = _pos.posX;
    y = _pos.posY;
    z = _pos.posZ;
}

void Object::GetPosition(float& x, float& y, float& z, float& o) const
{
    x = _pos.posX;
    y = _pos.posY;
    z = _pos.posZ;
    o = _pos.orientation;
}

float Object::GetPositionX() const
{
    return _pos.posX;
}

float Object::GetPositionY() const
{
    return _pos.posY;
}

float Object::GetPositionZ() const
{
    return _pos.posZ;
}

float Object::GetOrientation() const
{
    return _pos.orientation;
}

uint32 Object::GetModelId() const
{
    return _modelId;
}

void Object::SetMap(Map* map)
{
    _map = map;
}

void Object::AddMovementFlag(uint32 flag)
{
    _movementFlags |= flag;
}

void Object::RemoveMovementFlag(uint32 flag)
{
    _movementFlags &= ~flag;
}

uint32 Object::GetMovementFlags() const
{
    return _movementFlags;
}

void Object::UpdatePosition()
{
    if (_movementFlags & MOVE_FORWARD)
        MovePositionAt(_pos.posX, _pos.posY, 0.7 * _speed, M_PI);
    else if (_movementFlags & MOVE_BACKWARD)
        MovePositionAt(_pos.posX, _pos.posY, 0.4 * _speed, 0.0f);

    if (_movementFlags & MOVE_TURN_LEFT)
        _pos.orientation += (0.10 * _speed * 1.15);
    else if (_movementFlags & MOVE_TURN_RIGHT)
        _pos.orientation -= (0.10 * _speed * 1.15);
}

void Object::ModSpeed(float delta)
{
    if (_speed + delta < 0.0f)
        _speed = 0.0f;
    else
        _speed += delta;
}

void Object::MovePositionAt(float& x, float& y, float dist, float angle) const
{
    if (!_map)
        return;

    float destx, desty, currDist;
    angle += _pos.orientation;
    currDist = dist;

    while (currDist > 0.0f)
    {

        destx = x + currDist * cos(angle);
        desty = y + currDist * sin(angle);

        if (_map->IsValidePosition(destx, desty, _pos.posZ) && !_map->IntersectObjectAt(destx, desty, _pos.posZ, this))
            break;

        currDist -= 0.2f;
    }

    if (currDist <= 0.0f)
        return;

    x = destx;
    y = desty;
}

bool Object::IntersectObject(Object const* obj) const
{
    ModelBox other = sModelMgr->GetModelBox(obj);
    return IntersectObject(other);
}

bool Object::IntersectObject(ModelBox const& other) const
{
    ModelBox self = sModelMgr->GetModelBox(this);

    if (self.max.x < other.min.x || self.min.x > other.max.x)
        return false;
    if (self.max.y < other.min.y || self.min.y > other.max.y)
        return false;
    if (self.max.z < other.min.z || self.min.z > other.max.z)
        return false;
    return true;
}

