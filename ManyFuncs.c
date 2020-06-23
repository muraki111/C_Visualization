/* anim1*/

#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

float distance = 20., twist=0., elevation=30., azimuth=20.;

void display(void)
{
	void my_axis();
	int i;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);// Z Buffer �̗L���͈͂̎w��

	glPushMatrix();
	  gluLookAt(0.34*distance,distance*1.732/3,-distance, 0.,0.,0., 0.,1.,0.);
	  glPushMatrix();
	          glutWireCube(1.0);/*�@�����́@*/
	  glPopMatrix();
	glPopMatrix();

	glDisable(GL_DEPTH_TEST); // Z Buffer�̗L���͈͂̏I���
	glutSwapBuffers();
}

void idle(void)
{
	azimuth += 0.01;
	glutPostRedisplay();
}

void myReshape(int width, int height)
{
	printf(" Reshape\n");
	printf("w,h = %d %d\n",width, height);
	glutPostRedisplay();
}

void myMotion(int width, int height)
{
	printf(" Motion\n");
	printf("w,h = %d %d\n",width, height);
	glutPostRedisplay();
}

void myMouse(int button, int state, int x, int y)
{
	printf(" Mouse\n");
	printf("x,y = %d %d\n",x, y);
	printf("button = %d\n", button);
	printf("state = %d\n", state);
	glutPostRedisplay();
}

void mySkey(int key, int x , int y)
{
	printf(" mySkey\n");
	printf("x,y = %d %d\n",x, y);
	printf("key = %d\n", key);
	glutPostRedisplay();
}


void myKbd(unsigned char key, int x , int y)
{
	printf(" myKnd\n");
	printf("x,y = %d %d\n",x, y);
	printf("key = %d\n", key);
	glutPostRedisplay();
}

void myInit(char *progname)
{
	int width=500,height=500;
	float aspect=(float)width/(float)height;

	glutInitWindowPosition(0,0);
	glutInitWindowSize(width,height);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH |GLUT_DOUBLE);
	glutCreateWindow(progname);
	glClearColor(0.3,0.3,0.3,1.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();/*�s��X�^�b�N���N���A*/
	gluPerspective(30.,aspect,1.,50.);
	glMatrixMode(GL_MODELVIEW);/*�􉽕ϊ��s��X�^�b�N�𑀍�Ώۂ���*/
}

int main(int argc, char *argv[])
{
	glutInit(&argc,argv);
	myInit(argv[0]);
	glutDisplayFunc(display);
	glutKeyboardFunc(myKbd);
	glutSpecialFunc(mySkey);
	glutMouseFunc(myMouse);
	glutMotionFunc(myMotion);
	glutReshapeFunc(myReshape);
//	glutIdleFunc(idle);
	glutMainLoop();
	return(0);
}
