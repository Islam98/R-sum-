#include <Windows.h>
#include <iostream>
#include <mmsystem.h>
#include <string>     // std::string, std::to_string
#include <sstream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <glut.h>

//#include <stb_image.h>
//j#include <texture.h>
#define STB_IMAGE_IMPLEMENTATION


#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)
#define RADPERDEG 0.0174533

//unsigned char* image =SOIL_load_image("img.png", &width, &height, 0, SOIL_LOAD_RGB);

float ballmovement = 0;
float arrowmovement = 0;

////////////////////////////////////

float arrowx = 0.5;
float arrowy = 0.5;
float farthestz = 0.00001;		//farthest point in z
float arrowz = 0.00001;
//////////////////////////////////

float ballx = 0.5;
float bally = 0.2;
float ballz = 2;

bool hitright = false;
bool hitleft = false;
bool hittop = false;
bool hitbot = false;

bool minusx = true;
bool minusy = true;
bool start = false;

int varx = 1;
int vary = 1;
float ratio = 0.03;
float ratioy = 0.045;
int varxleft = 1;
float ratioxleft = 0.03;
int varyleft = 1;
float ratioyleft = 0.045;
bool fire = false;
bool ended = false;
bool hidearrow = false;

int score = 0;
int runs = 0;
bool gamestarted = false;

class Vector3f {
public:
	float x, y, z;

	Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f operator+(Vector3f& v) {
		return Vector3f(x + v.x, y + v.y, z + v.z);
	}

	Vector3f operator-(Vector3f& v) {
		return Vector3f(x - v.x, y - v.y, z - v.z);
	}

	Vector3f operator*(float n) {
		return Vector3f(x * n, y * n, z * n);
	}

	Vector3f operator/(float n) {
		return Vector3f(x / n, y / n, z / n);
	}

	Vector3f unit() {
		return *this / sqrt(x * x + y * y + z * z);
	}

	Vector3f cross(Vector3f v) {
		return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};

class Camera {
public:
	Vector3f eye, center, up;

	Camera(float eyeX = 1.0f, float eyeY = 1.0f, float eyeZ = 1.0f, float centerX = 0.0f, float centerY = 0.0f, float centerZ = 0.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f) {
		eye = Vector3f(eyeX, eyeY, eyeZ);
		center = Vector3f(centerX, centerY, centerZ);
		up = Vector3f(upX, upY, upZ);
	}

	void moveX(float d) {
		Vector3f right = up.cross(center - eye).unit();
		eye = eye + right * d;
		center = center + right * d;
	}

	void moveY(float d) {
		eye = eye + up.unit() * d;
		center = center + up.unit() * d;
	}

	void moveZ(float d) {
		Vector3f view = (center - eye).unit();
		eye = eye + view * d;
		center = center + view * d;
	}

	void rotateX(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + up * sin(DEG2RAD(a));
		up = view.cross(right);
		center = eye + view;
	}

	void rotateY(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
		right = view.cross(up);
		center = eye + view;
	}

	void look() {
		gluLookAt(
			0.5f, 0.5f, 3.0f,
			0.5f, 0.5f, 0.0f,
			up.x, up.y, up.z
		);
	}
};

Camera camera;

void drawWall(double thickness) {

	glPushMatrix();
	glColor3f(0.9, 0.9, 0.9);
	glTranslated(0.5, 0.5 * thickness, 0.5);
	glScaled(1.0, thickness, 3);
	glutSolidCube(1);
	glPopMatrix();
	int color = 0;
	bool left = true;
	for (double i = 0.05; i < 1.0; i += 0.5) {
		for (double j = 0.5; j < 2.0; j += 0.5) {
			if (left)
			{
				if (color > 3) {
					color = 0;
				}
			}
			else
			{
				if (color > 3)
				{
					color = 1;
				}
			}
			glPushMatrix();
			if (color == 0) {
				glColor3f(1, 0, 0);
			}
			if (color == 1) {
				glColor3f(1, 1, 0);
			}
			if (color == 2) {
				glColor3f(0, 1, 0);
			}
			if (color == 3) {
				glColor3f(0, 0, 1);
			}
			glTranslated(0.25 + i, 0.2 * thickness, j);
			glScaled(0.95, 0.1, 0.9);
			glutSolidCube(0.5);
			glPopMatrix();

			color += 2;
		}
		if (left)
		{
			left = false;
			color = 1;
		}
		else
		{
			left = true;
			color = 0;
		}
	}
}
void drawWally(double thickness) {
	glPushMatrix();
	glColor3f(0, 0, 1);
	glTranslated(0.5, 0.5 * thickness, 0.5);
	glScaled(1.0, thickness, 1.0);
	glutSolidCube(1);
	glPopMatrix();
}

void drawBall(float ballmovement)
{
	glPushMatrix();
	glTranslatef(ballx, bally, ballz);
	//	glTranslatef(0.089, 0.09, 1.76); //0.089
		//			0.5,  0.2,	2
		//			0.91<x then touches, 
		//	glTranslatef(0.91, 0.5, -0.05);		//hidden ball at z = -0.06
	glScalef(0.52, 0.52, 0.52);
	glColor3f(0.5f, 0.5f, 0.5f);
	glutSolidSphere(0.1, 155, 155);
	glPopMatrix();
}

void drawarrow(float arrowmovement)
{
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.5f, -0.7f);
	glVertex3f(0.5f, 0.2f, 2.0f);
	glVertex3f(arrowx, arrowy, arrowz);
	//glVertex3f(0.52f, 10.0f, 2.0f);
	//glVertex3f(0.52f, 0.0f, 2.0f);
	glEnd();
}

