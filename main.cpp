#include <math.h>
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

enum { DRAW_NULL, DRAW_AIM, DRAW_SHOT };

vector<Ball> balls;
vector<Ball> scored;
vector<Ball> holes;
GLint mouse_down[2];
int draw_line = DRAW_NULL;
float shot_time = 0;
float last_time = glutGet(GLUT_ELAPSED_TIME);
int pageState = 0;
int playerSize = 0;
int playerTurn = -1;
bool playerScored = false;
bool onShot = true;
int scorePlayer[999];
bool freeBall = false;
int btn_menu_stat = 0;

Color colors[] = {kYellow, kBlue, kBrown, kViolet, kOrange, kGreen, kRed};

void printString(int x, int y, float r, float g, float b, char *str)
{
  glColor3f( r, g, b );
  glRasterPos2f(x, y);
  int len, i;
  len = (int)strlen(str);
  for (i = 0; i < len; i++) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
  }
}

void TableSetup() {
  // Pocket Setup
  holes.push_back(Ball(15.0 + table_xinit - 7, 15.0 + table_yinit - 7, 30, kDarkWhite, false, false));
  holes.push_back(Ball(15.0 + table_xinit, 15.0 + table_yinit, 25, kBlack, false, false));

  holes.push_back(Ball(15.0 + table_xinit - 7, table_height-15 + 7 + table_yinit, 30, kDarkWhite, false, false));
  holes.push_back(Ball(15.0 + table_xinit, table_height-15 + table_yinit, 25, kBlack, false, false));

  holes.push_back(Ball(table_width/2 + table_xinit, 15.0 + table_yinit - 9, 28, kDarkWhite, false, false));
  holes.push_back(Ball(table_width/2 + table_xinit, 15.0 + table_yinit, 25, kBlack, false, false));

  holes.push_back(Ball(table_width/2 + table_xinit, table_height-15 + 9 + table_yinit, 28, kDarkWhite, false, false));
  holes.push_back(Ball(table_width/2 + table_xinit, table_height-15 + table_yinit, 25, kBlack, false, false));

  holes.push_back(Ball(table_width-15 + 7 + table_xinit, 15.0 - 7 + table_yinit, 30, kDarkWhite, false, false));
  holes.push_back(Ball(table_width-15 + table_xinit, 15.0 + table_yinit, 25, kBlack, false, false));

  holes.push_back(Ball(table_width-15 + 7 + table_xinit, table_height-15 + 7 + table_yinit, 30, kDarkWhite, false, false));
  holes.push_back(Ball(table_width-15 + table_xinit, table_height-15 + table_yinit, 25, kBlack, false, false));

  // Cueball Setup
  balls.push_back(Ball(150 + table_xinit, table_height/2 + table_yinit, 13, kWhite, false, false));

  // 15-ball Billiards Setup
  int ball_num = 1;
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < i + 1; j++) {
      if (ball_num < 8)
        balls.push_back(Ball(500+(26*i) + table_xinit,
                             (table_height/2)-(26*i/2)+(27*j) + table_yinit,
                             13, colors[(ball_num - 1)%7], true, true) );
      else if (ball_num == 8)
        balls.push_back(Ball(500+(26*i) + table_xinit,
                             (table_height/2)-(26*i/2)+(27*j)+ table_yinit,
                             13, kBlack, false, true) );
      else
        balls.push_back(Ball(500+(26*i) + table_xinit,
                             (table_height/2)-(26*i/2)+(27*j)+ table_yinit,
                             13, colors[(ball_num - 1)%7], false, true) );

      ball_num++;
    }
  }
}

