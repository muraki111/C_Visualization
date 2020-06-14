/************************************************************************/
/*       Program　　"c9-1.c"                                            */
/*                                                                      */
/*       Copyright (c) 2003 by Kiyotaka Kato and Takefumi Hayashi       */
/*                       All rights reserved                            */
/*       And modified by Miyachi 2016                                   */
/*                                                                      */
/************************************************************************/
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <stdio.h>
#include "read_bitmap.h"
#define KEY_ESC 27
#define imageWidth 8
#define imageHeight 8

int ReadBitMapData();
void polarview( void );
void resetview( void );
void read_file1( void );
unsigned char *image;

int xBegin = 0, yBegin = 0;
int mButton;
float distance, twist, elevation, azimuth;
float nearw = 1.0, farw = 30.0, fovy = 60.0;
int tnum;
float *r1,*g1,*b1;
float *x,*y,*z;
float *nx,*ny,*nz;
int width,height;
FILE *fp1;


void initTexture(void);

void display(void)
{
    float diffuse[] = { 0.0, 0.0, 1.0, 1.0 };
    float specular[] = { 1.0, 1.0, 1.0, 1.0 };
    float ambient[] = { 0.1, 0.1, 0.1, 1.0 };
    int i;
				 
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glPushMatrix();
	polarview();
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_LIGHTING );
/*
	glMaterialfv( GL_FRONT, GL_DIFFUSE, diffuse );
	glMaterialfv( GL_FRONT, GL_SPECULAR, specular );
	glMaterialfv( GL_FRONT, GL_AMBIENT, ambient );
	glMaterialf( GL_FRONT, GL_SHININESS, 128.0 );
*/
	
//	glScalef(0.1,0.1,0.1);
	for(i=0;i<tnum;i++){
	glBegin(GL_POLYGON);
		glNormal3f(nx[i],ny[i],nz[i]);
		glVertex3f(x[3*i+0],y[3*i+0],z[3*i+0]);
		glVertex3f(x[3*i+1],y[3*i+1],z[3*i+1]);
		glVertex3f(x[3*i+2],y[3*i+2],z[3*i+2]);
	glEnd();
	}

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
  	default:
   		break;
	}
}

void makeImage(void)
{
	int i,j;
//	printf("width, height = %d %d\n",width,height);
	for (i = 0; i < height; i++) {
   		for (j = 0; j < width; j++) {
        	image[0+4*(j+i*width)] = (unsigned char) r1[j+width*i];			// bitmap2に変えたり
        	image[1+4*(j+i*width)] = (unsigned char) g1[j+width*i];			// bitmap1の内容を変えてみる
        	image[2+4*(j+i*width)] = (unsigned char) b1[j+width*i];
        	image[3+4*(j+i*width)] = (unsigned char) 50;
//		printf(" %d %d\n", image[i+j*width+0*width*height] ,image[i+j*width+1*width*height]); 
     	}
	}
}


