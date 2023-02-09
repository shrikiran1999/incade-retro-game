#include "yssimplesound.h"
#include "fssimplewindow.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <chrono>
#include <thread>
#include "ysglfontdata.h"
#include "yspng.h"
#include "AsteroidsForSharing.h"

void Target::Initialize() {
	//x = 100 + rand() % 600;
	x = 10 + rand() % 750;
	y = 50;
	vx = 30 + rand() % 20;
	int sign = rand() % 10;
	if (sign < 5) {
		vx = -vx;
	}
	//printf("%f", vx);
	state = 1;
	timeExploded = 0;
}

void Target::Draw(YsRawPngDecoder& targetL, YsRawPngDecoder& targetR) {

	if (vx > 0) {
		glRasterPos2i(x, y);
		glDrawPixels(targetR.wid, targetR.hei, GL_RGBA, GL_UNSIGNED_BYTE, targetR.rgba);
	}
	if (vx < 0) {
		glRasterPos2i(x, y);
		glDrawPixels(targetL.wid, targetL.hei, GL_RGBA, GL_UNSIGNED_BYTE, targetL.rgba);
	}

}

void Target::Update(double dt) {
	x += vx * dt;
	if (vx > 0) {
		if (x > 750) {
			vx = -vx;
		}
	}
	if (vx < 0) {
		if (x < 10) {
			vx = -vx;
		}
	}
}

