//Base includes
#include <stdio.h>
#include <math.h>
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include<string>


//asteroids includes

//redundant
#include <stdio.h>
#include <math.h>
#include "fssimplewindow.h"
#include "ysglfontdata.h"
//not redundant
#include "yssimplesound.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <chrono>
#include <thread>
#include "yspng.h"
#include "AsteroidsForSharing.h"



//wacamole includes
//redundant
#include "fssimplewindow.h"
#include "yssimplesound.h"
#include <time.h>
#include<chrono>
#include <string.h>
#include <math.h>
#include <stdio.h>

//not redundant
#include <iostream>
#include <stdlib.h>
#include <stddef.h>
#include "whacamole.h"



//rotate rocket 
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string>
#include <math.h>
#include <time.h>
#include <vector>
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include "yssimplesound.h"
#include "rotaterocket.h"


//aviators

#include <time.h>
#include <math.h>
#include "fssimplewindow.h"
#include "yssimplesound.h"
#include "yspng.h"
#include "ysglfontdata.h"
#include "AviatorsForSharing.h"


const double YsPi = 3.1415927;
int window_width = 600;
int window_height = 800;

class CameraObject
{
public:
    double x, y, z;
    double h, p, b;

    double fov, nearZ, farZ;

    CameraObject();
    void Initialize(void);
    void SetUpCameraProjection(void);
    void SetUpCameraTransformation(void);

    void GetForwardVector(double& vx, double& vy, double& vz);
    void GetUpVector(double& vx, double& vy, double& vz);
    void GetRightVector(double& vx, double& vy, double& vz);
    float GetX();
    float GetZ();
};

CameraObject::CameraObject()
{
    Initialize();
}

void CameraObject::Initialize(void)
{
    x = 0;
    y = 0;
    z = 0;
    h = 0;
    p = 0;
    b = 0;

    fov = YsPi / 6.0;  // 30 degree
    nearZ = 0.1;
    farZ = 200.0;
}

void CameraObject::SetUpCameraProjection(void)
{
    int wid, hei;
    double aspect;

    FsGetWindowSize(wid, hei);
    aspect = (double)wid / (double)hei;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov * 180.0 / YsPi, aspect, nearZ, farZ);
}

void CameraObject::SetUpCameraTransformation(void)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotated(-b * 180.0 / YsPi, 0.0, 0.0, 1.0);  // Rotation about Z axis
    glRotated(-p * 180.0 / YsPi, 1.0, 0.0, 0.0);  // Rotation about X axis
    glRotated(-h * 180.0 / YsPi, 0.0, 1.0, 0.0);  // Rotation about Y axis
    glTranslated(-x, -y, -z);
}

void CameraObject::GetForwardVector(double& vx, double& vy, double& vz)
{
    vx = -cos(p) * sin(h);
    vy = sin(p);
    vz = -cos(p) * cos(h);
}

void CameraObject::GetUpVector(double& vx, double& vy, double& vz)
{
    const double ux0 = 0.0;
    const double uy0 = 1.0;
    const double uz0 = 0.0;

    const double ux1 = cos(b) * ux0 - sin(b) * uy0;
    const double uy1 = sin(b) * ux0 + cos(b) * uy0;
    const double uz1 = uz0;

    const double ux2 = ux1;
    const double uy2 = cos(p) * uy1 - sin(p) * uz1;
    const double uz2 = sin(p) * uy1 + cos(p) * uz1;

    vx = cos(-h) * ux2 - sin(-h) * uz2;
    vy = uy2;
    vz = sin(-h) * ux2 + cos(-h) * uz2;
}

void CameraObject::GetRightVector(double& vx, double& vy, double& vz)
{
    const double ux0 = 1.0;
    const double uy0 = 0.0;
    const double uz0 = 0.0;

    const double ux1 = cos(b) * ux0 - sin(b) * uy0;
    const double uy1 = sin(b) * ux0 + cos(b) * uy0;
    const double uz1 = uz0;

    const double ux2 = ux1;
    const double uy2 = cos(p) * uy1 - sin(p) * uz1;
    const double uz2 = sin(p) * uy1 + cos(p) * uz1;

    vx = cos(-h) * ux2 - sin(-h) * uz2;
    vy = uy2;
    vz = sin(-h) * ux2 + cos(-h) * uz2;
}

float CameraObject::GetX()
{
    return x;
}

float CameraObject::GetZ()
{
    return z;
}
//==================================================================================

