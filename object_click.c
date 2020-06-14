/*C5-2*/

#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#define KEY_ESC 27

#define BUFMAX 100

void polarview(void);
void resetview(void);
unsigned char wireFlag = GL_TRUE;
unsigned char revolveFlag = GL_FALSE;/*��]��ON/OFF�؂�ւ�*/
int xBegin,yBegin;/*�h���b�O�J�n���_�̃}�E�X�|�C���^�̍��W*/
int mButton;/*�h���b�O���ɉ�����Ă���}�E�X�{�^���̔���*/
float distance,twist,elevation,azimuth;
float theta =15.0;
void mydraw();
void myPickUp(int,int);
int click_obj=0;

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);
	glPushMatrix();

	mydraw();

	glPopMatrix();
	glDisable(GL_DEPTH_TEST);
	glutSwapBuffers();
}

void mydraw(){ 
	glEnable(GL_DEPTH_TEST);
	polarview();
	glRotatef(theta,0.0,1.0,0.0);
	glColor3f(1.0,1.0,1.0);
	if( click_obj == 1){ // click_obj (grobal) has the name value clicked. 
		glColor3f(1.0,0.0,0.0); // set Red color if it is clicked
	}
	glLoadName(1);
	glutSolidCube(1.0);
	glTranslatef(2.,0.,0.);
	glColor3f(1.0,1.0,1.0);
	if( click_obj == 2){// click_obj (grobal) has the name value clicked. 
		glColor3f(0.0,1.0,0.0);// set Green color if it is clicked
	}
	glLoadName(2);
	glutSolidCube(1.0);
	glDisable(GL_DEPTH_TEST);
}

void myPickUp(int x, int y)
{
	float aspect =1.;
	GLuint selectBuf[BUFMAX]; // prepare selectBuf
	int depth[BUFMAX],min_depth;
	GLuint hits;
	GLint viewport[4];
	int i,j,ptr;
	int layer;
	int mind;

	for(i=0;i<BUFMAX;i++){
		selectBuf[i]=0;
	}

	glGetIntegerv(GL_VIEWPORT,viewport);
	glSelectBuffer(BUFMAX,selectBuf);
	(void) glRenderMode(GL_SELECT);
	glInitNames();
	glPushName(0);
	glMatrixMode(GL_PROJECTION);

	glPushMatrix();
	 glLoadIdentity();
	 gluPickMatrix(x,viewport[3]-y,3.,3.,viewport);
	 aspect = (float)viewport[2]/(float)viewport[3];
	 gluPerspective( 45.0,aspect,1.0,20.0);
	 glMatrixMode(GL_MODELVIEW);
//	 glLoadIdentity();
	 mydraw();
	 glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	hits = glRenderMode(GL_RENDER);
	fprintf(stderr," hits = %d\n",hits);
	if( 0 < hits){
	  ptr = 0;
	  min_depth = selectBuf[1];

	  for(i=0;i<hits;i++){
		layer = selectBuf[ptr];
		fprintf(stderr," layer : %d\n",  selectBuf[ptr]);
		ptr++;
		fprintf(stderr,"  min depth: %d\n", selectBuf[ptr]);
		mind = selectBuf[ptr];
		ptr++;
		fprintf(stderr,"  max depth: %d\n", selectBuf[ptr]);
		ptr++;
		if( mind <= min_depth) {
			min_depth = mind;
			click_obj = selectBuf[ptr]; // Store only the top name
		}
		for(j=0;j< layer;j++){
		  fprintf(stderr,"  name: %d\n", selectBuf[ptr]);
		  ptr++; // Notice that it needs error handling when str over the range.
		}
	  }
	} else {
	   click_obj=0;
	}
	glMatrixMode(GL_MODELVIEW);
   	glutPostRedisplay();
}

void idle(void)
{
	theta = fmod(theta+0.5,360.);
	glutPostRedisplay();
}

void myKbd(unsigned char key,int x, int y)
{
	switch(key){
	case 'w':
		wireFlag = !wireFlag;
		break;
	case 'R':
		resetview();
		break;
	case KEY_ESC:
		exit(0);
	}
	glutPostRedisplay();
}

void myMouse(int button,int state,int x,int y)
{
	if(state == GLUT_DOWN){
		switch(button){
		case GLUT_LEFT_BUTTON:
			mButton = button;
			myPickUp(x,y);
			break;
		case GLUT_MIDDLE_BUTTON:
			revolveFlag = !revolveFlag;
			if(revolveFlag == GL_TRUE)
				glutIdleFunc(idle); /*idle���J��Ԃ����s����悤�ɐݒ�*/
			else
				glutIdleFunc(NULL);/*�A�j���[�V�������Ƃ߂�*/
			break;
		case GLUT_RIGHT_BUTTON:
			mButton = button;
			break;
		}
		xBegin = x; /*�h���b�O�J�n�_��x,y���W�l���擾*/
		yBegin = y;
	}
}

void myMotion( int x, int y)
{
	int xDisp, yDisp;
	
	xDisp = x- xBegin; /*�}�E�X�ړ������̌v�Z*/
	yDisp = y -yBegin;
	
	switch(mButton){
	case GLUT_LEFT_BUTTON:/*���{�^���̃h���b�O�ŕ��̂̎p����ς���*/
		azimuth += (float)xDisp/2.0;
		elevation -=(float)yDisp/2.0;
		break;
	case GLUT_RIGHT_BUTTON:/*�E�{�^���̃h���b�O�ŃY�[��*/
		distance +=(float)yDisp/40.0;
		break;
	}
	xBegin = x;/*���̃X�e�b�v�̃}�E�X�̏o���_*/
	yBegin = y;
	glutPostRedisplay();/*1�X�e�b�v���̃h���b�O�̌��ʂ�`��ɔ��f*/
}

void myInit(char *progname)
{
	int width=600,height=600;
	float aspect =(float)width/(float)height;
	glutInitWindowPosition(0,0);
	glutInitWindowSize(width,height);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
	glutCreateWindow(progname);
	glClearColor(0.0,0.0,0.0,1.0);
	
	glutKeyboardFunc(myKbd);
	glutMouseFunc(myMouse);/*�}�E�X�N���b�N�ɑ΂���R�[���o�b�N�֐��̓o�^*/
	glutMotionFunc(myMotion);/*�}�E�X�h���b�O�ɑ΂���R�[���o�b�N�֐��̓o�^*/	
	resetview();/*�r���[�C���O�ϊ�polarview�ɏ����p�����[�^�l��ݒ�*/
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0,aspect,1.0,20.0);
	glMatrixMode(GL_MODELVIEW);
}

void resetview(void)
{
	distance =5.0;
	twist = 0.0;
	elevation = 0.0;
	azimuth = 0.0;
}

void polarview(void)
{
	glTranslatef(0.0,0.0,-distance);
	glRotatef(-twist,0.0,0.0,1.0);
	glRotatef(-elevation,1.0,0.0,0.0);
	glRotatef(-azimuth,0.0,1.0,0.0);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	myInit(argv[0]);
	glutDisplayFunc(display);
	glutIdleFunc(NULL);
	glutMainLoop();
	return(0);
}

	
	