/*C6-1*/
/*Shading*/

#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#define KEY_ESC 27
	
void polarview(void);
void resetview(void);
void drawNormal(float,float,float,float,float,float);/*法線方向の表示*/
int xBegin,yBegin;
int mButton;
float distance,twist,elevation,azimuth;

DrawString(char *str, float x0,float y0, double w, double h)
{
	int i,len;
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glColor3d(1.,0.,0.);
	glLoadIdentity();
	gluOrtho2D(0.,w,0.,h);
 	len = strlen(str);
	for(i=0;i<len;i++){
	glRasterPos2f(x0+20*i,y0);
//        glutStrokeString(GLUT_BITMAP_HELVETICA_18, str);
//        glutBitmapString(GLUT_BITMAP_HELVETICA_18, str);
          glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *str);
	  str++;
	}
//	printf(" %s %f %f %lf %lf\n",str,x0,y0,w,h);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
        DrawString("TITLE",200,400,500,500);
//        glutBitmapString(GLUT_BITMAP_9_BY_15, " TITLE");
	glPopMatrix();
	glPushMatrix();
	polarview();
	glEnable(GL_DEPTH_TEST);/*Z Buffer enable*/
	glEnable(GL_LIGHTING);/*Shading計算の開始*/
	
	glBegin(GL_POLYGON);
	glNormal3f(0.0,1.0,0.0);/*法線ベクトルの設定*/
		glVertex3f(1.0,0.0,1.0);
		glVertex3f(1.0,0.0,-1.0);
		glVertex3f(-1.0,0.0,-1.0);
		glVertex3f(-1.0,0.0,1.0);
	glEnd();
	
	glDisable(GL_LIGHTING);/*Shading計算終了*/
	drawNormal(0.0,0.0,0.0,0.0,1.0,0.0);
	glPopMatrix();
	glDisable(GL_DEPTH_TEST);
	glutSwapBuffers();
}

void drawNormal(float x0, float y0,float z0,float x1,float y1,float z1)/*法線を描く*/
{
	glColor3f(1.0,0.0,0.0);
	glLineWidth(2.0);
	glPushMatrix();
	glTranslatef(x0,y0,z0);
	glBegin(GL_LINES);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(x1,y1,z1);
	glEnd();
	glPopMatrix();
}
void myKbd(unsigned char key, int x, int y)
{
	switch(key){
		case 'R':
			resetview();
			glutPostRedisplay();
			break;
		case KEY_ESC:
			exit(0);
	}
}

void myMouse(int button, int state, int x, int y)
{
	if(state == GLUT_DOWN){
		xBegin = x;
		yBegin = y;
		mButton = button;
	}
}

void myMotion(int x, int y)
{
	int xDisp, yDisp;
	
	xDisp = x -xBegin;
	yDisp = y -yBegin;
	switch(mButton){
		case GLUT_LEFT_BUTTON:
			azimuth += (float) xDisp/5.0;
			elevation -= (float)yDisp/5.0;
			break;
		case GLUT_MIDDLE_BUTTON:
			twist = fmod(twist +xDisp/3.0,360.0);
			break;
		case GLUT_RIGHT_BUTTON:
			distance -= (float)(xDisp+yDisp)/60.0;
			break;
	}
	xBegin = x;
	yBegin = y;
	glutPostRedisplay();
}

void myInit(char * progname)
{
	int width = 600, height =600;
	
	glutInitWindowPosition(0,0);
	glutInitWindowSize(width,height);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
	glutCreateWindow(progname);
	glClearColor(0.3,0.3,0.3,1.0);
	glutKeyboardFunc(myKbd);
	glutMouseFunc(myMouse);
	glutMotionFunc(myMotion);
	resetview();
	
	glShadeModel(GL_FLAT);
	
	glEnable(GL_LIGHT0);/*0番のライトを利用可能にする*/
}

void myReshape(int width,int height)
{
	float aspect =(float)width/(float)height;
	
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0,aspect,1.0,30.0);
	glMatrixMode(GL_MODELVIEW);
}

void polarview(void)
{
	glTranslatef(0.0,0.0,-distance);
	glRotatef(-twist,0.0,0.0,1.0);
	glRotatef(-elevation,1.0,0.0,0.0);
	glRotatef(-azimuth,0.0,1.0,0.0);
}
void resetview(void)
{
	distance = 6.0;
	twist=0.0;
	elevation=-45.0;
	azimuth=30.0;
}

int main(int argc, char** argv)
{
	glutInit(&argc,argv);
	myInit(argv[0]);
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutMainLoop();
	exit(0);
}

	
	
		
