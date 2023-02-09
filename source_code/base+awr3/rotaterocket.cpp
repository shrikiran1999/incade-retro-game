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


using namespace std;




int main(int argc, char** argv)
{
	FsOpenWindow(16, 16, 800, 600, 1);

	RotateRocketGame* game = new RotateRocketGame(400, 300, 0);
	game->Run();

	return 0;
} 