void drawTable() {
  glColor3f(0.4666667, 0.23529411, 0.17254901);
  glBegin(GL_TRIANGLE_FAN);
  glVertex2f(0.0+ table_xinit-20, 0.0+ table_yinit);
  glVertex2f(15.0+ table_xinit, 0.0+ table_yinit);
  glVertex2f(15.0+ table_xinit, table_height+ table_yinit);
  glVertex2f(0.0+ table_xinit-20, table_height+ table_yinit);
  glEnd();

  glBegin(GL_TRIANGLE_FAN);
  glVertex2f(0.0+ table_xinit, 0.0+ table_yinit-20);
  glVertex2f(table_width+ table_xinit, 0.0+ table_yinit-20);
  glVertex2f(table_width+ table_xinit, 15.0+ table_yinit);
  glVertex2f(0.0+ table_xinit, 15.0+ table_yinit);
  glEnd();

  glBegin(GL_TRIANGLE_FAN);
  glVertex2f(table_width+ table_xinit+20, 0.0+ table_yinit);
  glVertex2f(table_width+ table_xinit+20, table_height+ table_yinit);
  glVertex2f(table_width - 15+ table_xinit, table_height+ table_yinit);
  glVertex2f(table_width - 15+ table_xinit, 0.0+ table_yinit);
  glEnd();

  glBegin(GL_TRIANGLE_FAN);
  glVertex2f(table_width+ table_xinit, table_height+ table_yinit+20);
  glVertex2f(table_width+ table_xinit, table_height - 15+ table_yinit);
  glVertex2f(0.0+ table_xinit, table_height - 15+ table_yinit);
  glVertex2f(0.0+ table_xinit, table_height+ table_yinit+20);
  glEnd();

  glColor3f(0.25098039,0.431372549,0.07450980);
  glBegin(GL_POLYGON);
  glVertex2f(15.0+ table_xinit, 41.0+ table_yinit);
  glVertex2f(30.0+ table_xinit, 55.0+ table_yinit);
  glVertex2f(30.0+ table_xinit, table_height-55+ table_yinit);
  glVertex2f(15.0+ table_xinit, table_height-41+ table_yinit);
  glEnd();

  glBegin(GL_POLYGON);
  glVertex2f(table_width - 15.0+ table_xinit, 41.0+ table_yinit);
  glVertex2f(table_width - 30.0+ table_xinit, 55.0+ table_yinit);
  glVertex2f(table_width - 30.0+ table_xinit, table_height-55+ table_yinit);
  glVertex2f(table_width - 15.0+ table_xinit, table_height-41+ table_yinit);
  glEnd();

  glBegin(GL_POLYGON);
  glVertex2f(0.0+ table_xinit + 41, 0.0+ table_yinit+15);
  glVertex2f(table_width/2  + table_xinit-25, 0.0+ table_yinit+15);
  glVertex2f(table_width/2  + table_xinit-37, 20.0+ table_yinit+10);
  glVertex2f(0.0+ table_xinit + 55, 20.0+ table_yinit+10);
  glEnd();

  glBegin(GL_POLYGON);
  glVertex2f(table_width/2  + table_xinit+25, 0.0+ table_yinit+15);
  glVertex2f(table_width + table_xinit-41, 0.0+ table_yinit+15);
  glVertex2f(table_width + table_xinit-55, 20.0+ table_yinit+10);
  glVertex2f(table_width/2  + table_xinit+37, 20.0+ table_yinit+10);
  glEnd();

  glBegin(GL_POLYGON);
  glVertex2f(0.0+ table_xinit + 41, 0.0+ table_yinit-15+table_height);
  glVertex2f(table_width/2  + table_xinit-25, 0.0+ table_yinit-15+table_height);
  glVertex2f(table_width/2  + table_xinit-37, table_yinit-30+table_height);
  glVertex2f(0.0+ table_xinit + 55, table_yinit-30+table_height);
  glEnd();

  glBegin(GL_POLYGON);
  glVertex2f(table_width/2  + table_xinit+25, 0.0+ table_yinit-15+table_height);
  glVertex2f(table_width + table_xinit-41, 0.0+ table_yinit-15+table_height);
  glVertex2f(table_width + table_xinit-55, table_yinit-30+table_height);
  glVertex2f(table_width/2  + table_xinit+37, table_yinit-30+table_height);
  glEnd();

  glBegin(GL_LINES);
  glVertex2f((table_width/2 + table_xinit) - 37, table_yinit);
  glVertex2f((table_width/2 + table_xinit) - 37, table_height + table_yinit);
  glVertex2f((table_xinit) + 55, table_yinit);
  glVertex2f((table_xinit) + 55, table_height + table_yinit);
  glVertex2f((table_xinit + table_width) - 55, table_yinit);
  glVertex2f((table_xinit + table_width) - 55, table_height + table_yinit);
  glVertex2f((table_xinit), table_yinit + table_height- 55 );
  glVertex2f((table_xinit + table_width), table_height- 55 + table_yinit);
  glVertex2f((table_xinit), table_yinit + 55 );
  glVertex2f((table_xinit + table_width), table_yinit + 55);
  glVertex2f((table_width/2 + table_xinit) + 37, table_yinit);
  glVertex2f((table_width/2 + table_xinit) + 37, table_height + table_yinit);
  glEnd();

}

