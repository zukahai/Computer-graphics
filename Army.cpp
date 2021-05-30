#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include <windows.h>
#include <ctime>

using namespace std;

#define M_PI    3.14159265358979323846
#define gt 0.1
int goc1 = 90, goc2 = 90;
int luc = 0;
float R = 0.1; 
float T = 0;
float x3 = 1.5, y3 = -0.1;
float x2 = -1.5, y2 = -0.1;
float xB, yB;
int turn = 1;
int hp1 = 100, hp2 = 100;
int dc = 100;
bool startLuc = false;
bool startBullet = false;
bool rig = true;
bool win = false;
int stepLuc = 1;
float vX, vY;
void army();
void updateScreen();
void sleep(int i);
void doanThang(float x1, float y1, float z1, float x2, float y2, float z2);
void veNhanVat(float x2, float y2, int goc, int hp);
void inChu();
void inChu2();
void hinhTron(float R, float X, float Y);
float gocToPi(int goc);
void bullet();
void thanhLuc();
void hp(float x, float y, int h);
string intToStr(int n);
void diChuyen();

void doanThang(float x1, float y1, float z1, float x2, float y2, float z2){
	 float a, b, c;
	 a = x2 - x1;
	 b = y2 - y1;
	 c = z2 - z1;
	 glBegin(GL_POINTS);
	 for (float t = 0; t <= 1; t += 0.001 / (sqrt(a*a + b*b + c*c))){
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
	string s = "Phan Duc Hai - Nguyen Van Nam"; 
	void * font = GLUT_BITMAP_TIMES_ROMAN_24;
	for (string::iterator i = s.begin(); i != s.end(); ++i) { 
	    char c = *i; 
	    glutBitmapCharacter(font, c); 
	}
	glRasterPos2f(0.5, 0.95);
	if (turn % 2)
		s = "Power: " + intToStr(luc) + "     Angle: " + intToStr(min(goc1, 180 - goc1)) + "   Turn: P" + intToStr(2 - turn % 2);
	else
		s = "Power: " + intToStr(luc) + "     Angle: " + intToStr(min(goc2, 180 - goc2)) + "   Turn: P" + intToStr(2 - turn % 2);
	for (string::iterator i = s.begin(); i != s.end(); ++i) { 
	    char c = *i; 
	    glutBitmapCharacter(font, c); 
	}
	glMatrixMode(GL_MODELVIEW); 
	glPopMatrix(); 
	
	glMatrixMode(GL_PROJECTION); 
	glPopMatrix();
}
void inChu2(){
	glMatrixMode(GL_PROJECTION); 
	glPushMatrix(); 
	glLoadIdentity(); 
	gluOrtho2D(0.0, 1, 0.0, 1); 
	
	glMatrixMode(GL_MODELVIEW); 
	glPushMatrix(); 
	glLoadIdentity(); 
	glColor3f(1.0, 1.0, 0.0);
	glRasterPos2f(0.4, 0.5); 
	string s = "Nhan phim 'H' de choi lai!"; 
	void * font = GLUT_BITMAP_TIMES_ROMAN_24;
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
		for (float x = 0; x <= M_PI / 2; x += 0.02 * R){
			glVertex3f(X + R * cos(x), Y + R * sin(x),0);
		}
		glVertex3f(X, Y, 0);
	glEnd();
	glBegin(GL_POLYGON);
		for (float x = 0; x <= M_PI / 2; x += 0.02 * R){
			glVertex3f(X - R * cos(x), Y + R * sin(x), 0);
		}
		glVertex3f(X, Y, 0);
	glEnd();
	glBegin(GL_POLYGON);
		for (float x = 0; x <= M_PI / 2; x += 0.02 * R){
			glVertex3f(X + R * cos(x), Y - R * sin(x),0);
		}
		glVertex3f(X, Y, 0);
	glEnd();
	glBegin(GL_POLYGON);
		for (float x = 0; x <= M_PI / 2; x += 0.02 * R){
			glVertex3f(X - R * cos(x), Y - R * sin(x),0);
		}
		glVertex3f(X, Y, 0);
	glEnd();
}
void veNhanVat(float x2, float y2, int goc, int h){
	if (h > 0){
		glColor3f(1, 0, 1);
		hinhTron(R, x2, y2);
		glColor3f(0, 1, 0);
		hinhTron(R / 1.5, x2, y2 + R + R / 1.5);
		glColor3f(1, 1, 0);
		doanThang(x2, y2, 0, x2 + 1.5 * R * cos(gocToPi(goc)), y2 + 1.5 * R * sin(gocToPi(goc)), 0);
		hinhTron(R / 5,x2 + 1.5 * R * cos(gocToPi(goc)), y2 + 1.5 * R * sin(gocToPi(goc)));
		hp(x2, y2 + 3 * R, h);	
	} else
		win = true;
}

void hp(float x, float y, int h){
	float k = (100 - h) * 4 * R / 100;
	glColor3f (0.0, 1.0, 0.0); 
	glBegin(GL_POLYGON); 
		glVertex3f (x - 2 * R, y - R / 4, 0); 
		glVertex3f (x + 2 * R - k, y - R / 4, 0);
		glVertex3f (x + 2 * R - k, y + R / 4, 0);
		glVertex3f (x - 2 * R, y + R / 4, 0);
	glEnd();
	glColor3f (1.0, 0.0, 0.0);
	doanThang(x -2 * R, y - R / 4, 0,  x + 2 * R, y - R / 4, 0);
	doanThang(x + 2 * R, y - R / 4, 0, x + 2 * R, y + R / 4, 0);
	doanThang(x + 2 * R, y + R / 4, 0, x - 2 * R, y + R / 4, 0);
	doanThang(x - 2 * R, y + R / 4, 0, x -2 * R, y - R / 4, 0);
}

void army(){
	if (!win){
		veNhanVat(x2, y2, goc1, hp1);
		veNhanVat(x3, y3, goc2, hp2);
		if (startLuc){
				luc += stepLuc;
			if (luc > 99 || luc < 1)
				stepLuc = - stepLuc;
		}
		if (startBullet)
			bullet();
		sleep(5);
		thanhLuc();
		diChuyen();
		inChu();
	} else {
		goc1 = rand() % 91;
		goc2 = rand() % 90 + 90;
		luc = 0;
		x3 = 1.5, y3 = -0.1;
		x2 = -1.5, y2 = -0.1;
		turn = 1;
		hp1 = 100, hp2 = 100;
		dc = 100;
		startLuc = false;
		startBullet = false;
		rig = true;
		inChu2();
	}
}

void bullet(){
	if (startBullet){
		T += 0.03;
		xB += vX;
		yB += (vY - gt * T);
		hinhTron(0.05, xB, yB);	
		if (turn % 2){
			if (sqrt(fabs(xB - x2) * fabs(xB - x2) + fabs(yB - y2) * fabs(yB - y2)) < R){
				startBullet = false;
				hp1 -= 15;	
			}
			if (sqrt(fabs(xB - x2) * fabs(xB - x2) + fabs(yB - (y2 + R + R / 1.5)) * fabs(yB - (y2 + R + R / 1.5))) < R / 1.5){
				startBullet = false;
				hp1 -= 30;
			}	
		} else {
			if (sqrt(fabs(xB - x3) * fabs(xB - x3) + fabs(yB - y3) * fabs(yB - y3)) < R){
				startBullet = false;
				hp2 -= 15;
			}
			if (sqrt(fabs(xB - x3) * fabs(xB - x3) + fabs(yB - (y3 + R + R / 1.5)) * fabs(yB - (y3 + R + R / 1.5))) < R / 1.5){
				startBullet = false;
				hp2 -= 30;
			}
		}		
	}
}

void diChuyen(){
	float kCach = 1.9 - (dc / 100.0) * 1.9;
	glColor3f (0.0, 0.0, 1.0); 
	glBegin(GL_POLYGON); 
		glVertex3f (0, -0.8 + 1.5 * R, 0); 
		glVertex3f (1.9 - kCach, -0.8 + 1.5 * R, 0);
		glVertex3f ( 1.9 - kCach, -0.9 + 1.5 * R, 0);
		glVertex3f (0, -0.9 + 1.5 * R, 0);
	glEnd();
	glColor3f (1.0, 0.0, 0.0);
	doanThang(0, -0.8 + 1.5 * R, 0, 1.9, -0.8 + 1.5 * R, 0);
	doanThang(0, -0.9 + 1.5 * R, 0, 1.9, -0.9 + 1.5 * R, 0);
	doanThang(0, -0.8 + 1.5 * R, 0, 0, -0.9 + 1.5 * R, 0);
	doanThang(1.9, -0.8 + 1.5 * R, 0, 1.9, -0.9 + 1.5 * R, 0);
}

void thanhLuc(){
	float kCach = 3.8 - (luc / 100.0) * 3.8;
	glColor3f (0.0, 1.0, 0.0); 
	glBegin(GL_POLYGON); 
		glVertex3f (-1.9, -0.8, 0); 
		glVertex3f (1.9 - kCach, -0.8, 0);
		glVertex3f (1.9 - kCach, -0.9, 0);
		glVertex3f (-1.9, -0.9, 0);
	glEnd();
	glColor3f (1.0, 0.0, 0.0);
	doanThang(-1.9, -0.8, 0, 1.9, -0.8, 0);
	doanThang(-1.9, -0.9, 0, 1.9, -0.9, 0);
	doanThang(-1.9, -0.8, 0, -1.9, -0.9, 0);
	doanThang(1.9, -0.8, 0, 1.9, -0.9, 0);
}
void sleep(int i){
	for(int time = 0 ; time < i ;time++){
		for(int count = 0 ; count < 2000000 ; count++);
	}
}
void mydisplay(){
	army();
	updateScreen();
}
void updateScreen(void){
	glutPostRedisplay();
	glFlush();
	glClear(GL_COLOR_BUFFER_BIT);
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
void normalKeyHandler (unsigned char key, int x, int y){
	if (turn % 2){
		if(key == 's' && dc > 0)
			y2 -= 0.02;
		else if(key == 'w' && dc > 0)
			y2 += 0.02;
		else if(key == 'd'){
			rig = true;
			if (goc1 > 90)
				goc1 = 180 - goc1;
			if (dc > 0)
				x2 += 0.02;
		} else if(key == 'a'){
			rig = false;
			if (goc1 < 90)
				goc1 = 180 - goc1;
			if (dc > 0)
				x2 -= 0.02;
		}
	} else {
		if(key == 's' && dc > 0)
			y3 -= 0.02;
		else if(key == 'w' && dc > 0)
			y3 += 0.02;
		else if(key == 'd'){
			rig = true;
			if (goc2 > 90)
				goc2 = 180 - goc2;
			if (dc > 0)
				x3 += 0.02;
		} else if(key == 'a'){
			rig = false;
			if (goc2 < 90)
				goc2 = 180 - goc2;
			if (dc > 0)
				x3 -= 0.02;
		}
	}
	if (dc > 0 && (key == 'a' || key == 's' || key == 'd' || key == 'w'))
		dc -= 2;	
	if (key == 'h')
		win = false;
//	cout << key << endl;
 	glutPostRedisplay();
}
void myMouseFunc(int button, int state, int x, int y){
	
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		luc = 0;
		stepLuc = 1;
		dc = 100;
		startLuc = true;
	}
	else if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		startLuc = false;
		if (turn % 2){
			xB = x2 + 1.5 * R * cos(gocToPi(goc1));
			yB = y2 + 1.5 * R * sin(gocToPi(goc1));
			vX = luc * cos(gocToPi(goc1)) / 500.0;
			vY = luc * sin(gocToPi(goc1)) / 500.0;
		} else {
			xB = x3 + 1.5 * R * cos(gocToPi(goc2));
			yB = y3 + 1.5 * R * sin(gocToPi(goc2));
			vX = luc * cos(gocToPi(goc2)) / 500.0;
			vY = luc * sin(gocToPi(goc2)) / 500.0;
		}
		startBullet = true;
		T = 0;
		turn ++;
		// y = A^x^2 + B * x + c;
	}
}
float gocToPi(int goc){
	return M_PI * goc / 180.0;
}
void PhimNhanXoayHinh(int key, int x, int y){
	if (turn % 2){
		if(key == GLUT_KEY_DOWN)
		if (goc1 > 0)
		if (rig)	
			goc1 -= 1;
		else
			goc1 += 1;
		if(key == GLUT_KEY_UP)
			if (rig)
				goc1 += 1;
			else
				goc1 -= 1;			
		if(key == GLUT_KEY_RIGHT){
			rig = true;
			if (goc1 > 90)
				goc1 = 180 - goc1;
			if (dc > 0){
				x2 += 0.02;
				dc -= 2;	
			}	
		}
		if(key == GLUT_KEY_LEFT){
			rig = false;
			if (goc1 < 90)
				goc1 = 180 - goc1;
			if (dc > 0){
				x2 -= 0.02;
				dc -= 2;	
			}
		}	
	} else {
		if(key == GLUT_KEY_DOWN)
		if (goc2 > 0)
		if (rig)	
			goc2 -= 1;
		else
			goc2 += 1;
		if(key == GLUT_KEY_UP)
			if (rig)
				goc2 += 1;
			else
				goc2 -= 1;			
		if(key == GLUT_KEY_RIGHT && dc > 0){
			rig = true;
			if (goc2 > 90)
				goc2 = 180 - goc2;
			x3 += 0.02;
			dc -= 2;
		}
		if(key == GLUT_KEY_LEFT && dc > 0){
			rig = false;
			if (goc2 < 90)
				goc2 = 180 - goc2;
			x3 -= 0.02;
			dc -= 2;
		}	
	}
//	cout << key << endl;
 	glutPostRedisplay();
}

void chuot(int x, int y){
	
}
int main(int argc, char** argv){
	srand(time(NULL));
	goc1 = rand() % 91;
	goc2 = rand() % 90 + 90;
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize (1200, 600);
	glutInitWindowPosition (0, 0);
	glutCreateWindow("Phan Duc Hai");
	glutMouseFunc(myMouseFunc);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glOrtho(-2, 2, -1, 1, -10, 10);
	glutKeyboardFunc(normalKeyHandler);
	glutDisplayFunc(mydisplay);
	glutPassiveMotionFunc(chuot);
	glutMouseFunc(myMouseFunc);
	glutSpecialFunc(PhimNhanXoayHinh);
	glutMainLoop();
}