class arcadeMachine
{
private:
    //constant values
    double halfwidth = 2.75; //for 1, x direction
    double halfdepth = 3; //for 1, z direction
    double baseHeight = 2.75; //always y direction
    double smallThickness = 4;
    double midHeight = 4;
    double topHeight = 2;
    double topDepth = 5;
    //

    double x, z;
    double x_lim1, x_lim2, z_lim1, z_lim2;
    double x_act1, x_act2, z_act1, z_act2;
    int gameID;
    std::string gameName;

    void DrawCube(double x1, double y1, double z1, double x2, double y2, double z2);


public:
    //arcadeMachine(int xinit, int zinit, int gameID_input, std::string gameName_input);
    void Initialize(int x, int z, int gameID, std::string gameName);

    bool detectCollision(int x, int z);
    int detectActivation(int x, int z, bool dispText);

    int getGameID();
    std::string getGameString();
    float GetX();
    float GetZ();

    void render();

    void cleanUp();
};


void arcadeMachine::Initialize(int xinit, int zinit, int gameID_input, std::string gameName_input)
{
    x = xinit;
    z = zinit;
    gameID = gameID_input;
    gameName = gameName_input;

    x_lim1 = x - halfwidth;
    x_lim2 = x + halfwidth;

    z_lim1 = z - halfdepth;
    z_lim2 = z + halfdepth;


    //double x_act1, x_act2, z_act1, z_act2
    x_act1 = x_lim1;
    x_act2 = x_lim2;
    z_act1 = z_lim2 - 0.5;
    z_act2 = z_lim2 + 3.5 * halfdepth;

}


bool arcadeMachine::detectCollision(int x, int z) // returns an integer 0 is no collision, 1 is foward, -1 is reverse,  2 is left, -2 is right
{
    if (x > x_lim1 && x < x_lim2 && z > z_lim1 && z < z_lim2)
    {
        printf("Collision with %d\n", gameID);
        return true;
    }
    return false;
}

int arcadeMachine::detectActivation(int x, int z, bool dispText)
{
    if (x > x_act1 && x < x_act2 && z > z_act1 && z < z_act2)
    {
        if (dispText)
        {
            glColor3ub(255, 0, 0);
            glRasterPos2i(40, window_height - 40);
            YsGlDrawFontBitmap24x40(("Press Space to play " + gameName).c_str());
            glEnd();
        }


        printf("Activation of %d\n", gameID);
        return gameID;

    }
    return -1;
}

int arcadeMachine::getGameID()
{
    return gameID;
}

float arcadeMachine::GetX()
{
    return x;
}

float arcadeMachine::GetZ()
{
    return z;
}

void arcadeMachine::render()
{
    int orientation = 1;
    if (orientation == 1)
    {
        //first coordinates are generally the same across all three (except for Y) because it is the back bottom

        //base of machine
        DrawCube(x - halfwidth, 0, z - halfdepth, x + halfwidth, baseHeight, z + halfdepth);

        //middle part of machine
        DrawCube(x - halfwidth, baseHeight, z - halfdepth, x + halfwidth, midHeight + baseHeight, z - halfdepth + smallThickness);

        //top of machine
        DrawCube(x - halfwidth, midHeight + baseHeight, z - halfdepth, x + halfwidth, midHeight + baseHeight + topHeight, z - halfdepth + topDepth);

        //shows the activation area
        bool renderActivation = true;
        if (renderActivation)
        {
            glBegin(GL_QUADS);

            glColor3ub(255, 165, 0);
            glVertex3d(x_act1, 0, z_act1);
            glVertex3d(x_act2, 0, z_act1);
            glVertex3d(x_act2, 0, z_act2);
            glVertex3d(x_act1, 0, z_act2);

            glEnd();
        }
    }

}

std::string arcadeMachine::getGameString()
{
    return gameName;
}

