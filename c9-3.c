/************************************************************************/
/*       Program   "c8-3.c"                                             */
/*                                                                      */
/*       Copyright (c) 2003 by Kiyotaka Kato and Takefumi Hayashi       */
/*                       All rights reserved                            */
/*                                                                      */
/************************************************************************/
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#define KEY_ESC 27

#define IMAGE_HEIGHT	64
#define IMAGE_WIDTH		64

void polarview( void );
void resetview( void );

float xOrigin = 0.0, yOrigin = 0.0; 
int xBegin = 0, yBegin = 0;
float distance, twist, elevation, azimuth;
float nearw = 1.0, farw = 30.0, fovy = 60.0;
int mButton;
float tval=0.0;
unsigned int texname=1;

unsigned char image1[IMAGE_HEIGHT][IMAGE_WIDTH][4];
void MakeImage1(void)
{
	int i, j, color;
	glBindTexture(GL_TEXTURE_2D, texname);
    for (i = 0; i < IMAGE_HEIGHT; i++) {
        for (j = 0; j < IMAGE_WIDTH; j++) {
            color = ((((i&0x8)==0)^((j&0x8)==0)))*255;
            image1[i][j][0] = (unsigned char)color;
            image1[i][j][1] = (unsigned char)color;
            image1[i][j][2] = (unsigned char)0;
            image1[i][j][3] = (unsigned char)100;
        }
    }
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 4,IMAGE_WIDTH ,IMAGE_HEIGHT,0,
								GL_RGBA, GL_UNSIGNED_BYTE, image1);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL); 

}

void display(void)
{
	float diffuse[] = { 0.0, 0.0, 1.0, 1.0 };
	float specular[] = { 1.0, 1.0, 1.0, 1.0 };
	float ambient[] = { 0.1, 0.1, 0.1, 1.0 };

	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glPushMatrix();
	polarview();
    glEnable( GL_DEPTH_TEST );

	glMaterialfv( GL_FRONT, GL_DIFFUSE, diffuse );
	glMaterialfv( GL_FRONT, GL_SPECULAR, specular );
	glMaterialfv( GL_FRONT, GL_AMBIENT, ambient );
	glMaterialf( GL_FRONT, GL_SHININESS, 128.0 );
    glEnable( GL_LIGHTING );
//	glTranslatef(0.0,tval,0.0);
//
	glTranslatef(0.0,0.6,0.0);
	glutSolidTeapot(1.0);
//	glutSolidCube(1.0);
//	glutSolidSphere(1.0,20,20);
//
	glPopMatrix();
	glDisable( GL_LIGHTING );
    glDisable( GL_DEPTH_TEST );
	glutSwapBuffers();
}


void SetupTextures(void)
{

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//
	MakeImage1();
	glTexGenf(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
	glTexGenf(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
//	glTexGenf(GL_S,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
//	glTexGenf(GL_T,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
//	glTexGenf(GL_S,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
//	glTexGenf(GL_T,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glEnable(GL_TEXTURE_2D);
}

void DeleteTextures(void)
{
	glDeleteTextures(1,&texname);
}

void myKbd(unsigned char key, int x, int y)
{
	switch(key) {
    case KEY_ESC:
		DeleteTextures();
        exit( 0 );
  	default:
   		break;
	}
}

void myMouse( int button, int state, int x, int y )
{
    if (state == GLUT_DOWN) {
        xBegin = x;
        yBegin = y;
		mButton=button;
    }
}


void myMotion(int x, int y)
{
    int xDisp, yDisp;

    xDisp = x - xBegin;
    yDisp = y - yBegin;
    switch(mButton){
    case GLUT_LEFT_BUTTON:
        azimuth += (float) xDisp/2.0;
        elevation -= (float) yDisp/2.0;
        break;
    case GLUT_MIDDLE_BUTTON:
        twist = fmod (twist + xDisp, 360.0);
        break;
    case GLUT_RIGHT_BUTTON:
         distance -= (float) yDisp/40.0;
         break;
    }
    xBegin = x;
    yBegin = y;
    glutPostRedisplay();
}


void myInit(char *progname)
{
    glutInitWindowPosition(0, 0);
	glutInitWindowSize( 400, 400);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow(progname);
    glClearColor (0.0, 0.0, 0.0, 1.0);
    glutKeyboardFunc(myKbd);
    glutMouseFunc(myMouse);
    glutMotionFunc(myMotion);
	resetview();
	SetupTextures();
	glShadeModel( GL_SMOOTH );
	glEnable( GL_LIGHT0 );
}


void myReshape(int width, int height)
{
    float aspect = (float) width / (float) height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovy, aspect, nearw, farw);
    glMatrixMode(GL_MODELVIEW);
}


void polarview( void )
{
    glTranslatef( 0.0, 0.0, -distance);
    glRotatef( -twist, 0.0, 0.0, 1.0);
    glRotatef( -elevation, 1.0, 0.0, 0.0);
    glRotatef( -azimuth, 0.0, 1.0, 0.0);
}


void resetview( void )
{
	distance = 5.0;
    twist = 0.0;
    elevation = -45.0;
    azimuth = 30.0;
}

int tflag=0;
void idle( void )
{
	if (tflag) {
		tval-=0.05;
		if (tval<0.0) tflag=0;
	}
	else {
		tval+=0.05;
		if (tval> 2.0) tflag=1;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	myInit(argv[0]);
	glutReshapeFunc (myReshape);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutMainLoop();
	return(0);
}