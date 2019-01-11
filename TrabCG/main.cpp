//#include <windows.h>
#include <iostream>
//#include <stdlib.h>
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

//Ilumination variables
GLfloat lightposition[4] = { 50.0f, 0.0f, 0.0f, 0.0 };
GLfloat luzAmbiente[4] = { 0.19, 0.19, 0.19, 0.0 };
GLfloat luzDifusa[4] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat luzEspecular[4] = { 0.5, 0.5, 0.5, 1.0 };

//defines light source (LIGHT0)
void initialize(void)
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	// Especifica que a cor de fundo da janela será preta
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Habilita o modelo de colorização de Gouraud
	glShadeModel(GL_SMOOTH);

	// Ativa o uso da luz ambiente 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

	// Define os parâmetros da luz de número 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
	glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
	glLightfv(GL_LIGHT0, GL_POSITION, lightposition);

	// Materials will follow current color
	glEnable(GL_COLOR_MATERIAL);
}

//makes the image into a texture, and returns the id of the texture
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
		case '1':
			if (leftArm < 14) leftArm += 2;		
			glutPostRedisplay();
			break;
		case '2':
			if (leftArm > -8) leftArm -= 2;
			glutPostRedisplay();
			break;
		case '3':
			if (rightArm > -14) rightArm -= 2;
			glutPostRedisplay();
			break;
		case '4':
			if (rightArm < 8) rightArm += 2;
			glutPostRedisplay();
			break;
		case '5':
			if (leftClaw < 40) leftClaw += 2;
			glutPostRedisplay();
			break;
		case '6':
			if (leftClaw > 8) leftClaw -= 2;
			glutPostRedisplay();
			break;
		case '7':
			if (rightClaw < 40) rightClaw += 2;
			glutPostRedisplay();
			break;
		case '8':
			if (rightClaw > 8) rightClaw -= 2;
			glutPostRedisplay();
			break;
		case '9':		
			if (angleTail < 40) angleTail += 2;
			glutPostRedisplay();
			break;
		case '0':
			if (angleTail > 20) angleTail -= 2;
			glutPostRedisplay();
			break;
		case 'q':
			if (leftLegs[0] < 10) leftLegs[0] += 1;
			glutPostRedisplay();
			break;
		case 'w':
			if (leftLegs[0] > -5) leftLegs[0] -= 1;
			glutPostRedisplay();
			break;
		case 'e':
			if (leftLegs[1] < 10) leftLegs[1] += 1;
			glutPostRedisplay();
			break;
		case 'r':
			if (leftLegs[1] > -5) leftLegs[1] -= 1;
			glutPostRedisplay();
			break;
		case 't':
			if (leftLegs[2] < 10) leftLegs[2] += 1;
			glutPostRedisplay();
			break;
		case 'y':
			if (leftLegs[2] > -5) leftLegs[2] -= 1;
			glutPostRedisplay();
			break;
		case 'u':
			if (leftLegs[3] < 10) leftLegs[3] += 1;
			glutPostRedisplay();
			break;
		case 'i':
			if (leftLegs[3] > -5) leftLegs[3] -= 1;
			glutPostRedisplay();
			break;
		case 'a':
			if (rightLegs[0] < 10) rightLegs[0] += 1;
			glutPostRedisplay();
			break;
		case 's':
			if (rightLegs[0] > -5) rightLegs[0] -= 1;
			glutPostRedisplay();
			break;
		case 'd':
			if (rightLegs[1] < 10) rightLegs[1] += 1;
			glutPostRedisplay();
			break;
		case 'f':
			if (rightLegs[1] > -5) rightLegs[1] -= 1;
			glutPostRedisplay();
			break;
		case 'g':
			if (rightLegs[2] < 10) rightLegs[2] += 1;
			glutPostRedisplay();
			break;
		case 'h':
			if (rightLegs[2] > -5) rightLegs[2] -= 1;
			glutPostRedisplay();
			break;
		case 'j':
			if (rightLegs[3] < 10) rightLegs[3] += 1;
			glutPostRedisplay();
			break;
		case 'k':
			if (rightLegs[3] > -5) rightLegs[3] -= 1;
			glutPostRedisplay();
			break;
		case 'z':
			textureOn = !textureOn;
			glutPostRedisplay();
			break;
		case 32:
			break;
	}
	cout << "Test " << textureOn << endl;
}

