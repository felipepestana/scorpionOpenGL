#include <iostream>
#include <GL/glut.h>
#include "RgbImage.h"

#define PI 3.141592654

using namespace std;

//Texture variables
char* filenameTexDark = "./darkTexture.bmp";
char* filenameTexLight = "./lightTexture.bmp";
char* filenameTexRed = "./redTexture.bmp";

GLuint _textureIdDark;
GLuint _textureIdLight;
GLuint _textureIdRed;

bool textureOn = false;

GLUquadric *quadSphere;
GLUquadric *quadCylinder;

//Camera variables
float eyeDistance = 30.0;
float eyeX;
float eyeY;
float eyeZ;
float viewAngleX = 90.0;
float viewAngleZ = 90.0;

//Moviment variables
float leftLegs[4] = { 0, 0, 0, 0 };
float rightLegs[4] = { 0, 0, 0, 0 };
float leftArm = 0.0;
float rightArm = 0.0;
float leftClaw = 30.0;
float rightClaw = 30.0;
float angleTail = 30.0;

//Animation variables
int walkingState = 0;
float walkLeftLegs[4] = { 0, 0, 0, 0 };
float walkRightLegs[4] = { 0, 0, 0, 0 };

//Ilumination variables
GLfloat lightposition[4] = { 50.0f, 0.0f, 0.0f, 0.0 };
GLfloat luzAmbiente[4] = { 0.19, 0.19, 0.19, 0.0 };
GLfloat luzDifusa[4] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat luzEspecular[4] = { 0.5, 0.5, 0.5, 1.0 };

//Defines light source 
void initialize(void)
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	// Set screen color as black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Activate Gouraud Colorization Mode
	glShadeModel(GL_SMOOTH);

	// Define model and values for light
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
	glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
	glLightfv(GL_LIGHT0, GL_POSITION, lightposition);

	// Materials will follow current color
	glEnable(GL_COLOR_MATERIAL);
}

//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(char *filename) {
	GLuint textureId;

	RgbImage theTexMap(filename); //Image with texture

	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId);	//Tell OpenGL which texture to edit
												//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,	//Always GL_TEXTURE_2D
		0,						//0 for now
		GL_RGB,					//Format OpenGL uses for image
		theTexMap.GetNumCols(),	//Width 
		theTexMap.GetNumRows(),	//Height
		0,						//The border of the image
		GL_RGB,					//GL_RGB, because pixels are stored in RGB format
		GL_UNSIGNED_BYTE,		//GL_UNSIGNED_BYTE, because pixels are stored as unsigned numbers
		theTexMap.ImageData());	//The actual pixel data
	return textureId; //Returns the id of the texture
}

//Initialize variables to apply textures
void initRendering(void) {

	quadSphere = gluNewQuadric();
	quadCylinder = gluNewQuadric();
	_textureIdDark = loadTexture(filenameTexDark);
	_textureIdLight = loadTexture(filenameTexLight);
	_textureIdRed = loadTexture(filenameTexRed);
}

//Function to handle special keys 
void specialKeyboardHandler(int key, int x, int y) {
	switch (key)
	{
	case GLUT_KEY_UP: //Increase view angle z axis
		if (viewAngleZ < 90) viewAngleZ += 3;
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN: //Decrease view angle z axis
		if (viewAngleZ > -90) viewAngleZ -= 3;
		glutPostRedisplay();
		break;
	case GLUT_KEY_LEFT: //Decrease view angle x axis
		if (viewAngleX > 0) viewAngleX -= 3;
		else viewAngleX = (viewAngleX -= 3) + 360;
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT: //Increase view angle x axis
		if (viewAngleX < 360) viewAngleX += 3;
		else viewAngleX = (viewAngleX += 3) - 360;
		glutPostRedisplay();
		break;
	}
}

