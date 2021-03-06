/*
** Map.h for Heex
*/

#ifndef MAP_H_
# define MAP_H_

#include <list>
#include <iostream>
#include "SharedDefines.h"

struct Position;
class Object;

class Map
{
public:
    explicit Map(float w, float h);
    virtual ~Map();

    typedef std::list<Object*> ObjectList;
    typedef ObjectList::const_iterator ObjectListConstIterator;

    void AddObject(Object* obj);

    void GetWidthAndHeight(float& w, float& h);
    static float GetDistance2d(Object const* obj1, Object const* obj2);
    static float GetDistance2d(Position const& pos1, Position const& pos2);
    static float GetDistance2d(float x1, float y1, float x2, float y2);
    void GetObjectListInRange(Position const& pos, float range, ObjectList& list);
    void GetObjectListInRange(float x, float y, float range, ObjectList& list);

    bool IsValidePosition(Position const& pos) const;
    bool IsValidePosition(float x, float y, float z) const;
    bool IntersectObjectAt(Position const& pos, Object const* obj) const;
    bool IntersectObjectAt(float x, float y, float z, Object const* obj) const;

    void GetAllObjectList(ObjectList& list) { list = _objectList; }
protected:
    ObjectList _objectList;
    float _width;
    float _height;
};

#endif /* !MAP_H_ */