void setupLights() {
	GLfloat ambient[] = { 0.7f, 0.7f, 0.7, 1.0f };
	GLfloat diffuse[] = { 0.6f, 0.6f, 0.6, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0, 1.0f };
	GLfloat shininess[] = { 50 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	GLfloat lightIntensity[] = { 0.7f, 0.7f, 1, 1.0f };
	GLfloat lightPosition[] = { -7.0f, 6.0f, 3.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightIntensity);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
}
void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 640 / 480, 0.001, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.look();
}



void print(float x, float y, char const* string)
{
	int len, i;

	//set the position of the text in the window using the x and y coordinates
	glRasterPos3f(x, y, 2.0);

	//get the length of the string to display
	len = (int)strlen(string);

	//loop to display character by character
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}


void Display() {
	setupCamera();
	setupLights();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (runs == 0 && !gamestarted)
	{
		print(0.350, 0.450, "press x to start");
		print(0.050, 0.750, "red bricks adds 50 to score, blue adds 20, green adds 100");
		print(0.35, 0.68, "and yellow minuses 10");
		print(0.32, 0.18, "'game is of 3 rounds'");
	}

	if (runs < 3 && gamestarted)
	{
		drawWall(0.02);
		glPushMatrix();
		glRotated(90, 0, 0, 1.0);
		drawWall(0.02);
		glPopMatrix();
		glPushMatrix();
		glRotated(-90, 1.0, 0.0, 0.0);
		drawWally(0.0);
		glPopMatrix();

		glPushMatrix();
		glTranslated(1.0, 0.0, 0.0);
		glRotated(90, 0.0, 0.0, 1.0);
		drawWall(0.02);
		glPopMatrix();
		glPushMatrix();
		glTranslated(0.0, 1.0, 0.0);
		drawWall(0.02);
		glPopMatrix();

		if (!hidearrow)
		{
			glPushMatrix();
			drawarrow(arrowmovement);
			glPopMatrix();
		}

		glPushMatrix();
		drawBall(ballmovement);
		glPopMatrix();

		//		glFlush();
	}

	if (runs == 3)
	{
		glPushMatrix();
		//		glColor3f(0.44f, 0.56f, 0.9f);
		glColor3f(35 / 256.0f, 240 / 256.0f, 220 / 256.0f);
		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0.200, 0.250, 2.0);
		glVertex3f(0.200, 0.300, 2.0);
		glVertex3f(0.100, 0.250, 2.0);
		glVertex3f(0.200, 0.200, 2.0);
		glVertex3f(0.300, 0.250, 2.0);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glColor3f(0.9f, 0.3f, 0.5f);
		//		glColor3f(35 / 256.0f, 240 / 256.0f, 220 / 256.0f);
		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0.700, 0.250, 2.0);
		glVertex3f(0.700, 0.300, 2.0);
		glVertex3f(0.800, 0.250, 2.0);
		glVertex3f(0.700, 0.200, 2.0);
		glVertex3f(0.600, 0.250, 2.0);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glColor3f(0.1f, 0.8f, 0.3f);
		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0.700, 0.450, 2.0);
		glVertex3f(0.600, 0.450, 2.0);
		glVertex3f(0.700, 0.500, 2.0);
		glVertex3f(0.800, 0.450, 2.0);
		glVertex3f(0.700, 0.400, 2.0);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glColor3f(0.6f, 0.6f, 0.2f);
		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0.200, 0.450, 2.0);
		glVertex3f(0.200, 0.400, 2.0);
		glVertex3f(0.100, 0.450, 2.0);
		glVertex3f(0.200, 0.500, 2.0);
		glVertex3f(0.300, 0.450, 2.0);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glColor3f(0, 0, 0);
		glPointSize(0);
		glBegin(GL_POINTS);
		glVertex3f(0, 1.0000, 0);
		glEnd();
		glPopMatrix();
		std::string scoreresult = "score is " + std::to_string(score);
		char const* scoretobeshown = scoreresult.c_str();
		print(0.325, 0.325, scoretobeshown);

		gamestarted = false;
		glPushMatrix();
		glColor3f(1, 0.34, 0);
		glPointSize(0);
		glBegin(GL_POINTS);
		glVertex3f(0, 1.0000, 0);
		glEnd();
		glPopMatrix();
		print(0.255, 0.125, "press x to start again");

		//		glFlush();
	}
	glFlush();
}

