#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include <windows.h>
#include <ctime>

using namespace std;
float R = 0.1;
float x2 = 0.0, y2 = 0.0;
float vt = 0.01;
float xCot = 1.1 + R, yCot = 0;
float vtCot = 0.01;
int soCot = 0;
int score = 0;
bool start = false;
bool die = false;
bool checkScore = true;

void updateScreen();
void sleep(int i);
void doanThang(float x1, float y1, float z1, float x2, float y2, float z2);
void ve();
void veCot(float xx, float yy);
void inChu();
void hinhTron(float R, float X, float Y);
void conChim();

string intToStr(int n);

void doanThang(float x1, float y1, float z1, float x2, float y2, float z2){
	 float a, b, c;
	 a = x2 - x1;
	 b = y2 - y1;
	 c = z2 - z1;
	 glBegin(GL_POINTS);
	 for (float t = 0; t <= 1; t += 0.1){
	 	glVertex3f(x1 + a * t, y1 + b * t, z1 + c * t);
	 }
	 glEnd();
}
void inChu(){
	glMatrixMode(GL_PROJECTION); 
	glPushMatrix(); 
	glLoadIdentity(); 
	gluOrtho2D(0.0, 1, 0.0, 1); 
	
	glMatrixMode(GL_MODELVIEW); 
	glPushMatrix(); 
	glLoadIdentity(); 
	glColor3f(1.0, 1.0, 0.0);
	glRasterPos2f(0.05, 0.95); 
	string s = "Score: " + intToStr(score); 
	void * font = GLUT_BITMAP_TIMES_ROMAN_24;
	for (string::iterator i = s.begin(); i != s.end(); ++i) { 
	    char c = *i; 
	    glutBitmapCharacter(font, c); 
	}
	glRasterPos2f(0.5, 0.95);
	s = "Phan Duc Hai";
	for (string::iterator i = s.begin(); i != s.end(); ++i) { 
	    char c = *i; 
	    glutBitmapCharacter(font, c); 
	}
	glMatrixMode(GL_MODELVIEW); 
	glPopMatrix(); 
	
	glMatrixMode(GL_PROJECTION); 
	glPopMatrix();
}
void hinhTron(float R, float X, float Y){
	glBegin(GL_POLYGON);
		for (float x = 0; x <= M_PI / 2; x += 0.02){
			glVertex3f(X + R * cos(x), Y + R * sin(x),0);
		}
		glVertex3f(X, Y, 0);
	glEnd();
	glBegin(GL_POLYGON);
		for (float x = 0; x <= M_PI / 2; x += 0.02){
			glVertex3f(X - R * cos(x), Y + R * sin(x), 0);
		}
		glVertex3f(X, Y, 0);
	glEnd();
	glBegin(GL_POLYGON);
		for (float x = 0; x <= M_PI / 2; x += 0.02){
			glVertex3f(X + R * cos(x), Y - R * sin(x),0);
		}
		glVertex3f(X, Y, 0);
	glEnd();
	glBegin(GL_POLYGON);
		for (float x = 0; x <= M_PI / 2; x += 0.02){
			glVertex3f(X - R * cos(x), Y - R * sin(x),0);
		}
		glVertex3f(X, Y, 0);
	glEnd();
}
void ve(){
	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_POLYGON); 
		glVertex3f (x2 + R - R / 10, y2 - R / 3, 0); 
		glVertex3f (x2 + 2 * R, y2, 0);
		glVertex3f (x2 + R - R / 10, y2 + R / 3, 0);
	glEnd();
	glColor3f(1.0, 0.0, 0.0);
	hinhTron(R, x2, y2);
	glColor3f(0.0, 0.0, 1.0);
	hinhTron(R / 3, x2 + R / 3, y2 + R / 3);
	veCot(xCot, yCot);
}
void veCot(float xx, float yy){
	glColor3f (0.0, 1.0, 0.0); 
	glBegin(GL_POLYGON); 
		glVertex3f (xx - R, yy, 0); 
		glVertex3f (xx + R, yy, 0);
		glVertex3f (xx + R, -2, 0);
		glVertex3f (xx - R, -2, 0);
	glEnd();
	glBegin(GL_POLYGON);
		glVertex3f (xx - R, yy + 7 * R, 0);
		glVertex3f (xx + R, yy + 7 * R, 0);
		glVertex3f (xx + R, 2, 0);
		glVertex3f (xx - R, 2, 0); 
	glEnd();
}
void conChim(){
	ve();
	if (!die && start){
		if (soCot == 0){
			srand(time(NULL));
			yCot = (((rand() % 5) - (rand() % 6)) / 10.0);
			soCot ++;
			checkScore = true;
		}
		y2 -= vt;
		vt *= 1.15;
		xCot -= vtCot;
		if (xCot < -1.1 - R){
			soCot --;
			xCot = 1.1 + R;
		}
		if (fabs(xCot - x2) <= 2 * R && ((yCot >= y2) || yCot + 6 * R <= y2)){
			die = true;
		}
		if (fabs(xCot - x2) <= R && (fabs(yCot - y2) <= R || fabs(yCot + 6 * R - y2) <= 0)){
			die = true;
		}
		if (y2 >= 0.9 || y2 <= -0.9){
			die  = true;
		}
		if (x2 > xCot && checkScore){
			checkScore = false;
			score++;
		}	
		sleep(5);
	}
}

void sleep(int i){
	for(int time = 0 ; time < i ;time++){
		for(int count = 0 ; count < 2000000 ; count++);
	}
}
void mydisplay(){
	conChim();
	inChu();
	updateScreen();
}
void updateScreen(void){
	glutPostRedisplay();
	glFlush();
	glClear(GL_COLOR_BUFFER_BIT);
}
void NormalKeyHandler (unsigned char key, int x, int y){
	start = true;
	if (!die){
		vt = 0.01;
	    if (key == 'w')
			y2 += 0.2;
	    updateScreen();
	}
}
string intToStr(int n){
	string kq = "";
	while (n > 0){
		kq = char(n % 10 + '0') + kq;
		n /= 10;
	}
	if (kq.length() == 0)
		kq = "0";
	return kq;
}
void myMouseFunc(int button, int state, int x, int y){
	start = true;
	if (!die){
		vt = 0.01;
		if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			y2 += 0.2;
			glutPostRedisplay();
		}
	}
}
int main(int argc, char** argv){
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize (500, 600);
	glutInitWindowPosition (180, 100);
	glutCreateWindow("Phan Duc Hai - Flappy bird.");
	glutKeyboardFunc(NormalKeyHandler);
	glutMouseFunc(myMouseFunc);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glutDisplayFunc(mydisplay);
	glutMainLoop();
}
