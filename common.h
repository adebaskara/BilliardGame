#ifndef __COMMON_H__
#define __COMMON_H__

#ifdef _WIN32
#include <windows.h>
#endif
#ifdef __MAC__
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif

const double PI = 3.14159;
const double velocity_coeff = 3.2;  // Scales the mouse drag vector for velocity
const double drag_coeff = -0.99;   // The drag coefficient for decceleration
const int window_width = 800;
const int window_height = 600;
const int table_width = 750;
const int table_height = 375;
const int table_xinit = 25;
const int table_yinit = 100;
const int powerbar_xinit = 25;
const int powerbar_yinit = 500;
const int pointball_xinit = 300;
const int pointball_yinit = 550;

struct Color {
  Color() {}
  Color(GLfloat _r, GLfloat _g, GLfloat _b) {
    c[0] = _r;
    c[1] = _g;
    c[2] = _b;
  }
  operator GLfloat*() { return c; }
  GLfloat c[3];
};


const Color kRed = Color(1, 0, 0);
const Color kGreen = Color(0, 1, 0);
const Color kBlue = Color(0, 0, 1);
const Color kYellow = Color(1, 1, 0);
const Color kViolet = Color(0.541176, 0.168627, 0.886275);
const Color kBrown = Color(0.647, 0.1647, 0.1647);
const Color kOrange = Color(1, 0.498039, 0.313725);
const Color kBlack = Color(0, 0, 0);
const Color kWhite = Color(1, 1, 1);

#endif

