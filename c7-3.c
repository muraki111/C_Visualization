/*  c7-3.c   Copyright (c) 2003 by T. HAYASHI and K. KATO  */
/*                                    All rights reserved  */
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <math.h>
#define KEY_ESC 27

void polarview( void );
void resetview( void );
float distance, twist, elevation, azimuth;

void display(void)
{
    float whiteDiffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    float redDiffuse[] = { 1.0, 0.0, 0.0, 1.0 };
    float greenDiffuse[] = { 0.0, 1.0, 0.0, 1.0 };
    float whiteSpecular[] = { 1.0, 1.0, 1.0, 1.0 };
    float redAmbient[] = { 0.3, 0.2, 0.2, 1.0 };
    float whiteAmbient[] = { 0.1, 0.1, 0.1, 1.0 };
	float light0[] = { 1.0, 1.0, 1.0, 0.0 };
    float light1[] = { 0.0, 2.0, 1.0, 1.0 };
    float spotDirection[] = { 0.0, -1.0, -0.5 };
    int i, j;
    float x0, y0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    polarview();
    glLightfv( GL_LIGHT1, GL_POSITION, light1 );
    glLightfv( GL_LIGHT1, GL_SPOT_DIRECTION, spotDirection );
    glLightfv( GL_LIGHT0, GL_POSITION, light0 );

    glEnable( GL_DEPTH_TEST );
    glMaterialfv( GL_FRONT, GL_DIFFUSE, greenDiffuse );
    glMaterialfv( GL_FRONT, GL_SPECULAR, whiteSpecular );
    glMaterialfv( GL_FRONT, GL_AMBIENT, whiteAmbient );
    glMaterialf( GL_FRONT, GL_SHININESS, 128.0 );
    glEnable( GL_LIGHTING );

    glPushMatrix();
    glRotatef( 90.0, 1.0, 0.0, 0.0 );
    glutSolidSphere( 1.0, 50, 50 );
    glPopMatrix();

    glTranslatef( 0.0, -1.8, 0.0 );
    glRotatef( -90.0, 1.0, 0.0, 0.0 );

    glMaterialfv( GL_FRONT, GL_DIFFUSE, redDiffuse );
    glMaterialfv( GL_FRONT, GL_SPECULAR, whiteSpecular );
    glMaterialfv( GL_FRONT, GL_AMBIENT, whiteAmbient );
    for( i = -100; i < 100 ; i++) {
        x0 = 0.04 * (float) i;
        glBegin( GL_QUAD_STRIP );
            glNormal3f( 0.0, 0.0, 1.0 );
            for( j = -100; j < 100 ; j++) {
                y0 = 0.04 * (float) j;
                glVertex3f( x0, y0, 0.0 );
                glVertex3f( x0 + 0.04, y0, 0.0 );
            }
        glEnd();
    }
    glPopMatrix();
    glDisable( GL_LIGHTING );
    glDisable( GL_DEPTH_TEST );
    glFlush();
}


void initLighting( void )
{
    float  diffuse0[] = { 0.5, 0.5, 0.5, 1.0 };
    float  specular0[] = { 0.2, 0.2, 0.2, 1.0 };
    float  diffuse1[] = { 1.0, 1.0, 1.0, 1.0 };
    float  specular1[] = { 1.0, 1.0, 1.0, 1.0 };

    float  spot_cutoff = 30.0;
    float  spot_exp = 0.0;

    glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse0 );
    glLightfv( GL_LIGHT0, GL_SPECULAR, specular0 );
    glLightfv( GL_LIGHT1, GL_DIFFUSE, diffuse1 );
    glLightfv( GL_LIGHT1, GL_SPECULAR, specular1 );
    glLightf( GL_LIGHT1, GL_SPOT_EXPONENT, spot_exp );
    glLightf( GL_LIGHT1, GL_SPOT_CUTOFF, spot_cutoff );

    glEnable( GL_LIGHT0 );
    glEnable( GL_LIGHT1 );
}


void myKbd( unsigned char key, int x, int y )
{
	switch(key) {
    case KEY_ESC:
        exit( 0 );
	}
}


void myInit (char *progname)
{
    glutInitWindowPosition(0, 0);
	glutInitWindowSize( 600, 600);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
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
    gluPerspective(40.0, aspect, 1.0, 40.0);
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
	distance = 10.0;
	twist = 0.0;
	elevation = -30.0;
	azimuth = 0.0;
}


void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	myInit(argv[0]);
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutMainLoop();
}
