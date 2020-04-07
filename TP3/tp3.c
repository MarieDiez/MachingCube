#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdbool.h>

int WindowName;
GLfloat angle_h = 0.0;
GLfloat angle_v = 0.0;
int MAX_POINT = 100;
int MIN_POINT = 3;
int prev_x = -1;
int prev_y = -1;
bool drawVecteur = true;
bool showPoint = false;
bool showCube = false;
int n = 3;
int config = 1;
char * fonction = "euler";
typedef struct _Point{
	double x;
	double y;
	double z;
} Point;
Point* lst_point;

void drawLineInCub(Point p1, Point p2);

bool inCube(Point p){
	if((p.x >= -0.5 && p.x <= 0.5) &&
	   (p.y >= -0.5 && p.y <= 0.5)&& 
	   (p.z >= -0.5 && p.z <= 0.5)){
		return true;
	}
	return false;
}

bool rangeKutta(Point * p, int deltaT){
	Point pNext;
	double Vx = -3 + 6*p->x -4*p->x*(p->y+1) -4*p->z;
	double Vy = 12*p->x -4*p->x*p->x -12*p->z + 4*p->z*p->z;
	double Vz = 3 + 4*p->x -4*p->x*(p->y+1) -6*p->z + 4*(p->y+1)*p->z;
	Point pE;
	pE.x = p->x + (Vx/100) * deltaT;
	pE.y = p->y + (Vy/100) * deltaT;
	pE.z = p->z + (Vz/100) * deltaT;

	double Vx_E = -3 + 6*pE.x -4*pE.x*(pE.y+1) -4*pE.z;
	double Vy_E = 12*pE.x -4*pE.x*pE.x -12*pE.z + 4*pE.z*pE.z;
	double Vz_E = 3 + 4*pE.x -4*pE.x*(pE.y+1) -6*pE.z + 4*(pE.y+1)*pE.z;

	pNext.x = p->x + (Vx/100 + Vx_E/200) * deltaT;
	pNext.y = p->y + (Vy/100 + Vy_E/200) * deltaT;
	pNext.z = p->z + (Vz/100 + Vz_E/200) * deltaT;

	if (inCube(pNext)){
		glColor3f(0,1,0);
		drawLineInCub(*p,pNext);
		p->x = pNext.x;
		p->y = pNext.y;
		p->z = pNext.z;
		return true;
	} else {
		drawLineInCub(*p,pNext);
		return false;
	}
}

bool euler(Point * p, int deltaT){
	Point pNext;
	double Vx = -3 + 6*p->x -4*p->x*(p->y+1) -4*p->z;
	double Vy = 12*p->x -4*p->x*p->x -12*p->z + 4*p->z*p->z;
	double Vz = 3 + 4*p->x -4*p->x*(p->y+1) -6*p->z + 4*(p->y+1)*p->z;
	pNext.x = p->x + (Vx/100) * deltaT;
	pNext.y = p->y + (Vy/100) * deltaT;
	pNext.z = p->z + (Vz/100) * deltaT;

	if (inCube(pNext)){
		glColor3f(0,1,0);
		drawLineInCub(*p,pNext);
		p->x = pNext.x;
		p->y = pNext.y;
		p->z = pNext.z;
		return true;
	} else {
		drawLineInCub(*p,pNext);
		return false;
	}
}

void pathLine(){
	if(config == 1){
		Point p1;
		p1.x = 0.1;
		p1.y = 0.1;
		p1.z = -0.1;
		Point p2;
		p2.x = 0;
		p2.y = 0;
		p2.z = 0;
		Point p3;
		p3.x = 0.2;
		p3.y = 0.2;
		p3.z = 0.1;
		lst_point[0] = p1;
		lst_point[1] = p2;
		lst_point[2] = p3;
	} else if(config == 2){
		Point p1;
		p1.x = 0.3;
		p1.y = 0.2;
		p1.z = -0.1;
		Point p2;
		p2.x = 0.4;
		p2.y = -0.2;
		p2.z = 0;
		Point p3;
		p3.x = -0.3;
		p3.y = 0;
		p3.z = -0.4;
		Point p4;
		p4.x = 0;
		p4.y = 0;
		p4.z = 0;
		Point p5;
		p5.x = 0.45;
		p5.y = -0.2;
		p5.z = -0.3;
		lst_point[0] = p1;
		lst_point[1] = p2;
		lst_point[2] = p3;
		lst_point[3] = p4;
		lst_point[4] = p5;
	} else {
		Point p1;
		p1.x = 0.4;
		p1.y = -0.4;
		p1.z = -0.4;
		Point p2;
		p2.x = -0.2;
		p2.y = 0.1;
		p2.z = -0.3;
		Point p3;
		p3.x = 0.2;
		p3.y = -0.1;
		p3.z = -0.4;
		lst_point[0] = p1;
		lst_point[1] = p2;
		lst_point[2] = p3;
	}
	
	
	int temps = 100;
	double i = 0;
	bool stop = false;	
	for (int j = 0; j < n; j++){
		while(stop == false){
			if (fonction == "euler"){
				if(euler(&lst_point[j],i) == false){
					stop = true;
				}
			} else {
				if(rangeKutta(&lst_point[j],i) == false){
					stop = true;
				}
			}
			i+=0.001;
		}
		stop = false;		
	}
}

