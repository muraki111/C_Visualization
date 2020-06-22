/*C7-1*/

#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#define ESC_KEY 27
	
void polarview();
void resetview();
float distance,twist,elevation,azimuth;

void display(void)
{
	float diffuse[] ={1.0,1.0,1.0,1.0};
	float specular[]={1.0,1.0,1.0,1.0};
	float ambient[]={0.1,0.1,0.1,1.0};
	float light[]={1.0,1.0,1.0,0.0};/*光源の位置と種類*/
	
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	
	glPushMatrix();
	polarview();
	glLightfv(GL_LIGHT0,GL_POSITION,light);/*平行光源の設定*/
	glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
	glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);	
	glMaterialf(GL_FRONT,GL_SHININESS,128.0);
	
	glRotatef(90.0,1.0,0.0,0.0);
	glutSolidSphere(1.0,200,200);
	
	glPopMatrix();
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glFlush();
}

void initLighting(void)/*光源の属性の設定*/
{
	float diffuse[] ={0.2,0.2,1.0,1.0};
	float specular[]={0.5,0.5,1.0,1.0};
	float ambient[]={0.5,0.5,0.5,1.0};

	glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuse);/*拡散反射成分の強度*/
	glLightfv(GL_LIGHT0,GL_SPECULAR,specular);/*鏡面反射成分の強度*/
	glLightfv(GL_LIGHT0,GL_AMBIENT,ambient);/*環境光成分の強度*/
	glEnable(GL_LIGHT0);
}

void myKbd(unsigned char key, int x, int y)
{
	switch(key){
		case ESC_KEY:
			exit(0);
	}
}
void myInit( char *progname)
{
	glutInitWindowPosition(0,0);
	glutInitWindowSize(600,600);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGBA|GLUT_DEPTH);
	glutCreateWindow(progname);
	glClearColor(0.0,0.0,0.0,1.0);
	glutKeyboardFunc(myKbd);
	resetview();
	glShadeModel(GL_SMOOTH);
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

		
		