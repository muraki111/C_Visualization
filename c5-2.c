#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#define KEY_ESC 27

void polarview(void);
void resetview(void);
unsigned char wireFlag = GL_TRUE;
unsigned char revolveFlag = GL_FALSE;
int xBegin,yBegin;
int mButton;
float distance,twist,elevation,azimuth;
float theta =15.0;

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();
	polarview();
	glRotatef(theta,0.0,1.0,0.0);
	glColor3f(1.0,1.0,1.0);
	if(wireFlag ==GL_TRUE)
		glutWireCube(1.0);
	else
		glutSolidCube(1.0);
	glPopMatrix();
	glutSwapBuffers();
}

void idle(void)
{
	theta = fmod(theta+0.5,360.);
	glutPostRedisplay();
}

void myKbd(unsigned char key,int x, int y)
{
	switch(key){
	case 'w':
		wireFlag = !wireFlag;
		break;
	case 'R':
		resetview();
		break;
	case KEY_ESC:
		exit(0);
	}
	glutPostRedisplay();
}

void myMouse(int button,int state,int x,int y)
{
	if(state == GLUT_DOWN){
		switch(button){
		case GLUT_LEFT_BUTTON:
			mButton = button;
			break;
		case GLUT_MIDDLE_BUTTON:
			revolveFlag = !revolveFlag;
			if(revolveFlag == GL_TRUE)
				glutIdleFunc(idle);
			else
				glutIdleFunc(NULL);
			break;
		case GLUT_RIGHT_BUTTON:
			mButton = button;
			break;
		}
		xBegin = x;
		yBegin = y;
	}
}

void myMotion( int x, int y)
{
	int xDisp, yDisp;

	xDisp = x- xBegin;
	yDisp = y -yBegin;

	switch(mButton){
	case GLUT_LEFT_BUTTON:
		azimuth += (float)xDisp/2.0;
		elevation -=(float)yDisp/2.0;
		break;
	case GLUT_RIGHT_BUTTON:
		distance +=(float)yDisp/40.0;
		break;
	}
	xBegin = x;
	yBegin = y;
	glutPostRedisplay();
}

void myInit(char *progname)
{
	int width=600,height=600;
	float aspect =(float)width/(float)height;
	glutInitWindowPosition(0,0);
	glutInitWindowSize(width,height);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
	glutCreateWindow(progname);
	glClearColor(0.0,0.0,0.0,1.0);

	glutKeyboardFunc(myKbd);
	glutMouseFunc(myMouse);
	glutMotionFunc(myMotion);
	resetview();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0,aspect,1.0,20.0);
	glMatrixMode(GL_MODELVIEW);
}

void resetview(void)
{
	distance =5.0;
	twist = 0.0;
	elevation = 0.0;
	azimuth = 0.0;
}

void polarview(void)
{
	glTranslatef(0.0,0.0,-distance);
	glRotatef(-twist,0.0,0.0,1.0);
	glRotatef(-elevation,1.0,0.0,0.0);
	glRotatef(-azimuth,0.0,1.0,0.0);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	myInit(argv[0]);
	glutDisplayFunc(display);
	glutIdleFunc(NULL);
	glutMainLoop();
	return(0);
}