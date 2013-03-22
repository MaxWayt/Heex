/*
** main.cpp for Heex
*/

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <time.h>
#include <vector>
#include <algorithm>
#include <GL/glut.h>

#include "Object.h"
#include "Map.h"
#include "MapGenerator.h"
#include "ModelMgr.h"

#define HEIGHT 200
#define WIDTH 200

#define PRECISION 5

enum Key
{
    KEY_ESC     = 27,
    KEY_SPACE   = 32,
    KEY_LEFT    = 100,
    KEY_UP      = 101,
    KEY_RIGHT   = 102,
    KEY_DOWN    = 103
};

double pos[3] = {0.0, 0.0, 30.0};
double dir[3] = {10.0, 10.0, 0.0};
double dist = 45.0;
double orrientation = 0.0;
bool keyStates[257];
float speed = 1.0f;
Map* map;
Object* player;

void drawCube(double baseX, double baseY, double baseZ, double width, double height, double zsize, int r = 0, int g = 0, int b = 0)
{
    glBegin(GL_QUADS);

    glColor3ub(r, g, b);
    // face
    glVertex3f(baseX, baseY, baseZ);
    glVertex3f(baseX, baseY, baseZ + zsize);
    glVertex3f(baseX + width, baseY, baseZ + zsize);
    glVertex3f(baseX + width, baseY, baseZ);

    // haut
    glVertex3f(baseX, baseY, baseZ + zsize);
    glVertex3f(baseX, baseY + height, baseZ + zsize);
    glVertex3f(baseX + width, baseY + height, baseZ + zsize);
    glVertex3f(baseX + width, baseY, baseZ + zsize);

    // base
    glVertex3f(baseX, baseY, baseZ);
    glVertex3f(baseX, baseY + height, baseZ);
    glVertex3f(baseX + width, baseY + height, baseZ);
    glVertex3f(baseX + width, baseY, baseZ);

    // droit
    glVertex3f(baseX + width, baseY, baseZ);
    glVertex3f(baseX + width, baseY, baseZ + zsize);
    glVertex3f(baseX + width, baseY + height, baseZ + zsize);
    glVertex3f(baseX + width, baseY + height, baseZ);

    // gauche
    glVertex3f(baseX, baseY, baseZ);
    glVertex3f(baseX, baseY, baseZ + zsize);
    glVertex3f(baseX, baseY + height, baseZ + zsize);
    glVertex3f(baseX, baseY + height, baseZ);

    // dos
    glVertex3f(baseX, baseY + height, baseZ);
    glVertex3f(baseX, baseY + height, baseZ + zsize);
    glVertex3f(baseX + width, baseY + height, baseZ + zsize);
    glVertex3f(baseX + width, baseY + height, baseZ);

    glEnd();
}

void drawObject(Object const* obj)
{
    float x, y, z;
    obj->GetPosition(x, y, z);
    Model const* model = sModelMgr->GetModel(obj);
    drawCube(x - model->width / 2, y - model->height / 2, z, model->width, model->height, model->zsize, model->colorR, model->colorG, model->colorB);
}
void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float x, y, z;
    player->GetPosition(x, y, z);
    gluLookAt(pos[0], pos[1], pos[2], x, y, z, 0, 0, 1);

    // Axe
    glBegin(GL_LINES);
    glColor3ub(255, 0, 0); // Rouge
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 20.0, 0.0);
    glColor3ub(0, 0, 255); // Bleu
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(20.0, 0.0, 0.0);
    glColor3ub(0, 255, 0); // Vert
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 20.0);
    glEnd();

    // fond
    glBegin(GL_QUADS);
    float height, width;
    map->GetWidthAndHeight(width, height);
    glColor3ub(0, 0, 255);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(width, 0.0, 0.0);
    glVertex3f(width, height, 0.0);
    glVertex3f(0.0f, height, 0.0);
    glEnd();

    Map::ObjectList list;
    map->GetObjectListInRange(x, y, (dist * 0.8) * PRECISION, list);
    std::for_each(list.begin(), list.end(), drawObject);

    // player

    glutSwapBuffers();
}

void handleResize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / h, 1.0, 3000.0);
}

