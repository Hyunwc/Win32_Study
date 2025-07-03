#pragma once

#define GLUT_DOUBLE		1
#define GLUT_RGBA		2
#define GLUT_DEPTH		4
typedef signed int nsInt;

//void nsInit(int* argc, char** argv);
//void nsInitDisplayMode(nsInt dm); //GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH
//void nsInitWindowSize(nsInt w, nsInt h); //DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT
//void nsInitWindowPosition(nsInt x, nsInt y); //100, 100
//void nsCreateWindow(const char* szFormat, ...); // "ViewScene"

int nsAdd(int a, int b);