void Special(int key, int x, int y) {
	/*	float a = 1.0;

		switch (key) {
		case GLUT_KEY_UP:
			camera.rotateX(a);
			break;
		case GLUT_KEY_DOWN:
			camera.rotateX(-a);
			break;
		case GLUT_KEY_LEFT:
	//		camera.rotateY(a);
			leftkeypressed = true;
			break;
		case GLUT_KEY_RIGHT:
	//		camera.rotateY(-a);
			rightkeypressed = true;
			break;
		}
		*/
	glutPostRedisplay();
}

void Anim()
{

}

void speUp(int k, int x, int y)// keyboard special key function is called whenever the special key is released.
{
	/*
	if (k == GLUT_KEY_RIGHT)
		rightkeypressed = false;
	if (k == GLUT_KEY_LEFT)
		leftkeypressed = false;
	*/
	glutPostRedisplay();
}

void key(unsigned char k, int x, int y)
{
	if (k == 'x' && !gamestarted)
	{
		gamestarted = true;
		arrowx = 0.5;
		arrowy = 0.5;
		farthestz = 0.00001;
		arrowz = 0.00001;

		ballx = 0.5;
		bally = 0.2;
		ballz = 2;

		hitright = false;
		hitleft = false;
		hittop = false;
		hitbot = false;

		varx = 1;
		vary = 1;
		ratio = 0.03;
		ratioy = 0.045;
		fire = false;
		ended = false;
		hidearrow = false;

		score = 0;
		runs = 0;
	}

	if (k == 'd' && gamestarted)
	{
		arrowx += 0.01;
		varx += 1;
		if (varx % 10 == 0)
			ratio += 0.01;
		if (arrowx >= 0.9 + 1.5)
		{
			arrowx = 0.5;
			varx = 1;
		}

		//		if (arrowx > 1.5)
		//			arrowz -= 0.01;
	}
	if (k == 'a' && gamestarted)
	{
		arrowx -= 0.01;
		varxleft += 1;
		if (varxleft % 10 == 0)
			ratioxleft += 0.01;
		if (arrowx <= 0.1 - 0.99)
		{
			arrowx = 0.5;
			varxleft = 1;
		}
	}
	if (k == 'w' && gamestarted)
	{
		arrowy += 0.01;
		vary += 1;
		if (vary % 90 == 0)
			ratioy += 0.045;

		if (arrowy >= 0.87 + 2)
		{
			arrowy = 0.5;
			vary = 1;
		}
	}
	if (k == 's' && gamestarted)
	{
		arrowy -= 0.01;
		varyleft += 1;
		if (varyleft % 90 == 0)
			ratioyleft += 0.045;

		if (arrowy <= 0.14 - 0.5)
		{
			arrowy = 0.5;
			varyleft = 1;
		}
	}
	if (k == ' ' && gamestarted)
	{
		if (!fire)
			fire = true;
	}
	glutPostRedisplay();
}

void keyUp(unsigned char k, int x, int y)//player 1 released q button
{
	/*	if (k == 'd')
		{

		}
		*/
	glutPostRedisplay();
}


