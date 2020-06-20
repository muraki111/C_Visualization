/* anim1*/

#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "myShape.h"

float distance = 7.;

void display(void)
{
	void my_axis();
	int i;

	glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();
	  gluLookAt(-distance,distance,-distance, 0.,0.,0., 0.,1.,0.);
	  glPushMatrix();
	          glutWireCube(1.0);/*�@�����́@*/
	  glPopMatrix();
	glPopMatrix();

	glutSwapBuffers();
}

void myInit(char *progname)
{
	int width=500,height=500;
	float aspect=(float)width/(float)height;

	glutInitWindowPosition(0,0);
	glutInitWindowSize(width,height);
	glutInitDisplayMode(GLUT_RGBA |GLUT_DOUBLE);
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
	glutMainLoop();
	return(0);
}
