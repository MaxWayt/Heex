/*
** MapGenerator.cpp
*/

#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include "MapGenerator.h"
#include "Object.h"

Map* MapGenerator::CreateNewRandomMap(const uint32 width, const uint32 height, float complexity, float density)
{
    std::cout << ">> Generating random map ";
    std::cout.flush();

    uint32** map = new uint32*[height + 1];
    for (uint32 i = 0; i <= height; ++i)
    {
        map[i] = new uint32[width + 1];
        for (uint32 j = 0; j <= width; ++j)
            map[i][j] = 0;
    }

    uint32 shape[2];
    shape[0] = ((height / 2) * 2 + 1);
    shape[1] = ((width / 2) * 2 + 1);

    complexity = (int32)(complexity * (5 * (shape[0] + shape[1])));
    density = (int32)(density * (shape[0] / 2 * shape[1] / 2));

    for (int32 i = 0; i < (int32)density; ++i)
    {
        uint32 x = (rand() % (shape[1] / 2)) * 2;
        uint32 y = (rand() % (shape[0] / 2)) * 2;
        map[y][x] = 1;
        if (i % ((int32)density / 10) == 0)
        {
            std:: cout << ".";
            std::cout.flush();
        }

        for (int32 j = 0; j < (int32)complexity; ++j)
        {
            std::vector<std::pair<int32, int32>> neighbours;
            if (x > 1)
                neighbours.push_back(std::make_pair(y, x - 2));
            if (x < shape[1] - 2)
                neighbours.push_back(std::make_pair(y, x + 2));
            if (y > 1)
                neighbours.push_back(std::make_pair(y - 2, x));
            if (y < shape[0] - 2)
                neighbours.push_back(std::make_pair(y + 2, x));
            if (neighbours.size() > 0)
            {
                std::pair<int32, int32> pair = neighbours[rand() % neighbours.size()];
                uint32 y_ = pair.first;
                uint32 x_ = pair.second;
                if (map[y_][x_] == 0)
                {
                    map[y_][x_] = 1;
                    map[y_ + ((int32)y - (int32)y_) / 2][x_ + ((int32)x - (int32)x_) / 2] = 1;
                    x = x_;
                    y = y_;
                }
            }
        }
    }

    std::ofstream file;
    file.open("map.txt");
    Map* newMap = new Map(width * MAP_PRECISION, height * MAP_PRECISION);
    for (uint32 y = 0; y < height; ++y)
    {
        for (uint32 x = 0; x < width; ++x)
        {
            if (map[y][x] == 1)
                newMap->AddObject(new Object(1, x * MAP_PRECISION, y * MAP_PRECISION, 0.0f, 0.0f));
            file << map[y][x];
        }
        file << std::endl;
    }
    file.close();
    std::cout << std::endl << "Map generated" << std::endl;

    return newMap;
}
