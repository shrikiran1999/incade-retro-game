

class Board
{
public:

    int cx[9] = { 100, 300, 500, 100, 300, 500, 100, 300, 500 };
    int cy[9] = { 100, 100, 100, 300, 300, 300, 500, 500, 500 };
    double r = 75.0;

    void DrawBoard();
    void DrawLife(int);
    void DrawCircle(int, int, double);

};


void Board::DrawCircle(int hole_x, int hole_y, double r)
{
    const double YS_PI = 3.1415927;

    int i, c_points = 100;
    /*glColor3ub(100, 0, 0);*/
    glBegin(GL_POLYGON);

    for (i = 0; i < c_points; i++)
    {
        double theta = (double)i * YS_PI * 2 / c_points;
        double x = (double)hole_x + cos(theta) * (double)r;
        double y = (double)hole_y + sin(theta) * (double)r;
        glVertex2d(x, y);
    }

    glEnd();
}

void Board::DrawBoard()
{
    glBegin(GL_QUADS);
    glColor3ub(0, 255, 0);
    glVertex2i(0, 0);
    glVertex2i(800, 0);
    glVertex2i(800, 600);
    glVertex2i(0, 600);

    glEnd();

    glColor3ub(100, 0, 0);

    for (int i = 0; i < 9; i++)
    {
        DrawCircle(cx[i], cy[i], 75.0);
    }

}

void Board::DrawLife(int lives_left)
{
    for (int i = 1; i <= lives_left; ++i)
    {
        glBegin(GL_QUADS);
        glColor3ub(255, 0, 0);
        glVertex2i(650, 100 * i);
        glVertex2i(750, 100 * i);
        glVertex2i(750, 100 * (i + 0.5));
        glVertex2i(650, 100 * (i + 0.5));

        glEnd();
    }

}

class Mole : public Board
{
public:
    int molestate;
    int moleloc;
    void DrawMole(int);
    int mousecheck(int, int);
};

void Mole::DrawMole(int moleloc)
{
    glColor3ub(232, 190, 172);
    DrawCircle(cx[moleloc], cy[moleloc], 40.0);
    glColor3ub(255, 255, 255);
    DrawCircle(cx[moleloc] + 15, cy[moleloc] - 10, 7.0);
    glColor3ub(255, 255, 255);
    DrawCircle(cx[moleloc] - 15, cy[moleloc] - 10, 7.0);
    glColor3ub(0, 0, 0);
    DrawCircle(cx[moleloc] + 15, cy[moleloc] - 10, 5.0);
    glColor3ub(0, 0, 0);
    DrawCircle(cx[moleloc] - 15, cy[moleloc] - 10, 5.0);

    glColor3ub(0, 0, 0);
    DrawCircle(cx[moleloc], cy[moleloc], 7.0);
    glColor3ub(0, 0, 0);
    DrawCircle(cx[moleloc], cy[moleloc] + 5, 10.0);

}

int Mole::mousecheck(int mx, int my)
{
    double dist = (double)sqrt(pow((mx - cx[moleloc]), 2) + pow((my - cy[moleloc]), 2));

    if (dist <= 40.0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


class WacMoleGame
{
public:
    int gamestate = 1;
    Board b;
    Mole m;

    int score = 0;
    int lives_left = 6;
    YsSoundPlayer player;
    YsSoundPlayer::SoundData wav;

    void initialize();
    void music(int);

    void run();
};

void WacMoleGame::initialize()
{
    m.moleloc = rand() % 9;
    m.molestate = 1;
}
void WacMoleGame::music(int action)
{
    if (action == 1)
    {
        char fName[256] = "whacamole2.wav";
        if (YSOK != wav.LoadWav(fName)) {
            printf("Failed to read %s\n", fName);
            /*return 1;*/
        }
        player.Start();
        player.PlayOneShot(wav);
        FsSleep(2000);
    }

    if (action == 0)
    {
        player.End();
    }

}

void WacMoleGame::run()
{
    auto tpopped = std::chrono::system_clock::now();
    auto tremoved = std::chrono::system_clock::now();

    while (gamestate == 1)
    {
        FsPollDevice();
        int key = FsInkey();

        switch (key)
        {
        case FSKEY_ESC:
            gamestate = 0;
            break;
        }


        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        //drawing the empty board
        b.DrawBoard();
        b.DrawLife(lives_left);
        /*auto tcurrent = time(nullptr);*/
        auto tcurrent = std::chrono::system_clock::now();

        //getting mouse input
        int mouseEvent;
        int lb, mb, rb, mx, my;
        mouseEvent = FsGetMouseEvent(lb, mb, rb, mx, my);

        if (m.molestate == 1 && (std::chrono::duration_cast<std::chrono::milliseconds>(tcurrent - tpopped).count() * 0.001) <= 0.7) {

            if ((m.mousecheck(mx, my) == 1) && mouseEvent == FSMOUSEEVENT_LBUTTONDOWN)
            {
                /*std::cout << FSMOUSEEVENT_LBUTTONDOWN;*/
                score += 1;

                m.molestate = 0;
                /*tremoved = time(nullptr);*/
                tremoved = std::chrono::system_clock::now();
                /*YsSoundPlayer molesnd;
                YsSoundPlayer::SoundData wa;

                char fName2[256] = "punch.wav";
                if (YSOK != wa.LoadWav(fName2)) {
                    printf("Failed to read %s\n", fName2);
                    return 1;
                }
                molesnd.Start();
                molesnd.PlayOneShot(wa);
                auto tmolesnds = std::chrono::system_clock::now();
                if (std::chrono::duration_cast<std::chrono::milliseconds>(tcurrent - tmolesnds).count() * 0.001 > 1)
                {
                    molesnd.End();
                }*/
            }

            else
            {
                m.DrawMole(m.moleloc);
            }


        }


        else if (m.molestate == 1 && ((std::chrono::duration_cast<std::chrono::milliseconds>(tcurrent - tpopped).count() * 0.001) > 0.7)) {
            /*std::cout << " more than 2s, so mole removed \n";*/
            m.molestate = 0;
            lives_left -= 1;

            tremoved = std::chrono::system_clock::now();
        }


        /*else if (m.molestate == 0 && ((tcurrent - tremoved) >= 1))*/
        else if (m.molestate == 0 && ((std::chrono::duration_cast<std::chrono::milliseconds>(tcurrent - tremoved).count() * 0.001) >= 2))
        {

            m.molestate = 1;
            m.moleloc = rand() % 9;
            m.DrawMole(m.moleloc);
            /*tpopped = time(nullptr);*/
            tpopped = std::chrono::system_clock::now();

        }

        if (lives_left == 0)
        {
            gamestate = 0;
        }



        FsSwapBuffers();
        FsSleep(10);
    }
}
