#pragma once
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


using namespace std;


class SpaceDebris; //forward declaration

class RotateRocketGame
{
public:
	RotateRocketGame(int x, int y, float theta);
	~RotateRocketGame();
	void DebugTest();
	void DrawRotateRocketBG(int x, int y);
	void DrawCircle(int c_x, int c_y, int r);
	void DrawRect(int x1, int y1, int x2, int y2);
	//void RotateRocketMusic();
	//void Init();
	void Run();
	//vector<space_RotateRocketGame> vecDebris;
	//vector<space_RotateRocketGame> vecLaser;
	//space_RotateRocketGame player;

	int x;
	int y;
	float theta;
	int num;

	//float ship_X, ship_Y;
	//int numLives;
	//float acceleration;

	//float laser_x, laser_y, laser_theta;

	//int debrisState[6];
	//float debrisX[6], debrisY[6];
	//int rand_Vy[6], rand_Vx[6];
	////int Vy[6], Vx[6];
	//bool flag_ma;

	int debrisState[6];
	float debrisX[6], debrisY[6];
	int rand_Vy[6], rand_Vx[6];


protected:

	const float YS_PI = 3.1415927;

	int gamestate;
	int clearall;
	int points;
	bool shoot;

};

class Laser_RR : public RotateRocketGame
{
public:
	Laser_RR(int x, int y, float theta);
	~Laser_RR();

	//Laser_RR* FireLaser(float laser_x, float laser_y, float ship_X, float ship_Y, float theta, float& laser_theta);
	void FireLaser(float& laser_x, float& laser_y, float ship_X, float ship_Y, float theta, float& laser_theta);
	void LaserMotion(float& laser_x, float& laser_y, float laser_theta, const float dt);
	void Draw(float laser_x, float laser_y);
	void DebugTest();


	float laser_x, laser_y, laser_theta;

protected:

};

class Spaceship : public RotateRocketGame
{
public:
	Spaceship(int x, int y, float theta);
	~Spaceship();
	void rotateLeft(const float delta = 0.09);
	void rotateRight(const float delta = 0.09);
	void accelerate(const float delta = 0.09);
	void decelerate(const float delta = 0.09);
	//Laser_RR* Fire();
	void Draw(int x, int y, float& ship_X, float& ship_Y, float theta);
	//bool IsSpaceshipDestroyed(Spaceship*, SpaceDebris*); 
	void DrawLives(int numLives, int points);
	void DebugTest();

	float ship_X, ship_Y;
	float acceleration;
	int numLives;

protected:

};

class SpaceDebris /*: public RotateRocketGame*/
{
public:
	SpaceDebris(/*int x, int y, float theta*/);
	~SpaceDebris();
	void DebugTest();

	void CreateSpaceDebris(vector<SpaceDebris*>& asteroids, int num, int debrisState[], float debrisX[], float debrisY[]);
	void DrawPolygon(int p_x, int p_y, int r);
	void MoveSpaceDebris(int num, float debrisX[], float debrisY[], const float dt, int rand_Vy[], int rand_Vx[], bool flag_ma);
	void Draw(int debrisState[], float debrisX[], float debrisY[]);
	//bool IsAsteroidShipDestroyed(Spaceship*, SpaceDebris*, /*int debrisX, int debrisY,*/ int num);

	bool flag_ma;

protected:
	//int debrisState[6];
	//float debrisX[6], debrisY[6];
	//int rand_Vy[6], rand_Vx[6];
	//int Vy[6], Vx[6];
};

//---------------------------------------------------- RotateRocketGame -------------------------------------------------------------------------------

RotateRocketGame::RotateRocketGame(int x, int y, float speed)
{
	this->x = x;
	this->y = y;
	this->theta = 0;
	this->num = 6;
	this->gamestate = 0;
	this->clearall = 0;
	this->points = 0;
	this->shoot = false;

	for (int i = 0; i < 6; i++)
	{
		this->debrisState[i] = 1;
		this->debrisX[i] = 0;
		this->debrisY[i] = 0;

		//this->Vy[i] = 0;
		//this->Vx[i] = 0;
	}

	for (int j = 0; j < 3; j++)
	{
		this->rand_Vy[j] = (int)rand() % 4 + (1);
		this->rand_Vx[j] = (int)rand() % 4 + (1);
	}
	for (int j = 3; j < 6; j++)
	{
		this->rand_Vy[j] = -((int)rand() % 4 + (1));
		this->rand_Vx[j] = -((int)rand() % 4 + (1));
	}

}

RotateRocketGame::~RotateRocketGame()
{
}

