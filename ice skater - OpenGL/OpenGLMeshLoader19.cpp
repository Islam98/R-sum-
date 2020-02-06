#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <glut.h>
#include <iostream>
#include<string> 
using std::cout;
using std::endl;
std::string s = std::to_string(123);
void renderBitmapString(float x, float y, float z, const char *string) {
	const char * c;

	//glRasterPos2f(x, y);
	// glutBitmapCharacter(font, string);
	glRasterPos3f(x, y, z);
	//glRasterPos3i(x, y ,z);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	}
}

void Anim();
void renerObstcales();

int WIDTH = 1280;
int HEIGHT = 720;
int x;
int y;
int z;

int zAxis;
bool col = false;
int skater = 100;
int score = 0;
bool level = true;
bool start = false;
bool camMode = true;
bool end = false;
bool colCup = false;
bool mouse = false;
float rotLight = 0;
float affLight = 1;

GLuint tex;
char title[] = "ice skater";

// 3D Projection Options
GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.1;
GLdouble zFar = 1000;

class Vector
{
public:
	GLdouble x, y, z;
	Vector() {}
	Vector(GLdouble _x, GLdouble _y, GLdouble _z) : x(_x), y(_y), z(_z) {}
	//================================================================================================//
	// Operator Overloading; In C++ you can override the behavior of operators for you class objects. //
	// Here we are overloading the += operator to add a given value to all vector coordinates.        //
	//================================================================================================//
	void operator +=(float value)
	{
		x += value;
		y += value;
		z += value;
	}
};

Vector Eye(skater, 6, 0);
Vector At(100, 0, 50);
Vector Up(0, 1, 0);

int cameraZoom = 0;

// Model Variables
//Model_3DS model_house;
Model_3DS model_tree;
Model_3DS model_man;
Model_3DS model_ship;
Model_3DS model_cup;

// Textures
GLTexture tex_ground;
GLTexture tex_ground2;


//=======================================================================
// Lighting Configuration Function
//=======================================================================
void InitLightSource()
{
	//// Enable Lighting for this OpenGL Program
	//glEnable(GL_LIGHTING);

	//// Enable Light Source number 0
	//// OpengL has 8 light sources
	//glEnable(GL_LIGHT0);

	//// Define Light source 0 ambient light
	//GLfloat ambient[] = { 1, 1, 1, 1.0f };
	//glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	//// Define Light source 0 diffuse light
	//GLfloat diffuse[] = { 1, 1, 1, 1.0f };
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	//// Define Light source 0 Specular light
	//GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	//// Finally, define light source 0 position in World Space
	//GLfloat light_position[] = { 100, 40, 20, 0.0f };
	//glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	GLfloat lmodel_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

	GLfloat l0Diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat l0Spec[] = { 1.0f, 1.0f, 1, 1.0f };
	GLfloat l0Ambient[] = { 0.1, 0.1, 0.1 , 1.0f };
	GLfloat l0Position[] = { 100, 60, 20, 0 };

	glLightfv(GL_LIGHT0, GL_DIFFUSE, l0Diffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, l0Ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, l0Position);
	//glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 30.0);
//	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 90.0);
}

//=======================================================================
// Material Configuration Function
//======================================================================
void InitMaterial()
{
	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);

	// Sich will be assigneet Material Properties whd by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Set Material's Specular Color
	// Will be applied to all objects
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 96.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

//=======================================================================
// OpengGL Configuration Function
//=======================================================================
void myInit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(fovy, aspectRatio, zNear, zFar);
	//*******************************************************************************************//
	// fovy:			Angle between the bottom and top of the projectors, in degrees.			 //
	// aspectRatio:		Ratio of width to height of the clipping plane.							 //
	// zNear and zFar:	Specify the front and back clipping planes distances from camera.		 //
	//*******************************************************************************************//

	glMatrixMode(GL_MODELVIEW);

	/*glLoadIdentity();

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);*/

	InitLightSource();

	InitMaterial();

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
}