void drawPowerBar(){
    glBegin(GL_POLYGON);
    glColor3f(1,1,1);
    glVertex2f(0.0 + powerbar_xinit, 50.0 + powerbar_yinit);
    glVertex2f(170.0 + powerbar_xinit, powerbar_yinit - 10.0);
    glVertex2f(170.0 + powerbar_xinit, 50.0 + powerbar_yinit);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1,1,0);
    glVertex2f(0.0 + powerbar_xinit, 50.0 + powerbar_yinit);
    glColor3f(1,0,0);
    glVertex2f(150.0 + powerbar_xinit, powerbar_yinit);
    glVertex2f(150.0 + powerbar_xinit, 50.0 + powerbar_yinit);
    glEnd();
}

void drawCueStick() {
  float tip_scale, end_scale;
  if (draw_line == DRAW_AIM) {
    tip_scale = .5;
    end_scale = 5;
  } else {
    tip_scale = .95;
    end_scale = 4;
  }

  glLineWidth(7);
  glColor3f(.647, .1647, .1647);

  glBegin(GL_LINES);
  glVertex2f(mouse_down[0] + tip_scale*(balls[0].x[0] - mouse_down[0]),
             mouse_down[1] + tip_scale*(balls[0].x[1] - mouse_down[1]));
  glVertex2f(mouse_down[0] - end_scale*(balls[0].x[0] - mouse_down[0]),
             mouse_down[1] - end_scale*(balls[0].x[1] - mouse_down[1]));
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
}

void score(int index) {
  for (int i = 0; i < holes.size(); i++) {
    if (balls[index].collideToHole(&holes[i])) {
      if(index==0) { // <-- white balls become free ball
        balls[index] = Ball(150 + table_xinit, table_height/2 + table_yinit, 13, kWhite, false, true);
        freeBall = true;
        break;
      }
      balls[index].x[0] = holes[i].x[0];
      balls[index].x[1] = holes[i].x[1];
      balls[index].setVelocity(0, 0);
      scored.push_back(balls[index]);
      balls.erase(balls.begin()+index);
      playerScored = true;
      scorePlayer[playerTurn]+=10;
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
        if(playerTurn<playerSize && !playerScored) { playerTurn++; }
        else if(playerTurn==playerSize && !playerScored) playerTurn = 1;
        playerScored = false;
        onShot = true;
  }

  last_time = present_time;
  glutPostRedisplay();
}

void Display() {
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);

  //glColor3f(0, 0.4, 0.1);
  glColor3f(0.35686274, 0.65490196, 0.0862745);
  glBegin(GL_POLYGON);
  glVertex2f(0.0 + table_xinit, 0.0 + table_yinit);
  glVertex2f(table_xinit + table_width, 0.0 + table_yinit);
  glVertex2f(table_xinit + table_width, table_height + table_yinit);
  glVertex2f(0.0 + table_xinit, table_height + table_yinit);
  glEnd();

  string playerName = "Player ";
  stringstream ss;
  ss << playerTurn;
  playerName.append(ss.str());
  char *cstr = new char[playerName.length() + 1];
  strcpy(cstr, playerName.c_str());
  printString(20,30,1,1,0,cstr);

  string scoreBoard = "Score: ";
  stringstream ss1;
  ss1 << scorePlayer[playerTurn];
  scoreBoard.append(ss1.str());
  char *cstrscore = new char[playerName.length() + 1];
  strcpy(cstrscore, scoreBoard.c_str());
  printString(200,30,1,1,0,cstrscore);

  if(freeBall) printString(400,30,1,1,0,"Free Ball");

  drawTable();
  drawPowerBar();

  for (int i = 0; i < holes.size(); i++)
    holes[i].draw();
  for (int i = 0; i < balls.size(); i++)
    balls[i].draw();
  for (int i = 0; i < scored.size(); i++)
    scored[i].draw();

  if(ballsIsStopped())
  {
      string playerName = "Go!!!!!! ";
        char *cstr = new char[playerName.length() + 1];
        strcpy(cstr, playerName.c_str());
        printString(20,60,1,1,0,cstr);
  }
  else
  {
      glColor4f(0, 0, 0, 0.4);
      glBegin(GL_POLYGON);
        glVertex2d(0,0);
        glVertex2d(window_width,0);
        glVertex2d(window_width,window_height);
        glVertex2d(0,window_height);
      glEnd();

      string playerName = "Wait!!!!!! ";
        char *cstr = new char[playerName.length() + 1];
        strcpy(cstr, playerName.c_str());
        printString(360, 300,1,1,0,cstr);
  }

  if (draw_line == DRAW_SHOT && glutGet(GLUT_ELAPSED_TIME) - shot_time > 25) {
    draw_line = DRAW_NULL;
  } else if (draw_line != DRAW_NULL) {
    drawCueStick();
    drawAimHelper();
  }

  glFlush();
  glutSwapBuffers();
}

