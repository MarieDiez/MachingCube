#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdbool.h>

#define MAX_INPUT_SIZE 10

char * FILE_NAME2D = "donnees";
char * FILE_NAME3D = "donnees3d";
int WindowName;
GLfloat angle_h = 0.0;
GLfloat angle_v = 0.0;
int prev_x = -1;
int prev_y = -1;
double ** x;
double ** y;
double * z;
double ** s;
int n;
bool showNumber = false;
bool showCube = false;
bool showNumber3D = false;
double minVal;
double maxVal;
double meanVal;
double isoVal = 0;

double max(double * tab,int n);
double min(double * tab,int n);
double mean(double * tab,int n);

/* STRUCTURE */
typedef struct _Point{
	int i;
	int j;
	int z;
	double val;
} Point;
Point *lst_point;

/* TABLEAUX */
int sommetsArretes[12][2] = {
	{0,1},
	{1,2},
	{2,3},
	{3,0},
	{4,5},
	{5,6},
	{6,7},
	{7,4},
	{0,4},
	{1,5},
	{2,6},
	{3,7}
};

int TableDesTriangles[256][16] = {
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, {0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	{0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	{1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	{1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	{0, 8, 3, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	{9, 2, 10, 0, 2, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	{2, 8, 3, 2, 10, 8, 10, 9, 8, -1, -1, -1, -1, -1, -1, -1},	{3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	{0, 11, 2, 8, 11, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	{1, 9, 0, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	{1, 11, 2, 1, 9, 11, 9, 8, 11, -1, -1, -1, -1, -1, -1, -1},	{3, 10, 1, 11, 10, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	{0, 10, 1, 0, 8, 10, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1},	{3, 9, 0, 3, 11, 9, 11, 10, 9, -1, -1, -1, -1, -1, -1, -1},{9, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	{4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	{4, 3, 0, 7, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	{0, 1, 9, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	{4, 1, 9, 4, 7, 1, 7, 3, 1, -1, -1, -1, -1, -1, -1, -1},	{1, 2, 10, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	{3, 4, 7, 3, 0, 4, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1},	{9, 2, 10, 9, 0, 2, 8, 4, 7, -1, -1, -1, -1,-1, -1, -1},	{2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4, -1, -1, -1, -1},	{8, 4, 7, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	{11, 4, 7, 11, 2, 4, 2, 0, 4, -1, -1, -1, -1, -1, -1, -1},	{9, 0, 1, 8, 4, 7, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},	{4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1, -1, -1, -1, -1},	{3, 10, 1, 3, 11, 10, 7, 8, 4, -1, -1, -1, -1, -1, -1, -1},	{1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4, -1, -1, -1, -1},	{4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3, -1, -1, -1, -1},	{4, 7, 11, 4, 11, 9, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1},	{9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	{9, 5, 4, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	{0, 5, 4, 1, 5, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	{8, 5, 4, 8, 3, 5, 3, 1, 5, -1, -1, -1, -1, -1, -1, -1},	{1, 2, 10, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	{3, 0, 8, 1, 2, 10, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},	{5, 2, 10, 5, 4, 2, 4, 0, 2, -1, -1, -1, -1, -1, -1, -1},	{2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8, -1, -1, -1, -1},	{9, 5, 4, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	{0, 11, 2, 0, 8, 11, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},	{0, 5, 4, 0, 1, 5, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},	{2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5, -1, -1, -1, -1},	{10, 3, 11, 10, 1, 3, 9, 5, 4, -1,-1, -1, -1, -1, -1, -1},	{4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10, -1, -1, -1, -1},	{5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3, -1, -1, -1, -1},	{5, 4, 8, 5, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1},	{9, 7, 8, 5, 7, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	{9, 3, 0, 9, 5, 3, 5, 7, 3, -1, -1, -1, -1, -1, -1, -1},	{0, 7, 8, 0, 1, 7, 1, 5, 7, -1, -1, -1, -1, -1, -1, -1},	{1, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	{9, 7, 8, 9, 5, 7, 10, 1, 2, -1, -1, -1, -1, -1, -1, -1},	{10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3, -1, -1, -1, -1},	{8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2, -1, -1, -1, -1},	{2, 10, 5, 2, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1},	{7, 9, 5, 7, 8, 9, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1},	{9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11, -1, -1, -1,-1},	{2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7, -1, -1, -1, -1},	{11, 2, 1, 11, 1, 7, 7, 1, 5, -1, -1, -1, -1, -1, -1, -1},	{9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11, -1, -1, -1, -1},	{5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0, -1},	{11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0, -1},	{11, 10, 5, 7, 11, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	{10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	{0, 8, 3, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	{9, 0, 1, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	{1, 8, 3, 1, 9, 8, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},	{1, 6, 5, 2, 6, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	{1, 6, 5, 1, 2, 6, 3, 0, 8, -1, -1, -1, -1, -1, -1, -1},	{9, 6, 5, 9, 0, 6, 0, 2, 6, -1, -1, -1, -1, -1, -1, -1},	{5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8, -1, -1, -1, -1},	{2, 3, 11, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	{11, 0, 8, 11, 2, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},{0, 1, 9, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},{5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11, -1, -1, -1, -1},{6, 3, 11, 6, 5, 3, 5, 1, 3, -1, -1, -1, -1, -1, -1, -1},{0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6, -1, -1, -1, -1},{3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9, -1, -1, -1, -1},{6, 5, 9, 6, 9, 11, 11, 9, 8, -1, -1, -1, -1, -1, -1, -1},{5, 10, 6, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},{4, 3, 0, 4, 7, 3, 6, 5, 10, -1, -1, -1, -1, -1, -1, -1},{1, 9, 0, 5, 10, 6, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},{10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4, -1, -1, -1, -1},{6, 1, 2, 6, 5, 1, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1},{1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7, -1, -1, -1, -1},{8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6, -1, -1, -1, -1},{7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9, -1},{3, 11, 2, 7, 8, 4, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},{5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11, -1, -1, -1, -1},{0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1},{9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6, -1},{8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6, -1, -1, -1, -1},{5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11, -1},{0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7, -1},{6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9, -1, -1, -1, -1},{10, 4, 9, 6, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},{4, 10, 6, 4, 9, 10, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1},{10, 0, 1, 10, 6, 0, 6, 4, 0, -1, -1, -1, -1, -1, -1, -1},{8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10, -1, -1, -1, -1},{1, 4, 9, 1, 2, 4, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1},{3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4, -1, -1, -1, -1},{0, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},{8, 3, 2, 8, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1},{10, 4, 9, 10, 6, 4, 11, 2, 3, -1, -1, -1, -1, -1, -1, -1},{0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6, -1, -1, -1, -1},{3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10, -1, -1, -1, -1},{6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1, -1},{9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3, -1, -1, -1, -1},{8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1, -1},{3, 11, 6, 3, 6, 0, 0, 6, 4, -1, -1, -1, -1, -1, -1, -1},{6, 4, 8, 11, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},{7, 10, 6, 7, 8, 10, 8, 9, 10, -1, -1, -1, -1, -1, -1, -1},{0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10, -1, -1, -1, -1},{10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0, -1, -1, -1, -1},{10, 6, 7, 10, 7, 1, 1, 7, 3, -1, -1, -1, -1, -1, -1, -1},{1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7, -1, -1, -1, -1},{2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9, -1},{7, 8, 0, 7, 0, 6, 6, 0, 2, -1, -1, -1, -1, -1, -1, -1},{7, 3, 2, 6, 7, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},{2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7, -1, -1, -1, -1},{2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7, -1},{1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11, -1},{11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1, -1, -1, -1, -1},{8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6, -1},{0, 9, 1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},{7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0, -1, -1, -1, -1},{7, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},{7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},{3, 0, 8, 11, 7, 6, -1,-1, -1, -1, -1, -1, -1, -1, -1, -1},{0, 1, 9, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},{8, 1, 9, 8, 3, 1, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},{10, 1, 2, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},{1, 2, 10, 3, 0, 8, 6, 11, 7, -1, -1,-1, -1, -1, -1, -1},{2, 9, 0, 2, 10, 9, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},{6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8, -1, -1, -1, -1},{7, 2, 3, 6, 2, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},{7, 0, 8, 7, 6, 0, 6, 2, 0, -1, -1, -1, -1, -1, -1, -1},{2, 7, 6, 2, 3, 7, 0, 1, 9, -1, -1, -1, -1, -1, -1, -1},{1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6, -1, -1, -1, -1},{10, 7, 6, 10, 1, 7, 1, 3, 7, -1, -1, -1, -1, -1, -1, -1},{10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8, -1, -1, -1, -1},{0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7, -1, -1, -1, -1},{7, 6, 10, 7, 10, 8, 8, 10, 9, -1, -1, -1, -1, -1, -1, -1},{6, 8, 4, 11, 8, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},{3, 6, 11, 3, 0, 6, 0, 4, 6, -1, -1, -1, -1, -1, -1, -1},{8, 6, 11, 8, 4, 6, 9, 0, 1, -1, -1, -1, -1, -1, -1, -1},{9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6, -1, -1, -1, -1},{6, 8, 4, 6, 11, 8, 2, 10, 1, -1, -1, -1, -1, -1, -1, -1},{1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6, -1, -1, -1, -1},{4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9, -1, -1, -1, -1},{10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3, -1},{8, 2, 3, 8, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1},{0, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},{1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8, -1, -1, -1, -1},{1, 9, 4, 1, 4, 2, 2, 4, 6, -1, -1, -1, -1, -1, -1,-1},{8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1, -1, -1, -1, -1},{10, 1, 0, 10, 0, 6, 6, 0, 4, -1, -1, -1, -1, -1, -1, -1},{4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3, -1},{10, 9, 4, 6, 10, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},{4, 9, 5, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},{0, 8, 3, 4, 9, 5, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},{5, 0, 1, 5, 4, 0, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},{11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5, -1, -1, -1, -1},{9, 5, 4, 10, 1, 2, 7, 6, 11, -1, -1, -1, -1,-1, -1, -1},{6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5, -1, -1, -1, -1},{7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2, -1, -1, -1, -1},{3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6, -1},{7, 2, 3, 7, 6, 2, 5, 4, 9, -1, -1, -1, -1, -1, -1, -1},{9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7, -1, -1, -1, -1},{3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0, -1, -1, -1, -1},{6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8, -1},{9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7, -1, -1, -1, -1},{1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4, -1},{4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10, -1},{7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10, -1, -1, -1, -1},{6, 9, 5, 6, 11, 9, 11, 8, 9, -1, -1, -1, -1, -1, -1, -1},{3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5, -1, -1, -1, -1},{0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11, -1, -1, -1, -1},{6, 11, 3, 6, 3, 5, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1},{1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6, -1, -1, -1, -1},{0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10, -1},{11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5, -1},{6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3, -1, -1, -1, -1},{5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2, -1, -1, -1, -1},{9, 5, 6, 9, 6, 0, 0, 6, 2, -1, -1, -1, -1, -1, -1, -1},{1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8, -1},{1, 5, 6, 2, 1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},{1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6, -1},{10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0, -1, -1, -1, -1},{0, 3, 8, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},{10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},{11, 5, 10, 7, 5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},{11, 5, 10, 11, 7, 5, 8, 3, 0, -1, -1, -1, -1, -1, -1, -1},{5, 11, 7, 5, 10, 11, 1, 9, 0, -1, -1, -1, -1, -1, -1, -1},{10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1, -1, -1, -1, -1},{11, 1, 2, 11, 7, 1, 7, 5, 1, -1, -1, -1, -1, -1, -1, -1},{0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11, -1, -1, -1, -1},{9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7, -1, -1, -1, -1},{7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2, -1},{2, 5, 10, 2, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1},{8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5, -1, -1, -1, -1},{9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2, -1, -1, -1, -1},{9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2, -1},{1, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},{0, 8, 7, 0, 7, 1, 1, 7, 5, -1, -1, -1, -1, -1, -1, -1},{9, 0, 3, 9, 3, 5, 5, 3, 7, -1, -1, -1, -1, -1, -1, -1},{9, 8, 7, 5, 9, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},{5, 8, 4, 5, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1},{5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0, -1, -1, -1, -1},{0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5, -1, -1, -1, -1},{10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4, -1},{2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8, -1, -1, -1, -1},{0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11, -1},{0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5, -1},{9, 4, 5, 2, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},{2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4, -1, -1, -1, -1},{5, 10, 2, 5, 2, 4, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1},{3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9, -1},{5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2, -1, -1, -1, -1},{8, 4, 5, 8, 5, 3, 3, 5, 1, -1, -1, -1, -1, -1, -1, -1},{0, 4, 5, 1, 0, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},{8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5, -1, -1, -1, -1},{9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},{4, 11, 7, 4, 9, 11, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1},{0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11, -1, -1, -1, -1},{1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11, -1, -1, -1, -1},{3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4, -1},{4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2, -1, -1, -1, -1},{9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3, -1},{11, 7, 4, 11, 4, 2, 2, 4, 0, -1, -1, -1, -1, -1, -1, -1},{11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4, -1, -1, -1, -1},{2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9, -1, -1, -1, -1},{9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7, -1},{3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10, -1},{1, 10, 2, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},{4, 9, 1, 4, 1, 7, 7, 1, 3, -1, -1, -1, -1, -1, -1, -1},{4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1, -1, -1, -1, -1},{4, 0, 3, 7, 4, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},{4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},{9, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},{3, 0, 9, 3, 9, 11, 11, 9, 10, -1, -1, -1, -1, -1, -1, -1},{0, 1, 10, 0, 10, 8, 8, 10, 11, -1, -1, -1, -1, -1, -1, -1},{3, 1, 10, 11, 3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},{1, 2, 11, 1, 11, 9, 9, 11, 8, -1, -1, -1, -1, -1, -1, -1},{3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9, -1, -1, -1, -1},{0, 2, 11, 8, 0, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},{3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},{2, 3, 8, 2, 8, 10, 10, 8, 9, -1, -1, -1, -1, -1, -1, -1},{9, 10, 2, 0, 9, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},{2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8, -1, -1, -1, -1},{1, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},{1, 3, 8, 9, 1, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},{0, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},{0, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
};

int numSommetToCoord[8][2] = {
	{0,0},
	{1,0},
	{1,1},
	{1,0},
	{0,0},
	{1,0},
	{1,1},
	{1,0}
};

int possCasMC10[4][4] = {
	{0,1,3,4},
	{0,1,2,5},
	{1,2,3,6},
	{2,3,0,7}
};

int possCasMC12[4][4] = {
	{0,1,3,5},
	{0,1,2,6},
	{1,2,3,7},
	{2,3,0,4}
};

int possCasMC13[4][4] = {
	{0,1,2,7},
	{1,2,3,4},
	{2,3,0,5},
	{3,0,1,6}
};

int possCasMC15[4][4] = {
	{0,1,2,4},
	{1,2,3,5},
	{2,3,0,6},
	{3,0,1,7}
};

/* INTERPOLATIONS */
double getInterpolatePositionX(double x1, double x2, double v, double v1, double v2){
	return x1 + ((v - v1)/(v2 - v1))*(x2 - x1);
}

double getInterpolatePositionY(double y1, double y2, double v, double v1, double v2){
	return y1 + ((v - v1)/(v2 - v1))*(y2 - y1);
}
double getInterpolatePositionZ(double z1, double z2, double v, double v1, double v2){
	return z1 + ((v - v1)/(v2 - v1))*(z2 - z1);
}

/* CHARGER DONNEES */
void charger(char * name){
	FILE * file = fopen(name, "r");
	char char_number[MAX_INPUT_SIZE] = "";
	char * char_nb = "";	

	if (file){
		fgets(char_number, MAX_INPUT_SIZE, file);
		n = atoi(char_number);
		double tmp[n];		
		double nb;
		int k = 0;
		while(fgets(char_number, MAX_INPUT_SIZE, file) != NULL){
			if (strcmp(char_number, "\n") >  0){
				sscanf(char_number, "%lf", &nb);
				tmp[k] = nb;
				k++;
			}
		}
		k=0;
		x = malloc(n*(n*sizeof(double)));
		for (int i=0;i<n;i++){
			x[i] = malloc(n*sizeof(double));
		}

		y = malloc(n*(n*sizeof(double)));
		for (int i=0;i<n;i++){
			y[i] = malloc(n*sizeof(double));
		}

		s = malloc(n*(n*sizeof(double)));
		for (int i=0;i<n;i++){
			s[i] = malloc(n*sizeof(double));
		}
		
		int limite = sqrt(n);

		for (int i = 0; i < limite; i++) {
			for (int j = 0; j < limite; j++) { 	
				s[i][j] = tmp[k];
				k++;
			}
		}

		double nbs = sqrt(n);
		double step = 1.4 / (nbs+1);
		for (int i = 0; i < nbs; i++){
			for (int j = 0; j < nbs; j++){
			    x[i][j] = i*step - 0.7 + step;
			    y[i][j] = j*step - 0.7 + step;
			}
		}


		fclose(file);

	} else {
		printf("%s : %s\n", "Problème lors de la lecture du fichier", name);
	}
}

void charger3d(char * name){
	FILE * file = fopen(name, "r");
	char char_number[MAX_INPUT_SIZE] = "";
	char * char_nb = "";	
	lst_point = malloc(n*2*sizeof(Point));
	z = realloc(z, 2*sizeof(double));
	z[0] = 0.2;
	z[1] = 0.4;
	if (file){
		fgets(char_number, MAX_INPUT_SIZE, file);
		double tmp[n*2];		
		double nb;
		int k = 0;
		while(fgets(char_number, MAX_INPUT_SIZE, file) != NULL){
			if (strcmp(char_number, "\n") >  0){
				sscanf(char_number, "%lf", &nb);
				tmp[k] = nb;
				k++;
			}
		}
		maxVal = max(tmp,n*2);
		minVal = min(tmp,n*2);
		meanVal = mean(tmp,n*2);
		isoVal = meanVal;
		k=0;
		double step = 1;
		int limite = sqrt(n);
		int z = 0;
		for (int i = 0; i < limite; i++) {
			for (int j = 0; j < limite; j++) {
				Point pts;
				pts.i = i;
				pts.j = j;
				pts.z = 0;
				pts.val = tmp[k];
				lst_point[k] = pts;
				k++;
			}
		}
		for (int i = 0; i < limite; i++) {
			for (int j = 0; j < limite; j++) {
				Point pts;
				pts.i = i;
				pts.j = j;
				pts.z = 1;
				pts.val = tmp[k];
				lst_point[k] = pts;
				k++;
			}
		}
		fclose(file);

	} else {
		printf("%s : %s\n", "Problème lors de la lecture du fichier", name);
	}
}

/* MACHING CUBE */
int toOctect(double * liste_val_cube){
	int octet = 0;
	for (int i = 0; i < 8; i++){
		if (liste_val_cube[i] > isoVal){
			octet += (int)pow(2,i);
		}
	}
	return octet;
}

int * listeSup(double * liste_val_cube){
	int * octet = malloc(8*sizeof(int));
	for (int i = 0; i < 8; i++){
			octet[i] = -1;
	}
	int k = 0;
	for (int i = 0; i < 8; i++){
		if (liste_val_cube[i] > isoVal){
			octet[k] = i;
			k++;
		}
	}
	return octet;
}

bool inListe(int val, int * liste, int n){
	bool rep = false;
	int i=0;

	while(i<n && !rep){
		if(val == liste[i]){
			rep = true;
		}
		i++;
	}
	return rep;
}

double max(double * tab,int n){
	double max = tab[0];
	for (int i = 0 ; i < n ; i++){
		if (tab[i] > max){
			max = tab[i];
		}
	}
	return max;
}

double min(double * tab,int n){
	double min = tab[0];
	for (int i = 0 ; i < n ; i++){
		if (tab[i] < min){
			min = tab[i];
		}
	}
	return min;
}

double mean(double * tab, int n){
	double somme = 0; 
	for (int i = 0 ; i < n ; i++){
		somme += tab[i];
	}
	return somme/n;
}

void casMC(int * listeSommetsSup, double * liste_val_cube, Point * cube){
	int octet = toOctect(liste_val_cube);
	int l = 0;
	double triangles[16][3];
	for (int k = 0 ; k<16;k++){
		if (TableDesTriangles[octet][k] != -1){
			int numSommet1 = sommetsArretes[TableDesTriangles[octet][k]][0];
			int numSommet2 = sommetsArretes[TableDesTriangles[octet][k]][1];

			Point pts1 = cube[numSommet1];
			Point pts2 = cube[numSommet2];

			double posX = getInterpolatePositionX(x[pts1.i][pts1.j],x[pts2.i][pts2.j],isoVal,pts1.val,pts2.val);
			double posY = getInterpolatePositionY(y[pts1.i][pts1.j],y[pts2.i][pts2.j],isoVal,pts1.val,pts2.val);
			double posZ = getInterpolatePositionZ(z[pts1.z],z[pts2.z],isoVal,pts1.val,pts2.val);
			triangles[l][0] = posX;
			triangles[l][1] = posY;
			triangles[l][2] = posZ;
			l++;
		}

	}
	for (int i = 0; i < l; i+=3){
		glBegin(GL_LINE_LOOP);
		glVertex3f(triangles[i][0], triangles[i][1],triangles[i][2]);
		glVertex3f(triangles[i+1][0], triangles[i+1][1],triangles[i+1][2]);
		glVertex3f(triangles[i+2][0], triangles[i+2][1],triangles[i+2][2]);
		glEnd();
	}
}

void maching_cube(){
	Point * cube = malloc(8*sizeof(Point));
	for (int i = 0; i < sqrt(n)-1; i++) {
		for (int j = 0; j < sqrt(n)-1; j++) {
			int indexP1 = (sqrt(n)*i)+j;
			int indexP2 = (sqrt(n)*(i+1))+j;
			int indexP3 = (sqrt(n)*(i+1))+(j+1);
			int indexP4 = (sqrt(n)*i)+(j+1);
			int indexP5 = (sqrt(n)*i)+j +n;
			int indexP6 = (sqrt(n)*(i+1))+j+n;
			int indexP7 = (sqrt(n)*(i+1))+(j+1)+n;
			int indexP8 = (sqrt(n)*i)+(j+1)+n;
			cube[0] = lst_point[indexP1];
			cube[1] = lst_point[indexP2];
			cube[2] = lst_point[indexP3];
			cube[3] = lst_point[indexP4];
			cube[4] = lst_point[indexP5];
			cube[5] = lst_point[indexP6];
			cube[6] = lst_point[indexP7];
			cube[7] = lst_point[indexP8];
			double liste_val_cube[8] = {lst_point[indexP1].val, lst_point[indexP2].val, lst_point[indexP3].val, lst_point[indexP4].val, lst_point[indexP5].val,lst_point[indexP6].val,lst_point[indexP7].val,lst_point[indexP8].val};
			int * listeSommetsSup = listeSup(liste_val_cube);
			casMC(listeSommetsSup,liste_val_cube, cube);
		}
	}
}

/* MACHING SQUARE */
void case1(double xa, double ya, double sa, double xb, double yb, double sb, double xc, double yc, double sc, double xd, double yd, double sd){
	double x,y;
	if ((sa > isoVal && sb < isoVal && sc < isoVal) || (sa < isoVal && sb > isoVal && sc > isoVal)){
		glBegin(GL_LINES);
		y = getInterpolatePositionY(ya, yc, isoVal, sa, sc);
		glVertex2f(xa, y);
		x = getInterpolatePositionX(xa, xb, isoVal, sa, sb);
		glVertex2f(x, ya);
		glEnd();
	} else if ((sb > isoVal && sa < isoVal && sc < isoVal) || (sb < isoVal && sa > isoVal && sc > isoVal)){
		glBegin(GL_LINES);
		x = getInterpolatePositionX(xa, xb, isoVal, sa, sb);
		glVertex2f(x, yb);
		y = getInterpolatePositionY(yb, yd, isoVal, sb, sd);
		glVertex2f(xb, y);
		glEnd();
	}else if ((sc > isoVal && sb < isoVal && sa < isoVal) || (sc < isoVal && sb > isoVal && sa > isoVal)){
		glBegin(GL_LINES);
		y = getInterpolatePositionY(ya, yc, isoVal, sa, sc);
	    glVertex2f(xa, y);
	    x = getInterpolatePositionX(xc, xd, isoVal, sc, sd);
	    glVertex2f(x, yc);
		glEnd();
	}else {
		glBegin(GL_LINES);
		x = getInterpolatePositionX(xc, xd, isoVal, sc, sd);
		glVertex2f(x, yc);
		y = getInterpolatePositionY(yb, yd, isoVal, sb, sd);
		glVertex2f(xb, y);
		glEnd();
	}	
}

void case2(double xa, double ya, double sa, double xb, double yb, double sb, double xc, double yc, double sc, double xd, double yd, double sd){
	double x,y;
	if ((sa < isoVal && sc < isoVal) || (sb < isoVal && sd < isoVal) || (sa > isoVal && sc > isoVal) || (sb > isoVal && sd > isoVal)){
		glBegin(GL_LINES);
		x = getInterpolatePositionX(xa, xb, isoVal, sa, sb);
	    glVertex2f(x, ya);
	    x = getInterpolatePositionX(xc, xd, isoVal, sc, sd);
	    glVertex2f(x, yc);
		glEnd();
	} else if ((sc < isoVal && sd < isoVal) || (sa < isoVal && sb < isoVal) || (sc > isoVal && sd > isoVal) || sa > isoVal && sb > isoVal){
		glBegin(GL_LINES);
		y = getInterpolatePositionY(ya, yc, isoVal, sa, sc);
	    glVertex2f(xa, y);
	    y = getInterpolatePositionY(yb, yd, isoVal, sb, sd);
	    glVertex2f(xb, y);
		glEnd();
	}
}

void case3(double xa, double ya, double sa, double xb, double yb, double sb, double xc, double yc, double sc, double xd, double yd, double sd){
}

void case4(double xa, double ya, double sa, double xb, double yb, double sb, double xc, double yc, double sc, double xd, double yd, double sd){
	double x,y;
	if (sa < isoVal){
		glBegin(GL_LINES);
		y = getInterpolatePositionY(ya, yc, isoVal, sa, sc);
	    glVertex2f(xa, y);
	    x = getInterpolatePositionX(xc, xd, isoVal, sc, sd);
	    glVertex2f(x, yc);
		glEnd();
		glBegin(GL_LINES);
		y = getInterpolatePositionY(yb, yd, isoVal, sb, sd);
	    glVertex2f(xb, y);
	    x = getInterpolatePositionX(xa, xb, isoVal, sa, sb);
	    glVertex2f(x, ya);
		glEnd();
	} else {
		glBegin(GL_LINES);
		x = getInterpolatePositionX(xa, xb, isoVal, sa, sb);
	    glVertex2f(x, ya);
	    y = getInterpolatePositionY(ya, yc, isoVal, sa, sc);
	    glVertex2f(xa, y);
		glEnd();
		glBegin(GL_LINES);
		x = getInterpolatePositionX(xc, xd, isoVal, sc, sd);
	    glVertex2f(x, yc);
	    y = getInterpolatePositionY(yb, yd, isoVal, sb, sd);
	    glVertex2f(xb, y);
		glEnd();
	}
}

void drawIsoLigne(){
	double nb = sqrt(n);
	for (int i=0; i < nb - 1; i++){
		for (int j=0; j < nb - 1; j++){
			double sa = s[i][j];
			double sb = s[i+1][j];
			double sc = s[i][j+1];
			double sd = s[i+1][j+1];

			double xa = x[i][j];
			double ya = y[i][j];
			double xb = x[i+1][j];
			double yb = y[i+1][j];
			double xc = x[i][j+1];
			double yc = y[i][j+1];
			double xd = x[i+1][j];
			double yd = y[i+1][j+1];

			// case 1
			if ((sa < isoVal && sb < isoVal && sc < isoVal && sd > isoVal) || 
				(sa < isoVal && sb < isoVal && sc > isoVal && sd < isoVal) || 
				(sa < isoVal && sb > isoVal && sc < isoVal && sd < isoVal) || 
				(sa > isoVal && sb < isoVal && sc < isoVal && sd < isoVal) || 

				(sa > isoVal && sb > isoVal && sc > isoVal && sd < isoVal) ||
				(sa > isoVal && sb > isoVal && sc < isoVal && sd > isoVal) ||
				(sa > isoVal && sb < isoVal && sc > isoVal && sd > isoVal) ||
				(sa < isoVal && sb > isoVal && sc > isoVal && sd > isoVal)){
				case1(xa,ya,sa,xb,yb,sb,xc,yc,sc,xd,yd,sd);
			} 
			// case 2
			else if ((sa < isoVal && sb > isoVal && sc < isoVal && sd > isoVal) || 
					(sa > isoVal && sb < isoVal && sc > isoVal && sd < isoVal) || 
					
					(sa < isoVal && sb < isoVal && sc > isoVal && sd > isoVal) ||
					(sa > isoVal && sb > isoVal && sc < isoVal && sd < isoVal)){
				case2(xa,ya,sa,xb,yb,sb,xc,yc,sc,xd,yd,sd);
			}
			// case 3
			else if ((sa < isoVal && sb < isoVal && sc < isoVal && sd < isoVal) || 
				(sa > isoVal && sb > isoVal && sc > isoVal && sd > isoVal)){
				case3(xa,ya,sa,xb,yb,sb,xc,yc,sc,xd,yd,sd);
			}
			// case 4
			else if ((sa < isoVal && sb > isoVal && sc > isoVal && sd < isoVal) || 
				(sa > isoVal && sb < isoVal && sc < isoVal && sd > isoVal)){
				case4(xa,ya,sa,xb,yb,sb,xc,yc,sc,xd,yd,sd);
			}
		}
	}
}

/* AFFICHAGE */
void display(void) {  
	glClear(GL_COLOR_BUFFER_BIT);  
	int const w = glutGet(GLUT_WINDOW_WIDTH);
	int const h = glutGet(GLUT_WINDOW_HEIGHT);
	glLoadIdentity() ;  
	glRotatef(angle_h, 0, 1, 0);
    glRotatef(angle_v, 1, 0, 0);
	glBegin(GL_LINE_LOOP);
    glVertex2f(-0.7,-0.7);
    glVertex2f(0.7,-0.7);
    glVertex2f(0.7,0.7);
    glVertex2f(-0.7,0.7);
	glEnd();
	int ki = 0;
	int kj = 0;
	glRasterPos2f(-0.6,-0.65);
	char char_isoVal[sizeof(double)];
	snprintf(char_isoVal, sizeof(double), "%.2f", isoVal);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, char_isoVal);
 
	/* NOMBRES */
	if (showNumber){
		double nb = sqrt(n);
		for (int i = 0; i < nb; i++){
			for (int j = 0; j < nb; j++){
				double number = s[i][j];
				char output[sizeof(double)];
				snprintf(output, sizeof(double), "%f", number);
				glRectf(x[i][j],y[i][j]-0.03,x[i][j]+0.01,y[i][j]-0.02);
				glRasterPos2f(x[i][j],y[i][j]);
			    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10, output);
			}
		}
	}
	if (showNumber3D){
		int k=0;
		for (int z = 0; z < 2; z++){
			for (int i = 0; i < sqrt(n); i++){
				for (int j = 0; j < sqrt(n); j++){
					double number = lst_point[k].val;
					char output[sizeof(double)];
					snprintf(output, sizeof(double), "%.2f", number);
					int iCoord = lst_point[k].i;
					int jCoord = lst_point[k].j;
					glRectf(x[iCoord][jCoord],y[iCoord][jCoord]-0.03,x[iCoord][jCoord]+0.01,y[iCoord][jCoord]-0.02);
					if (lst_point[k].z == 0){
						glRasterPos3f(x[(int)lst_point[k].i][(int)lst_point[k].j],y[(int)lst_point[k].i][(int)lst_point[k].j],0.2);
					} else {
						glRasterPos3f(x[(int)lst_point[k].i][(int)lst_point[k].j],y[(int)lst_point[k].i][(int)lst_point[k].j],0.4);
					}
					glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, output);	
					k++;
				}
			}
		}
	}
	if (showCube){
		double nb = sqrt(n);
		for (int i = 0; i < nb -1; i++){
			for (int j = 0; j < nb -1; j++){
		  		glBegin(GL_LINE_LOOP);
		  		glVertex3f( x[i+1][j+1], y[i+1][j+1], 0.4  ); 
		  		glVertex3f( x[i+1][j], y[i+1][j], 0.4  ); 
				glVertex3f( x[i][j], y[i][j], 0.4  ); 
				glVertex3f( x[i][j+1], y[i][j+1], 0.4  ); 
				glEnd();
				glBegin(GL_LINE_LOOP);
				glVertex3f( x[i+1][j+1], y[i+1][j+1], 0.2  ); 
		  		glVertex3f( x[i+1][j], y[i+1][j], 0.2  ); 
				glVertex3f( x[i][j], y[i][j], 0.2  ); 
				glVertex3f( x[i][j+1], y[i][j+1], 0.2  ); 
				glEnd();
				glBegin(GL_LINE_LOOP);
				glVertex3f( x[i+1][j+1], y[i+1][j+1], 0.2  ); 
				glVertex3f( x[i+1][j], y[i+1][j], 0.2  );
				glVertex3f( x[i+1][j], y[i+1][j], 0.4  ); 
				glVertex3f( x[i+1][j+1], y[i+1][j+1], 0.4  ); 
				glEnd();
				glBegin(GL_LINE_LOOP);
				glVertex3f( x[i][j+1], y[i][j+1], 0.2  ); 
				glVertex3f( x[i][j], y[i][j], 0.2  ); 
				glVertex3f( x[i][j], y[i][j], 0.4  );
				glVertex3f( x[i][j+1], y[i][j+1], 0.4  ); 
				glEnd();
			}
		}	
	}
	drawIsoLigne(isoVal);
	maching_cube();
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y){
	switch(key){
		case 43:
			if (isoVal < (maxVal-0.01)){
				isoVal += 0.01;
			}
			break;
		case 45:
			if (isoVal > (minVal+0.01)){
				isoVal -= 0.01;
			}
			break;
		case 27:
			exit(1);
	}
	glutPostRedisplay();
}

void motion(int x, int y) {  
	if (prev_x!=-1) {    
		angle_h += x-prev_x;   
		glutPostRedisplay();  
	}  
	prev_x = x;

	if (prev_y!=-1) {    
		angle_v += y-prev_y;   
		glutPostRedisplay();  
	}  
	prev_y = y;
}


int main(int argc, char ** argv){
	charger(FILE_NAME2D);
	charger3d(FILE_NAME3D);
	glutInit(&argc, argv);  
	glutInitWindowSize(1080, 720);
	WindowName = glutCreateWindow("Windows");
    glutDisplayFunc(display);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);

	drawIsoLigne();
	maching_cube();
	glutMainLoop();

	return EXIT_SUCCESS;
}