void Target::Explode(YsRawPngDecoder& explosionSmall, double dt) {

	timeExploded += dt;

	if (timeExploded < 0.75) {
		glRasterPos2i(x, y);
		glDrawPixels(explosionSmall.wid, explosionSmall.hei, GL_RGBA, GL_UNSIGNED_BYTE, explosionSmall.rgba);
	}
	else {
		state = 0;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Command(int key) {
	if (FSKEY_LEFT == key) {
		if (x > 10) {
			x -= 10;
		}
	}
	else if (FSKEY_RIGHT == key) {
		if (x < 740) {
			x += 10;
		}
	}

}

void Player::Draw(YsRawPngDecoder& player) {
	glRasterPos2i(x, y);
	glDrawPixels(player.wid, player.hei, GL_RGBA, GL_UNSIGNED_BYTE, player.rgba);

}

void Player::Initialize() {
	x = 400;
	y = 599;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Missile::Initialize() {
	state = 0;
}

void Missile::Launch(int x0, int y0) {
	state = 1;
	x = x0 + 20;
	y = y0;
	vy = 300;
}

void Missile::UpdatePos(double dt) {
	if (1 == state) {
		y -= vy * dt;
		if (y < 0)
		{
			state = 2;
		}
	}

}

void Missile::Draw(YsRawPngDecoder& missileImg)
{
	glRasterPos2i(x, y);
	glDrawPixels(missileImg.wid, missileImg.hei, GL_RGBA, GL_UNSIGNED_BYTE, missileImg.rgba);
}

int Missile::CheckCollision(int tx, int ty, int tw, int th)
{
	int relX = x - tx;
	int relY = y - ty;

	if (wid / 2 <= relX && relX < (tw - wid / 2) && 0 <= relY && relY < hei)
	{
		return 1;
	}
	return 0;
}

void Missile::BlowUp(void)
{
	state = 2;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Asteroid::Initialize() {
	state = 1;
	x = rand() % 800;
	y = 100 + rand() % 450;
	vx = 20 - rand() % 40;
	vy = 20 - rand() % 40;
	size = rand() % 5;
	switch (size) {
	case 0: //small
		wid = 50;
		hei = 38;
		break;
	case 1:
		wid = 50;
		hei = 38;
		break;
	case 2:
		wid = 100;
		hei = 76;
		break;
	case 3:
		wid = 100;
		hei = 76;
		break;
	case 5:
		wid = 150;
		hei = 114;
		break;
	default:
		break;
	}
	timeExploded = 0;
}

void Asteroid::UpdatePos(double dt) {
	x += vx * dt;
	y += vy * dt;
	if (vx > 0) {
		if (x > 799 - wid) {
			vx = -vx;
		}
	}
	if (vx < 0) {
		if (x < 1) {
			vx = -vx;
		}
	}
	if (vy > 0) {
		if (y > 599) {
			vy = -vy;
		}
	}
	if (vy < 0) {
		if (y < 1 + hei) {
			vy = -vy;
		}
	}


}

void Asteroid::Draw(YsRawPngDecoder& asteroidSmall, YsRawPngDecoder& asteroidMedium, YsRawPngDecoder& asteroidLarge) {


	glRasterPos2i(x, y);
	switch (size) {
	case 0:
		glDrawPixels(asteroidSmall.wid, asteroidSmall.hei, GL_RGBA, GL_UNSIGNED_BYTE, asteroidSmall.rgba);
		break;
	case 1:
		glDrawPixels(asteroidSmall.wid, asteroidSmall.hei, GL_RGBA, GL_UNSIGNED_BYTE, asteroidSmall.rgba);
		break;
	case 2:
		glDrawPixels(asteroidMedium.wid, asteroidMedium.hei, GL_RGBA, GL_UNSIGNED_BYTE, asteroidMedium.rgba);
		break;
	case 3:
		glDrawPixels(asteroidMedium.wid, asteroidMedium.hei, GL_RGBA, GL_UNSIGNED_BYTE, asteroidMedium.rgba);
		break;
	case 5:
		glDrawPixels(asteroidLarge.wid, asteroidLarge.hei, GL_RGBA, GL_UNSIGNED_BYTE, asteroidLarge.rgba);
		break;
	default:
		break;
	}

}

void Asteroid::Explode(YsRawPngDecoder& explosionSmall, YsRawPngDecoder& explosionMedium, YsRawPngDecoder& explosionLarge, double dt) {

	timeExploded += dt;

	if (timeExploded < 0.75) {
		glRasterPos2i(x, y);
		switch (size) {
		case 0:
			glDrawPixels(explosionSmall.wid, explosionSmall.hei, GL_RGBA, GL_UNSIGNED_BYTE, explosionSmall.rgba);
			break;
		case 1:
			glDrawPixels(explosionSmall.wid, explosionSmall.hei, GL_RGBA, GL_UNSIGNED_BYTE, explosionSmall.rgba);
			break;
		case 2:
			glDrawPixels(explosionMedium.wid, explosionMedium.hei, GL_RGBA, GL_UNSIGNED_BYTE, explosionMedium.rgba);
			break;
		case 3:
			glDrawPixels(explosionMedium.wid, explosionMedium.hei, GL_RGBA, GL_UNSIGNED_BYTE, explosionMedium.rgba);
			break;
		case 5:
			glDrawPixels(explosionLarge.wid, explosionLarge.hei, GL_RGBA, GL_UNSIGNED_BYTE, explosionLarge.rgba);
			break;
		default:
			break;
		}
	}
	else {
		state = 0;
	}


}

void Asteroid::Settle(double dt) {
	state = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void AsteroidsGame::Initialize() {

	//terminate = false;
		
	//Initializing PNGs
	if (YSOK == asteroidSmall.Decode("asteroidSmall.png")) {
		//printf("Read Width=%d Height=%d\n", asteroidSmall.wid, asteroidSmall.hei);
		asteroidSmall.Flip();
	}
	else {
		printf("Read Error!\n");
		//return 0;
	}

	if (YSOK == asteroidMedium.Decode("asteroidMedium.png")) {
		//printf("Read Width=%d Height=%d\n", asteroidMedium.wid, asteroidMedium.hei);
		asteroidMedium.Flip();
	}
	else {
		printf("Read Error!\n");
		//return 0;
	}

	if (YSOK == asteroidLarge.Decode("asteroidLarge.png")) {
		//printf("Read Width=%d Height=%d\n", asteroidLarge.wid, asteroidLarge.hei);
		asteroidLarge.Flip();
	}
	else {
		printf("Read Error!\n");
		//return 0;
	}

	if (YSOK == player.Decode("spaceShip.png")) {
		//printf("Read Width=%d Height=%d\n", player.wid, player.hei);
		player.Flip();
	}
	else {
		printf("Read Error!\n");
		//return 0;
	}

	
	if (YSOK == explosionSmall.Decode("explosionSmall.png")) {
		//printf("Read Width=%d Height=%d\n", asteroidSmall.wid, asteroidSmall.hei);
		explosionSmall.Flip();
	}
	else {
		printf("Read Error!\n");
		//return 0;
	}

	
	if (YSOK == explosionMedium.Decode("explosionMedium.png")) {
		//printf("Read Width=%d Height=%d\n", asteroidSmall.wid, asteroidSmall.hei);
		explosionMedium.Flip();
	}
	else {
		printf("Read Error!\n");
		//return 0;
	}


	
	if (YSOK == explosionLarge.Decode("explosionLarge.png")) {
		//printf("Read Width=%d Height=%d\n", asteroidSmall.wid, asteroidSmall.hei);
		explosionLarge.Flip();
	}
	else {
		printf("Read Error!\n");
		//return 0;
	}

	
	if (YSOK == targetL.Decode("badGuyL.png")) {
		//printf("Read Width=%d Height=%d\n", player.wid, player.hei);
		targetL.Flip();
	}
	else {
		printf("Read Error!\n");
		//return 0;
	}

	
	if (YSOK == targetR.Decode("badGuyR.png")) {
		//printf("Read Width=%d Height=%d\n", player.wid, player.hei);
		targetR.Flip();
	}
	else {
		printf("Read Error!\n");
		//return 0;
	}

	
	if (YSOK == missileImg.Decode("missile.png")) {
		//printf("Read Width=%d Height=%d\n", player.wid, player.hei);
		missileImg.Flip();
	}
	else {
		printf("Read Error!\n");
		//return 0;
	}

	if (YSOK == background.Decode("background.png")) {
		//printf("Read Width=%d Height=%d\n", player.wid, player.hei);
		background.Flip();
	}
	else {
		printf("Read Error!\n");
		//return 0;
	}

	// I am assuming that this stuff is done outside the loop:
	/*FsOpenWindow(0, 0, 800, 600, 1);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	FsPassedTime();*/

	//Initializations
	for (auto& a : asteroids) {
		a.Initialize();
	}
	for (auto& m : missiles) {
		m.Initialize();
	}
	ship.Initialize();
	for (auto& e : enemy) {
		e.Initialize();
	}


}

void AsteroidsGame::Run(){

	FsPassedTime();
	for (;;) {
		FsPollDevice();

		auto millisec = FsPassedTime();
		double dt = (double)millisec / 1000.0;

		int key = FsInkey();

		if (FSKEY_ESC == key)
		{
			break;
		}


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glRasterPos2i(0, 599);
		glDrawPixels(background.wid, background.hei, GL_RGBA, GL_UNSIGNED_BYTE, background.rgba);

		for (auto& a : asteroids) {
			a.UpdatePos(dt);
			for (auto& m : missiles) {
				if (m.state == 1 && a.state == 1) {
					if (0 != m.CheckCollision(a.x, a.y, a.wid, a.hei)) {
						a.state = 2;
						m.BlowUp();
					}
				}
			}
		}

		for (auto& e : enemy) {
			e.Update(dt);
			for (auto& m : missiles) {
				if (m.state == 1 && e.state == 1) {
					if (0 != m.CheckCollision(e.x, e.y, e.wid, e.hei)) {
						e.state = 2;
						m.BlowUp();
					}
				}
			}
		}

		for (auto& a : asteroids) {
			if (a.state == 2) {
				a.Explode(explosionSmall, explosionMedium, explosionLarge, dt);
			}
		}

		for (auto& e : enemy) {
			if (e.state == 2) {
				e.Explode(explosionSmall, dt);
			}
		}

		for (auto& a : asteroids) {
			if (a.state == 1) {
				a.Draw(asteroidSmall, asteroidMedium, asteroidLarge);
			}
		}

		if (FSKEY_LEFT == key || FSKEY_RIGHT == key)
		{
			ship.Command(key);
		}

		if (FSKEY_SPACE == key)
		{

			for (auto& m : missiles)
			{
				if (0 == m.state)
				{
					m.Launch(ship.x, ship.y);
					break;
				}
			}
		}



		for (auto& m : missiles)
		{
			m.UpdatePos(dt);
		}



		for (auto& m : missiles)
		{
			if (1 == m.state)
			{
				m.Draw(missileImg);
			}
		}

		ammoUsed = 0;
		for (auto& m : missiles)
		{
			if (2 == m.state)
			{
				ammoUsed++;
			}
		}

		if (ammoUsed == ammo) {
			break;
		}

		ship.Draw(player);


		for (auto& e : enemy)
		{
			if (1 == e.state)
			{
				e.Draw(targetL, targetR);
			}
		}

		enemiesDestroyed = 0;
		for (auto& e : enemy)
		{
			if (0 == e.state)
			{
				enemiesDestroyed++;
			}
		}

		if (enemiesDestroyed == 4) { //this must get changed when enemy number is parametrized for level increment
			break;
		}

		FsSwapBuffers();

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

}