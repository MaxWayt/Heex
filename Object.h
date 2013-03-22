/*
** Object.h for Heex
*/

#ifndef OBJECT_H_
# define OBJECT_H_

#include "SharedDefines.h"
#include "ModelMgr.h"

class Map;

struct Position
{
    float posX;
    float posY;
    float posZ;
    float orientation;
    Position() : posX(0.0f), posY(0.0f), posZ(0.0f), orientation(0.0f) {}
    Position(float x, float y, float z, float o) : posX(x), posY(y), posZ(z), orientation(o) {}
    Position(Position const& other) : posX(other.posX), posY(other.posY), posZ(other.posZ), orientation(other.orientation) {}
};

enum MovementFlags
{
    MOVE_NONE       = 0x00000000,
    MOVE_FORWARD    = 0x00000001,
    MOVE_BACKWARD   = 0x00000002,
    MOVE_TURN_LEFT  = 0x00000004,
    MOVE_TURN_RIGHT = 0x00000008
};

class Object
{
public:
    explicit Object(uint32 modelId, Position const& pos);
    explicit Object(uint32 modelId, float x, float y, float z, float o);
    virtual ~Object() {}

    void GetPosition(Position& pos) const;
    void GetPosition(float& x, float &y) const;
    void GetPosition(float& x, float& y, float& z) const;
    void GetPosition(float& x, float& y, float& z, float& o) const;
    float GetPositionX() const;
    float GetPositionY() const;
    float GetPositionZ() const;
    float GetOrientation() const;

    uint32 GetModelId() const;

    void SetMap(Map* map);
    void AddMovementFlag(uint32 flag);
    void RemoveMovementFlag(uint32 flag);
    uint32 GetMovementFlags() const;
    void UpdatePosition();
    void ModSpeed(float delta);
    void MovePositionAt(float& x, float& y, float dist, float angle) const;
    bool IntersectObject(Object const* obj) const;
    bool IntersectObject(ModelBox const& other) const;

protected:
    uint32 _modelId;
    Position _pos;
    uint32 _movementFlags;
    Map* _map;
    float _speed;
};

#endif /* !OBJECT_H_ */
