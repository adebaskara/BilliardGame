#include <math.h>

#include "ball.h"
#include "common.h"
#include "geometry.h"
#include <vector>


Ball::Ball(GLfloat _x, GLfloat _y,
           GLint _r, Color _c, bool _solid, bool _is_ball): radius(_r), c(_c), solid(_solid), is_ball(_is_ball) {
  x[0] = _x;
  x[1] = _y;
  v = Vector(0, 0);
  a[0] = a[1] = 0;
  mass = 1;
}

void Ball::draw() {
  glBegin(GL_TRIANGLE_FAN);

  for (int angle = 1; angle < 360; angle += 1) {
    int xn = x[0] + radius*sin(PI*angle/180);
    int yn = x[1] + radius*cos(PI*angle/180);

    glColor3fv(c);

    glVertex2f(xn, yn);
  }
  glEnd();

  if (solid) {
    glBegin(GL_TRIANGLE_FAN);

    for (int angle = 0; angle < 360; angle += 1) {
      int xn = x[0] + (radius/2)*sin(PI*angle/180);
      int yn = x[1] + (radius/2)*cos(PI*angle/180);
      glColor4f(1.0, 1.0, 1.0, 0.6);
      glVertex2f(xn, yn);
    }
  }

  glEnd();

  if(is_ball)
  {
      glBegin(GL_TRIANGLE_FAN);
        int xn = x[0] + (radius/1.1)*sin(PI*0/180);
        int yn = x[1] + (radius/1.9)*cos(PI*0/180);
        glColor4f(1.0, 1.0, 1.0, 0.5);
        glVertex2f(xn, yn + 4);
        for (int angle = 1; angle < 360; angle += 1) {
            int xn = x[0] + (radius/1.1)*sin(PI*angle/180);
            int yn = x[1] + (radius/1.9)*cos(PI*angle/180);
            glColor4f(1.0, 1.0, 1.0, 0.0);
            glVertex2f(xn, yn + 4);
        }
        glEnd();
  }
}

bool Ball::isStopped(){
    return ((int)v[0]==0 && (int)v[1]==0);
}

void Ball::update(float elapsed_time, vector<Ball> holes) {
  for (int i = 0; i < 2; i++) {
    x[i] = find_position(a[i], elapsed_time, v[i], x[i]);
    v.x[i] = find_velocity(a[i], elapsed_time, v[i]);
    a[i] = deaccel(v[i]);
  }

  checkBounds(holes);
}

bool isHoles(int x, int y, vector<Ball> holes)
{
    bool yes = false;
    for(int i=0; i<holes.size(); i++)
    {

    }
}

bool cekAtas(GLfloat x[2], GLint radius)
{
    return (x[1] - radius < 30 + table_yinit) && (((x[0] - radius < (table_width/2 + table_xinit) - 37) && (x[0] - radius > table_xinit + 55)) || ((x[0] - radius > (table_width/2 + table_xinit) + 37) && (x[0] - radius < table_xinit + table_width - 55)));
}

bool cekBawah(GLfloat x[2], GLint radius)
{
    return (x[1] + radius > table_height - 30 + table_yinit) && ((x[0] - radius < (table_width/2 + table_xinit) - 37) || (x[0] - radius > (table_width/2 + table_xinit) + 37));
}


void Ball::checkBounds(vector<Ball> holes) {
  // Horizontal Bounds
  if (x[0] - radius < table_xinit + 30) {
    x[0] = radius + table_xinit + 30;
    v.x[0] = abs(v.x[0]);
    cout << "kanan";
  } else if (x[0] + radius > table_width - 30 + table_xinit) {
     x[0] = table_width - 30 - radius + table_xinit;
    v.x[0] = -abs(v.x[0]);
    cout << "kiri";
  }

  // Vertical Bounds
  if (cekAtas(x,radius)){
     x[1] = radius + 30  + table_yinit;
    v.x[1] = abs(v[1]);
  } else if (cekBawah(x,radius)) {
     x[1] = table_height - 30 - radius + table_yinit;
    v.x[1] = -abs(v.x[1]);
  }
}

bool Ball::collide(Ball *other) {
  return dist(x[0], x[1], other->x[0], other->x[1]) <= radius + other->radius;
}

bool Ball::collideToHole(Ball *hole) {
  return dist(x[0], x[1], hole->x[0], hole->x[1])/2 < radius;
}

void Ball::setVelocity(GLfloat vx, GLfloat vy) {
  v.x[0] = vx;
  v.x[1] = vy;

  a[0] = deaccel(v.x[0]);
  a[1] = deaccel(v.x[1]);
}

void Ball::elasticCollision(Ball *other) {
  GLfloat position[2], velocity[2];
  GLfloat dot, d, a;

  position[0] = other->x[0] - x[0];
  position[1] = other->x[1] - x[1];
  velocity[0] = other->v.x[0] - v.x[0];
  velocity[1] = other->v.x[1] - v.x[1];

  dot = velocity[0]*position[0] + velocity[1]*position[1];

  if (dot <= 0) {
    a = position[1]/position[0];
    d = -2*(velocity[0] + (a*velocity[1]))/((1 + a*a)*(1+(other->mass/mass)));
    other->v.x[0] = other->v.x[0] + d;
    other->v.x[1] = other->v.x[1] + a*d;
    v.x[0] = v.x[0] - (other->mass/mass)*d;
    v.x[1] = v.x[1] - a*(other->mass/mass)*d;
  }
}

