/*
** ModelMgr.h for Heex
*/

#ifndef MODELMGR_H_
# define MODELMGR_H_

#include <map>
#include <exception>
#include "SharedDefines.h"
#include "MapGenerator.h"

class Object;

struct Model
{
    uint32 modelId;
    float width;
    float height;
    float zsize;
    uint8 colorR;
    uint8 colorG;
    uint8 colorB;
    Model(uint32 id, float w, float h, float z, int8 r = 0, int8 g = 0, int8 b = 0) :
        modelId(id), width(w), height(h), zsize(z), colorR(r), colorG(g), colorB(b) {}
};
struct ModelBox
{
    struct
    {
        float x;
        float y;
        float z;
    } max;
    struct
    {
        float x;
        float y;
        float z;
    } min;
};

class ModelMgr
{
public:
    static ModelMgr* instance();
    ModelBox GetModelBox(Object const* obj) const;
    ModelBox GetModelBoxAtPos(float x, float y, float z, uint32 modelid) const;
    Model const* GetModel(Object const* obj) const;
    Model const* GetModel(uint32 id) const;
private:
    ModelMgr();
    ~ModelMgr();

    std::map<uint32, Model*> _modelMap;

public:
    class UnknowModelExcept : public std::exception
    {
    public:
        const char* what() const throw()
        {
            return "unknow model id";
        }
    };
};

#define sModelMgr ModelMgr::instance()

#endif /* !MODELMGR_H_ */
