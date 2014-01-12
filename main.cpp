#include <cmath>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>

#include "ball.h"
#include "common.h"
#include "geometry.h"
#include "imageLoader.h"

using namespace std;

vector<Ball> balls;
vector<Ball> scored;
vector<Ball> holes;
GLint mouse_down[2];
int ball_down[2];
int draw_line = DRAW_NULL;
float shot_time = 0;
float last_time = glutGet(GLUT_ELAPSED_TIME);
int pageState = 0;
int playerSize = 0;
int playerMode = 0;
int playerTurn = -1;
int winnerPlayer = -1;
bool putWhiteBall = true;
bool playerScored = false;
bool onShot = true;
int scorePlayer[999];
bool freeBall = false;
int btn_menu_stat = 0;
int n_goal = 0;
int playHover = 0;
int helpHover = 0;
int endGame = false;
bool pause = false;
bool mouseWithBall = false;

Color colors[] = {kYellow, kBlue, kBrown, kViolet, kOrange, kGreen, kRed};
void *fonts[] =
{
  GLUT_BITMAP_8_BY_13,
  GLUT_BITMAP_9_BY_15,
  GLUT_BITMAP_TIMES_ROMAN_10,
  GLUT_BITMAP_TIMES_ROMAN_24,
  GLUT_BITMAP_HELVETICA_10,
  GLUT_BITMAP_HELVETICA_12,
  GLUT_BITMAP_HELVETICA_18,
};

void printString(int x, int y, Color c, char *str, int fontType)
{
  glColor3fv(c);
  glRasterPos2f(x, y);
  int len, i;
  len = (int)strlen(str);
  for (i = 0; i < len; i++) {
    glutBitmapCharacter(fonts[fontType], str[i]);
  }
}

void TableSetup() {
  // Pocket Setup
  holes.clear();
  balls.clear();
  scored.clear();
  memset(scorePlayer,0,sizeof(scorePlayer[0])*10);
  holes.push_back(Ball(15.0 + table_xinit - 7, 15.0 + table_yinit - 7, 30, kDarkWhite, false, false,0,kBlack));
  holes.push_back(Ball(15.0 + table_xinit, 15.0 + table_yinit, 25, kBlack, false, false,0,kBlack));

  holes.push_back(Ball(15.0 + table_xinit - 7, table_height-15 + 7 + table_yinit, 30, kDarkWhite, false, false,0,kBlack));
  holes.push_back(Ball(15.0 + table_xinit, table_height-15 + table_yinit, 25, kBlack, false, false,0,kBlack));

  holes.push_back(Ball(table_width/2 + table_xinit, 15.0 + table_yinit - 9, 28, kDarkWhite, false, false,0,kBlack));
  holes.push_back(Ball(table_width/2 + table_xinit, 15.0 + table_yinit, 25, kBlack, false, false,0,kBlack));

  holes.push_back(Ball(table_width/2 + table_xinit, table_height-15 + 9 + table_yinit, 28, kDarkWhite, false, false,0,kBlack));
  holes.push_back(Ball(table_width/2 + table_xinit, table_height-15 + table_yinit, 25, kBlack, false, false,0,kBlack));

  holes.push_back(Ball(table_width-15 + 7 + table_xinit, 15.0 - 7 + table_yinit, 30, kDarkWhite, false, false,0,kBlack));
  holes.push_back(Ball(table_width-15 + table_xinit, 15.0 + table_yinit, 25, kBlack, false, false,0,kBlack));

  holes.push_back(Ball(table_width-15 + 7 + table_xinit, table_height-15 + 7 + table_yinit, 30, kDarkWhite, false, false,0,kBlack));
  holes.push_back(Ball(table_width-15 + table_xinit, table_height-15 + table_yinit, 25, kBlack, false, false,0,kBlack));

  // Cueball Setup
  balls.push_back(Ball(150 + table_xinit, table_height/2 + table_yinit, 13, kWhite, false, false,0,kWhite));

  // 15-ball Billiards Setup
  int ball_num = 1;
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < i + 1; j++) {
      if (ball_num < 8)
        balls.push_back(Ball(500+(26*i) + table_xinit,
                             (table_height/2)-(26*i/2)+(27*j) + table_yinit,
                             13, colors[(ball_num - 1)%7], false, true,ball_num,kBlack) );
      else if (ball_num == 8)
        balls.push_back(Ball(500+(26*i) + table_xinit,
                             (table_height/2)-(26*i/2)+(27*j)+ table_yinit,
                             13, kBlack, false, true,ball_num,kWhite) );
      else balls.push_back(Ball(500+(26*i) + table_xinit,
                             (table_height/2)-(26*i/2)+(27*j)+ table_yinit,
                             13, colors[(ball_num - 2)%7], true, true,ball_num,kBlack) );

      ball_num++;
    }
  }
}

