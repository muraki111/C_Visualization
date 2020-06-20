#include <GL/glut.h>

int WindowPositionX = 100;  //生成するウィンドウ位置のX座標
int WindowPositionY = 100;  //生成するウィンドウ位置のY座標
int WindowWidth = 512;    //生成するウィンドウの幅
int WindowHeight = 512;    //生成するウィンドウの高さ
char WindowTitle[] = "世界の始まり";  //ウィンドウのタイトル

//----------------------------------------------------
// 直方体の定義
//----------------------------------------------------
GLdouble vertex[][3] = {
  { 0.0, 0.0, 0.0 },
  { 2.0, 0.0, 0.0 },
  { 2.0, 2.0, 0.0 },
  { 0.0, 2.0, 0.0 },
  { 0.0, 0.0, 30.0 },
  { 2.0, 0.0, 30.0 },
  { 2.0, 2.0, 30.0 },
  { 0.0, 2.0, 30.0 }
};
int face[][4] = {//面の定義
  { 0, 1, 2, 3 },
  { 1, 5, 6, 2 },
  { 5, 4, 7, 6 },
  { 4, 0, 3, 7 },
  { 4, 5, 1, 0 },
  { 3, 2, 6, 7 }
};

//----------------------------------------------------
// 関数プロトタイプ（後に呼び出す関数名と引数の宣言）
//----------------------------------------------------
void Initialize(void);
void Display(void);
void Ground(void);  //大地の描画
//----------------------------------------------------
// メイン関数
//----------------------------------------------------
int main(int argc, char *argv[]){
  glutInit(&argc, argv);//環境の初期化
  glutInitWindowPosition(WindowPositionX, WindowPositionY);//ウィンドウの位置の指定
  glutInitWindowSize(WindowWidth, WindowHeight); //ウィンドウサイズの指定
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);//ディスプレイモードの指定
  glutCreateWindow(WindowTitle);  //ウィンドウの作成
  glutDisplayFunc(Display); //描画時に呼び出される関数を指定する（関数名：Display）
  Initialize(); //初期設定の関数を呼び出す
  glutMainLoop();
  return 0;
}
//----------------------------------------------------
// 初期設定の関数
//----------------------------------------------------
void Initialize(void){
  glClearColor(1.0, 1.0, 1.0, 1.0); //背景色
  glEnable(GL_DEPTH_TEST);//デプスバッファを使用：glutInitDisplayMode() で GLUT_DEPTH を指定する

  gluPerspective(30.0, (double)WindowWidth/(double)WindowHeight, 0.1, 1000.0); //透視投影法の視体積gluPerspactive(th, w/h, near, far);

  gluLookAt(
       0.0, -100.0, 50.0, // 視点の位置x,y,z;
       0.0, 100.0, 0.0,   // 視界の中心位置の参照点座標x,y,z
       0.0, 0.0, 1.0);  //視界の上方向のベクトルx,y,z
}
//----------------------------------------------------
// 描画の関数
//----------------------------------------------------
void Display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //バッファの消去

  //直方体
  glPushMatrix();
  glColor3d(1.0, 0, 0);//色の設定
  glTranslated(0.0, 0.0, 0.0);//平行移動値の設定
  glBegin(GL_QUADS);
  for (int j = 0; j < 6; ++j) {
    for (int i = 0; i < 4; ++i) {
      glVertex3dv(vertex[face[j][i]]);
    }
  }
  glEnd();
  glPopMatrix();

  Ground();

  glutSwapBuffers(); //glutInitDisplayMode(GLUT_DOUBLE)でダブルバッファリングを利用可
}
//----------------------------------------------------
// 大地の描画
//----------------------------------------------------
void Ground(void) {
    double ground_max_x = 300.0;
    double ground_max_y = 300.0;
    glColor3d(0.8, 0.8, 0.8);  // 大地の色
    glBegin(GL_LINES);
    for(double ly = -ground_max_y ;ly <= ground_max_y; ly+=10.0){
      glVertex3d(-ground_max_x, ly,0);
      glVertex3d(ground_max_x, ly,0);
    }
    for(double lx = -ground_max_x ;lx <= ground_max_x; lx+=10.0){
      glVertex3d(lx, ground_max_y,0);
      glVertex3d(lx, -ground_max_y,0);
    }
    glEnd();
}