//Function to handle "normal" keys 
void keyboardHandler(unsigned char key, int x, int y) {
	switch (key) {
	case 27: //Escape key
		exit(0);
	case '1': //Open angle left arm
		if (leftArm < 12) leftArm += 2;
		glutPostRedisplay();
		break;
	case '2'://Close angle left arm
		if (leftArm > -8) leftArm -= 2;
		glutPostRedisplay();
		break;
	case '3'://Open angle right arm
		if (rightArm > -12) rightArm -= 2;
		glutPostRedisplay();
		break;
	case '4'://Close angle right arm
		if (rightArm < 8) rightArm += 2;
		glutPostRedisplay();
		break;
	case '5'://Open angle left claw
		if (leftClaw < 40) leftClaw += 2;
		glutPostRedisplay();
		break;
	case '6'://Close angle left claw
		if (leftClaw > 8) leftClaw -= 2;
		glutPostRedisplay();
		break;
	case '7'://Open angle right claw
		if (rightClaw < 40) rightClaw += 2;
		glutPostRedisplay();
		break;
	case '8'://Close angle right claw
		if (rightClaw > 8) rightClaw -= 2;
		glutPostRedisplay();
		break;
	case '9'://Open angle tail
		if (angleTail < 40) angleTail += 2;
		glutPostRedisplay();
		break;
	case '0'://Close angle tail
		if (angleTail > 20) angleTail -= 2;
		glutPostRedisplay();
		break;
	case 'q'://Open angle back left leg
		if (leftLegs[0] < 10) leftLegs[0] += 1;
		glutPostRedisplay();
		break;
	case 'w'://Close angle back left leg
		if (leftLegs[0] > -5) leftLegs[0] -= 1;
		glutPostRedisplay();
		break;
	case 'e'://Open angle second back left leg
		if (leftLegs[1] < 10) leftLegs[1] += 1;
		glutPostRedisplay();
		break;
	case 'r'://Close angle second back left leg
		if (leftLegs[1] > -5) leftLegs[1] -= 1;
		glutPostRedisplay();
		break;
	case 't'://Open angle second front left leg
		if (leftLegs[2] < 10) leftLegs[2] += 1;
		glutPostRedisplay();
		break;
	case 'y'://Close angle second front left leg
		if (leftLegs[2] > -5) leftLegs[2] -= 1;
		glutPostRedisplay();
		break;
	case 'u'://Open angle front left leg
		if (leftLegs[3] < 10) leftLegs[3] += 1;
		glutPostRedisplay();
		break;
	case 'i'://Close angle front left leg
		if (leftLegs[3] > -5) leftLegs[3] -= 1;
		glutPostRedisplay();
		break;
	case 'a'://Open angle back right leg
		if (rightLegs[0] < 10) rightLegs[0] += 1;
		glutPostRedisplay();
		break;
	case 's'://Close angle back right leg
		if (rightLegs[0] > -5) rightLegs[0] -= 1;
		glutPostRedisplay();
		break;
	case 'd'://Open angle second back right leg
		if (rightLegs[1] < 10) rightLegs[1] += 1;
		glutPostRedisplay();
		break;
	case 'f'://Close angle second back right leg
		if (rightLegs[1] > -5) rightLegs[1] -= 1;
		glutPostRedisplay();
		break;
	case 'g'://Open angle second front right leg
		if (rightLegs[2] < 10) rightLegs[2] += 1;
		glutPostRedisplay();
		break;
	case 'h'://Close angle second front right leg
		if (rightLegs[2] > -5) rightLegs[2] -= 1;
		glutPostRedisplay();
		break;
	case 'j'://Open angle front right leg
		if (rightLegs[3] < 10) rightLegs[3] += 1;
		glutPostRedisplay();
		break;
	case 'k'://Close angle front right leg
		if (rightLegs[3] > -5) rightLegs[3] -= 1;
		glutPostRedisplay();
		break;
	case 'z'://Activate/Deactivate textures
		textureOn = !textureOn;
		glutPostRedisplay();
		break;
	case 32://Walk movement. Hold for animation
		if (walkingState < 4 )
		{
			walkLeftLegs[3] -= 3.5;
			walkLeftLegs[2] += 3.5;
		}
		else if (walkingState < 8 )
		{
			walkRightLegs[3] -= 3.5;
			walkRightLegs[2] += 3.5;
		}
		else if (walkingState < 12 )
		{
			walkLeftLegs[2] -= 7;
			walkLeftLegs[1] += 3.5;
		}
		else if (walkingState < 16)
		{
			walkRightLegs[2] -= 7;
			walkRightLegs[1] += 3.5;
		}
		else if (walkingState < 20)
		{
			walkLeftLegs[1] -= 7;
			walkLeftLegs[0] += 3.5;
		}
		else if (walkingState < 24)
		{
			walkRightLegs[1] -= 7;
			walkRightLegs[0] += 3.5;
		}
		else if (walkingState < 28)
		{
			walkLeftLegs[3] += 3.5;
			walkLeftLegs[2] += 3.5;
			walkLeftLegs[1] += 3.5;
			walkLeftLegs[0] -= 3.5;
		}
		else if (walkingState < 32)
		{
			walkRightLegs[3] += 3.5;
			walkRightLegs[2] += 3.5;
			walkRightLegs[1] += 3.5;
			walkRightLegs[0] -= 3.5;
		}
		walkingState = (walkingState + 1) % 32;
		glutPostRedisplay();
		break;
	}
}

