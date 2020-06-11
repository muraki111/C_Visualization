/*  c6-4.c   Copyright (c) 2003 by T. HAYASHI and K. KATO  */
/*                                    All rights reserved  */
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#define KEY_ESC 27
#define KEY_SPC 32
#define SHADE_FLAT1   0
#define SHADE_FLAT2  1
#define SHADE_SMOOTH 2

void polarview( void );
void resetview( void );
void drawNormal1( void );
void drawNormal2( void );
void sphereNormal( float, float, float, float * );

void myKbd(unsigned char c, int x, int y );
void myMouse(int button, int state, int x, int y);
void myMotion(int x, int y);
void chgShade( void );
void sphereNormal( float, float, float, float * );

static float xOrigin = 0.0, yOrigin = 0.0;
static int xBegin = 0, yBegin = 0;

int shadeStyle = SHADE_FLAT1;
unsigned char smoothFlag = GL_FALSE;
int mButton;
float distance, twist, elevation, azimuth;
float rSphere = 3.0;
int division = 1;


void display(void)
{
    float ambient[] = { 0.1, 0.1, 0.3, 1.0 };
    float diffuse[] = { 1.0, 1.0, 0.0, 1.0 };
    float specular[] = { 1.0, 1.0, 1.0, 1.0 };
	float x0, x1, y0, z0, xm, ym, zm;
	float nv[3];
	int i, j;

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	polarview();
    glEnable( GL_DEPTH_TEST );
	glMaterialfv( GL_FRONT, GL_AMBIENT, ambient );
	glMaterialfv( GL_FRONT, GL_DIFFUSE, diffuse );
	glMaterialfv( GL_FRONT, GL_SPECULAR, specular );
	glMaterialf( GL_FRONT, GL_SHININESS, 128.0 );
    glEnable( GL_LIGHTING );
	if( smoothFlag == GL_TRUE ) {
    	for( i = -division; i < division ; i++) {
        	x0 = (float) i;
        	glBegin( GL_QUAD_STRIP );
			for( j = -division; j <= division ; j++ ) {
            	y0 =  (float) j;
            	z0 = - rSphere + sqrt( rSphere*rSphere - x0*x0 -y0*y0);

				sphereNormal( x0, y0, z0, nv );
            	glNormal3fv( nv );
               	glVertex3f( x0, y0, z0 );

				x1 = x0 + 1.0;
           		z0 = - rSphere + sqrt( rSphere*rSphere - x1*x1 -y0*y0);

				sphereNormal( x1, y0, z0, nv );
            	glNormal3fv( nv );
               	glVertex3f( x1, y0, z0 );
        	}
        	glEnd();
		}
	glDisable( GL_LIGHTING );
	drawNormal1();
	}
	else {
        for( i = -division; i < division ; i++) {
            x0 = (float) i;
            glBegin( GL_QUAD_STRIP );
            for( j = -division; j <= division ; j++ ) {
                y0 =  (float) j;
                z0 = - rSphere + sqrt( rSphere*rSphere - x0*x0 -y0*y0);
				xm = x0 + 0.5, ym = y0 + 0.5;
				zm = - rSphere + sqrt( rSphere*rSphere - xm*xm -ym*ym);
                sphereNormal( xm, ym, zm, nv );
                glNormal3fv( nv );
                glVertex3f( x0, y0, z0 );
                x1 = x0 + 1.0;
                z0 = - rSphere + sqrt( rSphere*rSphere - x1*x1 -y0*y0);
                glVertex3f( x1, y0, z0 );
            }
            glEnd();
        }
    glDisable( GL_LIGHTING );
    drawNormal2();
	}
	glPopMatrix ();
    glDisable( GL_DEPTH_TEST );
	glutSwapBuffers();
}


void sphereNormal( float x0, float y0, float z0, float *nv )
{
	int i;
	float length;

	nv[0] = x0;
	nv[1] = y0;
	nv[2] = z0 + rSphere;
	length = sqrt( nv[0]*nv[0] + nv[1]*nv[1] + nv[2]*nv[2] );
	for( i = 0; i < 3; i++ ) nv[i] = nv[i] / length;
}