void initTexture3( void)
{
unsigned char *image3[1];
int imageHeightb, imageWidthb;

	makeImage();

	glBindTexture(GL_TEXTURE_2D,3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 4,width ,height,0,
								GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
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
	initTexture3();
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
	distance = 10.0;
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
//        elevation -= (float) yDisp/2.0;
        break;
    case GLUT_MIDDLE_BUTTON:
        elevation -= (float) yDisp/2.0;
//        twist = fmod (twist + xDisp, 360.0);
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


	printf(" argc = %d\n", argc);
	if( argc < 2 ) {
		fprintf(stderr," Usage: comstl cube.stl\n");
		exit(-1);
	}

	fp1 = fopen(argv[1],"r");
	printf(" file = %s\n", argv[1]);
	if( fp1 == 0 ) { // ファイルを開くのに失敗
		fprintf(stderr," File open error : %s\n",argv[1]);
		exit(-1);
	} else { // こちらは成功
		fprintf(stderr,"File open success : %s\n",argv[1]);
	}
	read_file1();

	myInit(argv[0]);
	glutReshapeFunc (myReshape);
	glutDisplayFunc(display);
	glutMouseFunc(myMouse);
	glutMotionFunc( myMotion );
	glutMainLoop();
}

void read_file1(){
	int i,j,k;
	int maxi = 100000;
	char buf[10000];
	char key[64],key2[64];;
	unsigned int width1, height1;
	float xx,yy,zz;
	float nxx,nyy,nzz;

	nxx=nyy=nzz=0.;
	tnum=0;

	fgets(buf,10000,fp1); // title //
	printf(" buf = %s\n", buf);
	sscanf(buf,"%s",key);
	if (strcmp(key,"solid") == 0 ) {
		printf("OK 1\n");
	} else {
		printf("NG 1: %s\n", key);
	}
	for(i=0;i<maxi;i++){
		fgets(buf,10000,fp1); // title //
		sscanf(buf,"%s",key);
		if (strcmp(key,"facet") == 0 ) {
//			printf("OK: %d\n",i);
		} else 	if (strcmp(key,"endsolid") == 0 ) {
			printf("Normal termination: %s\n", key);
			break;
		} else {
			printf("NG: %d, %s\n",i, key);
			exit(-1);
		}
		fgets(buf,10000,fp1); // outer loop
		fgets(buf,10000,fp1); // point1
		fgets(buf,10000,fp1); // point2
		fgets(buf,10000,fp1); // point3
		fgets(buf,10000,fp1); // end loop

		fgets(buf,10000,fp1); // title //
		sscanf(buf,"%s",key);
		if (strcmp(key,"endfacet") == 0 ) {
//			printf("OK end \n");
		} else {
			printf("NG end: %s\n", key);
			exit(-1);
		}

	}
	tnum = i;
	printf(" tnum = %d\n", tnum);
	x = (float *) malloc(3*tnum*sizeof(float));
	y = (float *) malloc(3*tnum*sizeof(float));
	z = (float *) malloc(3*tnum*sizeof(float));
	nx = (float *) malloc(tnum*sizeof(float));
	ny = (float *) malloc(tnum*sizeof(float));
	nz = (float *) malloc(tnum*sizeof(float));
	rewind(fp1);

	fgets(buf,10000,fp1); // title //
	printf(" buf = %s\n", buf);
	sscanf(buf,"%s",key);
	if (strcmp(key,"solid") == 0 ) {
		printf("OK 1\n");
	} else {
		printf("NG 1: %s\n", key);
	}
	for(i=0;i<tnum;i++){
		fgets(buf,10000,fp1); // title //
		sscanf(buf,"%s %s %f %f %f",key,key2,&nxx,&nyy,&nzz);
		if (strcmp(key,"facet") == 0 ) {
//			printf("OK: %d\n",i);
			nx[i] = nxx;
			ny[i] = nyy;
			nz[i] = nzz;
		} else 	if (strcmp(key,"endsolid") == 0 ) {
			printf("Normal termination: %s\n", key);
			break;
		} else {
			printf("NG: %d, %s\n",i, key);
			exit(-1);
		}
		fgets(buf,10000,fp1); // outer loop
		fgets(buf,10000,fp1); // point1
		sscanf(buf,"%s %f %f %f",key,&xx,&yy,&zz);
		x[3*i+0] = xx;
		y[3*i+0] = yy;
		z[3*i+0] = zz;
		fgets(buf,10000,fp1); // point2
		sscanf(buf,"%s %f %f %f",key,&xx,&yy,&zz);
		x[3*i+1] = xx;
		y[3*i+1] = yy;
		z[3*i+1] = zz;
		fgets(buf,10000,fp1); // point3
		sscanf(buf,"%s %f %f %f",key,&xx,&yy,&zz);
		x[3*i+2] = xx;
		y[3*i+2] = yy;
		z[3*i+2] = zz;
		fgets(buf,10000,fp1); // end loop

		fgets(buf,10000,fp1); // title //
		sscanf(buf,"%s",key);
		if (strcmp(key,"endfacet") == 0 ) {
//			printf("OK STL data was stored \n");
		} else {
			printf("NG end: %s\n", key);
			exit(-1);
		}

	}
	printf("OK STL data was stored \n");

}