//Function to handle mouse events
void mouseHandler(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
		if (state == GLUT_DOWN) {  // Zoom-out
			if (eyeDistance < 50) eyeDistance += 1;
		}
	if (button == GLUT_RIGHT_BUTTON)
		if (state == GLUT_DOWN) {  // Zoom-in
			if (eyeDistance > 0) eyeDistance -= 1;
		}
	glutPostRedisplay();
}

void resizeHandler(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (float)w / (float)h, 1.0, 50.0);
}

//Change the current color according to the texture to be used
void setColorTexture(GLuint _textureId) {
	if (_textureId == _textureIdDark)
		glColor3f(0.6f, 0.5f, 0.1f);
	else if (_textureId == _textureIdLight)
		glColor3f(0.9f, 0.74f, 0.0f);
	else
		glColor3f(1.0f, 1.0f, 1.0f);
}

//Draw Cylinder using current texture or color
void drawCylinder(float diameterBegin, float diameterEnd, float length, GLuint _textureId) {
	if (textureOn && _textureId != -1) {
		glBindTexture(GL_TEXTURE_2D, _textureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadCylinder, 1);
		setColorTexture(_textureId);
	}
	else
		gluQuadricTexture(quadCylinder, 0);
	gluCylinder(quadCylinder, diameterBegin, diameterEnd, length, 40.0, length*30.0);
}

//Draw Sphere using current texture or color
void drawSphere(float diameter, GLuint _textureId) {
	if (textureOn && _textureId != -1) {
		glBindTexture(GL_TEXTURE_2D, _textureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadSphere, 1);
		setColorTexture(_textureId);
	}
	else
	{
		gluQuadricTexture(quadSphere, 0);
	}

	gluSphere(quadSphere, diameter, 40.0, 40.0);
}

//Draw main figure body
void drawBody() {
	glColor3f(0.4f, 0.2f, 0.0f);

	glPushMatrix();
	glScalef(0.6, 0.8, 2);
	drawSphere(4.0, _textureIdDark);
	glPopMatrix();

	//right eye
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(2, -1, -4.2);
	gluSphere(quadSphere, 0.25, 10.0, 10.0);

	//right teeth
	glColor3f(0.8, 0.45, 0.1);
	glTranslatef(-1.5, 0, -3);
	drawSphere(0.7, _textureIdLight);

	glTranslatef(0.0, 0.0, -0.5);
	glRotatef(200, 1, 0, 0);
	drawCylinder(0.5, 0, 1.5, _textureIdLight);
	glPopMatrix();

	//left eye	
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(2, 1, -4.2);
	gluSphere(quadSphere, 0.25, 10.0, 10.0);

	//left teeth
	glColor3f(0.8, 0.45, 0.1);
	glTranslatef(-1.5, 0, -3);
	drawSphere(0.7, _textureIdLight);

	glTranslatef(0.0, 0.0, -0.5);
	glRotatef(160, 1, 0, 0);
	drawCylinder(0.5, 0, 1.5, _textureIdLight);
	glPopMatrix();
}

void drawTail() {

	glPushMatrix();

	glColor3f(0.55f, 0.22f, 0.0f);
	glTranslatef(0.0, 0.0, 8);

	//Draw each intermediate part of the tail
	for (int i = 1; i <= 5; i++)
	{
		drawSphere(0.8, _textureIdRed);
		glTranslatef(0.0, 0.0, 0.4);
		glRotatef(angleTail, 0, 1, 0);
		drawCylinder(1.0, 1.0, 1.8, _textureIdRed);
		glTranslatef(0.0, 0.0, 1.6);
	}

	gluSphere(quadSphere, 0.8, 10.0, 10.0);
	glTranslatef(0.0, 0.0, 0.4);
	glRotatef(angleTail, 0, 1, 0);
	drawCylinder(0.8, 0.0, 3.0, _textureIdRed);

	glPopMatrix();
}