Point finVecteur(Point ptsDebut){
	Point ptsFin;
	double x = ptsDebut.x;
	double y = ptsDebut.y;
	double z = ptsDebut.z;
	double Vx = -3 + 6*x -4*x*(y+1) -4*z;
	double Vy = 12*x -4*x*x -12*z + 4*z*z;
	double Vz = 3 + 4*x -4*x*(y+1) -6*z + 4*(y+1)*z;
	ptsFin.x = x + Vx/100;
	ptsFin.y = y + Vy/100;
	ptsFin.z = z + Vz/100;
	return ptsFin;
}

void drawLineInCub(Point p1, Point p2){
	glBegin(GL_LINES);
	glVertex3f(p1.x,p1.y,p1.z); 
	if (p2.x > 0.5){
		p2.x = 0.5;
	}
	if (p2.x < -0.5){
		p2.x = -0.5;
	}
	if (p2.y > 0.5){
		p2.y = 0.5;
	}
	if (p2.y < -0.5){
		p2.y = -0.5;
	}
	if (p2.z > 0.5){
		p2.z = 0.5;
	}
	if (p2.z < -0.5){
		p2.z = -0.5;
	}
	glVertex3f(p2.x,p2.y,p2.z);
	glEnd();
}

void drawCubeVecteur(){
	glColor3f(0.3,0.3,0.3);
	for (double i = -0.5; i <= 0.5; i+=0.1){
		for (double j = -0.5; j <= 0.5; j+=0.1){
			for (double k = -0.5; k <= 0.5; k+=0.1){
				Point p1;
				p1.x = i;
				p1.y = j;
				p1.z = k;
				Point p2 = finVecteur(p1);
				drawLineInCub(p1,p2);
			}
		}
	}
}
void rectangle3D(double i, double j, double k){
	glColor3f(1,1,1);
	glBegin(GL_LINE_LOOP);
	glVertex3f( -i, j, -k); 
	glVertex3f( -i, -j, -k); 
	glVertex3f( i, -j, -k); 
	glVertex3f( i, j, -k); 
	glEnd();
	// haut
	glBegin(GL_LINE_LOOP);
	glVertex3f( -i, j, k); 
	glVertex3f( -i, -j, k); 
	glVertex3f( i, -j, k); 
	glVertex3f( i, j, k); 
	glEnd();
	// gauche
	glBegin(GL_LINE_LOOP);
	glVertex3f( -i, j, k); 
	glVertex3f( -i, -j, k); 
	glVertex3f( -i, -j, -k); 
	glVertex3f( -i, j, -k); 
	glEnd();

	// droite
	glBegin(GL_LINE_LOOP);
	glVertex3f( i, j, k); 
	glVertex3f( i, -j, k); 
	glVertex3f( i, -j, -k); 
	glVertex3f( i, j, -k); 
	glEnd();
}

/* AFFICHAGE */
void display(void) {  
	glClear(GL_COLOR_BUFFER_BIT); 
	glLoadIdentity() ;  
	glRotatef(angle_h, 0, 1, 0);
    glRotatef(angle_v, 1, 0, 0);
		
	glRasterPos2f(0,0);
	
	if (drawVecteur){
 		drawCubeVecteur();
 	}
	pathLine();

	// dimension 1x1x1 et non 100 pour visualisation
	rectangle3D(0.5,0.5,0.5);

	if (showPoint){
 		// cube de 0.1
		for (double i = -0.5; i < 0.5; i+=0.1){
			for (double j = -0.5; j < 0.5; j+=0.1){
				for (double k = -0.5; k < 0.5; k+=0.1){
					glBegin(GL_POINTS);
					glVertex3f(i,j,k);
					glEnd();
				}
			}
		}
	}
 
	if (showCube){
		for (double i = -0.5; i < 0.5; i+=0.1){
			for (double j = -0.5; j < 0.5; j+=0.1){
				for (double k = -0.5; k < 0.5; k+=0.1){
					rectangle3D(i,j,k);
				}
			}	
		}

 	}
 	
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y){
	switch(key){
		case 49: 
			config = 1;
			n = 3;
			lst_point = realloc(lst_point,n*sizeof(Point));
			pathLine();
			break;
		case 50:
			config = 2;
			n = 5;
			lst_point = realloc(lst_point,n*sizeof(Point));
			pathLine();
			break;
		case 51:
			config = 3;
			n = 3;
			lst_point = realloc(lst_point,n*sizeof(Point));
			pathLine();
			break;
		case 101:
			fonction = "euler";
			pathLine();
			break;
		case 114:
			fonction = "rangeKutta";
			pathLine();
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
	lst_point = malloc(n*sizeof(Point));
	glutInit(&argc, argv);  
	glutInitWindowSize(1080, 720);
	WindowName = glutCreateWindow("Windows");
    glutDisplayFunc(display);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return EXIT_SUCCESS;
}