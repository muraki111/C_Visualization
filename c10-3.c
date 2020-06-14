/************************************************************************/
/*       Program   "c9-3.c"                                             */
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
#define imageWidth 64
#define imageHeight 64

void polarview( void );
void resetview( void );
unsigned char image[imageHeight][imageWidth][4];
int xBegin = 0, yBegin = 0;
float distance, twist, elevation, azimuth;
float nearw = 1.0, farw = 30.0, fovy = 60.0;
float greenDiffuse[] = { 0.0, 1.0, 0.5, 1.0 };
float whiteSpecular[] = { 1.0, 1.0, 1.0, 1.0 };
float whiteAmbient[] = { 0.1, 0.1, 0.1, 1.0 };

float v=1.0;
float		cpoint[4][4][4]={
	 {
	 	{1.0, -1.0, 0.0, 1.0 },	
		{0.5, -1.0, 0.0, 1.0 },
		{-0.5, -1.0, 0.0, 1.0 },
		{-1.0, -1.0, 0.0, 1.0 }
	 },
	 {
	 	{1.0, -0.5, 1.0, 1.0 },	
		{0.5, -0.5, 1.0, 1.0 },
		{-0.5, -0.5, 1.0, 1.0 },
		{-1.0, -0.5, 1.0, 1.0 }
	 },
	 {
	 	{1.0, 0.5, -1.0, 1.0 },	
		{0.5, 0.5, -1.0, 1.0 },
		{-0.5, 0.5, -1.0, 1.0 },
		{-1.0, 0.5, -1.0, 1.0 }
	 },
	 {
	 	{1.0, 1.0, 0.0, 1.0 },	
		{0.5, 1.0, 0.0, 1.0 },
		{-0.5, 1.0, 0.0, 1.0 },
		{-1.0, 1.0, 0.0, 1.0 }
	 }
};

float knotvec_u[8]={0.0,0.0,0.0,0.0,1.0,1.0,1.0,1.0};
float knotvec_v[8]={0.0,0.0,0.0,0.0,1.0,1.0,1.0,1.0};

int mButton;


GLUnurbsObj *nrb_obj;
void create_nurbs()
{

	nrb_obj = gluNewNurbsRenderer();
	gluNurbsProperty(nrb_obj, GLU_SAMPLING_TOLERANCE, 25.0);
	gluNurbsProperty(nrb_obj, GLU_DISPLAY_MODE, GLU_FILL);
}

void drawCP()
{
	int i,j;
	float x,y,z;
	glColor3f(1.0,1.0,0.0);
	glPointSize(5.0);
	glBegin(GL_POINTS);
		for (i=0;i<4;i++) {
			for (j=0;j<4;j++) {
				x=cpoint[i][j][0]/cpoint[i][j][3];
				y=cpoint[i][j][1]/cpoint[i][j][3];
				z=cpoint[i][j][2]/cpoint[i][j][3];
				glVertex3f(x,y,z);
			}
		}
	glEnd();
}

void display(void)
{

				 
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glPushMatrix();
	polarview();
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_LIGHTING );

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, greenDiffuse );
	glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, whiteSpecular );
	glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, whiteAmbient );
	glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, 128.0 );
  
	glEnable( GL_NORMALIZE);
	glEnable( GL_AUTO_NORMAL);
	glPushMatrix();
	
//		glRotatef(200.0, 0.0, 1.0, 0.0);
//		glScalef(0.7,0.7,0.7);
//		glTranslatef(-2.0,-2.0,0.0);
		gluBeginSurface(nrb_obj);
		gluNurbsSurface(nrb_obj,
				8,knotvec_u,
				8,knotvec_v,
				4*4,
				4,
				&cpoint[0][0][0],				//
				4,4,				//
				GL_MAP2_VERTEX_4
				);
		gluEndSurface(nrb_obj);

	glPopMatrix();
	glDisable( GL_LIGHTING );
	drawCP();
    glDisable( GL_DEPTH_TEST );
	glPopMatrix();
	glutSwapBuffers();
}



void myKbd(unsigned char key, int x, int y)
{
switch(key) {
    	case KEY_ESC:
     		exit( 0 );
  	default:
   		break;
	}
}

void myMouse( int button, int state, int x, int y )
{
    if (state == GLUT_DOWN) {
		mButton = button;
		xBegin = x;
        yBegin = y;
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


int vflag=1;
void idle(void)
{
	if (vflag==1) {
		v += 1.0;
		if (v > 5.0) {
			v = 5.0;
			vflag=0;
		}
	}
	else {
		v -= 1.0;
		if (v < -5.0) {
			v = -5.0;
			vflag=1;
		}
	}
	cpoint[1][1][2] = v;
	glutPostRedisplay();
}

void myInit (char *progname)
{
    glutInitWindowPosition(0, 0);
	glutInitWindowSize(400, 400);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow(progname);
    glClearColor (0.0, 0.0, 0.0, 1.0);
    glutKeyboardFunc(myKbd);
	glutIdleFunc(idle);
    glutMouseFunc(myMouse);
    glutMotionFunc(myMotion);
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
	distance = 3.0;
	twist = 0.0;
	elevation = -45.0;
	azimuth = 30.0;
}


void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	myInit(argv[0]);
	create_nurbs();
	glutReshapeFunc (myReshape);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutMainLoop();
}