void generateColor(double r,double g, double b)
{
    glColor3f(r/255, g/255, b/255);
}

void drawRectangle(int x, int y, int width, int height)
{
    glBegin(GL_POLYGON);
    glVertex2d(x, y);
    glVertex2d(x, y + height);
    glVertex2d(x + width, y + height);
    glVertex2d(x + width, y);
    glEnd();
}

void drawPowerBar()
{
    glBegin(GL_POLYGON);
      glColor3f(0, 1, 0);
      glVertex2d(100, 570);
      glVertex2d(100, 580);
      glColor3f(1, 0, 0);
      glVertex2d(710, 580);
      glVertex2d(710, 570);
    glEnd();
}

void drawScoredBar()
{
   glBegin(GL_POLYGON);
      glColor3f(0.7, 0.7, 0.7);
      glVertex2d(100, 515);
      glVertex2d(100, 555);
      glVertex2d(710, 555);
      glVertex2d(710, 515);
  glEnd();

  glBegin(GL_POLYGON);
      glColor3f(0.9, 0.9, 0.9);
      glVertex2d(105, 520);
      glVertex2d(105, 550);
      glVertex2d(705, 550);
      glVertex2d(705, 520);
  glEnd();
}

void drawTable() {
  glColor3f(0.4666667, 0.23529411, 0.17254901);
  glBegin(GL_POLYGON);
  glVertex2f(table_xinit - 20.0, table_yinit);
  glVertex2f(table_xinit + 15.0, table_yinit);
  glVertex2f(table_xinit + 15.0, table_height + table_yinit);
  glVertex2f(table_xinit - 20.0, table_height + table_yinit);
  glEnd();

  glBegin(GL_POLYGON);
  glVertex2f(table_xinit, table_yinit - 20.0);
  glVertex2f(table_width+ table_xinit, table_yinit - 20.0);
  glVertex2f(table_width+ table_xinit, table_yinit + 15.0);
  glVertex2f(table_xinit, table_yinit + 15.0);
  glEnd();

  glBegin(GL_POLYGON);
  glVertex2f(table_width + table_xinit + 20.0, table_yinit);
  glVertex2f(table_width + table_xinit + 20.0, table_height + table_yinit);
  glVertex2f(table_width + table_xinit - 15.0, table_height + table_yinit);
  glVertex2f(table_width + table_xinit - 15.0, table_yinit);
  glEnd();

  glBegin(GL_POLYGON);
  glVertex2f(table_width + table_xinit, table_height + table_yinit + 20.0);
  glVertex2f(table_width + table_xinit, table_height + table_yinit - 15.0);
  glVertex2f(table_xinit, table_height + table_yinit - 15.0);
  glVertex2f(table_xinit, table_height + table_yinit + 20.0);
  glEnd();

  glColor3f(0.25098039,0.431372549,0.07450980);
  glBegin(GL_POLYGON);
  glVertex2f(table_xinit + 15.0, table_yinit + 41.0);
  glVertex2f(table_xinit + 30.0, table_yinit + 55.0);
  glVertex2f(table_xinit + 30.0, table_height + table_yinit - 55.0);
  glVertex2f(table_xinit + 15.0, table_height + table_yinit - 41.0);
  glEnd();

  glBegin(GL_POLYGON);
  glVertex2f(table_width + table_xinit - 15.0, table_yinit + 41.0);
  glVertex2f(table_width + table_xinit - 30.0, table_yinit + 55.0);
  glVertex2f(table_width + table_xinit - 30.0, table_height + table_yinit - 55.0);
  glVertex2f(table_width + table_xinit - 15.0, table_height + table_yinit - 41.0);
  glEnd();

  glBegin(GL_POLYGON);
  glVertex2f(table_xinit + 41.0, table_yinit + 15.0);
  glVertex2f(table_width/2  + table_xinit - 25.0, table_yinit + 15.0);
  glVertex2f(table_width/2  + table_xinit - 37.0, table_yinit + 30.0);
  glVertex2f(table_xinit + 55.0, table_yinit + 30.0);
  glEnd();

  glBegin(GL_POLYGON);
  glVertex2f(table_width/2  + table_xinit + 25.0, table_yinit + 15.0);
  glVertex2f(table_width + table_xinit - 41.0, table_yinit + 15.0);
  glVertex2f(table_width + table_xinit - 55.0, table_yinit + 30.0);
  glVertex2f(table_width/2  + table_xinit + 37.0, table_yinit + 30.0);
  glEnd();

  glBegin(GL_POLYGON);
  glVertex2f(table_xinit + 41.0, table_yinit + table_height - 15.0);
  glVertex2f(table_width/2  + table_xinit - 25.0, table_yinit + table_height - 15.0);
  glVertex2f(table_width/2  + table_xinit - 37.0, table_yinit + table_height - 30.0);
  glVertex2f(table_xinit + 55.0, table_yinit + table_height - 30.0);
  glEnd();

  glBegin(GL_POLYGON);
  glVertex2f(table_width/2  + table_xinit + 25.0, table_yinit + table_height - 15.0);
  glVertex2f(table_width + table_xinit - 41.0, table_yinit + table_height - 15.0);
  glVertex2f(table_width + table_xinit - 55.0, table_yinit + table_height - 30.0);
  glVertex2f(table_width/2  + table_xinit + 37.0, table_yinit + table_height - 30.0);
  glEnd();

  /*glBegin(GL_LINES);
  glVertex2f((table_width/2 + table_xinit) - 37, table_yinit);
  glVertex2f((table_width/2 + table_xinit) - 37, table_height + table_yinit);
  glVertex2f((table_xinit) + 55, table_yinit);
  glVertex2f((table_xinit) + 55, table_height + table_yinit);
  glVertex2f((table_xinit + table_width) - 55, table_yinit);
  glVertex2f((table_xinit + table_width) - 55, table_height + table_yinit);
  glVertex2f((table_xinit), table_yinit + table_height - 55 );
  glVertex2f((table_xinit + table_width), table_height - 55 + table_yinit);
  glVertex2f((table_xinit), table_yinit + 55 );
  glVertex2f((table_xinit + table_width), table_yinit + 55);
  glVertex2f((table_width/2 + table_xinit) + 37, table_yinit);
  glVertex2f((table_width/2 + table_xinit) + 37, table_height + table_yinit);
  glEnd();*/

}

