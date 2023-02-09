//
//  AviatorsTester.cpp
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

int main (void)
{
    AviatorsGame aviator_level1, aviator_level2, aviator_level3, aviator_level4, aviator_level5, aviator_level6;
    
    FsOpenWindow(0, 0, 800, 600, 1);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    srand(time(nullptr));
    
    aviator_level1.Initialize(0); // 0= speed of the enemy and boss birds
    aviator_level1.Run();
    aviator_level2.Initialize(5); // 5= speed of the enemy and boss birds
    aviator_level2.Run();
    aviator_level3.Initialize(10); // 10= speed of the enemy and boss birds
    aviator_level3.Run();
    aviator_level4.Initialize(15); // 15= speed of the enemy and boss birds
    aviator_level4.Run();
    aviator_level5.Initialize(20); // 20= speed of the enemy and boss birds
    aviator_level5.Run();
    aviator_level6.Initialize(25); // 25= speed of the enemy and boss birds
    aviator_level6.Run();
}