void Display1() {
  pageState = 1;

  loadtex("img/help.png", window_width, window_height, 0, 0);
  loadtex("img/btn_back.png", 154, 84, 35, 50);

  glFlush();
  glutSwapBuffers();
}

void MainMenuDisplay() {
  pageState = 0;

  loadtex("img/menu.png", window_width, window_height, 0, 0);
  loadtex("img/btn_play.png", 211, 68, 450, 380);
  loadtex("img/btn_help.png", 107, 49, 570, 460);
  loadtex("img/btn_exit.png", 131, 69, 630, 20);

  glFlush();
  glutSwapBuffers();
}

void drawButton(int x, int y, int w, int h)
{
    glBegin(GL_POLYGON);
    glVertex2d(x, y);
    glVertex2d(x+w, y);
    glVertex2d(x+w, y+h);
    glVertex2d(x, y+h);
    glEnd();
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
              glutDisplayFunc(Display);
              btn_menu_stat = 1;
          }
          // Help Button
          else if(x >= 574 && x <= 671 && y >= 472 && y <= 498)
          {
              glutDisplayFunc(Display1);
          }
          // Exit Button
          else if(x >= 634 && x <= 751 && y >= 30 && y <= 77)
          {
              exit(EXIT_SUCCESS);
          }
      }

      if(state == GLUT_UP && btn_menu_stat == 1 ) {
          btn_menu_stat = 0;
          pageState = 2;
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

      if(state == GLUT_UP && btn_menu_stat == 1 ) {
          pageState = 0;
      }

  }
  else if (button == GLUT_LEFT_BUTTON && pageState==2 && ballsIsStopped()) {
    if (state == GLUT_DOWN) {
      mouse_down[0] = x;
      mouse_down[1] = y;
      draw_line = DRAW_AIM;
    }
    if (state == GLUT_UP) {
      balls[0].v.x[0] = velocity_coeff * (balls[0].x[0] - mouse_down[0]);
      balls[0].v.x[1] = velocity_coeff * (balls[0].x[1] - mouse_down[1]);
      balls[0].a[0] = drag_coeff*balls[0].v[0];
      balls[0].a[1] = drag_coeff*balls[0].v[1];
      draw_line = DRAW_SHOT;
      shot_time = glutGet(GLUT_ELAPSED_TIME);
      onShot = false;
      if(freeBall) freeBall = false;
    }
  }
  glutPostRedisplay();
}

void MouseMotion(int x, int y) {
  mouse_down[0] = x;
  mouse_down[1] = y;

  glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y) {
  switch (key) {
  case 'q':
    exit(EXIT_SUCCESS);
    break;
  case 'h':
    glutDisplayFunc(Display1);
    break;
  case 'b':
    glutDisplayFunc(Display);
    break;
  }
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  cout << "How many users?";
  cin >> playerSize;
  playerTurn = 1;
  memset(scorePlayer,0,sizeof(scorePlayer));
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(window_width, window_height);
  glutInitWindowPosition(100, 50);
  glutCreateWindow("My Billiard Game 2D");
  glutDisplayFunc(MainMenuDisplay);
  glutMouseFunc(Mouse);
  glutMotionFunc(MouseMotion);
  glutKeyboardFunc(Keyboard);
  glutIdleFunc(idle);
  Init();
  glutMainLoop();
}