void drawCueStick() {
  float tip_scale, end_scale;
  if (draw_line == DRAW_AIM) {
    tip_scale = 0.5;
    end_scale = 5;
  } else {
    tip_scale = 0.95;
    end_scale = 4;
  }

  glLineWidth(7);
  glColor3f(0.647, 0.1647, 0.1647);
  double x1 = balls[0].x[0];
  double y1 = balls[0].x[1];
  double x2 = mouse_down[0];
  double y2 = mouse_down[1];
  double jarak = 150 + sqrt(pow(x2-x1,2)+pow(y2-y1,2));
  if(jarak > 1000){
    x2 = 973;
    jarak = 1000;
  }
  glBegin(GL_LINES);
  glVertex2f(x2 + tip_scale * (x1 - x2), y2 + tip_scale * (y1 - y2));
  glVertex2f(x2 - end_scale * (x1 - x2), y2 - end_scale * (y1 - y2));
  glEnd();
  glColor3f(1,1,1);
  glBegin(GL_LINES);
  glVertex2f(jarak * 0.70, 560);
  glVertex2f(jarak * 0.70, 580);
  glEnd();
}

void drawAimHelper() {
  int xAim = mouse_down[0] + 3*(balls[0].x[0] - mouse_down[0]);
  int yAim = mouse_down[1] + 3*(balls[0].x[1] - mouse_down[1]);

  glLineWidth(2);
  glColor3f(1, 1, 1);
  glLineStipple(1, 10);
  glEnable(GL_LINE_STIPPLE);

  glBegin(GL_LINES);
  glVertex2f(balls[0].x[0], balls[0].x[1]);
  glVertex2f(xAim, yAim);
  glEnd();
  glDisable(GL_LINE_STIPPLE);
}

