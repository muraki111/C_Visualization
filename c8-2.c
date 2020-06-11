/*  c6-2.c   Copyright (c) 2003 by T. HAYASHI and K. KATO  */
/*                                    All rights reserved  */
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#define KEY_ESC 27

void polarview( void );
void resetview( void );
void drawNormal( float*, float* );

int xBegin = 0, yBegin = 0;
int mButton;
float distance, twist, elevation, azimuth;


void display(void)
{
    float vertex_data[4][3] = {  1.0,  0.0,  1.0,
                                 1.0,  0.0, -1.0,
                                -1.0,  0.0, -1.0,
                                -1.0,  0.0,  1.0 };

    float normal_data[4][3] = { 0.447214,  0.774597,  0.447214,
                                0.447214,  0.774597, -0.447214,
                               -0.447214,  0.774597, -0.447214,
                               -0.447214,  0.774597,  0.447214 };
    int i;

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix ();
	polarview();
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_LIGHTING );

    glBegin( GL_POLYGON );
    for( i = 0 ; i < 4; ++i ) {
        glNormal3fv( normal_data[i] );
        glVertex3fv( vertex_data[i] );
    }
    glEnd();

	glDisable( GL_LIGHTING );
    for( i = 0 ; i < 4; ++i )
        drawNormal( vertex_data[i], normal_data[i] );
	glPopMatrix ();
	glDisable( GL_DEPTH_TEST );
	glutSwapBuffers();
}


void drawNormal( float *v0, float *v1 )
{
    float x0, y0, z0;

    glColor3f( 1.0, 0.0, 0.0 );
	glLineWidth( 2.0 );
    x0= v0[0], y0= v0[1], z0= v0[2];
    glPushMatrix();
    glTranslatef( x0, y0, z0 );
    glBegin( GL_LINES );
        glVertex3f( 0.0, 0.0, 0.0 );
        glVertex3fv( v1 );
    glEnd();
    glPopMatrix();
}


void myKbd( unsigned char key, int x, int y )
{
	switch( key ) {
	case 'R':
        resetview();
		glutPostRedisplay();
        break;
    case KEY_ESC:
        exit( 0 );
    }
}


void myMouse(int button, int state, int x, int y)
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
        azimuth += (double) xDisp/2.0;
        elevation -= (double) yDisp/2.0;
        break;
    case GLUT_MIDDLE_BUTTON:
        twist = fmod (twist + xDisp, 360.0);
        break;
   case GLUT_RIGHT_BUTTON:
        distance -= (double) yDisp/40.0;
        break;
	}
    xBegin = x;
    yBegin = y;
	glutPostRedisplay();
}


void myInit (char *progname)
{
    glutInitWindowPosition(0, 0);
	glutInitWindowSize( 600, 600);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow(progname);
    glClearColor (0.0, 0.0, 0.0, 1.0);
    glutKeyboardFunc( myKbd );
    glutMouseFunc( myMouse );
    glutMotionFunc( myMotion );
	resetview();
	glShadeModel( GL_SMOOTH );

	glEnable( GL_LIGHT0 );
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