void drawNormal1( void )
{
	float x0, y0, z0;
	float nv[3];
	int i, j;

	glLineWidth( 2.0 );
    glColor3f( 1.0, 0.0, 0.0 );
    for( j = -division; j <= division ; j++ ) {
		y0 = (float) j;
    	for( i = -division; i <= division ; i++ ) {
			x0 = (float) i;
        	z0 = - rSphere + sqrt( rSphere*rSphere - x0*x0 -y0*y0);
			sphereNormal( x0, y0, z0, nv );
        	glPushMatrix();
            glTranslatef( x0, y0, z0 );
            glScalef( 0.5, 0.5, 0.5 );
            glBegin( GL_LINES );
    	        glVertex3f( 0.0, 0.0, 0.0 );
                glVertex3fv( nv );
            glEnd();
        	glPopMatrix();
		}
	}
}


void drawNormal2( void )
{
    float x1, y1, z1;
    float nv[3];
    int i, j;

	glLineWidth( 2.0 );
    glColor3f( 1.0, 0.0, 0.0 );
    for( j = -division; j <division ; j++ ) {
        y1 = 0.5 + (float) j;
        for( i = -division; i < division ; i++ ) {
            x1 = 0.5 + (float) i;
            z1 = - rSphere + sqrt( rSphere*rSphere - x1*x1 -y1*y1);
            sphereNormal( x1, y1, z1, nv );
            glPushMatrix();
            glTranslatef( x1, y1, z1 );
            glScalef( 0.5, 0.5, 0.5 );
            glBegin( GL_LINES );
                glVertex3f( 0.0, 0.0, 0.0 );
                glVertex3fv( nv );
            glEnd();
            glPopMatrix();
        }
    }
}


void chgShade( void )
{
	if (++shadeStyle > SHADE_SMOOTH)
    	shadeStyle = SHADE_FLAT1;

    switch (shadeStyle) {
    case SHADE_FLAT1:
    	glShadeModel( GL_FLAT );
		smoothFlag = GL_FALSE;
        break;
    case SHADE_FLAT2:
        glShadeModel( GL_FLAT );
		smoothFlag = GL_TRUE;
        break;
    case SHADE_SMOOTH:
        glShadeModel( GL_SMOOTH );
		smoothFlag = GL_TRUE;
        break;
    }

}


void myKbd( unsigned char key, int x, int y )
{
	switch( key ) {
	case 'R':
        resetview();
		glutPostRedisplay();
        break;
	case KEY_SPC:
		chgShade();
		glutPostRedisplay();
		break;
	case 'c':
		division += 1;
		if( division > 2 ) division = 1;
		glutPostRedisplay();
		break;
	case KEY_ESC:
		exit( 0 );
		break;
	}
}


void myMouse( int button, int state, int x, int y )
{
    if ( state == GLUT_DOWN ) {
        xBegin = x;
        yBegin = y;
        mButton = button;
    }
}


void myMotion( int x, int y )
{
    int xDisp, yDisp;

    xDisp = x - xBegin;
    yDisp = y - yBegin;

    switch (mButton) {
    case GLUT_LEFT_BUTTON:
        azimuth += (double) xDisp/2.0;
        elevation -= (double) yDisp/2.0;
        break;
    case GLUT_MIDDLE_BUTTON:
        twist = fmod( twist + xDisp, 360.0 );
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
	glutInitWindowSize( 600, 600 );
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow(progname);
    glClearColor (0.0, 0.0, 0.0, 1.0);

    glutKeyboardFunc( myKbd );
	glutMouseFunc( myMouse );
    glutMotionFunc( myMotion );
	resetview();
	glShadeModel( GL_FLAT );
	glEnable( GL_LIGHT0 );
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
    glRotatef( -azimuth, 0.0, 0.0, 1.0);
}


void resetview( void )
{
    distance = 5.0;
    twist = 0.0;
    elevation = 40.0;
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
