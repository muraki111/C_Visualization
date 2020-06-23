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
					{0.0, -1.0, 0.0, 1.0 },//曲線座標(x,y,z,?)
					{0.0, -0.5, 3.0, 1.0 },//曲線座標(x,y,z,?)
					{0.0, 0.5, -3.0, 1.0 },//曲線座標(x,y,z,?)
					{0.0, 1.0, 0.0, 1.0 }//曲線座標(x,y,z,?)
					};
float circle_x = 0;//S字から見て奥手前移動
float circle_y = 0;//S字から見て上下
float circle_z = 0;//S字から見て左右
float angle = 0;//輪っかの回転

GLUnurbsObj *nrb_obj;
void create_nurbs(void)
{
	nrb_obj = gluNewNurbsRenderer();
	gluNurbsProperty(nrb_obj, GLU_SAMPLING_TOLERANCE, 25.0);
}

void drawCP(void)//4つの点たち
{
	int i;
	float x,y,z;
	glColor3f(1.0,1.0,1.0);
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
		//gluLookAt(2,0,4, 2,1,1, 0,1,0);//(カメラ位置,カメラ向き,謎)
		polarview();
		glEnable( GL_DEPTH_TEST );//曲線
			glLineWidth(1);//線の太さ
			glColor3f(1.0, 1.0, 1.0);//曲線色

			gluBeginCurve(nrb_obj);//曲線
				gluNurbsCurve(nrb_obj,
							8, knotvec,
							4,
							&cpoint[0][0],
							4,
							GL_MAP1_VERTEX_4);
			gluEndCurve(nrb_obj);

			//drawCP();//4つの点
		glDisable( GL_DEPTH_TEST );

		glPushMatrix();//地面
			glBegin(GL_LINES);
				glColor3f(0, 0, 1.0);//地面色
				for (float f = -40; f < 60; f += 2)
				{
					glVertex3f((float)f, -5, -40.0);
					glVertex3f((float)f, -5, 60.0);
					glVertex3f(-50, -5, (float)f);
					glVertex3f(50, -5, (float)f);
				}
			glEnd();
		glPopMatrix();

		glPushMatrix();//サークル
		//glRotatef(angle, circle_z, 0, 0);//サークル回転
		glTranslatef(circle_x, circle_y, circle_z); //サークル位置
		glColor3f(1.0, 0, 0);						//サークル色
		glutWireTorus(0.05, 0.25, 50, 100);			//サークル(太さ,大きさ,?,?)
		glPopMatrix();

	glPopMatrix();

	glutSwapBuffers();
}

void myKbd(unsigned char key, int x, int y)//キーボード処理
{
	if (key == 'w')circle_y += 0.01; //S字から見て上下
	else if (key == 'a') circle_z += 0.01; //S字から見て左右
	else if (key == 's') circle_y -= 0.01; //S字から見て上下
	else if (key == 'd') circle_z -= 0.01; //S字から見て左右
	else if (key == 'q') angle += 1;//S字から見て上下
	else if (key == 'e') angle -= 1;//S字から見て左右

		glutPostRedisplay();
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
    case GLUT_LEFT_BUTTON://視点移動
        azimuth += (float) xDisp/2.0;
        elevation -= (float) yDisp/2.0;
        break;
    case GLUT_MIDDLE_BUTTON://回転
		circle_x += 0.001;
        //twist = fmod (twist + xDisp, 360.0);
        break;
    case GLUT_RIGHT_BUTTON://拡大縮小
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