//=======================================================================
// Render Ground Function
//=======================================================================
void RenderGround()
{
	glDisable(GL_LIGHTING);	// Disable lighting 
	glColor3f(1, 1, 1);	// Dim the ground texture a bit
	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing
	if (level == true) {
		glBindTexture(GL_TEXTURE_2D, tex_ground.texture[0]);	// Bind the ground texture
	}
	else {
		glBindTexture(GL_TEXTURE_2D, tex_ground2.texture[0]);	// Bind the ground texture
	}

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(0, 0, zAxis);
	glTexCoord2f(1, 0);
	glVertex3f(200, 0, zAxis);
	glTexCoord2f(1, 1);
	glVertex3f(200, 0, zAxis-100);
	glTexCoord2f(0, 1);
	glVertex3f(0, 0, zAxis-100);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(0, 0, zAxis);
	glTexCoord2f(1, 0);
	glVertex3f(200, 0, zAxis);
	glTexCoord2f(1, 1);
	glVertex3f(200, 0, zAxis+2000);
	glTexCoord2f(0, 1);
	glVertex3f(0, 0, zAxis+2000);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(0, 0, zAxis+2000);
	glTexCoord2f(1, 0);
	glVertex3f(200, 0, zAxis+ 2000);
	glTexCoord2f(1, 1);
	glVertex3f(200, 0, zAxis + 4000);
	glTexCoord2f(0, 1);
	glVertex3f(0, 0, zAxis + 4000);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(0, 0, zAxis + 4000);
	glTexCoord2f(1, 0);
	glVertex3f(200, 0, zAxis + 4000);
	glTexCoord2f(1, 1);
	glVertex3f(200, 0, zAxis + 6000);
	glTexCoord2f(0, 1);
	glVertex3f(0, 0, zAxis + 6000);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(0, 0, zAxis + 6000);
	glTexCoord2f(1, 0);
	glVertex3f(200, 0, zAxis + 6000);
	glTexCoord2f(1, 1);
	glVertex3f(200, 0, zAxis + 8000);
	glTexCoord2f(0, 1);
	glVertex3f(0, 0, zAxis + 8000);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}