void RotateRocketGame::DebugTest()
{
	printf("Printing RotateRocketGame data\n");
	printf("\tX value is : %d\n\tY value is : %d\n\tDirection is : %lf\n", x, y, theta);
}

void RotateRocketGame::DrawRotateRocketBG(int x, int y)
{
	glColor3ub(0, 0, 0);
	DrawRect(x, y, 0, 0);

	glColor3ub(255, 255, 255);
	for (int i = 0; i < 5; i++)
	{
		glBegin(GL_POINTS);
		glVertex3f(rand() % 800, rand() % 600, 0);
	}
	glEnd();
}



void RotateRocketGame::DrawRect(int x1, int y1, int x2, int y2)
{
	glColor3ub(0, 0, 0);
	glBegin(GL_QUADS);

	glVertex2i(x1, y1);
	glVertex2i(x2, y1);
	glVertex2i(x2, y2);
	glVertex2i(x1, y2);

	glEnd();
}

void RotateRocketGame::DrawCircle(int c_x, int c_y, int r)
{
	const double YS_PI = 3.1415927;
	int i, c_points = 100;

	glBegin(GL_POLYGON);

	for (i = 0; i < c_points; i++)
	{
		double theta = (double)i * YS_PI * 2 / c_points;
		double x = (double)c_x + cos(theta) * (double)r;
		double y = (double)c_y + sin(theta) * (double)r;
		glVertex2d(x, y);
	}

	glEnd();
}
//void RotateRocketGame::RotateRocketMusic()
//{
//	YsSoundPlayer player;
//	YsSoundPlayer::SoundData wav;
//
//	char filename[] = "rotaterocket.wav";
//	if (YSOK != wav.LoadWav(filename))
//	{
//		cout << "Failed to read: " << filename << endl;
//	}
//	player.PlayBackground(wav);
//}


//---------------------------------------------------- SPACESHIP -------------------------------------------------------------------------------

Spaceship::Spaceship(int x, int y, float theta) : RotateRocketGame(x, y, theta)
{
	this->x = x;
	this->y = y;
	this->theta = theta;
	this->numLives = 3;
	this->acceleration = 0;
	this->ship_X = 0;
	this->ship_Y = 0;

}

Spaceship::~Spaceship()
{
}

void Spaceship::rotateLeft(const float delta)
{
	this->theta -= delta;
}

void Spaceship::rotateRight(const float delta)
{
	this->theta += delta;
}

//void Spaceship::Move(/*int x, int y, float theta*/)
//{
//	speed += acceleration;
//	if (speed > 8)
//		speed = 8;
//
//	if (speed < 0)
//		speed = 0;
//
//}

void Spaceship::accelerate(const float delta)
{
	acceleration += delta;

	if (acceleration > 3)
		acceleration = 3;
}

void Spaceship::decelerate(const float delta)
{
	acceleration -= delta;

	if (acceleration < -3)
		acceleration = -3;
}

void Spaceship::Draw(int x, int y, float& ship_X, float& ship_Y, float theta)
{
	ship_X = (float)(x + 15 * cos(theta));
	ship_Y = (float)(y + 15 * sin(theta));

	glColor3ub(255, 0, 0);

	glBegin(GL_TRIANGLES);
	glVertex2i(x + 15 * cos(theta), y + 15 * sin(theta));
	glVertex2i(x + 15 * cos(theta + 2 * YS_PI / 3), y + 15 * sin(theta + 2 * YS_PI / 3));
	glVertex2i(x + 15 * cos(theta + 4 * YS_PI / 3), y + 15 * sin(theta + 4 * YS_PI / 3));

	glEnd();
}

//
//bool Spaceship::IsSpaceshipDestroyed(Spaceship* spaceship, SpaceDebris* asteroid)
//{
//	if ((spaceship->x - 8 > asteroid->x + 20 - 1) || (spaceship->y - 11 > asteroid->y + 20 - 1) || (asteroid->x > spaceship->x - 20 + 15 - 1) || (asteroid->y > spaceship->y + 20 - 1))
//		return false;
//	else
//	{
//		return true;
//	}
//}

void Spaceship::DrawLives(int numLives, int points)
{
	string buffer = "Points: ";
	string str = to_string(points);
	buffer += str;
	int i, n = 0;
	for (i = 0; i < numLives; i++)
	{
		glColor3ub(0, 255, 0);
		DrawCircle(780 - n, 20, 5);
		n += 20;
	}

	glColor3ub(0, 0, 255);
	glRasterPos2i(680, 50);
	YsGlDrawFontBitmap10x14(buffer.c_str());


}

