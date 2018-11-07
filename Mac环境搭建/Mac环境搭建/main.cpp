#include <iostream>
#include "math3d.h"

#include "GLShaderManager.h"
#include "GLTools.h"
#include <GLUT/GLUT.h>

//正方形边长
GLfloat blockSize = 0.1;
//正方形四个顶点坐标，顺序：左下、右下、右上、左上
GLfloat vVerts[] = {
    -blockSize,-blockSize,0.0f,
    blockSize,-blockSize,0.0f,
    blockSize,blockSize,0.0f,
    -blockSize,blockSize,0.0f
};

//定义一个着色管理器
GLShaderManager shaderManager;
//GLTools的一个简单容器
GLBatch triangleBatch;

void drawSquare();
void drawCircle();
void drawSin();

int moveSquare(int argc, char *argv[]);
int playUpTriangle(int argc,char* argv[]);
void changeSize(int w, int h);
void specialKeys(int key, int x, int y);
void renderScene();
void setUpRC();
void moveSSetUpRC();

void draw()
{
//    drawSquare();
//    drawCircle();
    drawSin();
}

void drawSin()
{
    glClearColor(0.3f, 0.8f, 0.6f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.3f, 0.3f, 0.9f);
    
    //绘图模式：两点连成直线
    glBegin(GL_LINES);
    //绘制X轴
    glVertex2f(-1.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);
    //绘制Y轴
    glVertex2f(0.0f, -1.0f);
    glVertex2f(0.0f, 1.0f);
    glEnd();
    
    //绘图模式：绘制连续的直线
    glBegin(GL_LINE_STRIP);
    //factor值越大，则正弦周期越长；反之，越小
    const GLfloat factor = 0.1f;
    GLfloat x;
    for (x=-1.0f/factor; x<1.0f/factor; x+=0.01f) {
        //y=k*sinx为正弦函数，k>0,原点左侧半个周期处在第二象限；k<0为第三象限
        glVertex2f(x*factor, sin(x)*factor);
    }
    glEnd();
    
    glFlush();
}

void drawSquare()
{
    //设置窗口背景颜色：red\green\blue\alpha
    glClearColor(0.3f, 0.2f, 0.1f, 0.0f);
    //清楚缓存区
    glClear(GL_COLOR_BUFFER_BIT);
    //设置图形颜色
    glColor3f(0.0f, 0.0f, 1.0f);
    //设置绘图坐标系
    glOrtho(1.0f, 0.0f, 1.0f, 0.0f, 1.0f, -1.0f);
    
    //开始渲染
    glBegin(GL_POLYGON);
    
    //设置正方形的四个顶点
    glVertex3f(0.25f, 0.25f, 0.0f);
    glVertex3f(0.75f, 0.25f, 0.0f);
    glVertex3f(0.75f, 0.75f, 0.0f);
    glVertex3f(0.25f, 0.75f, 0.0f);
    
    //结束渲染
    glEnd();
    
    //强制刷新缓存区，保证绘制命令得以执行
    glFlush();
}

void drawCircle()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.3f, 1.0f, 0.4f);
    glBegin(GL_POLYGON);
    
    const int n = 55;
    const GLfloat r = 0.5f;
    const GLfloat pi = 3.1415926f;
    for (int i = 0; i < n; i++) {
        glVertex2f(r*cos(2*pi/n*i), r*sin(2*pi/n*i));
    }
    glEnd();
    glFlush();
}

void drawGraphics(int argc,char* argv[])
{
    //初始化一个GLUT库
    glutInit(&argc, (char **)argv);
    //创建窗口并命名
    glutCreateWindow("LYB_Window");
    //注册绘图函数
    glutDisplayFunc(draw);
    //开启循环：处理窗口和操作系统的用户输入等操作
    glutMainLoop();
}

int main(int argc,char* argv[])
{
    drawGraphics(argc,argv);
//    playUpTriangle(argc,argv);
//    moveSquare(argc, argv);
    
    return 0;
}

int moveSquare(int argc, char *argv[])
{
    //设置当前工作目录——仅针对Mac OS
    gltSetWorkingDirectory(argv[0]);
    
    glutInit(&argc, (char **)argv);
    /*初始化双缓冲窗口
     GLUT_DOUBLE：双缓冲窗口；
     GLUT_RGBA：RGBA颜色模式；
     GLUT_DEPTH：深度测试；
     GLUT_STENCIL：模版缓冲区
     */
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH|GLUT_STENCIL);
    //设置窗口大小
    glutInitWindowSize(800, 600);
    glutCreateWindow("MoveSquare");
    
    //注册重塑函数
    glutReshapeFunc(changeSize);
    //注册显示函数
    glutDisplayFunc(renderScene);
    glutSpecialFunc(specialKeys);
    
    //Open GL API可用和驱动程序初始化正常的容错判断
    GLenum status = glewInit();
    if (status != GLEW_OK) {
        printf("GLew Error : %s\n", glewGetErrorString(status));
        return 1;
    }
    
    //设置渲染环境
    moveSSetUpRC();
    glutMainLoop();
    
    return 0;
}