void arcadeMachine::DrawCube(double x1, double y1, double z1, double x2, double y2, double z2)
{

    bool textureEnable = false;
    if (!textureEnable)
    {
        glBegin(GL_QUADS);

        glColor3ub(0, 0, 255);
        glVertex3d(x1, y1, z1);
        glVertex3d(x2, y1, z1);
        glVertex3d(x2, y2, z1);
        glVertex3d(x1, y2, z1);

        glColor3ub(0, 255, 0);
        glVertex3d(x1, y1, z2);
        glVertex3d(x2, y1, z2);
        glVertex3d(x2, y2, z2);
        glVertex3d(x1, y2, z2);

        glColor3ub(0, 255, 255);
        glVertex3d(x1, y1, z1);
        glVertex3d(x2, y1, z1);
        glVertex3d(x2, y1, z2);
        glVertex3d(x1, y1, z2);

        glColor3ub(255, 0, 0);
        glVertex3d(x1, y2, z1);
        glVertex3d(x2, y2, z1);
        glVertex3d(x2, y2, z2);
        glVertex3d(x1, y2, z2);

        glColor3ub(255, 0, 255);
        glVertex3d(x1, y1, z1);
        glVertex3d(x1, y2, z1);
        glVertex3d(x1, y2, z2);
        glVertex3d(x1, y1, z2);

        glColor3ub(255, 255, 0);
        glVertex3d(x2, y1, z1);
        glVertex3d(x2, y2, z1);
        glVertex3d(x2, y2, z2);
        glVertex3d(x2, y1, z2);

        glEnd();
    }
    else
    {


        glBegin(GL_QUADS);

        glColor3ub(0, 0, 255);
        glVertex3d(x1, y1, z1);
        glVertex3d(x2, y1, z1);
        glVertex3d(x2, y2, z1);
        glVertex3d(x1, y2, z1);

        glColor3ub(0, 255, 0);
        glVertex3d(x1, y1, z2);
        glVertex3d(x2, y1, z2);
        glVertex3d(x2, y2, z2);
        glVertex3d(x1, y2, z2);

        glColor3ub(0, 255, 255);
        glVertex3d(x1, y1, z1);
        glVertex3d(x2, y1, z1);
        glVertex3d(x2, y1, z2);
        glVertex3d(x1, y1, z2);

        glColor3ub(255, 0, 0);
        glVertex3d(x1, y2, z1);
        glVertex3d(x2, y2, z1);
        glVertex3d(x2, y2, z2);
        glVertex3d(x1, y2, z2);

        glColor3ub(255, 0, 255);
        glVertex3d(x1, y1, z1);
        glVertex3d(x1, y2, z1);
        glVertex3d(x1, y2, z2);
        glVertex3d(x1, y1, z2);

        glColor3ub(255, 255, 0);
        glVertex3d(x2, y1, z1);
        glVertex3d(x2, y2, z1);
        glVertex3d(x2, y2, z2);
        glVertex3d(x2, y1, z2);

        glEnd();
    }

}

bool checkAllCollisions(int numMachines, arcadeMachine machines[], int intendedX, int intendedZ)
{
    int ceiling = 20;
    float floor = -0.1;
    int room_x1 = -25;
    int room_x2 = 25;
    int room_z1 = 1;
    int room_z2 = -45;

    for (int i = 0; i < numMachines; i++)
    {
        if (true == machines[i].detectCollision(intendedX, intendedZ))
        {
            return true;
        }
        if (intendedX > room_x2 - 0.1 || intendedX < room_x1 + 0.1 || intendedZ > room_z1 - 0.1 || intendedZ < room_z2 + 0.1)
        {
            return true;
        }
    }
    return false;
}

