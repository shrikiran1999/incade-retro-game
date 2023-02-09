//
//  AviatorsForSharing.cpp
//  Final Project Object Orineted
//
//  Created by Karan Veer Singh on 11/20/21.
//


#include <time.h>
#include <math.h>
#include "fssimplewindow.h"
#include "yssimplesound.h"
#include "yspng.h"
#include "ysglfontdata.h"
#include "AviatorsForSharing.h"

void AviatorsGame::Initialize(double x)
{
    wav.LoadWav("AviatorMusic.wav");
    background.Decode("BG.png");
    background.Flip();
    aviator1.Decode("Aviator_1.png");
    aviator1.Flip();
    aviator2.Decode("Aviator_2.png");
    aviator2.Flip();
    aviator3.Decode("Aviator_3.png");
    aviator3.Flip();
    aviator4.Decode("Aviator_4.png");
    aviator4.Flip();
    enemy.Decode("Enemy.png");
    enemy.Flip();
    boss.Decode("Boss.png");
    boss.Flip();
    boss2.Decode("Boss2.png");
    boss2.Flip();
    for (int i =0; i<5; i++)
    {
        enemy_state[i] =1;
        enemy_y[i] =10;
        enemy_x[i] = 10 + rand()%55;
        enemy_w[i] = 15;
        enemy_h[i] = 13.4;
        enemy_velocity[i] = x+ rand()%5;
    }
    boss_vy = -x-10.0;
}

void AviatorsGame:: DrawAviatorCannon(double x,double y, double angle,YsRawPngDecoder &aviator1)
{
    glRasterPos2i((x-3.5)*scale,60*scale -(y-3)*scale);
    glDrawPixels(aviator1.wid,aviator1.hei,GL_RGBA,GL_UNSIGNED_BYTE,aviator1.rgba);

    glColor3ub(0,255,0);
    glBegin(GL_LINES);
    glVertex2i(x*scale + 2.5,(60-y)*scale - 2.5);
    glVertex2d((x + cos(angle))*scale + 2.5, (60 - y - sin(angle))*scale - 2.5);
    glEnd();
}

void AviatorsGame:: DrawBackground(double x,double y,YsRawPngDecoder &background)
{
    glRasterPos2i(x,y);
    glDrawPixels(background.wid,background.hei,GL_RGBA,GL_UNSIGNED_BYTE,background.rgba);
}

void AviatorsGame:: DrawAviator(double x,double y,int state, YsRawPngDecoder &aviator1,YsRawPngDecoder &aviator2,YsRawPngDecoder &aviator3,YsRawPngDecoder &aviator4)
{
    if(state == 0)
    {
        return;
    }
    if (x>0 && x<=20)
        {glRasterPos2i((x-3.5)*scale,60*scale -(y-2.5)*scale);
        glDrawPixels(aviator1.wid,aviator1.hei,GL_RGBA,GL_UNSIGNED_BYTE,aviator1.rgba);}
    if (x>20 && x<=40)
        {glRasterPos2i((x-3.5)*scale,60*scale -(y-2.5)*scale);
         glDrawPixels(aviator2.wid,aviator2.hei,GL_RGBA,GL_UNSIGNED_BYTE,aviator2.rgba);}
    if (x>40 && x<=60)
        {glRasterPos2i((x-3.5)*scale,60*scale -(y-2.5)*scale);
        glDrawPixels(aviator3.wid,aviator3.hei,GL_RGBA,GL_UNSIGNED_BYTE,aviator3.rgba);}
    if (x>60 && x<=80)
        {glRasterPos2i((x-3.5)*scale,60*scale -(y-2.5)*scale);
        glDrawPixels(aviator4.wid,aviator4.hei,GL_RGBA,GL_UNSIGNED_BYTE,aviator4.rgba);}
}

void AviatorsGame:: DrawPowerBar(double x, double y, int v)
{
    glColor3ub(255,0,0);
    glBegin(GL_QUADS);
    glVertex2i((x)*scale,(60 -(y+20)*0.5)*scale);
    glVertex2i((x+1)*scale,(60 -(y+20)*0.5)*scale);
    glVertex2i((x+1)*scale,(60 -(0.1*v*y+20)*0.5)*scale);
    glVertex2i((x)*scale,(60 -(0.1*v*y+20)*0.5)*scale);
    glEnd();
}

void AviatorsGame:: DrawEnemy(double x,double y,double w,double h, int state, YsRawPngDecoder &enemy)
{
    if(state == 0)
    {
        return;
    }
    glRasterPos2i((x-5)*scale,60*scale -(y-6.2)*scale);
    glDrawPixels(enemy.wid,enemy.hei,GL_RGBA,GL_UNSIGNED_BYTE,enemy.rgba);
}