void keySpecial(int key, int x, int y)
{
    (void)x;
    (void)y;

    bool handled = true;

    switch (key)
    {
        case KEY_LEFT:
            dist += 5;
            break;
        case KEY_UP:
            pos[2] += 1;
            break;
        case KEY_RIGHT:
            dist -= 5;
            if (dist < 5)
                dist = 5;
            break;
        case KEY_DOWN:
            pos[2] += -1;
            break;
        default:
                handled = false;
    }
    if (handled)
    {
        pos[0] = dir[0] + dist * cos(orrientation);
        pos[1] = dir[1] + dist * sin(orrientation);
        glutPostRedisplay();
    }
}

void destroyInFront()
{
/*    double cubX, cubY;
    for (unsigned int i = 1; i <= PRECISION; ++i)
    {
        cubX = dir[0] + 0.7 * PRECISION * cos(orrientation + M_PI);
        cubY = dir[1] + 0.7 * PRECISION * sin(orrientation + M_PI);
        if (!isValideCubePosition(cubX, cubY))
            continue;;
        map[(int)(cubY) / PRECISION][(int)(cubX) / PRECISION] = 0;
        glutPostRedisplay();
        return;
    }*/
}

void keyPressed(unsigned char key, int x, int y)
{
    (void)x;
    (void)y;
    switch (key)
    {
        case 'w': // haut
            player->RemoveMovementFlag(MOVE_BACKWARD);
            player->AddMovementFlag(MOVE_FORWARD);
            break;
        case 's': // bas
            player->RemoveMovementFlag(MOVE_FORWARD);
            player->AddMovementFlag(MOVE_BACKWARD);
            break;
        case 'd': // droite
            player->RemoveMovementFlag(MOVE_TURN_LEFT);
            player->AddMovementFlag(MOVE_TURN_RIGHT);
            break;
        case 'a': // gauche
            player->RemoveMovementFlag(MOVE_TURN_RIGHT);
            player->AddMovementFlag(MOVE_TURN_LEFT);
            break;
        case KEY_ESC:
            std::exit(0);
            break;
        case KEY_SPACE:
            destroyInFront();
            break;
        case '+':
            player->ModSpeed(0.1f);
            break;
        case '-':
            player->ModSpeed(-0.1f);
            break;
        case 'k': // fly
        case 'j': // Down
        default:
            break;
    }
    glutPostRedisplay();
}

void keyUp(unsigned char key, int x, int y)
{
    (void)x;
    (void)y;
    switch (key)
    {
        case 'w': // haut
            player->RemoveMovementFlag(MOVE_FORWARD);
            break;
        case 's': // bas
            player->RemoveMovementFlag(MOVE_BACKWARD);
            break;
        case 'd': // droite
            player->RemoveMovementFlag(MOVE_TURN_RIGHT);
            break;
        case 'a': // gauche
            player->RemoveMovementFlag(MOVE_TURN_LEFT);
            break;
        case 'k': // fly
        case 'j': // Down
        default:
            break;
    }
}

void idle()
{
    glutPostRedisplay();
}

void visibility(int state)
{
    if (state == GLUT_VISIBLE)
        glutIdleFunc(idle);
    else
        glutIdleFunc(NULL);
}

void UpdatePlayerPosition(int i = 0)
{
    (void)i;
    glutTimerFunc(2, UpdatePlayerPosition, 0);
    player->UpdatePosition();
    float x, y, z, o;
    player->GetPosition(x, y, z, o);
    (void)o;
    pos[0] = x + dist * cos(o);
    pos[1] = y + dist * sin(o);
}

int main(int ac, char **av)
{
    srand(time(NULL));
    map = MapGenerator::CreateNewRandomMap(WIDTH, HEIGHT, 0.35, 0.35);
    player = new Object(2, 7.0f, 7.0f, 0.0f, 0.0f);
    map->AddObject(player);
    glutInit(&ac, av);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("OpenGL fps");
    glClearColor(0.2, 0.2, 0.2, 1.0);
    glEnable(GL_DEPTH_TEST);

    for (int i = 0; i < 257; ++i)
        keyStates[i] = false;

    glutDisplayFunc(draw);
    glutReshapeFunc(handleResize);
    glutSpecialFunc(keySpecial);
    glutKeyboardFunc(keyPressed);
    glutKeyboardUpFunc(keyUp);
    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

    glutIdleFunc(idle);
    glutVisibilityFunc(visibility);
    UpdatePlayerPosition();
    //keyOperations();
    glutPostRedisplay();

    glutMainLoop();
    return 0;
}
