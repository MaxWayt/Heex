/*
** MapGenerator.h
*/

#ifndef MAPGENERATOR_H_
# define MAPGENERATOR_H_

#include "SharedDefines.h"
#include "Map.h"

class MapGenerator
{
public:
    static Map* CreateNewRandomMap(const uint32 width, const uint32 height, float complexity, float density);

    static Map* GetNewMapFromNode(const uint32 width, const uint32 height, float complexity, float density, std::string const& address, std::string const& port);

    enum MapRequestReponses
    {
        RESPONSE_OK      = 1,
        RESPONSE_FAIL    = 2
    };
private:
    MapGenerator() {}
};

#endif /* !MAPGENERATOR_H_ */