void Spaceship::DebugTest()
{
	printf("The follwing is a spaceship!\n");
	RotateRocketGame::DebugTest();
}

 
//--------------------------------------------------- SPACE DEBRIS -----------------------------------------------------------------------------

SpaceDebris::SpaceDebris(/*int x, int y, float theta*/)/* : RotateRocketGame(x, y, theta)*/
{
	/*this->x = x;
	this->y = y;
	this->theta = theta;*/

	//for (int i = 0; i < 6; i++)
	//{
	//	this->debrisState[i] = 1;
	//	this->debrisX[i] = 0;
	//	this->debrisY[i] = 0;

	//	this->Vy[i] = 0;
	//	this->Vx[i] = 0;
	//}

	//for (int j = 0; j < 3; j++)
	//{
	//	this->rand_Vy[j] = (int)rand() % 4 + (1);
	//	this->rand_Vx[j] = (int)rand() % 4 + (1);
	//}
	//for (int j = 3; j < 6; j++)
	//{
	//	this->rand_Vy[j] = -((int)rand() % 4 + (1));
	//	this->rand_Vx[j] = -((int)rand() % 4 + (1));
	//}

	this->flag_ma = true;
}

SpaceDebris::~SpaceDebris()
{
}

void SpaceDebris::CreateSpaceDebris(vector<SpaceDebris*>& asteroids, int num, int debrisState[], float debrisX[], float debrisY[])
{

	for (int i = 0; i < num; i++)
	{
		if (debrisState[i] == 1)
		{
			debrisX[i] = (rand() % (800 - 20) + 20.0);;
			debrisY[i] = rand() % (600 - 20) + 20.0;

			if ((debrisX[i] > 370 || debrisX[i] < 430) && (debrisY[i] > 270 || debrisY[i] < 330))
			{
				bool flag = true;
				while (flag)
				{
					debrisX[i] = rand() % (800 - 20) + 20.0;;
					debrisY[i] = rand() % (600 - 20) + 20.0;
					if ((debrisX[i] > 370 || debrisX[i] < 430) && (debrisY[i] > 270 || debrisY[i] < 330))
					{
						flag = false;
					}
				}
			}
		}
	}
}

void SpaceDebris::DrawPolygon(int p_x, int p_y, int r)
{
	const double YS_PI = 3.1415927;
	int i, verts = 11;

	glBegin(GL_POLYGON);

	for (i = 0; i < verts; i++)
	{
		double theta = (double)i * YS_PI * 2 / verts;
		double x = (double)p_x + cos(theta) * (double)r + ((float)rand() / (float)RAND_MAX * cos(theta) * sin(theta) + 0.6);
		double y = (double)p_y + sin(theta) * (double)r + ((float)rand() / (float)RAND_MAX * cos(theta) * sin(theta) + 0.4);
		glVertex2d(x, y);
	}

	glEnd();
}

void SpaceDebris::MoveSpaceDebris(int num, float debrisX[], float debrisY[], const float dt, int rand_Vy[], int rand_Vx[], bool flag_ma)
{
	int Vy[6], Vx[6];
	if (flag_ma)
	{
		for (int i = 0; i < num; i++)
		{
			Vy[i] = rand_Vy[i];
			Vx[i] = rand_Vx[i];
		}
	}

	for (int i = 0; i < num; i++)
	{
		debrisY[i] -= (rand_Vy[i] * dt);
		if (0 > debrisY[i] || 600 < debrisY[i])
		{
			rand_Vy[i] = (-Vy[i]);
		}

		debrisX[i] -= (rand_Vx[i] * dt);
		if (0 > debrisX[i] || 800 < debrisX[i])
		{
			rand_Vx[i] = -Vx[i];
		}
	}
}

void SpaceDebris::Draw(int debrisState[], float debrisX[], float debrisY[])
{
	for (int i = 0; i < 6; i++)
	{
		if (debrisState[i] == 1)
		{
			glColor3ub(128, 128, 128);
			DrawPolygon(debrisX[i], debrisY[i], 30);

			glColor3ub(150, 150, 150);
			DrawPolygon(debrisX[i] + 2 * ((float)i + 1), debrisY[i] - 2 * ((float)i + 1), 5);

			glColor3ub(140, 140, 140);
			DrawPolygon(debrisX[i] - ((float)i + 2), debrisY[i] + 2 * ((float)i + 1), 7);

			//glEnd();
		}
	}
}

void SpaceDebris::DebugTest()
{
	printf("The follwing is a Space Debris!\n");
}

