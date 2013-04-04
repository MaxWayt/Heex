/*
** main.cpp for MapGeneratorNode in /home/fest/workspace/Heex/MapGeneratorNode
**
** Made by maxime ginters
** Login  <ginter_m@epitech.eu>
**
** Started on  Sun Mar 24 16:07:30 2013 maxime ginters
** Last update Thu Apr 04 23:52:53 2013 maxime ginters
*/

#include <iostream>
#include <list>
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/serialization/singleton.hpp>
#include <boost/asio.hpp>
#include "MapGenerator.h"
#include "Config.h"
#include "Opcodes.h"
#include "ByteBuffer.h"
#include "Object.h"

using boost::asio::ip::tcp;
using boost::asio::buffer;

class GameNodeSocket : public boost::enable_shared_from_this<GameNodeSocket>
{
public:
    GameNodeSocket(boost::asio::io_service& io_service) : _socket(io_service), _authed(false) {}

    tcp::socket& socket()
    {
        return _socket;
    }

    void start()
    {
        std::cout << "Socket start !" << std::endl;
        registerAsyncRead();
    }

    void registerAsyncRead()
    {
        memset(_buffer, 0, 512);
        _socket.async_read_some(boost::asio::buffer(_buffer, 512),
                boost::bind(&GameNodeSocket::handle_read, shared_from_this(),
                    boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }

    void close()
    {
        _socket.close();
        _authed = false;
    }

    void handle_read(boost::system::error_code const& e, size_t len)
    {
        std::cout << "handle read - size : " << len << std::endl;

        if (!e) // No error
        {
            uint16 opcode = *((uint16 const*)&_buffer[2]);
            switch (opcode)
            {
                case MSG_TEXT:
                    {
                        std::string str(&_buffer[4]);
                        std::cout << str << std::endl;
                        break;
                    }
                case SMSG_HASK_MAP:
                    {
                        if (_authed)
                            break;
                        uint32 index = 4;
                        uint32 width = *((uint32 const*)&_buffer[index]);
                        index += sizeof(uint32);
                        uint32 height = *((uint32 const*)&_buffer[index]);
                        index += sizeof(uint32);
                        float complexity = *((float const*)&_buffer[index]);
                        index += sizeof(float);
                        float density = *((float const*)&_buffer[index]);
                        index += sizeof(float);
                        std::string pass(&_buffer[index]);
                        std::cout << "New map request - width : " << width << " height : " << height << " complexity : " << complexity << " density : " << density << " pass : [" << pass << "]" << std::endl;
                        if (pass != "poney42")
                        {
                            sendHaskMapReponse(MapGenerator::RESPONSE_FAIL);
                            close();
                        }
                        _authed = true;
                        sendHaskMapReponse(MapGenerator::RESPONSE_OK);
                        Map* map = MapGenerator::CreateNewRandomMap(width, height, complexity, density);
                        Map::ObjectList List;
                        map->GetAllObjectList(List);
                        float percent = 0;
                        uint32 current = 0;
                        sendUpdateProgress(0.0f);
                        for (Map::ObjectList::const_iterator itr = List.begin(); itr != List.end(); ++itr)
                        {
                            sendObject(*itr);

                            if (current++ % (List.size() / 10) == 0)
                            {
                                percent += 10.0f;
                                if (percent >= 100.0f)
                                    percent = 99.9f;
                                sendUpdateProgress(percent);
                            }

                        }
                        sendUpdateProgress(100.0f);
                        break;
                    }
                default:
                    {
                        std::cout << "Receiv unknow opcode : " << opcode << std::endl;
                        break;
                    }
            }
        }
        else
        {
            std::cerr << e.message() << std::endl;
            close();
            return;
        }
        registerAsyncRead();
    }

    void sendObject(Object const* obj)
    {
        float x, y, z, o;
        obj->GetPosition(x, y, z, o);
        Pkt data;
        data << x;
        data << y;
        data << z;
        data << o;
        data << obj->GetModelId();
        data.SetOpcode(MMSG_SEND_OBJECT);
        sendPacket(data);
    }

    void sendUpdateProgress(float percent)
    {
        Pkt data;
        data << percent;
        data.SetOpcode(MMSG_PROGRESS);
        sendPacket(data);
    }

    void sendHaskMapReponse(MapGenerator::MapRequestReponses resp)
    {
        Pkt data;
        data << uint8(resp);
        data.SetOpcode(MMSG_MAP_RESPONSE);
        sendPacket(data);
    }

    void sendPacket(Pkt& data)
    {
        try
        {
            _socket.write_some(buffer(data.contents(), data.size()));
        }
        catch (std::exception& e)
        {
            std::cerr << "Exception: " << e.what() << std::endl;
            close();
        }
    }

private:
    tcp::socket _socket;
    char _buffer[512];
    bool _authed;
};

typedef boost::shared_ptr<GameNodeSocket> GameNodeSocketPtr;

class GameNodeMgr : public boost::serialization::singleton<GameNodeMgr>
{
public:
    GameNodeMgr() : _socketList() {}
    ~GameNodeMgr() {}
    void addSocket(GameNodeSocketPtr sock)
    {
        _socketList.push_back(sock);
    }
private:
    std::list<GameNodeSocketPtr> _socketList;
};
#define sGameNodeMgr GameNodeMgr::get_mutable_instance()

class GameNodeAcceptor
{
public:
    GameNodeAcceptor(boost::asio::io_service& io_service, unsigned short port) : _io_service(io_service),
        _acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
    {
        GameNodeSocketPtr newConn(new GameNodeSocket(_io_service));
        _acceptor.async_accept(newConn->socket(),
                boost::bind(&GameNodeAcceptor::handle_accept, this,
                    boost::asio::placeholders::error, newConn));
        std::cout << "GameNode Acceptor started on port : " << port << std::endl;
    }

    void handle_accept(boost::system::error_code const& e, GameNodeSocketPtr conn)
    {
        if (!e) // No error
        {
            conn->start();
            sGameNodeMgr.addSocket(conn);
            GameNodeSocketPtr newConn(new GameNodeSocket(_io_service));
            _acceptor.async_accept(newConn->socket(),
                    boost::bind(&GameNodeAcceptor::handle_accept, this,
                        boost::asio::placeholders::error, newConn));
        }
        else
            std::cerr << e.message() << std::endl;
    }

private:
    boost::asio::io_service& _io_service;
    boost::asio::ip::tcp::acceptor _acceptor;
};


int main(int ac, char **av)
{
    sConfig->addParam("-p", "--port", "bind port", TYPE_FLAG_AND_VALUE, "9000");
    sConfig->parseParam(ac, av);

    boost::asio::io_service io_service;

    try
    {
        GameNodeAcceptor GameAcceptor(io_service, 9000);
        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
