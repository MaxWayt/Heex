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

/*void keyOperations(int i = 0)
{
    glutTimerFunc(2, keyOperations, 0);
    (void)i;

    for (int i = 0; i < 256; ++i)
    {
        if (keyStates[i])
        {
            bool handled = true;
            switch ((char)i)
            {
                case 'w': // haut
                    movePos(dir[0], dir[1], dir[2], M_PI);
                    break;
                case 's': // bas
                    movePos(dir[0], dir[1], dir[2], 0.0);
                    break;
                case 'd': // droite
                    orrientation += (-0.10 * speed * 1.15);
                    //dir[1] += 1;
                    break;
                case 'a': // gauche
                    orrientation += (0.10 * speed * 1.15);
                    //dir[1] += -1;
                    break;
                case 'k':
                    dir[2] += 1;
                    pos[2] += 1;
                    break;
                case 'j':
                    if (dir[2] > 0)
                    {
                        dir[2] += -1;
                        pos[2] += -1;
                    }
                default:
                    handled = false;
            }
            if (handled)
            {
                pos[0] = dir[0] + dist * cos(orrientation);
                pos[1] = dir[1] + dist * sin(orrientation);
            }
        }
    }
}*/

void drawObject(Object const* obj)
{
    float x, y, z;
    obj->GetPosition(x, y, z);
    if (obj->GetModelId() == 1)
        drawCube(x, y, z, 1.0 * PRECISION, 1.0 * PRECISION, 4.0);
    else if (obj->GetModelId() == 2) // player
        drawCube(x - 0.3, y + 0.3, z, 0.6, 0.6, 1.80, 255, 0, 0);
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

/*void generateMap()
{
    std::cout << ">> Generating random map ";
    std::cout.flush();
    const unsigned int width = WIDTH;
    const unsigned int height = HEIGHT;
    double complexity = 0.35;
    double density = 0.35;

    map = new double*[height + 1];
    for (unsigned int i = 0; i <= height; ++i)
    {
        map[i] = new double[width + 1];
        for (unsigned int j = 0; j <= width; ++j)
            map[i][j] = 0;
    }

    unsigned int shape[2];
    shape[0] = ((height / 2) * 2 + 1);
    shape[1] = ((width / 2) * 2 + 1);

    complexity = (int)(complexity * (5 * (shape[0] + shape[1])));
    density = (int)(density * (shape[0] / 2 * shape[1] / 2));

    for (unsigned int y = 0; y > height; ++y)
    {
        map[y][0] = 4;
        map[y][width - 1] = 4;
    }
    for (unsigned int x = 0; x < width; ++x)
    {
        map[0][x] = 4;
        map[height - 1][x] = 4;
    }

    for (int i = 0; i < (int)density; ++i)
    {
        unsigned int x = (rand() % (shape[1] / 2)) * 2;
        unsigned int y = (rand() % (shape[0] / 2)) * 2;
        map[y][x] = 3;
        if (i % ((int)density / 10) == 0)
        {
            std:: cout << ".";
            std::cout.flush();
        }

        for (int j = 0; j < (int)complexity; ++j)
        {
            std::vector<std::pair<int, int>> neighbours;
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
                std::pair<int, int> pair = neighbours[rand() % neighbours.size()];
                unsigned int y_ = pair.first;
                unsigned int x_ = pair.second;
                if (map[y_][x_] == 0)
                {
                    map[y_][x_] = 3;
                    map[y_ + ((int)y - (int)y_) / 2][x_ + ((int)x - (int)x_) / 2] = 3;
                    x = x_;
                    y = y_;
                }
            }
        }
    }
    for (unsigned int i = 0; i <= height; ++i)
        map[i][width] = -1;
    for (unsigned int i = 0; i <= width; ++i)
        map[height][i] = -1;
    std::cout << std::endl << "Map generated" << std::endl;
}*/

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
    player = new Object(2, 10.0f, 10.0f, 0.0f, 0.0f);
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