void AviatorsGame:: DrawBoss(double x, double y, YsRawPngDecoder &boss, YsRawPngDecoder &boss2)
{
    if (y <30)
    {
        glRasterPos2i((x-5)*scale,60*scale -(y-5)*scale);
        glDrawPixels(boss.wid,boss.hei,GL_RGBA,GL_UNSIGNED_BYTE,boss.rgba);
    }
    if(y >=30)
    {
        glRasterPos2i((x-5)*scale,60*scale -(y-5)*scale);
        glDrawPixels(boss2.wid,boss2.hei,GL_RGBA,GL_UNSIGNED_BYTE,boss2.rgba);
    }
}

int AviatorsGame:: CheckCollision(double aviator_x, double aviator_y, double enemy_x[],
    double enemy_y[], double enemy_h[], double enemy_w[], int enemy_state[],
    double boss_x, double boss_y)
{
    //Check cannon ball and enemys
    for(int i = 0; i < number_enemies; ++i)
    {
        if(enemy_state[i])
        {
            if(aviator_x >= enemy_x[i] - enemy_w[i]/2  && aviator_x <= enemy_x[i]+enemy_w[i]/2 && aviator_y >= enemy_y[i] -enemy_h[i]/2 && aviator_y <= enemy_y[i] + enemy_h[i]/2)
            {
                return i;
            }
        }
    }

    if(aviator_x >= boss_x - 5.15 && aviator_x <= boss_x + 5.15 && aviator_y >= boss_y - 6.2 && aviator_y <= boss_y + 6.2) {
        return number_enemies;
    }

    return -1;
}

void AviatorsGame::Run(void)
{
    player.Start();
    player.PlayOneShot(wav);

    for(;;)
    {
        FsPollDevice();

        auto key=FsInkey();
        if(FSKEY_ESC==key)
        {
            break;
        }

        if(FSKEY_UP == key && aviator_cannon_y < 59 )
        {
            velocity +=10;
        }
        else if (FSKEY_DOWN == key && aviator_cannon_y >0)
        {
            velocity -=10;
        }
        if (FSKEY_LEFT == key)
        {
            angle += 10;
        }
        else if (FSKEY_RIGHT == key)
        {
            angle -=10;
        }
        if (FSKEY_SPACE == key && aviator_state == 0)
        {
            aviator_state = 1;

            aviator_x = aviator_cannon_x;
            aviator_y = aviator_cannon_y;
            vx = velocity*cos(angle* ( Pi / 180.0 ));
            vy = velocity*sin(angle* ( Pi / 180.0 ));

        }
        if (aviator_state ==1)
        {
            aviator_x = aviator_x + vx*dt;
            aviator_y = aviator_y + vy*dt;
            vx = vx;
            vy = vy-g*dt;
            if (aviator_x > 80 || aviator_y < 0 || aviator_x <0 || aviator_y >60)
            {
                aviator_state = 0;
            }
        }
        for (int i=0; i <5; ++i)
        {
            if(0!=enemy_state[i])
            {
                enemy_y[i] += enemy_velocity[i]*dt;

            }
            if(enemy_y[i] + enemy_h[i]>= 60 || enemy_y[i] <= 0)
            {
                enemy_velocity[i] = -enemy_velocity[i];
            }
        }
        if(boss_y <= 0)
        {
            boss_y = 55;
        }
        else
        {
            boss_y = boss_y + boss_vy*dt;
        }
        int collision_index = CheckCollision(aviator_x, aviator_y, enemy_x, enemy_y, enemy_h, enemy_w, enemy_state, boss_x, boss_y);

        if( collision_index > -1 && collision_index < number_enemies)
        {
            enemy_state[collision_index] = 0;
            aviator_state = 0;
        }
        else if( collision_index == number_enemies )
        {
            break;;
        }

        
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        DrawBackground(0, 599, background);
        for (int i =0; i<5; ++i)
        {
            DrawEnemy(enemy_x[i], enemy_y[i], enemy_w[i], enemy_h[i], enemy_state[i], enemy);
        }
        DrawAviatorCannon(aviator_cannon_x, aviator_cannon_y, angle*(Pi/ 180.0), aviator1);
        DrawAviator(aviator_x, aviator_y, aviator_state, aviator1,aviator2,aviator3,aviator4);
        DrawPowerBar(aviator_cannon_x, aviator_cannon_y, velocity);
        DrawBoss(boss_x, boss_y, boss, boss2);
        FsSwapBuffers();
        FsSleep(10);
    }
    player.Stop(wav);
    player.End();
}