void Init() {
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glColor3f(1.0, 0.0, 0.0);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, window_width, window_height, 0);
  glMatrixMode(GL_MODELVIEW);

  TableSetup();
  PlaySound("sfx/Unreal Superhero 3.wav", NULL, SND_ASYNC|SND_FILENAME);
}

int findWinner()
{
    int win = 0;
    int winner = -1;
    for(int i=1; i<=playerSize; i++)
    {
        if(scorePlayer[i]>win)
        {
            winner = i;
            win = scorePlayer[i];
        }
    }
    return winner;
}

void score(int index) {
  for (int i = 0; i < holes.size(); i++) {
    if (balls[index].collideToHole(&holes[i])) {
      PlaySound("sfx/Ball_Into_Pocket.wav", NULL, SND_ASYNC|SND_FILENAME);
      if(index==0) { // <-- white balls become free ball
        freeBall = true;
        mouseWithBall = true;
        putWhiteBall = false;
        break;
      }

      balls[index].x[0] = 125 + n_goal*40;
      balls[index].x[1] = 535;
      n_goal++;

      balls[index].setVelocity(0, 0);
      scored.push_back(balls[index]);
      balls.erase(balls.begin()+index);
      playerScored = true;
      scorePlayer[playerTurn]+=10;
      if(scored.size()==15)
      {
          winnerPlayer = findWinner();
          endGame = true;
      }
      break;
    }
  }
}

bool ballsIsStopped(){
    for (int i = 0; i < balls.size(); i++) {
        if(!balls[i].isStopped()) return false;
  }
  return true;
}

void idle(void) {
  float present_time = glutGet(GLUT_ELAPSED_TIME);
  float elapsed_time = .001*(present_time - last_time);

  for (int i = 0; i < balls.size(); i++) {
    if (i >= 0) score(i); // <-- ball hit the holes

    for (int j = i+1; j < balls.size(); j++) {
      if (balls[i].collide(&balls[j]))
        balls[i].elasticCollision(&balls[j]);
    }

    balls[i].update(elapsed_time, holes);
  }

  if(ballsIsStopped() && !onShot) {
        if(playerTurn<playerSize && !playerScored && putWhiteBall) { playerTurn++; }
        else if(playerTurn==playerSize && !playerScored && putWhiteBall) playerTurn = 1;
        playerScored = false;
        onShot = true;
  }

  last_time = present_time;
  glutPostRedisplay();
}