int playUpTriangle(int argc,char* argv[])
{
    gltSetWorkingDirectory(argv[0]);
    glutInit(&argc, (char **)argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH|GLUT_STENCIL);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Triangle");
    glutReshapeFunc(changeSize);
    glutDisplayFunc(renderScene);
    
    GLenum status = glewInit();
    if (status != GLEW_OK) {
        printf("GLew Error:%s\n", glewGetErrorString(status));
        return 1;
    }
    
    setUpRC();
    
    glutMainLoop();
    
    return 0;
}

void specialKeys(int key, int x, int y)//key由系统自动识别获取
{
    /*
     判断：x值以正方形最左边为基准；y值以正方形最上边为基准；
     blockX、blockY值不能赋值于常量——因为vVerts四个点是动态变化的（SpecialKeys也会随之被动态调用）；
     */
    GLfloat blockX = vVerts[0];
    GLfloat blockY = vVerts[10];
    GLfloat stepSize = 0.025;
    
    if (key == GLUT_KEY_UP) {
        blockY += stepSize;
    }
    
    if (key == GLUT_KEY_DOWN) {
        blockY -= stepSize;
    }
    
    if (key == GLUT_KEY_LEFT) {
        blockX -= stepSize;
    }
    
    if (key == GLUT_KEY_RIGHT) {
        blockX += stepSize;
    }
    
    //触碰到窗口边界处理
    if (blockX < -1.0) {
        blockX = -1.0;
    }
    
    if (blockX > (1.0-blockSize*2)) {
        blockX = 1.0-blockSize*2;
    }
    
    if (blockY > 1.0) {
        blockY = 1.0;
    }
    
    if (blockY < -1.0+blockSize*2) {
        blockY = -1.0+blockSize*2;
    }
    
    //正方形四个顶点坐标动态赋值
    vVerts[0] = blockX;
    vVerts[1] = blockY-blockSize*2;
    
    vVerts[3] = blockX+blockSize*2;
    vVerts[4] = blockY-blockSize*2;
    
    vVerts[6] = blockX+blockSize*2;
    vVerts[7] = blockY;
    
    vVerts[9] = blockX;
    vVerts[10] = blockY;
    
    triangleBatch.CopyVertexData3f(vVerts);
    
    glutPostRedisplay();
}

//设置图形宽高
void changeSize(int w, int h)
{
    //默认适配窗口宽高
    glViewport(0, 0, w, h);
}

void renderScene()
{
    /*清楚缓存区
     GL_COLOR_BUFFER_BIT：指示当前激活的用来进行颜色写入的缓冲区
     GL_DEPTH_BUFFER_BIT：指示深度缓存区
     GL_STENCIL_BUFFER_BIT：指示模版缓存区
     */
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    //设置颜色
    GLfloat vRed[] = {1.0,0.0, 0.0, 1.0};
    //传递到存储着色器并在屏幕上渲染图形
    shaderManager.UseStockShader(GLT_SHADER_IDENTITY, vRed);
    //提交着色器
    triangleBatch.Draw();
    //后台缓冲区渲染结束后提交给前台
    glutSwapBuffers();
}

void setUpRC()
{
    glClearColor(0.5, 0.9, 0.3, 1);
    //初始化一个渲染管理器
    shaderManager.InitializeStockShaders();
    //指定三角形三个顶点坐标
    GLfloat vVerts[] = {
        -0.5,0.0,0.0,
        0.5,0.0,0.0,
        0.0,0.5,0.0
    };

    triangleBatch.Begin(GL_TRIANGLES, 3);//三角形三个顶点
    triangleBatch.CopyVertexData3f(vVerts);
    triangleBatch.End();
}

void moveSSetUpRC()
{
    glClearColor(0.1, 0.4, 0.9, 1);
    shaderManager.InitializeStockShaders();
    triangleBatch.Begin(GL_TRIANGLE_FAN, 4);//正方形四个顶点
    triangleBatch.CopyVertexData3f(vVerts);
    triangleBatch.End();
}
