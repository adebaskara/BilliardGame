#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <cmath>

struct Vector {
  Vector() {}
  Vector(GLfloat _x, GLfloat _y) {
    x[0] = _x;
    x[1] = _y;
  }

  GLfloat x[2];
  GLfloat operator[](int i) { return x[i]; }
};

inline GLfloat find_position(GLfloat acc, float elapsed_time,
                             GLfloat vi, GLfloat xi) {
  return .5*acc*(elapsed_time*elapsed_time) + vi*elapsed_time + xi;
}

inline GLfloat find_velocity(GLfloat acc, float elapsed_time, GLfloat vi) {
  return acc*elapsed_time + vi;
}

inline GLfloat deaccel(GLfloat v) {
  return drag_coeff*v;
}

inline GLfloat dist(GLfloat x0, GLfloat y0, GLfloat x1, GLfloat y1) {
  return sqrt(pow(x1-x0, 2) + pow(y1-y0, 2));
}

#endif
