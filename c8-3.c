/*  c6-3.c   Copyright (c) 2003 by T. HAYASHI and K. KATO  */
/*                                    All rights reserved  */
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#define KEY_ESC 27

void polarview( void );
float distance=7.0, twist=0.0, elevation=-45.0, azimuth=30.0;

float diffuse[] = { 0.7, 0.6, 0.2, 1.0 };
float specular[] = { 0.6, 0.5, 0.4, 1.0 };
float ambient[] = { 0.3, 0.2, 0.1, 1.0 };
float shininess = 128.0;
float light[] = {1, 1, 0, 0};

void display(void)
{
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glPushMatrix();
	polarview();
    glEnable( GL_DEPTH_TEST );
	glMaterialfv( GL_FRONT, GL_DIFFUSE, diffuse );
	glMaterialfv( GL_FRONT, GL_SPECULAR, specular );
	glMaterialfv( GL_FRONT, GL_AMBIENT, ambient );
	glMaterialf( GL_FRONT, GL_SHININESS, shininess );
	glLightfv(GL_LIGHT0, GL_POSITION, light);
	glEnable(GL_LIGHTING);

	glutSolidSphere( 1.0, 100, 100 );

	glDisable( GL_LIGHTING );
    glDisable( GL_DEPTH_TEST );
	glPopMatrix();
	glFlush();
}


void myKbd(unsigned char key, int x, int y)
{
	switch( key ) {
		case KEY_ESC:
        exit( 0 );
	}
}


void myInit (char *progname)
{
    glutInitWindowPosition(0, 0);
	glutInitWindowSize( 600, 600);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow(progname);
    glClearColor (0.0, 0.0, 0.0, 1.0);
    glutKeyboardFunc(myKbd);
	glEnable( GL_LIGHT0 );
}


void myReshape(int width, int height)
{
    float aspect = (float) width / (float) height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, aspect, 1.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}


void polarview( void )
{
    glTranslatef( 0.0, 0.0, -distance);
    glRotatef( -twist, 0.0, 0.0, 1.0);
    glRotatef( -elevation, 1.0, 0.0, 0.0);
    glRotatef( -azimuth, 0.0, 1.0, 0.0);
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