void PlayDisplay() {
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glColor3f(1,0,0);
  drawRectangle(680,15,100,30);
  printString(705,35,kWhite,"MENU",6);

  glColor3f(0.35686274, 0.65490196, 0.0862745);
  glBegin(GL_POLYGON);
  glVertex2f(table_xinit, table_yinit);
  glVertex2f(table_xinit + table_width, table_yinit);
  glVertex2f(table_xinit + table_width, table_height + table_yinit);
  glVertex2f(table_xinit, table_height + table_yinit);
  glEnd();

  if(playerMode==1)
    printString(355,30,kWhite,"Free Mode",6);
  else
    printString(355,30,kWhite,"Match Mode",6);
  string playerName = "Player ";
  stringstream ss;
  ss << playerTurn;
  playerName.append(ss.str());
  char *cstr = new char[playerName.length() + 1];
  strcpy(cstr, playerName.c_str());
  if(playerMode==1) printString(20,30,kBlack,cstr,6);
  else printString(20,30,kYellow,cstr,6);

  string scoreBoard = "Score: ";
  stringstream ss1;
  ss1 << scorePlayer[playerTurn];
  scoreBoard.append(ss1.str());
  char *cstrscore = new char[playerName.length() + 1];
  strcpy(cstrscore, scoreBoard.c_str());
  if(playerMode==1) printString(20,60,kBlack,cstrscore,6);
  else printString(20,60,kYellow,cstrscore,6);

  if(freeBall) {
      printString(360,60,kYellow,"Free Ball",6);
  }
  if(!putWhiteBall){
      printString(360,60,kYellow,"Free Ball",6);
      balls[0] = Ball(ball_down[0], ball_down[1], 13, kWhite, false, true,0,kWhite);
  }

  drawScoredBar();
  drawPowerBar();
  drawTable();

  for (int i = 0; i < holes.size(); i++)
    holes[i].draw();
  for (int i = 0; i < balls.size(); i++)
    balls[i].draw();
  for (int i = 0; i < scored.size(); i++)
    scored[i].draw();

  if(ballsIsStopped() && !endGame && !freeBall)
  {
       printString(345,60,kYellow,"-- Go !!!!!! --",6);
  }
  else if(!ballsIsStopped() && !endGame && !freeBall)
  {
      glColor4f(0, 0, 0, 0.4);
      glBegin(GL_POLYGON);
        glVertex2d(0,0);
        glVertex2d(window_width,0);
        glVertex2d(window_width,window_height);
        glVertex2d(0,window_height);
      glEnd();
      printString(360, 300,kYellow,"Wait!!!!!!",6);
  }

  if(pause)
  {
      glColor4f(0, 0, 0, 0.4);
      drawRectangle(0,0,window_width,window_height);
      glColor3f(1,0,0);
      drawRectangle(window_width/2 - 75, window_height/2 - 100, 150, 75);
      printString(window_width/2 - 40, window_height/2 - 58,kWhite,"RESUME",6);
      glColor3f(1,0,0);
      drawRectangle(window_width/2 - 75, window_height/2, 150, 75);
      printString(window_width/2 - 52, window_height/2 + 42, kWhite,"MAIN MENU",6);
      printString(335,150,kYellow,"GAME PAUSED",6);
  }

  if(endGame && playerMode==2)
  {
      glColor4f(0, 0, 0, 0.4);
      drawRectangle(0,0,window_width,window_height);
      printString(315,250,kYellow,"CONGRATULATION",6);
      string playerName = "The Winner is player ";
      stringstream ss;
      ss << winnerPlayer;
      playerName.append(ss.str());
      char *cstr = new char[playerName.length() + 1];
      strcpy(cstr, playerName.c_str());
      printString(310,280,kYellow,cstr,6);
  }

  if (draw_line == DRAW_SHOT) {
    draw_line = DRAW_NULL;
  } else if (draw_line != DRAW_NULL) {
    drawCueStick();
    drawAimHelper();
  }

  glFlush();
  glutSwapBuffers();
}

