/*  c7-2.c   Copyright (c) 2003 by T. HAYASHI and K. KATO  */
/*                                    All rights reserved  */
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <math.h>
#define KEY_ESC 27

void polarview( void );
void resetview( void );
unsigned char rotateFlag = GL_TRUE;
float distance, twist, elevation, azimuth;
float spin = 0.0;

void display(void)
{
    float diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    float specular[] = { 0.8, 0.8, 0.8, 1.0 };
    float ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	float light0[] = { -3.0, 0.0, 0.0, 1.0 };
	float light1[] = { 0.0, 1.0, 0.0, 0.0 };
				 
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glEnable( GL_DEPTH_TEST );
	glEnable( GL_LIGHTING );

	glPushMatrix();
	polarview();
	glLightfv( GL_LIGHT1, GL_POSITION, light1 );
	glPushMatrix();
	glRotatef (spin, 0.0, 1.0, 0.0);
    glLightfv( GL_LIGHT0, GL_POSITION, light0 );
	glPopMatrix();
	glMaterialfv( GL_FRONT, GL_DIFFUSE, diffuse );
	glMaterialfv( GL_FRONT, GL_SPECULAR, specular );
	glMaterialfv( GL_FRONT, GL_AMBIENT, ambient );
	glMaterialf( GL_FRONT, GL_SHININESS, 128.0 );

	glRotatef( 90.0, 1.0, 0.0, 0.0 );
	glutSolidSphere( 1.0, 100, 100 );

	glPopMatrix();
	glDisable( GL_LIGHTING );
    glDisable( GL_DEPTH_TEST );

    glutSwapBuffers();
}


void idle( void )
{
    if( rotateFlag == GL_TRUE ){
        spin = fmod( (spin + 1.0), 360.0 );
	glutPostRedisplay();
}
}


void initLighting( void )
{
	float  diffuse[] = { 0.0, 0.0, 1.0, 1.0 };
	float  specular[] = { 0.8, 0.8, 0.8, 1.0 };
	float  ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	float  whiteColor[] = { 0.8, 0.8, 0.8, 1.0 };

	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse );
	glLightfv( GL_LIGHT0, GL_SPECULAR, specular );
	glLightfv( GL_LIGHT0, GL_AMBIENT, ambient );

    glLightfv( GL_LIGHT1, GL_DIFFUSE, whiteColor );
    glLightfv( GL_LIGHT1, GL_SPECULAR, specular );
    glLightfv( GL_LIGHT1, GL_AMBIENT, ambient );

    glEnable( GL_LIGHT0 );
	glEnable( GL_LIGHT1 );
}


void myKbd( unsigned char key, int x, int y )
{
	switch(key) {
	case 'r':
		rotateFlag = !rotateFlag;
		break;
    case KEY_ESC:
        exit( 0 );
	}
}


void myInit (char *progname)
{
    glutInitWindowPosition(0, 0);
	glutInitWindowSize( 600, 600);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow(progname);
    glClearColor (0.0, 0.0, 0.0, 1.0);
    glutKeyboardFunc( myKbd );
	resetview();
	glShadeModel( GL_SMOOTH );
	initLighting();
}


void myReshape(int width, int height)
{
    float aspect = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, aspect, 1.0, 20.0);
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
	elevation = 0.0;
	azimuth = 0.0;
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	myInit(argv[0]);
	glutReshapeFunc(myReshape);
	glutIdleFunc(idle);
	glutDisplayFunc(display);
	glutMainLoop(); 
	return( 0 );
}
