/************************************************************************/
/*       Program   "c8-1.c"                                             */
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
#define imageWidth 8
#define imageHeight 8

void polarview( void );
void resetview( void );
unsigned char image[imageHeight][imageWidth][4];
float xOrigin = 0.0, yOrigin = 0.0;
int xBegin = 0, yBegin = 0;
int mButton;
float distance, twist, elevation, azimuth;
float znear = 1.0, zfar = 80.0, fovy = 40.0;


unsigned char bitmap1[imageHeight][imageWidth]=
{
	{0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00},
	{0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00},
	{0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00},
	{0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff},
	{0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff},
	{0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff},
	{0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff}
};
unsigned char bitmap2[imageHeight][imageWidth]=
{
	{0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x00},
	{0x00,0x00,0xff,0xff,0xff,0xff,0x00,0x00},
	{0x00,0xff,0xff,0xff,0xff,0xff,0xff,0x00},
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},
	{0x00,0xff,0xff,0xff,0xff,0xff,0xff,0x00},
	{0x00,0x00,0xff,0xff,0xff,0xff,0x00,0x00},
	{0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x00}
};

void display(void)
{
	float diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
    float specular[] = { 1.0, 1.0, 1.0, 1.0 };
    float ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	float texcolor[] = { 1.0, 1.0, 1.0, 1.0 };

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, texcolor);
	glEnable(GL_TEXTURE_2D);

	glPushMatrix ();
	polarview();

	glMaterialfv( GL_FRONT, GL_DIFFUSE, diffuse );
	glMaterialfv( GL_FRONT, GL_SPECULAR, specular );
	glMaterialfv( GL_FRONT, GL_AMBIENT, ambient );
	glMaterialf( GL_FRONT, GL_SHININESS, 128.0 );

    glEnable( GL_LIGHTING );
	glEnable( GL_DEPTH_TEST );

	glNormal3f(0.0,1.0,0.0);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0,0.0); glVertex3f(1.0,0.0,1.0);
		glTexCoord2f(0.0,5.0); glVertex3f(1.0,0.0,-1.0);
		glTexCoord2f(5.0,5.0); glVertex3f(-1.0,0.0,-1.0);
		glTexCoord2f(5.0,0.0); glVertex3f(-1.0,0.0,1.0);
	glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glutSwapBuffers();
}

void myKbd( unsigned char key, int x, int y )
{
	switch(key) {
	case 'R':
        resetview();
		break;
    case 's':
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        break;
    case 'S':
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        break;
    case 't':
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        break;
    case 'T':
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        break;
    case KEY_ESC:
        exit( 0 );
    default :
		break;
    }
	glutPostRedisplay();
}


void myMouse( int button, int state, int x, int y )
{
    if (state == GLUT_DOWN) {
        xBegin = x;
        yBegin = y;
		mButton = button;
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
        twist = fmod (twist + xDisp/3.0, 360.0);
        break;
    case GLUT_RIGHT_BUTTON:
        distance -= (float) yDisp/60.0;
        break;
    }
    xBegin = x;
    yBegin = y;
    glutPostRedisplay();
}

void makeImage(void)
{
	int i,j;
	for (i = 0; i < imageHeight; i++) {
   		for (j = 0; j < imageWidth; j++) {
        	image[i][j][0] = (unsigned char) bitmap1[i][j];			// bitmap2�ɕς�����
        	image[i][j][1] = (unsigned char) bitmap1[i][j];			// bitmap1�̓��e��ς��Ă݂�
        	image[i][j][2] = (unsigned char) bitmap1[i][j];
        	image[i][j][3] = (unsigned char) 50;
     	}
	}
}


void initTexture(void)
{
	makeImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	/* GL_LINEAR�ɕς��Ă݂� */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  /* GL_LINEAR�ɕς��Ă݂� */
	glTexImage2D(GL_TEXTURE_2D, 0, 4, imageWidth, imageHeight,
                0, GL_RGBA, GL_UNSIGNED_BYTE, image);
}


void myInit (char *progname)
{
    int width = 400, height = 400;

    glutInitWindowPosition(0, 0); glutInitWindowSize( width, height);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow(progname);
    glClearColor (0.0, 0.0, 0.0, 1.0);
    glutKeyboardFunc( myKbd );
    glutMouseFunc( myMouse );
    glutMotionFunc( myMotion );
	initTexture();
	resetview();
	glEnable( GL_LIGHT0 );
}


void myReshape(int width, int height)
{
    float aspect;

    aspect = (float) width / (float) height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovy, aspect, znear, zfar);
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
	distance = 6.0;
	twist = 0.0;
	elevation = -45.0;
	azimuth = 30.0;
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	myInit(argv[0]);
	glutReshapeFunc (myReshape);
	glutDisplayFunc(display);
	glutMainLoop();
	return(0);
}