void ConfDisplay()
{
    pageState = 3;
    glClearColor(0.8,0.8,0.8,0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    generateColor(0,96,84);
    drawRectangle(0,150,window_width,50);
    for(int i=0;i<2;i++)
    {
        if(i==0)
            generateColor(255,104,4);
        else if(i==1)
            generateColor(201,49,44);

        if(i+1==playerMode)
            generateColor(0,0,0);
        drawRectangle(150 + i*275,220,225,50);
    }
    printString(345,180,kWhite,"Game mode",6);
    printString(225,250,kWhite,"Free mode",6);
    printString(495,250,kWhite,"Match mode",6);

    if(playerMode==2)
    {
        generateColor(51,103,151);
        drawRectangle(0,340,window_width,50);
        for(int i=0;i<5;i++)
        {
            if(i==0)
                generateColor(100,15,108);
            else if(i==1)
                generateColor(0,170,240);
            else if(i==2)
                generateColor(0,255,0);
            else if(i==3)
                generateColor(212,64,50);
            else if(i==4)
                generateColor(77,77,77);

            if(i+1==playerSize)
                generateColor(0,0,0);
            drawRectangle(80+i*135,415,100,100);
        }

        printString(325,370,kWhite,"How many player?",6);
        printString(125,470,kWhite,"1",6);
        printString(260,470,kWhite,"2",6);
        printString(395,470,kWhite,"3",6);
        printString(530,470,kWhite,"4",6);
        printString(665,470,kWhite,"5",6);
    }

    if((playerMode==2 && playerSize!=0) || (playerMode==1))
    {
        glColor3f(1,0,0);
        drawRectangle(645,40,120,75);
        printString(675,85,kWhite,"PLAY",3);
    }
    loadtex("img/btn_back.png", 154, 84, 35, 30);
    glFlush();
    glutSwapBuffers();
}

void HelpDisplay() {
  pageState = 1;

  loadtex("img/help.png", window_width, window_height, 0, 0);
  loadtex("img/btn_back.png", 154, 84, 35, 50);

  glFlush();
  glutSwapBuffers();
}

void MainMenuDisplay() {
  pageState = 0;
  playerSize = 0;
  playerMode = 0;
  playerTurn = 1;
  winnerPlayer = -1;
  pause = false;
  freeBall = false;
  n_goal = 0;
  mouseWithBall = false;
  endGame = false;
  loadtex("img/menu.png", window_width, window_height, 0, 0);
  if(playHover==0)
    loadtex("img/btn_play.png", 211, 68, 450, 380);
  else
    loadtex("img/btn_play.png", 250, 77, 450, 375);

  if(helpHover==0)
    loadtex("img/btn_help.png", 107, 49, 570, 460);
  else
    loadtex("img/btn_help.png", 118, 55, 570, 455);
  loadtex("img/btn_exit.png", 131, 69, 630, 20);

  glFlush();
  glutSwapBuffers();
}

void Mouse(int button, int state, int x, int y) {
  if(button == GLUT_LEFT_BUTTON && pageState==0)
  {
      if (state == GLUT_DOWN) {
          mouse_down[0] = x;
          mouse_down[1] = y;

          // Play Button
          if(x >= 456 && x <= 655 && y >= 393 && y <= 437)
          {
              glutDisplayFunc(ConfDisplay);
          }
          // Help Button
          else if(x >= 574 && x <= 671 && y >= 472 && y <= 498)
          {
              glutDisplayFunc(HelpDisplay);
          }
          // Exit Button
          else if(x >= 634 && x <= 751 && y >= 30 && y <= 77)
          {
              exit(EXIT_SUCCESS);
          }
      }
  }

  else if(button == GLUT_LEFT_BUTTON && pageState==1)
  {
      if (state == GLUT_DOWN) {
          mouse_down[0] = x;
          mouse_down[1] = y;

          // Back Button
          if(x >= 57 && x <= 186 && y >= 75 && y <= 117)
          {
              glutDisplayFunc(MainMenuDisplay);
          }
      }
  }

  else if (button == GLUT_LEFT_BUTTON && pageState==2 && ballsIsStopped()) {
    if (state == GLUT_DOWN) {
      mouse_down[0] = x;
      mouse_down[1] = y;
      if(x>=680 && x<=780 && y>=15 && y<=45)
      {
          draw_line = DRAW_NULL;
          pause = true;
      }
      else if(!putWhiteBall){
          balls[0] = Ball(ball_down[0], ball_down[1], 13, kWhite, false, true,0,kWhite);
          mouseWithBall = false;
          putWhiteBall = true;
      }
      else if(!pause && putWhiteBall) draw_line = DRAW_AIM;
    }
    if (state == GLUT_UP) {
      if(x>=window_width/2 - 75 && x<=window_width/2 + 75 && y>=window_height/2 - 100 && y<=window_height/2 + 25 && pause)
      {
          pause = false;
      }
      else if(x>=window_width/2 - 75 && x<=window_width/2 + 75 && y>=window_height/2 && y<=window_height/2 + 125 && pause)
      {
          Init();
          glutDisplayFunc(MainMenuDisplay);
      }
      else if(!pause)
      {
          balls[0].v.x[0] = velocity_coeff * (balls[0].x[0] - mouse_down[0]);
          balls[0].v.x[1] = velocity_coeff * (balls[0].x[1] - mouse_down[1]);
          balls[0].a[0] = drag_coeff*balls[0].v[0];
          balls[0].a[1] = drag_coeff*balls[0].v[1];
          PlaySound("sfx/White_Ball_Hit.wav", NULL, SND_ASYNC|SND_FILENAME);
          draw_line = DRAW_SHOT;
          shot_time = glutGet(GLUT_ELAPSED_TIME);
          onShot = false;
          if(freeBall && putWhiteBall) freeBall = false;
      }
    }
  }
  else if(button == GLUT_LEFT_BUTTON && pageState==3)
  {
        if (state == GLUT_DOWN)
        {
          if(x >= 57 && x <= 186 && y >= 55 && y <= 97) // back
          {
              glutDisplayFunc(MainMenuDisplay);
          }

          for(int i=0;i<2;i++) // select mode
            {
                int x0 = 150 + i*275;
                int y0 = 220;
                if(x >= x0 && x<= x0 + 225 && y>= y0 && y<= y0+50)
                {
                    playerMode = i + 1;
                    if(playerMode==1)
                    {
                        playerSize = 1;
                    }
                    else playerSize = 0;
                    break;
                }
            }

          for(int i=0;i<5;i++) // select player size
          {
                int x0 = 80+i*135;
                int y0 = 415;
                if(x >= x0 && x <= x0 + 100 && y >= y0 && y <= y0 + 100)
                {
                    playerSize = i + 1;
                    break;
                }
          }

          if(x >= 645 && x <= 765 && y >= 40 && y <= 115)
          {
              PlaySound(NULL, 0, 0);
              glutDisplayFunc(PlayDisplay);
              btn_menu_stat = 1;
          }
        }

        if(state == GLUT_UP && btn_menu_stat == 1 ) {
          btn_menu_stat = 0;
          pageState = 2;
        }
    }

  glutPostRedisplay();
}

void MouseMotion(int x, int y) {
  mouse_down[0] = x;
  mouse_down[1] = y;
  glutPostRedisplay();
}

void HoverFunc(int x, int y) {
  if(x >= 456 && x <= 655 && y >= 393 && y <= 437 && pageState==0 && playHover==0)
      playHover = 1;
  else if ((x < 456 || x > 655 || y < 393 || y > 437) && pageState==0 && playHover==1)
      playHover = 0;

  if(x >= 574 && x <= 671 && y >= 472 && y <= 498 && pageState==0 && helpHover==0)
      helpHover = 1;
  else if((x < 574 || x > 671 || y < 472 || y > 498) && pageState==0 && helpHover==1)
      helpHover = 0;

  if(pageState==2)
  {
    ball_down[0] = x;
    ball_down[1] = y;
  }
  glutPostRedisplay();
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  playerTurn = 1;
  memset(scorePlayer,0,sizeof(scorePlayer));
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(window_width, window_height);
  glutInitWindowPosition(100, 50);
  glutCreateWindow("My Billiard Game 2D");
  glutDisplayFunc(MainMenuDisplay);
  glutMouseFunc(Mouse);
  glutMotionFunc(MouseMotion);
  glutPassiveMotionFunc(HoverFunc);
  glutIdleFunc(idle);
  Init();
  glutMainLoop();
}