void drawRoom()
{

    int ceiling = 20;
    float floor = -0.1;
    int room_x1 = -25;
    int room_x2 = 25;
    int room_z1 = 1;
    int room_z2 = -45;
    glBegin(GL_QUADS);

    //floor
    glColor3ub(0, 0, 0);
    glVertex3d(room_x1, floor, room_z1);
    glVertex3d(room_x2, floor, room_z1);
    glVertex3d(room_x2, floor, room_z2);
    glVertex3d(room_x1, floor, room_z2);


    glColor3ub(160, 160, 160);
    //ceiling
    glVertex3d(room_x1, ceiling, room_z1);
    glVertex3d(room_x2, ceiling, room_z1);
    glVertex3d(room_x2, ceiling, room_z2);
    glVertex3d(room_x1, ceiling, room_z2);

    //walls
    glColor3ub(128, 128, 128);
    glVertex3d(room_x1, floor, room_z1);
    glVertex3d(room_x2, floor, room_z1);
    glColor3ub(160, 160, 160);
    glVertex3d(room_x2, ceiling, room_z1);
    glVertex3d(room_x1, ceiling, room_z1);

    glColor3ub(128, 128, 128);
    glVertex3d(room_x1, floor, room_z2);
    glVertex3d(room_x2, floor, room_z2);
    glColor3ub(160, 160, 160);
    glVertex3d(room_x2, ceiling, room_z2);
    glVertex3d(room_x1, ceiling, room_z2);

    glColor3ub(128, 128, 128);
    glVertex3d(room_x1, floor, room_z2);
    glVertex3d(room_x1, floor, room_z1);
    glColor3ub(160, 160, 160);
    glVertex3d(room_x1, ceiling, room_z1);
    glVertex3d(room_x1, ceiling, room_z2);

    glColor3ub(128, 128, 128);
    glVertex3d(room_x2, floor, room_z2);
    glVertex3d(room_x2, floor, room_z1);
    glColor3ub(160, 160, 160);
    glVertex3d(room_x2, ceiling, room_z1);
    glVertex3d(room_x2, ceiling, room_z2);


    glEnd();


    //int ceiling = 20;
    //int floor = -1;
    //int x1 = -25;
    //int x2 = 25;
    //int z1 = 0;
    //int z2 = -45;

    glBegin(GL_LINES);
    glColor3ub(255, 255, 255);

    //floor border
    glVertex3i(room_x1, floor, room_z1);
    glVertex3i(room_x1, floor, room_z2);

    glVertex3i(room_x2, floor, room_z1);
    glVertex3i(room_x2, floor, room_z2);

    glVertex3i(room_x1, floor, room_z1);
    glVertex3i(room_x2, floor, room_z1);

    glVertex3i(room_x1, floor, room_z2);
    glVertex3i(room_x2, floor, room_z2);

    //ceiling border
    glVertex3i(room_x1, ceiling, room_z1);
    glVertex3i(room_x1, ceiling, room_z2);

    glVertex3i(room_x2, ceiling, room_z1);
    glVertex3i(room_x2, ceiling, room_z2);

    glVertex3i(room_x1, ceiling, room_z1);
    glVertex3i(room_x2, ceiling, room_z1);

    glVertex3i(room_x1, ceiling, room_z2);
    glVertex3i(room_x2, ceiling, room_z2);

    //wall borders
    glVertex3i(room_x1, ceiling, room_z1);
    glVertex3i(room_x1, floor, room_z1);

    glVertex3i(room_x2, ceiling, room_z1);
    glVertex3i(room_x2, floor, room_z1);

    glVertex3i(room_x2, ceiling, room_z2);
    glVertex3i(room_x2, floor, room_z2);

    glVertex3i(room_x1, ceiling, room_z2);
    glVertex3i(room_x1, floor, room_z2);

    glEnd();
}

