#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#define KEY_ESC 27

void polarview( void );
void resetview( void );

int xBegin = 0, yBegin = 0;
float distance, twist, elevation, azimuth;
float nearw = 1.0, farw = 30.0, fovy = 60.0;
int mButton;
float knotvec[8]={0.0,0.0,0.0,0.0,1.0,1.0,1.0,1.0};
float cpoint[4][4]=	{
					{1.0, -1.0, 0.0, 1.0 },
					{0.5, -0.5, 1.0, 1.0 },
					{-0.5, 0.5, -1.0, 1.0 },
					{-1.0, 1.0, 0.0, 1.0 }
					};

GLUnurbsObj *nrb_obj;
void create_nurbs(void)
{
	nrb_obj = gluNewNurbsRenderer();
	gluNurbsProperty(nrb_obj, GLU_SAMPLING_TOLERANCE, 25.0);
}

void drawCP(void)
{
	int i;
	float x,y,z;
	glColor3f(1.0,1.0,0.0);
	glPointSize(5.0);
	glBegin(GL_POINTS);
		for (i=0;i<4;i++) {
			x=cpoint[i][0]/cpoint[i][3];
			y=cpoint[i][1]/cpoint[i][3];
			z=cpoint[i][2]/cpoint[i][3];
			glVertex3f(x,y,z);
		}
	glEnd();
}

void display(void)
{
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glPushMatrix();
	polarview();
    glEnable( GL_DEPTH_TEST );

	glColor3f(1.0,0.0,0.0);
	glPushMatrix();

	gluBeginCurve(nrb_obj);
		gluNurbsCurve(nrb_obj,
				8,knotvec,
				4,
				&cpoint[0][0],				//
				4,				//
				GL_MAP1_VERTEX_4
				);
	gluEndCurve(nrb_obj);

	glPopMatrix();
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

void myInit (char *progname)
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


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	myInit(argv[0]);
	create_nurbs();
	glutReshapeFunc (myReshape);
	glutDisplayFunc(display);
	glutMainLoop();
	return(0);
}