//First parameter is used to invert values to draw for left and right
void drawArm(int sideChooser)
{
	float angleArm;
	float angleClaw;
	if (sideChooser == -1) angleArm = rightArm;
	else angleArm = leftArm;

	//First arm segment
	glPushMatrix();
	glColor3f(0.75f, 0.29f, 0.0f);
	glTranslatef(0.4, sideChooser * 2.6, -5.4);
	drawSphere(0.8, _textureIdRed);

	//Second segment
	glRotatef(245 * sideChooser + angleArm * 2, 1, 0, 0);
	glRotatef(20, 0, 1, 0);
	glTranslatef(0, 0, 0.3);
	gluCylinder(quadCylinder, 0.8, 0.6, 2.8, 10, 10);
	glTranslatef(0.0, 0.0, 3.0);
	drawSphere(0.9, _textureIdRed);

	//Last segment
	glRotatef(-20, 0, 1, 0);
	glRotatef(315 * sideChooser + angleArm, 1, 0, 0);
	drawCylinder(0.8, 0.6, 3.3, _textureIdRed);
	glTranslatef(0.0, 0.0, 4);
	gluSphere(quadSphere, 0.9, 10.0, 10.0);

	if (sideChooser == -1) angleClaw = rightClaw;
	else angleClaw = leftClaw;

	//Claw
	glPushMatrix();
	glRotatef(sideChooser * 315, 1, 0, 0);
	glTranslatef(0, 0.0, 0.8);
	glScalef(1, 1, 1.5);
	drawSphere(1.0, _textureIdRed);

	glPushMatrix();
	glRotatef(angleClaw, 1, 0, 0);
	glTranslatef(0, 0.0, 0.8);
	drawCylinder(0.5, 0.0, 2.0, _textureIdRed);
	glPopMatrix();

	glPushMatrix();
	glRotatef(angleClaw * -1, 1, 0, 0);
	glTranslatef(0, 0.0, 0.8);
	drawCylinder(0.5, 0.0, 2.0, _textureIdRed);
	glPopMatrix();

	glPopMatrix();

	glPopMatrix();
}

//First parameter is used to invert values to draw for left and right
void drawLeg(int sideChooser, float angleLeg, float y, float z, float walkAngle)
{
	glPushMatrix();
	glColor3f(0.8, 0.45, 0.1);

	//First segment
	glTranslatef(0, y, z);
	drawSphere(0.5, _textureIdLight);
	glRotatef(sideChooser * 270 + sideChooser * walkAngle, 1, 0, 0);
	glTranslatef(0, 0.0, 0.4);
	glRotatef(45 - angleLeg, 0, 1, 0);
	drawCylinder(0.5, 0.5, 3, _textureIdLight);

	//Second segment
	glTranslatef(0, 0, 3);
	drawSphere(0.5, _textureIdLight);
	glRotatef(270 + angleLeg * 4, 0, 1, 0);
	glRotatef(sideChooser * walkAngle, 1, 0, 0);
	glTranslatef(0, 0.0, 0);
	drawCylinder(0.5, 0.5, 2, _textureIdLight);

	//Third segment
	glTranslatef(0, 0, 2);
	gluSphere(quadSphere, 0.5, 10.0, 10.0);
	glRotatef(310 + angleLeg * 3, 0, 1, 0);
	drawCylinder(0.5, 0.3, 2, _textureIdLight);

	glPopMatrix();
}

//Main drawing function
void drawScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (textureOn)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Calculate camera
	eyeX = eyeDistance * cos(viewAngleZ*PI / 180) * cos(viewAngleX*PI / 180);
	eyeY = eyeDistance * cos(viewAngleZ*PI / 180) * sin(viewAngleX*PI / 180);
	eyeZ = eyeDistance * sin(viewAngleZ*PI / 180);

	if (viewAngleZ < 90 && viewAngleZ > -90)
		gluLookAt(eyeX, eyeY, eyeZ, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	else
		gluLookAt(eyeX, eyeY, eyeZ, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0);

	//Change matrix to match camera view
	glRotatef(90, 0, 0, 1);
	glRotatef(270, 0, 1, 0);

	glLightfv(GL_LIGHT0, GL_POSITION, lightposition);

	// clears color and material parameters before begin
	glColor3f(1.0f, 1.0f, 1.0f);
	
	//Begin drawing
	drawBody();

	drawTail();

	//left arm and claw
	drawArm(1);

	//right arm and claw
	drawArm(-1);

	//right legs
	drawLeg(-1, rightLegs[0], -2.6, 4.5, walkLeftLegs[0]);
	drawLeg(-1, rightLegs[1], -2.9, 2.0, walkLeftLegs[1]);
	drawLeg(-1, rightLegs[2], -3.1, -0.5, walkLeftLegs[2]);
	drawLeg(-1, rightLegs[3], -2.9, -3.0, walkLeftLegs[3]);

	//left legs
	drawLeg(1, leftLegs[0], 2.6, 4.5, walkRightLegs[0]);
	drawLeg(1, leftLegs[1], 2.9, 2.0, walkRightLegs[1]);
	drawLeg(1, leftLegs[2], 3.1, -0.5, walkRightLegs[2]);
	drawLeg(1, leftLegs[3], 2.9, -3.0, walkRightLegs[3]);

	glutSwapBuffers();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Scorpion-Felipe_Pestana-Rodrigo_Dantas");

	initialize();
	initRendering();
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(keyboardHandler);
	glutSpecialFunc(specialKeyboardHandler);
	glutMouseFunc(mouseHandler);
	glutReshapeFunc(resizeHandler);

	glutMainLoop();
	return 0;
}