int main(void)
{
    int terminate = 0;
    CameraObject camera;

    camera.z = 0;
    camera.y = 5.0;

    FsOpenWindow(16, 16, window_height, window_width, 1);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    int activationLocation = -1; //used to keep track of what activator you are on. -1 is 3d space
    int gameMode = -1;
    //different game modes
    // -1 - 3D environment
    // 0 - Game 0
    // 1 - jumpy doggy
    // 2 - asteroids
    // 3 - Aviary Aviators
    // 4 - Rotate rocket

    //main loop

    //initating each machine
    const int NumMachines = 5;
    arcadeMachine machines[NumMachines];
    std::string gameNames[] = { "Asteroids",  "WacAMole", "Game 2", "Game 3", "Game 4" };

    //postional info of machines
    int frontDist = -40;
    int cadeLoc = -20;
    int steploc = 10;

    for (int i = 0; i < NumMachines; i++)
    {
        machines[i].Initialize(cadeLoc, frontDist, i, gameNames[i]);
        printf("Initalized at %d\n", i);
        cadeLoc += steploc;
    }


    //initalize games

    srand(time(nullptr));

    AsteroidsGame AsteroidGame;
    AsteroidGame.Initialize();

    WacMoleGame wacAMole;

    RotateRocketGame* RotateRocket = new RotateRocketGame(400, 300, 0);

    AviatorsGame aviator_level1;


    while (0 == terminate)
    {
        FsPollDevice();


        FsGetWindowSize(window_width, window_height);

        if (0 != FsGetKeyState(FSKEY_ESC))
        {
            break;
        }

        //int key = FsInkey();
        ////terminate check (Escape)
        //switch (key)
        //{
        //case FSKEY_ESC:
        //    terminate = 1;
        //    break;
        //}

        switch (gameMode)
        {
        case -1:
            //traditional planar motion with WASD
            if (0 != FsGetKeyState(FSKEY_A))
            {
                double vx, vy, vz;
                camera.GetRightVector(vx, vy, vz);

                double intendedX = camera.x - vx * 0.5;
                double intendedZ = camera.z - vz * 0.5;

                if (false == checkAllCollisions(NumMachines, machines, intendedX, intendedZ))
                {
                    camera.x -= vx * 0.5;
                    camera.z -= vz * 0.5;
                }

            }
            if (0 != FsGetKeyState(FSKEY_D))
            {
                double vx, vy, vz;
                camera.GetRightVector(vx, vy, vz);

                double intendedX = camera.x + vx * 0.5;
                double intendedZ = camera.z += vz * 0.5;

                if (false == checkAllCollisions(NumMachines, machines, intendedX, intendedZ))
                {
                    camera.x += vx * 0.5;
                    camera.z += vz * 0.5;
                }

            }
            if (0 != FsGetKeyState(FSKEY_W))
            {
                double vx, vy, vz;
                camera.GetForwardVector(vx, vy, vz);

                double intendedX = camera.x + vx * 0.5;
                double intendedZ = camera.z + vz * 0.5;

                if (false == checkAllCollisions(NumMachines, machines, intendedX, intendedZ))
                {
                    camera.x += vx * 0.5;
                    camera.z += vz * 0.5;
                }

            }
            if (0 != FsGetKeyState(FSKEY_S))
            {
                double vx, vy, vz;
                camera.GetForwardVector(vx, vy, vz);

                double intendedX = camera.x - vx * 0.5;
                double intendedZ = camera.z - vz * 0.5;

                if (false == checkAllCollisions(NumMachines, machines, intendedX, intendedZ))
                {
                    camera.x -= vx * 0.5;
                    camera.z -= vz * 0.5;
                }
            }


            //camera motion with arrow keys 
            // @todo move camera with mouse?
            if (0 != FsGetKeyState(FSKEY_LEFT))
            {
                camera.h += YsPi / 180.0;
            }
            if (0 != FsGetKeyState(FSKEY_RIGHT))
            {
                camera.h -= YsPi / 180.0;
            }
            if (0 != FsGetKeyState(FSKEY_UP))
            {
                camera.p += YsPi / 180.0;
            }
            if (0 != FsGetKeyState(FSKEY_DOWN))
            {
                camera.p -= YsPi / 180.0;
            }


            //checks game activation key <space bar>
            for (int i = 0; i < NumMachines; i++)
            {
                int tmpLoc = machines[i].detectActivation(camera.GetX(), camera.GetZ(), false);
                if (-1 != tmpLoc) //checksif you are standing on an activation
                {
                    activationLocation = tmpLoc;
                }
            }

            //game start key activation
            if (0 != FsGetKeyState(FSKEY_SPACE) && -1 != activationLocation)
            {
                gameMode = activationLocation;
            }

            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

            glViewport(0, 0, window_width, window_height);

            // Set up 3D drawing
            camera.SetUpCameraProjection();
            camera.SetUpCameraTransformation();

            glEnable(GL_DEPTH_TEST);
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(1, 1);

            // 3D drawing from here

            drawRoom();
            //render each machine    
            for (int i = 0; i < 5; i++)
            {
                machines[i].render();
            }




            //mesh lines
            glColor3ub(255, 255, 255);
            glBegin(GL_LINES);
            for (int x = -100; x <= 100; x += 5)
            {
                glVertex3i(x, -0.1, -100);
                glVertex3i(x, -.1, 100);
                glVertex3i(-100, -.1, x);
                glVertex3i(100, -.1, x);
            }
            glEnd();



            // Set up 2D drawing
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0, (float)window_width - 1, (float)window_height - 1, 0, -1, 1);


            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            glDisable(GL_DEPTH_TEST);

            //2D drawing here

            //

            for (int i = 0; i < 5; i++)
            {
                machines[i].detectActivation(camera.GetX(), camera.GetZ(), true);
            }

            FsSwapBuffers();
            break;
        case 0:
            //insert game stuff
            printf("IN 0");
            printf("Starting Asteorids");
            AsteroidGame.Initialize();
            AsteroidGame.Run();
            gameMode = -1;

            break;
        case 1:

            printf("Starting wac A mole");
            wacAMole.initialize();

            wacAMole.music(1);
            wacAMole.run();
            wacAMole.music(0);

            gameMode = -1;
            break;
        case 2:

            RotateRocket->Run();           
            printf("IN 2");

            gameMode = -1;
            break;
        case 3:
            aviator_level1.Initialize(0); 
            aviator_level1.Run();
            printf("IN 3");

            gameMode = -1;
            break;
        case 4:
            //insert game stuff
            printf("IN 4");

            gameMode = -1;
            break;

        }

        FsSleep(25);
    }

    return 0;
}