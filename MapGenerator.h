/*
** MapGenerator.h
*/

#ifndef MAPGENERATOR_H_
# define MAPGENERATOR_H_

#include "SharedDefines.h"
#include "Map.h"

#define MAP_PRECISION 5

class MapGenerator
{
public:
    static Map* CreateNewRandomMap(const uint32 width, const uint32 height, float complexity, float density);
private:
    MapGenerator() {}
};

#endif /* !MAPGENERATOR_H_ */