//---------------------------------------------------- LASER -------------------------------------------------------------------------------

Laser_RR::Laser_RR(int x, int y, float theta) : RotateRocketGame(x, y, theta)
{
	this->laser_theta = theta;
	this->laser_x = x;
	this->laser_y = y;
};

Laser_RR::~Laser_RR()
{
}

//Laser* Laser::FireLaser(float laser_x, float laser_y, float ship_X, float ship_Y, float theta, float& laser_theta)
void Laser_RR::FireLaser(float& laser_x, float& laser_y, float ship_X, float ship_Y, float theta, float& laser_theta)
{
	//Laser* blast = new Laser(0,0,0);
	//blast->laser_x = ship_X;
	//blast->laser_y = ship_Y;
	//blast->laser_theta = theta;
	//return blast;

	laser_x = ship_X;
	laser_y = ship_Y;
	laser_theta = theta;
}

void Laser_RR::LaserMotion(float& laser_x, float& laser_y, float laser_theta, float dt)
{
	if (laser_x * laser_y > 0 || laser_x < 800 || laser_y < 600)
	{
		laser_x += 20 * cos(laser_theta) * dt;
		laser_y += 20 * sin(laser_theta) * dt;
	}

}

void Laser_RR::Draw(float laser_x, float laser_y)
{
	glColor3ub(255, 255, 0);
	DrawCircle(laser_x, laser_y, 5);
};

void Laser_RR::DebugTest()
{
	printf("The follwing is a Laser!\n");
	RotateRocketGame::DebugTest();
}




void RotateRocketGame::Run()
{
	vector<SpaceDebris*> space_debris;
	vector<Laser_RR*> lasers;

	srand(time(NULL));
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	RotateRocketGame* obj = new RotateRocketGame(400, 300, 0);
	SpaceDebris* debris = new SpaceDebris();
	Spaceship* ship = new Spaceship(400, 300, 0);
	Laser_RR* laser = new Laser_RR(ship->x, ship->y, ship->theta);


	YsSoundPlayer player;
	YsSoundPlayer::SoundData wav;

	char filename[] = "rotaterocket.wav";
	if (YSOK != wav.LoadWav(filename))
	{
		cout << "Failed to read: " << filename << endl;
	}
	player.Start();
	player.PlayBackground(wav);


	debris->CreateSpaceDebris(space_debris, num, debrisState, debrisX, debrisY);
	ship->Draw(400, 300, ship->ship_X, ship->ship_Y, 0);
	ship->DrawLives(ship->numLives, points);


	while (gamestate == 0)
	{
		FsPollDevice();
		int key = FsInkey();
		switch (key)
		{
		case FSKEY_ESC:
			gamestate = 1;
			break;

		case FSKEY_LEFT:
			ship->rotateLeft(0.1);
			break;

		case FSKEY_RIGHT:
			ship->rotateRight(0.1);
			break;

		case FSKEY_UP:
			/*ship->accelerate(0.2);*/
			ship->theta += 0.2;
			break;

		case FSKEY_DOWN:
			/*ship->decelerate(0.2);*/
			ship->theta -= 0.2;
			break;

		case FSKEY_SPACE:
			laser->FireLaser(laser->laser_x, laser->laser_y, ship->ship_X, ship->ship_Y, ship->theta, laser->laser_theta);
			/*Laser* blast = laser->FireLaser(laser->laser_x, laser->laser_y, ship->ship_X, ship->ship_Y, ship->theta, laser->laser_theta);
			lasers.push_back(blast);*/
			shoot = true;
			break;
		}

		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		obj->DrawRotateRocketBG(800, 600);
		ship->Draw(400, 300, ship->ship_X, ship->ship_Y, ship->theta);
		ship->DrawLives(ship->numLives, points);

		debris->Draw(/*debris->*/debrisState, /*debris->*/debrisX, /*debris->*/debrisY);
		debris->MoveSpaceDebris(num,/* debris->*/debrisX,/* debris->*/debrisY, 0.25, /*debris->*/rand_Vy, /*debris->*/rand_Vx, debris->flag_ma);

		if (shoot == true)
		{
			laser->LaserMotion(laser->laser_x, laser->laser_y, laser->laser_theta, 0.25);
			laser->Draw(laser->laser_x, laser->laser_y);

			for (int i = 0; i < num; i++)
			{
				float diff_X, diff_Y;
				diff_X = laser->laser_x - /*debris->*/debrisX[i];
				if (diff_X < 0)
					diff_X = -diff_X;
				diff_Y = laser->laser_y -/* debris->*/debrisY[i];
				if (diff_Y < 0)
					diff_Y = -diff_Y;

				if (debrisState[i] == 1 && 0 <= diff_X && diff_X < 30 && 0 <= diff_Y && diff_Y < 30)
				{
					debrisState[i] = 0;
					shoot = false;
					clearall++;
					points += 50;
				}
			}
		}

		if (clearall > 5)
		{
			for (int i = 0; i < num; i++)
			{
				/*debris->*/debrisState[i] = 1;
				glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
				SpaceDebris* debris = new SpaceDebris();
				Spaceship* ship = new Spaceship(400, 300, 0);
				Laser_RR* laser = new Laser_RR(ship->x, ship->y, ship->theta);
				debris->CreateSpaceDebris(space_debris, num, debrisState, debrisX, debrisY);
				clearall = 0;
			}

		}

		if (ship->numLives > 0)
		{
			int i = 0;
			while (clearall != 6 && i < num)
			{
				float diff_X_ship, diff_Y_ship;
				diff_X_ship = ship->x - /*debris->*/debrisX[i];
				if (diff_X_ship < 0)
					diff_X_ship = -diff_X_ship;
				diff_Y_ship = ship->y - /*debris->*/debrisY[i];
				if (diff_Y_ship < 0)
					diff_Y_ship = -diff_Y_ship;
				if (/*debris->*/debrisState[i] == 1 && 0 <= diff_X_ship && diff_X_ship < 25 && 0 <= diff_Y_ship && diff_Y_ship < 25)
				{
					/*debris->*/debrisState[i] = 0;
					ship->numLives -= 1;
					printf("%d\n", ship->numLives);
					glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
					SpaceDebris* debris = new SpaceDebris(/*200, 200, 0*/);
					Spaceship* ship = new Spaceship(400, 300, 0);
					Laser_RR* laser = new Laser_RR(ship->x, ship->y, ship->theta);
					clearall = 6;
				}
				i += 1;
			}
		}
		if (ship->numLives <= 0)
		{
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
			glColor3ub(0, 0, 0);
			glRasterPos2i(340, 270);
			YsGlDrawFontBitmap16x20("Game Over!");
			player.End();
		}
		FsSwapBuffers();
		FsSleep(10);
	}
}





