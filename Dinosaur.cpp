#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include <windows.h>
#include <ctime>
#include <fstream>

using namespace std;

#define M_PI    3.14159265358979323846
#define gt 0.0005
int n = 2;
float vatCan[10000][3];
int diem = 0;
float h = 0.15, w = 0.08;
float yKl = -0.5;
float vY = 0;
float Xduong = -2;
float VVatCan = 0.01;
bool start = false;
bool lose = false;
float t = 0;
void khungLong();
void updateScreen();
void sleep(int i);
void doanThang(float x1, float y1, float z1, float x2, float y2, float z2);
void veKhungLong(float y);
void nhay();
string intToStr(int n);
int layDiemCao();
void ghiDiemCao(int diem);
void khoiTaoVatCan();
void ve(float, float, float);
void veVatCan();
void diemCao();
void veDuong();
void khoiTaoVatCan(){
	srand(time(NULL));
	for (int i = 0; i < n; i++)
		vatCan[i][0] = (rand() % 300 + 250) / 100.0;
	for (int i = 0; i < n; i++){
		int k = rand() % 3;
		if (k == 0)
			vatCan[i][1] = -0.5;
		if (k == 1)
			vatCan[i][1] = -0.1;
		if (k == 2)
			vatCan[i][1] = -0.4;
	}
	for (int i = 0; i < n; i++)
		if (vatCan[i][1] == -0.5){
			if (rand() % 2)
				vatCan[i][2] = 0.18;
			else
				vatCan[i][2] = 0.28;	
		}
		else
			vatCan[i][2] = 0.08;
}

void diemCao(){
	glMatrixMode(GL_PROJECTION); 
	glPushMatrix(); 
	glLoadIdentity(); 
	gluOrtho2D(0.0, 1, 0.0, 1); 
	
	glMatrixMode(GL_MODELVIEW); 
	glPushMatrix(); 
	glLoadIdentity(); 
	glColor3f(1.0, 1.0, 0.0);
	glRasterPos2f(0.55, 0.95); 
	string s = "Diem: " + intToStr(diem) + "   Diem cao: " + intToStr(layDiemCao());
	if (!lose)
		diem++;
	if (layDiemCao() < diem)
		ghiDiemCao(diem);
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

void veDuong(){
	doanThang(-3, -0.51, 0, 3, -0.51, 0);
	for (float i = Xduong; i <= 2.5; i += 0.08)
		doanThang(i, -0.51, 0, i - 0.1, -0.51 - 0.1, 0);
	Xduong -= (diem / 1000 + 2) * VVatCan;
	while (Xduong < -2.5 - 0.08)
		Xduong += 0.8;
//	cout << Xduong << endl;
}

void nhay(){
	if (!lose){
		yKl += vY;
		vY = vY - gt * t;
		t += 0.3;
		if (yKl < -0.5){
			yKl = -0.5;
			start = false;
		}
	}
}

void ve(float a, float b, float c){
	glColor3f(1, 0, 0);
	glBegin(GL_POLYGON); 
		glVertex3f (a, b, 0); 
		glVertex3f (a + w, b, 0);
		glVertex3f (a + w, b + c, 0);
		glVertex3f (a, b + c, 0);
	glEnd();
}

void veVatCan(){
	for (int i = 0; i < n; i++)
		ve(vatCan[i][0], vatCan[i][1], vatCan[i][2]);
	for (int i = 0; i < n; i++)
		vatCan[i][0] -= (diem / 1000 + 2) * VVatCan;
	for (int i = 0; i < n; i++){
		if (vatCan[i][0] >= -1 - w && vatCan[i][0] <= -1 + w){
		//	cout << yKl << " " << vatCan[i][1] << " " << vatCan[i][1] + vatCan[i][2] << " " << (yKl >= vatCan[i][1] && yKl <= (vatCan[i][1] + vatCan[i][2])) << endl;
			if ((yKl >= vatCan[i][1] && yKl <= (vatCan[i][1] + vatCan[i][2])) || (yKl + h >= vatCan[i][1] && yKl + h <= vatCan[i][1] + vatCan[i][2])){
				VVatCan = 0;
				lose = true;
			}
		}
	}
	bool kt = true;
	for (int i = 0; i < n; i++)
		if (vatCan[i][0] > -2){
			kt = false;
			break;
		}
	if (kt)
		khoiTaoVatCan();
}
void veKhungLong(float y){
	glColor3f(1, 1, 1);
	glBegin(GL_POLYGON); 
		glVertex3f (-1, y, 0); 
		glVertex3f (-1, y + h, 0);
		glVertex3f (w - 1, y + h, 0);
		glVertex3f (w - 1, y, 0);
	glEnd();
}

void khungLong(){
	veKhungLong(yKl);
	veVatCan();
	veDuong();
	diemCao();
	if (start)
		nhay();
}

void sleep(int i){
	for(int time = 0 ; time < i ;time++){
		for(int count = 0 ; count < 2000000 ; count++);
	}
}
void mydisplay(){
	khungLong();
	updateScreen();
}
void updateScreen(void){
	glutPostRedisplay();
	glFlush();
	glClear(GL_COLOR_BUFFER_BIT);
	sleep(5);
}

string intToStr(int n){
	string kq = "";
	while (n > 0){
		kq = char(n % 10 + '0') + kq;
		n /= 10;
	}
	while(kq.length() < 6)
		kq = "0" + kq;
	return kq;
}

int layDiemCao(){
	ifstream fileInput("diemCao.txt");
	if (fileInput.fail())
		return 0;
	int n;
	fileInput >> n;
	return n;
}

void ghiDiemCao(int diem){
	ofstream fileOutput("diemCao.txt");
	fileOutput << diem;
}

void normalKeyHandler (unsigned char key, int x, int y){
		if(key == 's');
//	cout << key << endl;
}

void PhimNhanXoayHinh(int key, int x, int y){
	if(key == GLUT_KEY_UP && yKl == -0.5){
		vY = 0.04;
		t = 0;
		start = true;
	}
 	glutPostRedisplay();
}

int main(int argc, char** argv){
	srand(time(NULL));
	khoiTaoVatCan();
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize (1200, 600);
	glutInitWindowPosition (0, 0);
	glutCreateWindow("HaiZuka");
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glOrtho(-2, 2, -1, 1, -10, 10);
	glutKeyboardFunc(normalKeyHandler);
	glutDisplayFunc(mydisplay);
	glutSpecialFunc(PhimNhanXoayHinh);
	glutMainLoop();
}
