/************************************************************************/
/*       Program   "c8-2.c"                                             */
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
#define imageWidth 512
#define imageHeight 512

void polarview( void );
void resetview( void );
unsigned char image[imageHeight][imageWidth][4];
float distance, twist, elevation, azimuth;
float nearw = 1.0, farw = 20.0, fovy = 60.0;
int mflag=1;
float dist=0.0;

void display(void)
{
    float diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    float specular[] = { 1.0, 1.0, 1.0, 1.0 };
    float ambient[] = { 0.1, 0.1, 0.1, 1.0 };

	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glPushMatrix();
	polarview();
    glEnable( GL_DEPTH_TEST );

	glMaterialfv( GL_FRONT, GL_DIFFUSE, diffuse );
	glMaterialfv( GL_FRONT, GL_SPECULAR, specular );
	glMaterialfv( GL_FRONT, GL_AMBIENT, ambient );
	glMaterialf( GL_FRONT, GL_SHININESS, 128.0 );
    glEnable( GL_LIGHTING );
	glTranslatef(0.0,0.0,dist);
	glutSolidSphere( 1.0 ,60 , 60);
	glPopMatrix();
	glDisable( GL_LIGHTING );
    glDisable( GL_DEPTH_TEST );
	glutSwapBuffers();
}

void idle( void )
{
	if (mflag==1) {
		if (dist<-farw) {
			mflag=0;
			dist=-farw;
		}
		else dist-=0.02;
	}
	else {
		if (dist>0.0) {
			mflag=1;
			dist=0.0;
		}
		else dist+=0.005;
	}
	glutPostRedisplay();
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

void makeImage(void)
{
	int i, j, c,d;

    for (i = 0; i < imageHeight; i++) {
        for (j = 0; j < imageWidth; j++) {
			d=150.0*(rand()/(float)RAND_MAX);
			c=d+50;
            image[i][j][0] = (unsigned char) c;
            image[i][j][1] = (unsigned char) 0;
            image[i][j][2] = (unsigned char) 0;
            image[i][j][3] = (unsigned char) 200;
        }
    }
    for (i = 0; i < imageHeight; i+=32) {
        for (j = 0; j < imageWidth; j++) {
			image[i][j][0] = (unsigned char) 0;
			image[i][j][1] = (unsigned char) 255;
			image[i][j][2] = (unsigned char) 255;
			image[i][j][3] = (unsigned char) 255;
        }
    }
}

void initTexture(void)
{
	makeImage();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, imageWidth, imageHeight,
                0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glEnable(GL_TEXTURE_2D);
}

/*
//MIPMAP設定を行なう場合は上記initTextureをコメントアウトしこちらを生かす
void initTexture(void)
{
	makeImage();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, imageWidth, imageHeight,
								GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glEnable(GL_TEXTURE_2D);
}
*/

void myInit (char *progname)
{
    int width = 400, height = 400;

    glutInitWindowPosition(0, 0);
	glutInitWindowSize( width, height);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow(progname);
    glClearColor (0.0, 0.0, 0.0, 1.0);
    glutKeyboardFunc(myKbd);
	resetview();
	initTexture();
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
    elevation = 0.0;
    azimuth = 0.0;
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	myInit(argv[0]);
	glutReshapeFunc (myReshape);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutMainLoop();
	return(0);
}