// Função callback chamada para gerenciar eventos do mouse
void mouseHandler(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
		if (state == GLUT_DOWN) {  // Zoom-out
			if (eyeDistance < 50) eyeDistance += 1;
		}
	if (button == GLUT_RIGHT_BUTTON)
		if (state == GLUT_DOWN) {  // Zoom-out
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
		glColor3f(0.6f, 0.4f, 0.0f);
	else if (_textureId == _textureIdLight)
		glColor3f(0.9f, 0.74f, 0.0f);
	else
		glColor3f(1.0f, 1.0f, 1.0f);
}

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

void drawBody() {
	glColor3f(0.4f, 0.2f, 0.0f);

	glPushMatrix();	
	glScalef(0.6, 0.8, 2);
	//gluSphere(quadSphere, 4, 40.0, 40.0);
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
	//gluSphere(quadSphere, 0.7, 10.0, 10.0);

	glTranslatef(0.0, 0.0, -0.5);
	glRotatef(200, 1, 0, 0);
	drawCylinder(0.5, 0, 1.5, _textureIdLight);
	//gluCylinder(quadCylinder, , 10.0, 10.0);
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
	//gluSphere(quadSphere, 0.7, 10.0, 10.0);

	glTranslatef(0.0, 0.0, -0.5);
	glRotatef(160, 1, 0, 0);
	drawCylinder(0.5, 0, 1.5, _textureIdLight);
	//gluCylinder(quadCylinder, 0.5, 0, 1.5, 10.0, 10.0);
	glPopMatrix();
}

void drawTail()	{

	glPushMatrix();

	glColor3f(0.55f, 0.22f, 0.0f);
	glTranslatef(0.0, 0.0, 8);	

	//Draw each intermediate part of the tail
	for (int i = 1; i <= 5; i++)
	{
		drawSphere(0.8, _textureIdRed);
		//gluSphere(quadSphere, 0.8, 10.0, 10.0);
		glTranslatef(0.0, 0.0, 0.4);
		glRotatef(angleTail, 0, 1, 0);
		drawCylinder(1.0, 1.0, 1.8, _textureIdRed);
		//gluCylinder(quadCylinder, 1, 1, 1.8, 10, 10);
		glTranslatef(0.0, 0.0, 1.6);
	}

	gluSphere(quadSphere, 0.8, 10.0, 10.0);
	glTranslatef(0.0, 0.0, 0.4);
	glRotatef(angleTail, 0, 1, 0);
	drawCylinder(0.8, 0.0, 3.0, _textureIdRed);
	//gluCylinder(quadCylinder, 0.8, 0, 3, 10, 10);

	glPopMatrix();
}

//First parameter is used to invert values to draw for left and right
void drawArm(int sideChooser)
{
	float angleArm;
	float angleClaw;
	if (sideChooser == -1) angleArm = rightArm; 
	else angleArm = leftArm; 

	//arm
	glPushMatrix();
	glColor3f(0.75f, 0.29f, 0.0f);
	glTranslatef(0.4, sideChooser * 2.6, -5.3);
	drawSphere(0.8, _textureIdRed);
	//gluSphere(quadSphere, 0.8, 10.0, 10.0);

	glRotatef(245 * sideChooser + angleArm * 2, 1, 0, 0);
	glRotatef(20, 0, 1, 0);
	glTranslatef(0, 0, 0.3);
	gluCylinder(quadCylinder, 0.8, 0.6, 2.8, 10, 10);
	glTranslatef(0.0, 0.0, 3.0);
	drawSphere(0.9, _textureIdRed);
	//gluSphere(quadSphere, 0.9, 10.0, 10.0);

	glRotatef(-20, 0, 1, 0);
	glRotatef(315 * sideChooser + angleArm, 1, 0, 0);
	drawCylinder(0.8, 0.6, 3.3, _textureIdRed);
	//gluCylinder(quadCylinder, 0.8, 0.6, 3.3, 10, 10);
	glTranslatef(0.0, 0.0, 4);
	gluSphere(quadSphere, 0.9, 10.0, 10.0);

	if (sideChooser == -1) angleClaw = rightClaw;
	else angleClaw = leftClaw;

	//claw
	glPushMatrix();
	glRotatef(sideChooser * 315, 1, 0, 0);
	glTranslatef(0, 0.0, 0.8);
	glScalef(1, 1, 1.5);
	drawSphere(1.0, _textureIdRed);
	//gluSphere(quadSphere, 1, 10.0, 10.0);

	glPushMatrix();
	glRotatef(angleClaw, 1, 0, 0);
	glTranslatef(0, 0.0, 0.8);
	drawCylinder(0.5, 0.0, 2.0, _textureIdRed);
	//gluCylinder(quadCylinder, 0.5, 0, 2, 10.0, 10.0);
	glPopMatrix();

	glPushMatrix();
	glRotatef(angleClaw * -1, 1, 0, 0);
	glTranslatef(0, 0.0, 0.8);
	drawCylinder(0.5, 0.0, 2.0, _textureIdRed);
	//gluCylinder(quadCylinder, 0.5, 0, 2, 10.0, 10.0);
	glPopMatrix();

	glPopMatrix();

	glPopMatrix();
}

void drawLeg(int sideChooser, float angleLeg, float y, float z)
{	
	glPushMatrix();
	glColor3f(0.8, 0.45, 0.1);	

	//First segment
	glTranslatef(0, y, z);
	drawSphere(0.5, _textureIdLight);
	//gluSphere(quadSphere, 0.5, 10.0, 10.0);
	glRotatef(270 * sideChooser, 1, 0, 0);
	glTranslatef(0, 0.0, 0.4);
	glRotatef(45 - angleLeg , 0, 1, 0);
	drawCylinder(0.5, 0.5, 3, _textureIdLight);
	//gluCylinder(quadCylinder, 0.5, 0.5, 3, 10.0, 10.0);

	//Second segment
	glTranslatef(0, 0, 3);
	drawSphere(0.5, _textureIdLight);
	//gluSphere(quadSphere, 0.5, 10.0, 10.0);
	glRotatef(270 + angleLeg * 4, 0, 1, 0);
	glTranslatef(0, 0.0, 0);
	drawCylinder(0.5, 0.5, 2, _textureIdLight);
	//gluCylinder(quadCylinder, 0.5, 0.5, 2, 10.0, 10.0);

	//Third segment
	glTranslatef(0, 0, 2);
	gluSphere(quadSphere, 0.5, 10.0, 10.0);
	glRotatef(310 + angleLeg *3, 0, 1, 0);
	drawCylinder(0.5, 0.3, 2, _textureIdLight);

	//gluCylinder(quadCylinder, 0.5, 0.3, 2, 10.0, 10.0);

	glPopMatrix();
}

void drawScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
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
	
	/*glMaterialfv(GL_FRONT, GL_SPECULAR, especularidade);
	glMateriali(GL_FRONT, GL_SHININESS, especMaterialNoTexture);

	glMaterialfv(GL_FRONT, GL_SPECULAR, semEspecularidade);
	glMateriali(GL_FRONT, GL_SHININESS, 0);

	if (textureOn) {
		glColor3f(0.9f, 0.9f, 0.9f);
		glMaterialfv(GL_FRONT, GL_SPECULAR, especularidade);
		glMateriali(GL_FRONT, GL_SHININESS, especMaterialTexture);
	}
	else {
		glColor3f(0.8f, 0.9f, 1.0f);
		glMaterialfv(GL_FRONT, GL_SPECULAR, especularidade);
		glMateriali(GL_FRONT, GL_SHININESS, especMaterialNoTexture);
	}*/
	
	drawBody();

	drawTail();

	//left arm and claw
	drawArm(1);

	//right arm and claw
	drawArm(-1);

	//right legs
	drawLeg(-1, rightLegs[0], -2.6, 4.5);
	drawLeg(-1, rightLegs[1], -2.9, 2.0);
	drawLeg(-1, rightLegs[2], -3.1, -0.5);
	drawLeg(-1, rightLegs[3], -2.9, -3.0);

	//left legs
	drawLeg(1, leftLegs[0], 2.6, 4.5);
	drawLeg(1, leftLegs[1], 2.9, 2.0);
	drawLeg(1, leftLegs[2], 3.1, -0.5);
	drawLeg(1, leftLegs[3], 2.9, -3.0);

	glutSwapBuffers();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Scorpion");

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
