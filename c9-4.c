/************************************************************************/
/*       ProgramÅ@Å@"c8-4.c"                                            */
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
#define imageWidth 8
#define imageHeight 8

void polarview( void );
void resetview( void );
unsigned char image1[imageHeight][imageWidth][4];
unsigned char image2[imageHeight][imageWidth][4];
int xBegin = 0, yBegin = 0;
int mButton;
float distance, twist, elevation, azimuth;
float nearw = 1.0, farw = 30.0, fovy = 60.0;
int m=1;

void initTexture(void);

void display(void)
{
    float diffuse[] = { 0.0, 0.0, 1.0, 1.0 };
    float specular[] = { 1.0, 1.0, 1.0, 1.0 };
    float ambient[] = { 0.1, 0.1, 0.1, 1.0 };
				 
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glPushMatrix();
	polarview();
    glEnable( GL_DEPTH_TEST );
	glEnable(GL_TEXTURE_2D);

	glMaterialfv( GL_FRONT, GL_DIFFUSE, diffuse );
	glMaterialfv( GL_FRONT, GL_SPECULAR, specular );
	glMaterialfv( GL_FRONT, GL_AMBIENT, ambient );
	glMaterialf( GL_FRONT, GL_SHININESS, 128.0 );
    glEnable( GL_LIGHTING );

	glBindTexture(GL_TEXTURE_2D,m);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0,0.0); glVertex3f(-0.5,-0.5,0.5);
		glTexCoord2f(0.0,5.0); glVertex3f(-0.5,0.5,0.5);
		glTexCoord2f(5.0,5.0); glVertex3f(0.5,0.5,0.5);
		glTexCoord2f(5.0,0.0); glVertex3f(0.5,-0.5,0.5);
	glEnd();

	glBindTexture(GL_TEXTURE_2D,m);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0,0.0); glVertex3f(0.5,-0.5,0.5);
		glTexCoord2f(0.0,5.0); glVertex3f(0.5,-0.5,-0.5);
		glTexCoord2f(5.0,5.0); glVertex3f(0.5,0.5,-0.5);
		glTexCoord2f(5.0,0.0); glVertex3f(0.5,0.5,0.5);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glDisable( GL_LIGHTING );
    glDisable( GL_DEPTH_TEST );
	glutSwapBuffers();
}

void myKbd(unsigned char key, int x, int y)
{
	switch(key) {
    case KEY_ESC:
        exit( 0 );
    case 'a':
        m=2;
	printf("push a\n");
	glutPostRedisplay();
	break;
    case 'b':
        m=1;
	printf("push a\n");
	glutPostRedisplay();
	break;
  	default:
   		break;
	}
}

unsigned char bitmap1[imageHeight][imageWidth]=
{
	{0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00},
	{0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00},
	{0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00},
	{0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff},
	{0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff},
	{0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff},
	{0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff}
};
unsigned char bitmap2[imageHeight][imageWidth]=
{
	{0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x00},
	{0x00,0x00,0xff,0xff,0xff,0xff,0x00,0x00},
	{0x00,0xff,0xff,0xff,0xff,0xff,0xff,0x00},
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},
	{0x00,0xff,0xff,0xff,0xff,0xff,0xff,0x00},
	{0x00,0x00,0xff,0xff,0xff,0xff,0x00,0x00},
	{0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x00}
};


void makeImage1(void)
{
	int i, j, c;

    for (i = 0; i < imageHeight; i++) {
        for (j = 0; j < imageWidth; j++) {
			c = bitmap1[i][j];
            image1[i][j][0] = (unsigned char) c;
            image1[i][j][1] = (unsigned char) c;
            image1[i][j][2] = (unsigned char) c;
            image1[i][j][3] = (unsigned char) 128;
        }
    }
}
void makeImage2(void)
{
	int i, j, c;

    for (i = 0; i < imageHeight; i++) {
        for (j = 0; j < imageWidth; j++) {
			c = bitmap2[i][j];
            image2[i][j][0] = (unsigned char) c;
            image2[i][j][1] = (unsigned char) 0;
            image2[i][j][2] = (unsigned char) 0;
            image2[i][j][3] = (unsigned char) 128;
        }
    }
}

void initTexture1(void)
{
	makeImage1();
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D,1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 4,imageWidth ,imageHeight,0,
								GL_RGBA, GL_UNSIGNED_BYTE, image1);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

}

void initTexture2(void)
{
	makeImage2();
	
//	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D,2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 4,imageWidth ,imageHeight,0,
								GL_RGBA, GL_UNSIGNED_BYTE, image2);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

}

void myInit (char *progname)
{
    glutInitWindowPosition(0, 0);
	glutInitWindowSize( 400, 400);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow(progname);
    glClearColor (0.0, 0.0, 0.0, 1.0);
    glutKeyboardFunc(myKbd);
		
	resetview();
	initTexture1();
	initTexture2();
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
    elevation = -30.0;
    azimuth = 30.0;
}

void myMouse( int button, int state, int x, int y )
{
	if( state == GLUT_DOWN) {
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

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	myInit(argv[0]);
	glutReshapeFunc (myReshape);
	glutDisplayFunc(display);
	glutMouseFunc(myMouse);
	glutMotionFunc( myMotion );
	glutMainLoop();
}