void time(int val)//timer animation function, allows the user to pass an integer value to the timer function.
{
	int t = 70;
	if (ended)
	{
		fire = false;
		ended = false;
		hidearrow = false;
		hitbot = false;
		hittop = false;
		hitleft = false;
		hitright = false;
		ballx = 0.5;
		bally = 0.2;
		ballz = 2;
		varx = 1;
		vary = 1;
		ratio = 0.03;
		ratioy = 0.045;
		minusx = true;
		minusy = true;
		start = false;
		varxleft = 1;
		ratioxleft = 0.03;
		varyleft = 1;
		ratioyleft = 0.045;

		arrowx = 0.5;
		arrowy = 0.5;
		arrowz = 0.00001;

		runs += 1;
	}
	if (fire)
	{
		hidearrow = true;
		float diffx;
		if (arrowx > 0.5)
			diffx = arrowx - 0.5;
		else
			diffx = 0.5 - arrowx;

		float diffy;
		if (arrowy > 0.2)
			diffy = arrowy - 0.2;
		else
			diffy = 0.2 - arrowy;

		if (!start)
		{
			if (diffx < 0.51)
			{
				while (ballx < arrowx && ballx < 0.9 && arrowx > 0.5)
					ballx += 0.01;
				while (ballx > arrowx&& ballx > 0.1 && arrowx <= 0.5)
					ballx -= 0.01;
				minusx = false;
			}
			if (diffy < 0.8)
			{
				while (bally < arrowy && bally < 0.9 && arrowy > 0.2)
					bally += 0.01;
				while (bally > arrowy&& bally > 0.1 && arrowy <= 0.2)
					bally -= 0.01;
				minusy = false;
			}
			start = true;
		}

		if (start)
		{
			if (minusy)
			{
				if (arrowy > 0.2)
				{
					if (!hittop)
					{
						bally += ratioy;
						if (bally > 0.9)
						{
							hittop = true;
							if (ballx > 0.089 && ballx < 0.53 && ballz < 1.76 && ballz > 1.31)			//near red is hit and adds 50 to score
							{
								score += 50;
								//								PlaySound(TEXT("red.mp3"), NULL, SND_ASYNC);
							}
							if (ballx > 0.089 && ballx < 0.53 && ballz < 1.29 && ballz > 0.82)			//green is hit and adds 100 to score
								score += 100;
							if (ballx > 0.089 && ballx < 0.53 && ballz < 0.82 && ballz > 0.35)			//far red is hit and adds 50 to score
							{
								score += 50;
								//								PlaySound(TEXT("red.mp3"), NULL, SND_ASYNC);
							}
							if (ballx > 0.56 && ballx < 0.91 && ballz < 1.76 && ballz > 1.31)			//yellow is hit and minuses 10 from score
								score -= 10;
							if (ballx > 0.56 && ballx < 0.91 && ballz < 1.29 && ballz > 0.82)			//blue is hit and adds 20 to score
								score += 20;
							if (ballx > 0.56 && ballx < 0.91 && ballz < 0.82 && ballz > 0.35)			//far yellow is hit and minuses 10 from score
								score -= 10;
						}
					}
					else
					{
						bally -= ratioy;
						if (bally < 0.1)
						{
							hittop = false;
							if (ballx > 0.089 && ballx < 0.53 && ballz < 1.76 && ballz > 1.31)			//near red is hit and adds 50 to score
							{
								score += 50;
								PlaySound(TEXT("red.mp3"), NULL, SND_ASYNC);
							}
							if (ballx > 0.089 && ballx < 0.53 && ballz < 1.29 && ballz > 0.82)			//green is hit and adds 100 to score
								score += 100;
							if (ballx > 0.089 && ballx < 0.53 && ballz < 0.82 && ballz > 0.35)			//far red is hit and adds 50 to score
							{
								score += 50;
								PlaySound(TEXT("red.mp3"), NULL, SND_ASYNC);
							}
							if (ballx > 0.56 && ballx < 0.91 && ballz < 1.76 && ballz > 1.31)			//yellow is hit and minuses 10 from score
								score -= 10;
							if (ballx > 0.56 && ballx < 0.91 && ballz < 1.29 && ballz > 0.82)			//blue is hit and adds 20 to score
								score += 20;
							if (ballx > 0.56 && ballx < 0.91 && ballz < 0.82 && ballz > 0.35)			//far yellow is hit and minuses 10 from score
								score -= 10;
						}
					}
				}
				else
				{
					if (0.0 > arrowy)
					{
						if (!hitbot)
						{
							bally -= ratioy;
							if (bally < 0.1)
							{
								hitbot = true;
								if (ballx > 0.089 && ballx < 0.53 && ballz < 1.76 && ballz > 1.31)			//near red is hit and adds 50 to score
								{
									score += 50;
									PlaySound(TEXT("red.mp3"), NULL, SND_ASYNC);
								}
								if (ballx > 0.089 && ballx < 0.53 && ballz < 1.29 && ballz > 0.82)			//green is hit and adds 100 to score
									score += 100;
								if (ballx > 0.089 && ballx < 0.53 && ballz < 0.82 && ballz > 0.35)			//far red is hit and adds 50 to score
								{
									score += 50;
									PlaySound(TEXT("red.mp3"), NULL, SND_ASYNC);
								}
								if (ballx > 0.56 && ballx < 0.91 && ballz < 1.76 && ballz > 1.31)			//yellow is hit and minuses 10 from score
									score -= 10;
								if (ballx > 0.56 && ballx < 0.91 && ballz < 1.29 && ballz > 0.82)			//blue is hit and adds 20 to score
									score += 20;
								if (ballx > 0.56 && ballx < 0.91 && ballz < 0.82 && ballz > 0.35)			//far yellow is hit and minuses 10 from score
									score -= 10;
							}
						}
						else
						{
							bally += ratioy;
							if (bally > 0.9)
							{
								hitbot = false;
								if (ballx > 0.089 && ballx < 0.53 && ballz < 1.76 && ballz > 1.31)			//near red is hit and adds 50 to score
								{
									score += 50;
									PlaySound(TEXT("red.mp3"), NULL, SND_ASYNC);
								}
								if (ballx > 0.089 && ballx < 0.53 && ballz < 1.29 && ballz > 0.82)			//green is hit and adds 100 to score
									score += 100;
								if (ballx > 0.089 && ballx < 0.53 && ballz < 0.82 && ballz > 0.35)			//far red is hit and adds 50 to score
								{
									score += 50;
									PlaySound(TEXT("red.mp3"), NULL, SND_ASYNC);
								}
								if (ballx > 0.56 && ballx < 0.91 && ballz < 1.76 && ballz > 1.31)			//yellow is hit and minuses 10 from score
									score -= 10;
								if (ballx > 0.56 && ballx < 0.91 && ballz < 1.29 && ballz > 0.82)			//blue is hit and adds 20 to score
									score += 20;
								if (ballx > 0.56 && ballx < 0.91 && ballz < 0.82 && ballz > 0.35)			//far yellow is hit and minuses 10 from score
									score -= 10;
							}
						}
					}
				}
			}
			if (minusx)
			{
				if (arrowx > 0.5)
				{
					if (!hitright)
					{
						ballx += ratio;
						if (ballx > 0.9)
						{
							hitright = true;
							if (bally > 0.11 && bally < 0.52 && ballz < 1.76 && ballz > 1.31)			//near red is hit and adds 50 to score
							{
								score += 50;
								PlaySound(TEXT("red.mp3"), NULL, SND_ASYNC);
							}
							if (bally > 0.579 && bally < 0.91 && ballz < 1.76 && ballz > 1.31)			//near yellow is hit and minus 10 from score
								score -= 10;
							if (bally > 0.579 && bally < 0.91 && ballz < 1.29 && ballz > 0.82)			//blue is hit and adds 20 to score
								score += 20;
							if (bally > 0.579 && bally < 0.91 && ballz < 0.82 && ballz > 0.35)			//far yellow is hit and minuses 10 from score
								score -= 10;
							if (bally > 0.11 && bally < 0.52 && ballz < 1.29 && ballz > 0.82)			//green is hit and adds 100 to score
								score += 100;
							if (bally > 0.11 && bally < 0.52 && ballz < 0.82 && ballz > 0.35)			//red is hit and adds 50 to score
							{
								score += 50;
								PlaySound(TEXT("red.mp3"), NULL, SND_ASYNC);
							}
						}
					}
					else
					{
						ballx -= ratio;
						if (ballx < 0.1)
						{
							hitright = false;
							if (bally > 0.11 && bally < 0.52 && ballz < 1.76 && ballz > 1.31)			//near red is hit and adds 50 to score
							{
								score += 50;
								PlaySound(TEXT("red.mp3"), NULL, SND_ASYNC);
							}
							if (bally > 0.579 && bally < 0.91 && ballz < 1.76 && ballz > 1.31)			//near yellow is hit and minus 10 from score
								score -= 10;
							if (bally > 0.579 && bally < 0.91 && ballz < 1.29 && ballz > 0.82)			//blue is hit and adds 20 to score
								score += 20;
							if (bally > 0.579 && bally < 0.91 && ballz < 0.82 && ballz > 0.35)			//far yellow is hit and minuses 10 from score
								score -= 10;
							if (bally > 0.11 && bally < 0.52 && ballz < 1.29 && ballz > 0.82)			//green is hit and adds 100 to score
								score += 100;
							if (bally > 0.11 && bally < 0.52 && ballz < 0.82 && ballz > 0.35)			//red is hit and adds 50 to score
							{
								score += 50;
								PlaySound(TEXT("red.mp3"), NULL, SND_ASYNC);
							}
						}
					}
				}
				else				   //subtract from x (move on x -ve)
				{
					if (0.5 > arrowx)
					{
						if (!hitleft)
						{
							ballx -= ratio;
							if (ballx < 0.1)
							{
								hitleft = true;
								if (bally > 0.11 && bally < 0.52 && ballz < 1.76 && ballz > 1.31)			//near red is hit and adds 50 to score
								{
									score += 50;
									PlaySound(TEXT("red.mp3"), NULL, SND_ASYNC);
								}
								if (bally > 0.579 && bally < 0.91 && ballz < 1.76 && ballz > 1.31)			//near yellow is hit and minus 10 from score
									score -= 10;
								if (bally > 0.579 && bally < 0.91 && ballz < 1.29 && ballz > 0.82)			//blue is hit and adds 20 to score
									score += 20;
								if (bally > 0.579 && bally < 0.91 && ballz < 0.82 && ballz > 0.35)			//far yellow is hit and minuses 10 from score
									score -= 10;
								if (bally > 0.11 && bally < 0.52 && ballz < 1.29 && ballz > 0.82)			//green is hit and adds 100 to score
									score += 100;
								if (bally > 0.11 && bally < 0.52 && ballz < 0.82 && ballz > 0.35)			//red is hit and adds 50 to score
								{
									score += 50;
									PlaySound(TEXT("red.mp3"), NULL, SND_ASYNC);
								}
							}
						}
						else
						{
							ballx += ratio;
							if (ballx > 0.9)
							{
								hitleft = false;
								if (bally > 0.11 && bally < 0.52 && ballz < 1.76 && ballz > 1.31)			//near red is hit and adds 50 to score
								{
									score += 50;
									PlaySound(TEXT("red.mp3"), NULL, SND_ASYNC);
								}
								if (bally > 0.579 && bally < 0.91 && ballz < 1.76 && ballz > 1.31)			//near yellow is hit and minus 10 from score
									score -= 10;
								if (bally > 0.579 && bally < 0.91 && ballz < 1.29 && ballz > 0.82)			//blue is hit and adds 20 to score
									score += 20;
								if (bally > 0.579 && bally < 0.91 && ballz < 0.82 && ballz > 0.35)			//far yellow is hit and minuses 10 from score
									score -= 10;
								if (bally > 0.11 && bally < 0.52 && ballz < 1.29 && ballz > 0.82)			//green is hit and adds 100 to score
									score += 100;
								if (bally > 0.11 && bally < 0.52 && ballz < 0.82 && ballz > 0.35)			//red is hit and adds 50 to score
								{
									score += 50;
									PlaySound(TEXT("red.mp3"), NULL, SND_ASYNC);
								}
							}
						}
					}
				}
			}
		}



		ballz -= 0.1;
		if (ballz <= farthestz)
		{
			ended = true;
			t = 2000;
		}
	}

	glutPostRedisplay();
	glutTimerFunc(t, time, 0);
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitWindowSize(640, 480);
	glutInitWindowPosition(250, 150);

	glutCreateWindow("Assignment 2");
	glutDisplayFunc(Display);

	glutIdleFunc(Anim);
	glutTimerFunc(0, time, 0);

	//	glutKeyboardFunc(Keyboard);

	glutKeyboardFunc(key);
	glutKeyboardUpFunc(keyUp);

	glutSpecialFunc(Special);
	glutSpecialUpFunc(speUp);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}