//=======================================================================
// Display Function
//=======================================================================
void myDisplay(void)
{
	glLoadIdentity();
	if (camMode == false) {
		gluLookAt(skater, 25, 22, skater, At.y, 200, Up.x, Up.y, Up.z);
	}
	else {
		gluLookAt(skater, 50, -100, skater, 10, 0, Up.x, Up.y, Up.z);
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat lightIntensity[] = { 0.7, 0.7, 0.7, 1.0f };
	//GLfloat lightPosition[] = { 0.0f, 100.0f, 0.0f, 0.0f };
	//glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);
	GLfloat light_position[] = { 100, 60, 20, 0.0f };

	glPushMatrix();
	glRotated(rotLight, 1, 1, 1);
	InitLightSource();
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glPopMatrix();

	GLfloat l0Diffuse[] = { affLight ,affLight, affLight, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, l0Diffuse);

	if (col) {
		renderBitmapString(skater, 30, 0, "GAME OVER");
	}

	//glColor3f(0.0f, 0.0f, 0.0f);
	renderBitmapString(skater-50, 45, 0, "score  : ");
	std::string s = std::to_string(score/10);
	const char * c = s.c_str();
	renderBitmapString(skater - 50, 40, 0, c);
	if (end) {
		renderBitmapString(skater, 35, 0, "YOU WIN");
		renderBitmapString(skater, 30, 0, "YOUR TOTAL SCORE IS");
		renderBitmapString(skater, 25, 0, c);
	}
	

	//sky box
	glPushMatrix();
	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	glTranslated(0, 0, 0);
	glRotated(90, 1, 0, 1);
	glBindTexture(GL_TEXTURE_2D, tex);
	gluQuadricTexture(qobj, true);
	gluQuadricNormals(qobj, GL_SMOOTH);
	gluSphere(qobj, 800, 60, 60);
	gluDeleteQuadric(qobj);
	glPopMatrix();

	// Draw Ground
	RenderGround();
	
	// Draw obstacles Model
	renerObstcales();

	// Draw man Model
	glPushMatrix();
	glTranslated(skater, 15, 20);
	glScaled(0.015, 0.015, 0.015);
    model_man.Draw();
	glPopMatrix();

	glPushMatrix();
	//glTranslated(100, 15, zAxis + 3250);
	glTranslated(150, 15, zAxis + 3250);
	glScaled(0.25, 0.25, 0.25);
	model_cup.Draw();
	glPopMatrix();



	glutSwapBuffers();
}

void myKeyboard(unsigned char button, int x, int y)
{
	switch (button)
	{
	case ' ':
		start = true;
		break;
	case 'm':
		camMode = !camMode;
		break;
	case 'e':
		mouse = !mouse;
		break;
	default:
		break;
	}

	glutPostRedisplay();
}


void passM(int x, int y)
{
	if (mouse) {
		/*	if (x > 165) {
				x = 165;
			}
			if (x < 45) {
				x = 45;
			}*/


		skater = 165.0f - ((((float)x / (float)WIDTH)) * 165.0f);
		if (skater < 45) {
			skater = 45;
		}


		cout << skater;
	}
	else {
	}
	glutPostRedisplay();
}

void LoadAssets()
{
	// Loading Model files
	//model_house.Load("Models/house/house.3DS");
	model_tree.Load("Models/tree/Tree1.3ds");
	model_man.Load("Models/bissball/Batsman N170518.3ds");
	model_ship.Load("Models/ship/Boat.3ds");
	model_cup.Load("Models/cup/Champions Cup stefano galli N020113.3ds");

	// Loading texture files
	tex_ground.Load("Textures/ice22.bmp");
	tex_ground2.Load("Textures/frozen3.bmp");
	loadBMP(&tex, "Textures/blu-sky-33.bmp", true);
}
void spe(int k, int x, int y)
{
	if (k == GLUT_KEY_LEFT && (!col) && (!end)) {
		skater = skater + 5;
		if (skater >= 165) {
			skater = 165;
		}
	}
	if (k == GLUT_KEY_RIGHT && (!col) && (!end)) {
		skater = skater - 5;
		if (skater <= 45) {
			skater = 45;
		}
	}
	
	glutPostRedisplay();
}
void Anim() {
	rotLight += 0.5;
	affLight -= 0.005;
	if (affLight < 0) {
		affLight = 1;
	}
	if (zAxis == -6500) {
		if (level == true) {
			level = false;
			start = false;
			zAxis = 0;
		}
		else {
			end = true;
		}
	}
	if ((!col) && (start)) {
		if (level == true) {
			zAxis = zAxis - 10;
			score = score + 3;
			if (colCup) {
				score = score * 2;
					colCup = false;
			}
		}
		else {
			if (end == false) {
				zAxis = zAxis - 2;
				score = score + 1;
			}
		}
	}
	if (zAxis == -500) {
		if ((40 < skater  && skater < 60) || (100 < skater  && skater < 120)) {
			col = true;}}
	if (zAxis == -1000 ) {
		if ((40 < skater  && skater < 60) || (70 < skater  && skater < 90) || (100 < skater  && skater < 120)) {
			col = true;}}
	if (zAxis == -1500) {
		if ((70 < skater  && skater < 90) || (100 < skater  && skater < 120) || (130 < skater  && skater < 150)) {
			col = true;}}
	if (zAxis == -2000) {
		if ((40 < skater  && skater < 60) || (100 < skater  && skater < 120) || (130 < skater  && skater < 150)) {
			col = true;
		}
	}
	if (zAxis == -2500) {
		if ((40 < skater  && skater < 60) || (70 < skater  && skater < 90) || (100 < skater  && skater < 120)) {
			col = true;
		}
	}
	if (zAxis == -3000) {
		if ((40 < skater  && skater < 60) || (70 < skater  && skater < 90) || (130 < skater  && skater < 150)) {
			col = true;
		}
	}
	if (zAxis == -3250) {
		if ((145 < skater  && skater < 155) ) {
			colCup = true;
		}
	}
	if (zAxis == -3500) {
		if ((40 < skater  && skater < 60) || (100 < skater  && skater < 120) || (130 < skater  && skater < 150)) {
			col = true;
		}
	}
	if (zAxis == -4000) {
		if ((70 < skater  && skater < 90) || (100 < skater  && skater < 120)) {
			col = true;
		}
	}
	if (zAxis == -4500) {
		if ((40 < skater  && skater < 60) || (100 < skater  && skater < 120) || (130 < skater  && skater < 150)) {
			col = true;
		}
	}
	if (zAxis == -5000) {
		if ((70 < skater  && skater < 90) || (100 < skater  && skater < 120) || (130 < skater  && skater < 150)) {
			col = true;
		}
	}
	if (zAxis == -5500) {
		if ((40 < skater  && skater < 60) || (70 < skater  && skater < 90) || (100 < skater  && skater < 120) || (130 < skater  && skater < 150)) {
			col = true;
		}
	}
	if (zAxis == -6000) {
		if ((40 < skater  && skater < 60) || (70 < skater  && skater < 90) || (100 < skater  && skater < 120) || (130 < skater  && skater < 150)) {
			col = true;
		}
	}
	glutPostRedisplay();
}
void main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);

	glutInitWindowPosition(100, 150);

	glutCreateWindow(title);

	glutDisplayFunc(myDisplay);

	glutIdleFunc(Anim);

	glutKeyboardFunc(myKeyboard);

	glutPassiveMotionFunc(passM);

	

	glutSpecialFunc(spe);

	myInit();

	LoadAssets();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}
