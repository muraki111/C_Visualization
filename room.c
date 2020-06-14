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
unsigned char *r1,*g1,*b1;
int width,height;
FILE *fp1;


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

	glBindTexture(GL_TEXTURE_2D,3);
	glBegin(GL_QUADS);
		glTexCoord2f(0.35,0.0); glVertex3f(-5,-5,-5);
		glTexCoord2f(0.35,0.5); glVertex3f(-5,5,-5);
		glTexCoord2f(0.0,0.5); glVertex3f(5,5,-5);
		glTexCoord2f(0.0,0.0); glVertex3f(5,-5,-5);
	glEnd();

	glBegin(GL_QUADS);
		glTexCoord2f(0.95,0.55); glVertex3f(-5,-5,5);
		glTexCoord2f(0.95,0.95); glVertex3f(-5,5,5);
		glTexCoord2f(0.72,0.95); glVertex3f(5,5,5);
		glTexCoord2f(0.72,0.55); glVertex3f(5,-5,5);
	glEnd();

	glBegin(GL_QUADS);
		glTexCoord2f(0.4,0.0); glVertex3f(-5,-5,-5);
		glTexCoord2f(0.7,0.0); glVertex3f(-5,-5,5);
		glTexCoord2f(0.7,0.5); glVertex3f(5,-5,5);
		glTexCoord2f(0.4,0.5); glVertex3f(5,-5,-5);
	glEnd();

	glBegin(GL_QUADS);
		glTexCoord2f(0.4,1.0); glVertex3f(-5,5,-5);
		glTexCoord2f(0.4,0.5); glVertex3f(-5,5,5);
		glTexCoord2f(0.7,0.5); glVertex3f(5,5,5);
		glTexCoord2f(0.7,1.0); glVertex3f(5,5,-5);
	glEnd();

	glBegin(GL_QUADS);
		glTexCoord2f(1.0,0.0); glVertex3f(5,-5,-5);
		glTexCoord2f(1.0,0.5); glVertex3f(5,5,-5);
		glTexCoord2f(0.7,0.5); glVertex3f(5,5,5);
		glTexCoord2f(0.7,0.0); glVertex3f(5,-5,5);
	glEnd();

	glBegin(GL_QUADS);
		glTexCoord2f(0.35,0.5); glVertex3f(-5,-5,-5);
		glTexCoord2f(0.35,1.0); glVertex3f(-5,5,-5);
		glTexCoord2f(0.0,1.0); glVertex3f(-5,5,5);
		glTexCoord2f(0.0,0.5); glVertex3f(-5,-5,5);
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
	distance = 1.0;
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
		fprintf(stderr," Usage: command <bitmap filename>\n");
		exit(-1);
	}

	fp1 = fopen(argv[1],"rb");
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
	unsigned int width1, height1;
	unsigned short color;
	unsigned int dumlong;
	unsigned char buf10[10];
	unsigned char buf2[2];
	unsigned char buf20[20];
	unsigned int image_start_point,header_size;
	unsigned char pad[4];

/* １．ヘッダ部の読み取り
	  1.1  ファイルヘッダ　14バイトの読み取り */

// バイナリのファイルを読み取るときは　fread 関数を使います。
//                     書き込むときは　fwrite関数を使います。
// アスキーのファイルを読み取るときは　fgets関数を使います。
//  　　　　　　　　　 書き込むときは　fprintf だったかな、を使います。
//
	fread(buf10,sizeof(unsigned char),10,fp1);  // 10byte
	fread(&image_start_point,sizeof(unsigned int),1,fp1); // 14byte
	fprintf(stderr," image start point = %d\n", image_start_point);
	//  1.2 画像ヘッダの読み取り　（image_start_point - 14）バイト
	//  (1) ヘッダサイズ
	fread(&header_size,sizeof(unsigned int),1,fp1); //18byte
        fprintf(stderr," header_size = %ld\n", header_size);
	if( image_start_point == (14 + header_size) ) {
		fprintf(stderr," header size OK! \n");
	} else {
		fprintf(stderr," header size error \n");
		exit(-1);
	} 
	//  (2) 画像サイズ　（幅と高さ）
	fread(&width1,sizeof(unsigned int),1,fp1); // 22byte
	fread(&height1,sizeof(unsigned int),1,fp1); //26byte
	fprintf(stderr," Width, Height = %d %d\n", width1, height1);

	r1 = (unsigned char *) malloc(width1*height1);
	g1 = (unsigned char *) malloc(width1*height1);
	b1 = (unsigned char *) malloc(width1*height1);
	image = (unsigned char *) malloc(4*width1*height1);

	width = width1;
	height = height1;

	//  (3) ２バイト飛ばし（プレーン数）
 	fread(buf2,sizeof(char),2,fp1); //28byte
	//  (4) Color サイズ　（このプログラムは24bit true color のみ）
	fread(&color,sizeof(unsigned short),1,fp1); //30byte
	if( color == 24 ) {  // 24bit true color
		fprintf(stderr," It's 24bit true color. OK! \n");
	} else {
		fprintf(stderr," Oh! No. It's not 24bit color. I hate it.\n");
		exit(-1);
	}
	//  (5) 圧縮形式
	fread(&dumlong,sizeof(unsigned int),1,fp1); //34byte
	if( dumlong == 0 ) {  // 無圧縮
		fprintf(stderr," It's non compress data. OK! \n");
	} else {
		fprintf(stderr," Oh! No. Why did you compress the image ? %d\n",dumlong);
		exit(-1);
	}

 	fread(buf20,sizeof(char),20,fp1);

	k=0;
	for(j=0;j<height1;j++){
	    for(i=0;i<width1;i++){
		fread(&b1[k],sizeof(unsigned char),1,fp1);
		fread(&g1[k],sizeof(unsigned char),1,fp1);
		fread(&r1[k],sizeof(unsigned char),1,fp1);
		k++;
	    }
	    if( (3*width)%4 != 0 ) fread(pad,sizeof(unsigned char), (4-(3*width)%4),fp1);
	}
}

