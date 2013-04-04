/*
** MapGenerator.cpp
*/

#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "MapGenerator.h"
#include "ByteBuffer.h"
#include "Opcodes.h"
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

    #pragma omp parallel for shared(map)
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

Map* MapGenerator::GetNewMapFromNode(const uint32 width, const uint32 height, float complexity, float density, std::string const& address, std::string const& port)
{
    using boost::asio::ip::tcp;
    boost::asio::io_service io_service;
    tcp::resolver resolver(io_service);
    tcp::resolver::query query(address.c_str(), port.c_str());
    tcp::resolver::iterator iterator = resolver.resolve(query);
    tcp::socket socket(io_service);

    std::cout << "Connecting to : " << address << ":" << port << " ..." << std::endl;
    socket.connect(*iterator);
    std::cout << "Requesting for map ..." << std::endl;
    Pkt data;
    data << width;
    data << height;
    data << complexity;
    data << density;
    std::string mdp = "poney42";
    data << mdp;
    data.SetOpcode(SMSG_HASK_MAP);
    data.hexlike();
    socket.write_some(boost::asio::buffer(data.contents(), data.size()));
    char buffer[512];
    memset(buffer, 0, 512);
    size_t size = socket.read_some(boost::asio::buffer(buffer, 512));
    uint16 opcode = *((uint16 const*)&buffer[2]);
    if (size == 0 || opcode != MMSG_MAP_RESPONSE)
    {
        std::cout << "Receiv an unexpected reponse, aborting ..." << std::endl;
        return NULL;
    }
    uint8 response = buffer[4];
    if (response != MapGenerator::RESPONSE_OK)
    {
        std::cout << "Fail to auth against map generator, aborting ..." << std::endl;
        return NULL;
    }
    std::cout << "Map request accepted, waiting data ..." << std::endl;
    Map* newMap = new Map(width * MAP_PRECISION, height * MAP_PRECISION);
    float progress = 0.0f;
    while (socket.is_open() && progress < 100.0f)
    {
        memset(buffer, 0, 512);
        size_t size = socket.read_some(boost::asio::buffer(buffer, 512));
        if (size == 0)
        {
            std::cout << "Received a 0 size Pkt" << std::endl;
            continue;
        }
        uint16 opcode = *((uint16 const*)&buffer[2]);
        switch (opcode)
        {
            case MMSG_PROGRESS:
                {
                    progress = *((float const*)&buffer[4]);
                    std::cout << "Progress : " << progress << "%" << std::endl;
                    break;
                }
            case MMSG_SEND_OBJECT:
                {
                    uint32 index = 4;
                    float x = *((float const*)&buffer[index]);
                    index += sizeof(float);
                    float y = *((float const*)&buffer[index]);
                    index += sizeof(float);
                    float z = *((float const*)&buffer[index]);
                    index += sizeof(float);
                    float o = *((float const*)&buffer[index]);
                    index += sizeof(float);
                    uint32 display = *((uint32 const*)&buffer[index]);
                    newMap->AddObject(new Object(display, x, y, z, o));
                    //std:: cout << "New object registerd" << std::endl;
                    break;
                }
            default:
                std::cout << "unknow opcode : " << opcode << std::endl;
                break;
        }
    }
    socket.close();
    return newMap;
}