void renerObstcales() {

	glPushMatrix();
	glTranslatef(50, 0, zAxis + 500);
	if (level == true) {
		glScalef(2, 2, 2);
		model_tree.Draw();
	}
	else {
		glTranslatef(0, 20, 0);
		glRotated(90, 1, 0, 0);
		glScaled(0.35, 0.35, 0.35);
		model_ship.Draw();
	}
	glPopMatrix();
    glPushMatrix();
	glTranslatef(110, 0, zAxis + 500);
	if (level == true) {
		glScalef(2, 2, 2);
		model_tree.Draw();
	}
	else {
		glTranslatef(0, 20,0);
		glRotated(90, 1, 0, 0);
		glScaled(0.35, 0.35, 0.35);
		model_ship.Draw();
	}
	glPopMatrix();

    glPushMatrix();
	glTranslatef(50, 0, zAxis + 1000);
	if (level == true) {
		glScalef(2, 2, 2);
		model_tree.Draw();
	}
	else {
		glTranslatef(0, 20, 0);
		glRotated(90, 1, 0, 0);
		glScaled(0.35, 0.35, 0.35);
		model_ship.Draw();
	}
	glPopMatrix();
	glPushMatrix();
	glTranslatef(80, 0, zAxis + 1000);
	if (level == true) {
		glScalef(2, 2, 2);
		model_tree.Draw();
	}
	else {
		glTranslatef(0, 20, 0);
		glRotated(90, 1, 0, 0);
		glScaled(0.35, 0.35, 0.35);
		model_ship.Draw();
	}
	glPopMatrix();
	glPushMatrix();
	glTranslatef(110, 0, zAxis + 1000);
	if (level == true) {
		glScalef(2, 2, 2);
		model_tree.Draw();
	}
	else {
		glTranslatef(0, 20, 0);
		glRotated(90, 1, 0, 0);
		glScaled(0.35, 0.35, 0.35);
		model_ship.Draw();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(80, 0, zAxis + 1500);
	if (level == true) {
		glScalef(2, 2, 2);
		model_tree.Draw();
	}
	else {
		glTranslatef(0, 20, 0);
		glRotated(90, 1, 0, 0);
		glScaled(0.35, 0.35, 0.35);
		model_ship.Draw();
	}
	glPopMatrix();
	glPushMatrix();
	glTranslatef(110, 0, zAxis + 1500);
	if (level == true) {
		glScalef(2, 2, 2);
		model_tree.Draw();
	}
	else {
		glTranslatef(0, 20, 0);
		glRotated(90, 1, 0, 0);
		glScaled(0.35, 0.35, 0.35);
		model_ship.Draw();
	}
	glPopMatrix();
	glPushMatrix();
	glTranslatef(140, 0, zAxis + 1500);
	if (level == true) {
		glScalef(2, 2, 2);
		model_tree.Draw();
	}
	else {
		glTranslatef(0, 20, 0);
		glRotated(90, 1, 0, 0);
		glScaled(0.35, 0.35, 0.35);
		model_ship.Draw();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(50, 0, zAxis + 2000);
	if (level == true) {
		glScalef(2, 2, 2);
		model_tree.Draw();
	}
	else {
		glTranslatef(0, 20, 0);
		glRotated(90, 1, 0, 0);
		glScaled(0.35, 0.35, 0.35);
		model_ship.Draw();
	}
	glPopMatrix();
	glPushMatrix();
	glTranslatef(110, 0, zAxis + 2000);
	if (level == true) {
		glScalef(2, 2, 2);
		model_tree.Draw();
	}
	else {
		glTranslatef(0, 20, 0);
		glRotated(90, 1, 0, 0);
		glScaled(0.35, 0.35, 0.35);
		model_ship.Draw();
	}
	glPopMatrix();
	glPushMatrix();
	glTranslatef(140, 0, zAxis + 2000);
	if (level == true) {
		glScalef(2, 2, 2);
		model_tree.Draw();
	}
	else {
		glTranslatef(0, 20, 0);
		glRotated(90, 1, 0, 0);
		glScaled(0.35, 0.35, 0.35);
		model_ship.Draw();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(50, 0, zAxis + 2500);
	if (level == true) {
		glScalef(2, 2, 2);
		model_tree.Draw();
	}
	else {
		glTranslatef(0, 20, 0);
		glRotated(90, 1, 0, 0);
		glScaled(0.35, 0.35, 0.35);
		model_ship.Draw();
	}
	glPopMatrix();
	glPushMatrix();
	glTranslatef(80, 0, zAxis + 2500);
	if (level == true) {
		glScalef(2, 2, 2);
		model_tree.Draw();
	}
	else {
		glTranslatef(0, 20, 0);
		glRotated(90, 1, 0, 0);
		glScaled(0.35, 0.35, 0.35);
		model_ship.Draw();
	}
	glPopMatrix();
	glPushMatrix();
	glTranslatef(110, 0, zAxis + 2500);
	if (level == true) {
		glScalef(2, 2, 2);
		model_tree.Draw();
	}
	else {
		glTranslatef(0, 20, 0);
		glRotated(90, 1, 0, 0);
		glScaled(0.35, 0.35, 0.35);
		model_ship.Draw();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(50, 0, zAxis + 3000);
	if (level == true) {
		glScalef(2, 2, 2);
		model_tree.Draw();
	}
	else {
		glTranslatef(0, 20, 0);
		glRotated(90, 1, 0, 0);
		glScaled(0.35, 0.35, 0.35);
		model_ship.Draw();
	}
	glPopMatrix();
	glPushMatrix();
	glTranslatef(80, 0, zAxis + 3000);
	if (level == true) {
		glScalef(2, 2, 2);
		model_tree.Draw();
	}
	else {
		glTranslatef(0, 20, 0);
		glRotated(90, 1, 0, 0);
		glScaled(0.35, 0.35, 0.35);
		model_ship.Draw();
	}
	glPopMatrix();
	glPushMatrix();
	glTranslatef(140, 0, zAxis + 3000);
	if (level == true) {
		glScalef(2, 2, 2);
		model_tree.Draw();
	}
	else {
		glTranslatef(0, 20, 0);
		glRotated(90, 1, 0, 0);
		glScaled(0.35, 0.35, 0.35);
		model_ship.Draw();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(50, 0, zAxis + 3500);
	if (level == true) {
		glScalef(2, 2, 2);
		model_tree.Draw();
	}
	else {
		glTranslatef(0, 20, 0);
		glRotated(90, 1, 0, 0);
		glScaled(0.35, 0.35, 0.35);
		model_ship.Draw();
	}
	glPopMatrix();
	glPushMatrix();
	glTranslatef(110, 0, zAxis + 3500);
	if (level == true) {
		glScalef(2, 2, 2);
		model_tree.Draw();
	}
	else {
		glTranslatef(0, 20, 0);
		glRotated(90, 1, 0, 0);
		glScaled(0.35, 0.35, 0.35);
		model_ship.Draw();
	}
	glPopMatrix();
	glPushMatrix();
	glTranslatef(140, 0, zAxis + 3500);
	if (level == true) {
		glScalef(2, 2, 2);
		model_tree.Draw();
	}
	else {
		glTranslatef(0, 20, 0);
		glRotated(90, 1, 0, 0);
		glScaled(0.35, 0.35, 0.35);
		model_ship.Draw();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(80, 0, zAxis + 4000);
	if (level == true) {
		glScalef(2, 2, 2);
		model_tree.Draw();
	}
	else {
		glTranslatef(0, 20, 0);
		glRotated(90, 1, 0, 0);
		glScaled(0.35, 0.35, 0.35);
		model_ship.Draw();
	}
	glPopMatrix();
	glPushMatrix();
	glTranslatef(110, 0, zAxis + 4000);
	if (level == true) {
		glScalef(2, 2, 2);
		model_tree.Draw();
	}
	else {
		glTranslatef(0, 20, 0);
		glRotated(90, 1, 0, 0);
		glScaled(0.35, 0.35, 0.35);
		model_ship.Draw();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(50, 0, zAxis + 4500);
	if (level == true) {
		glScalef(2, 2, 2);
		model_tree.Draw();
	}
	else {
		glTranslatef(0, 20, 0);
		glRotated(90, 1, 0, 0);
		glScaled(0.35, 0.35, 0.35);
		model_ship.Draw();
	}
	glPopMatrix();
	glPushMatrix();
	glTranslatef(110, 0, zAxis + 4500);
	if (level == true) {
		glScalef(2, 2, 2);
		model_tree.Draw();
	}
	else {
		glTranslatef(0, 20, 0);
		glRotated(90, 1, 0, 0);
		glScaled(0.35, 0.35, 0.35);
		model_ship.Draw();
	}
	glPopMatrix();
    glPushMatrix();
	glTranslatef(140, 0, zAxis + 4500);
	if (level == true) {
		glScalef(2, 2, 2);
		model_tree.Draw();
	}
	else {
		glTranslatef(0, 20, 0);
		glRotated(90, 1, 0, 0);
		glScaled(0.35, 0.35, 0.35);
		model_ship.Draw();
	}
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(80, 0, zAxis + 5000);
	if (level == true) {
		glScalef(2, 2, 2);
		model_tree.Draw();
	}
	else {
		glTranslatef(0, 20, 0);
		glRotated(90, 1, 0, 0);
		glScaled(0.35, 0.35, 0.35);
		model_ship.Draw();
	}
	glPopMatrix();
	glPushMatrix();
	glTranslatef(110, 0, zAxis + 5000);
	if (level == true) {
		glScalef(2, 2, 2);
		model_tree.Draw();
	}
	else {
		glTranslatef(0, 20, 0);
		glRotated(90, 1, 0, 0);
		glScaled(0.35, 0.35, 0.35);
		model_ship.Draw();
	}
	glPopMatrix();
	glPushMatrix();
	glTranslatef(140, 0, zAxis + 5000);
	if (level == true) {
		glScalef(2, 2, 2);
		model_tree.Draw();
	}
	else {
		glTranslatef(0, 20, 0);
		glRotated(90, 1, 0, 0);
		glScaled(0.35, 0.35, 0.35);
		model_ship.Draw();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(50, 0, zAxis + 5500);
	if (level == true) {
		glScalef(2, 2, 2);
		model_tree.Draw();
	}
	else {
		glTranslatef(0, 20, 0);
		glRotated(90, 1, 0, 0);
		glScaled(0.35, 0.35, 0.35);
		model_ship.Draw();
	}
	glPopMatrix();
	glPushMatrix();
	glTranslatef(80, 0, zAxis + 5500);
	if (level == true) {
		glScalef(2, 2, 2);
		model_tree.Draw();
	}
	else {
		glTranslatef(0, 20, 0);
		glRotated(90, 1, 0, 0);
		glScaled(0.35, 0.35, 0.35);
		model_ship.Draw();
	}
	glPopMatrix();
	glPushMatrix();
	glTranslatef(110, 0, zAxis + 5500);
	if (level == true) {
		glScalef(2, 2, 2);
		model_tree.Draw();
	}
	else {
		glTranslatef(0, 20, 0);
		glRotated(90, 1, 0, 0);
		glScaled(0.35, 0.35, 0.35);
		model_ship.Draw();
	}
	glPopMatrix();
	glPushMatrix();
	glTranslatef(140, 0, zAxis + 5500);
	if (level == true) {
		glScalef(2, 2, 2);
		model_tree.Draw();
	}
	else {
		glTranslatef(0, 20, 0);
		glRotated(90, 1, 0, 0);
		glScaled(0.35, 0.35, 0.35);
		model_ship.Draw();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(50, 0, zAxis + 6000);
	if (level == true) {
		glScalef(2, 2, 2);
		model_tree.Draw();
	}
	else {
		glTranslatef(0, 20, 0);
		glRotated(90, 1, 0, 0);
		glScaled(0.35, 0.35, 0.35);
		model_ship.Draw();
	}
	glPopMatrix();
	glPushMatrix();
	glTranslatef(80, 0, zAxis + 6000);
	if (level == true) {
		glScalef(2, 2, 2);
		model_tree.Draw();
	}
	else {
		glTranslatef(0, 20, 0);
		glRotated(90, 1, 0, 0);
		glScaled(0.35, 0.35, 0.35);
		model_ship.Draw();
	}
	glPopMatrix();
	glPushMatrix();
	glTranslatef(110, 0, zAxis + 6000);
	if (level == true) {
		glScalef(2, 2, 2);
		model_tree.Draw();
	}
	else {
		glTranslatef(0, 20, 0);
		glRotated(90, 1, 0, 0);
		glScaled(0.35, 0.35, 0.35);
		model_ship.Draw();
	}
	glPopMatrix();
	glPushMatrix();
	glTranslatef(140, 0, zAxis + 6000);
	if (level == true) {
		glScalef(2, 2, 2);
		model_tree.Draw();
	}
	else {
		glTranslatef(0, 20, 0);
		glRotated(90, 1, 0, 0);
		glScaled(0.35, 0.35, 0.35);
		model_ship.Draw();
	}
	glPopMatrix();
	
	
}