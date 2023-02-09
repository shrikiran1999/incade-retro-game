#ifndef ASTEROIDS_H_IS_INCLUDED
#define ASTEROIDS_H_IS_INCLUDED

class Target {
public:
	double x, y;
	double vx;
	double wid = 35;
	double hei = 47;
	int state;
	double timeExploded;
	void Initialize();
	void Draw(YsRawPngDecoder& targetL, YsRawPngDecoder& targetR);
	void Update(double dt);
	void Explode(YsRawPngDecoder& explosionSmall, double dt);
};

class Player {
public:
	int x, y;
	//double theta;
	void Command(int key);
	void Draw(YsRawPngDecoder& player);
	void Initialize();
};

class Missile {
public:
	int x, y;
	int wid = 8;
	int hei = 20;
	int state; //0 is unused, 1 is active, 2 is used
	int vx, vy;
	void Initialize();
	void Launch(int x0, int y0);
	void UpdatePos(double dt);
	void Draw(YsRawPngDecoder& missileImg);
	int CheckCollision(int tx, int ty, int tw, int th);
	void BlowUp(void);
};

class Obstacle {
public:
	double x, y;
};

class Asteroid : public Obstacle {
public:
	int size;
	double vx, vy;
	int wid, hei;
	int state;
	int num; //should i make this constant? how do i avoid error?
	double timeExploded;
	void Initialize();
	void Draw(YsRawPngDecoder& asteroidSmall, YsRawPngDecoder& asteroidMedium, YsRawPngDecoder& asteroidLarge);
	void Explode(YsRawPngDecoder& explosionSmall, YsRawPngDecoder& explosionMedium, YsRawPngDecoder& explosionLarge, double dt);
	void Settle(double dt);
	void UpdatePos(double dt);
};




class AsteroidsGame {
public:
	
	int ammoUsed = 0;
	int ammo = 20;
	int enemiesDestroyed = 0;
	Asteroid asteroids[20];
	Player ship;
	Target enemy[4];
	Missile missiles[20];
	YsRawPngDecoder asteroidSmall;
	YsRawPngDecoder asteroidMedium;
	YsRawPngDecoder asteroidLarge;
	YsRawPngDecoder player;
	YsRawPngDecoder explosionSmall;
	YsRawPngDecoder explosionMedium;
	YsRawPngDecoder explosionLarge;
	YsRawPngDecoder targetL;
	YsRawPngDecoder targetR;
	YsRawPngDecoder missileImg;
	YsRawPngDecoder background;
	
	
	
	//bool terminate;
	void Initialize();
	void Run();
};

#endif