//void DrawHeart()
//{
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	gluOrtho2D(-2, 2, -2, 2);
//	glPointSize(1);
//	glColor3ub(255, 0, 0);
//	glBegin(GL_POINTS);
//	for (float x = -1.139; x <= 1.139; x += 0.001)
//	{
//		float delta = cbrt(x * x) * cbrt(x * x) - 4 * x * x + 4;
//		float y1 = (cbrt(x * x) + sqrt(delta)) / 2;
//		float y2 = (cbrt(x * x) - sqrt(delta)) / 2;
//		glVertex2f(x, y1);
//		glVertex2f(x, y2);
//	}
//	glEnd();
//
//	glFlush();
//}

//if (ship->IsSpaceshipDestroyed() == true)
//{
//	numLives -= 1;
//	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
//	ship->DrawLives(numLives);
//	Asteroid* asteroid = new Asteroid(200, 200, 0);
//	Spaceship* ship = new Spaceship(400, 300, 0);
//	Laser* laser = new Laser(ship->x, ship->y, ship->theta);
//	asteroid->CreateSpaceDebris(asteroids, num, debrisState, debrisX, debrisY);	

/*for (int i = 0; i < lasers.size(); i++)
{
	lasers[i]->Draw(laser->laser_x, laser->laser_y);
	lasers[i]->LaserMotion(laser->laser_x, laser->laser_y, 0.25);
}*/

//void RotateRocketGame::Init()
//{
//	//gamestate = 0;
//	//numLives = 3;
//	//clearall = 0;
//	//points = 0;
//	//flag_ma = true;
//	//shoot = false;
//
//	//const int num = 6;
//	//int debrisState[num] = { 1,1,1,1,1,1 };
//
//	//for (int j = 0; j < 3; j++)
//	//{
//	//	rand_Vy[j] = (int)rand() % 4 + (1);
//	//	rand_Vx[j] = (int)rand() % 4 + (1);
//	//}
//	//for (int j = 3; j < 6; j++)
//	//{
//	//	rand_Vy[j] = -((int)rand() % 4 + (1));
//	//	rand_Vx[j] = -((int)rand() % 4 + (1));
//	//}
//
//
//}

//void Spaceship::RepositionSpaceship(Spaceship* spaceship)
//{
//	spaceship->x = 400;
//	spaceship->y = 300;
//	spaceship->speed = 0;
//}