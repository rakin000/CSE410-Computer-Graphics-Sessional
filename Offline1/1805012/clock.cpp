#include <GL/glut.h>
#include <math.h>
#include <ctime>
#include <chrono>
#include <iostream> 

using namespace std ;
#define GLCOLOR(color) glColor3f(color.R/255.0,color.G/255.0,color.B/255.0)
#define TO_RADIAN(deg) ((deg)*(M_PI/180.0))
#define DEG2RAD (M_PI/180.0) 


struct Color{
    GLfloat R,G,B ;
}   PINK = { 235, 52, 186},
    CYAN = {52, 201, 235},
    RED = {255,0,0},
    BLUE = {0,0,255},
    GREEN = {0,255,0},
    YELLOW = {252, 236, 3};


double hh=11,mm=25,ss=55 ;
double theta ;
double theta_max = 30*M_PI/180.0 ;
double omega ;
double cx=0.0,cy=0.5,r1=0.3,r2=0.4;
double hline_x,hline_y,mline_x,mline_y,sline_x,sline_y ;
double mline_len=r1;
double hline_len=0.2;
double sline_len=0.35 ; 
double pendulumL=0.5;
uint64_t t ; 

void glCircle2d(GLdouble cx,GLdouble cy,GLdouble radius,Color color){
    // glBegin(GL_LINE_LOOP);  // All vertices form a single loop of single pixel width
    glBegin(GL_POLYGON);  // All vertices form a single loop of single pixel width
        GLCOLOR(color);
        for (float theta = 0; theta < 360; theta += 10) {
            float x = cx + radius * cos(theta*M_PI/180);
            float y = cy + radius * sin(theta*M_PI/180);
            glVertex2f(x,y);
        }
    glEnd();
}

void drawClock(GLdouble cx,GLdouble cy){
    glBegin(GL_QUAD_STRIP);
        for(float theta=0;theta <= 360 ; theta += 10){
            GLCOLOR(CYAN);
            glVertex2f(cx + r2 * cos(theta*M_PI/180),cy + r2 * sin(theta*M_PI/180)); 
            GLCOLOR(BLUE);
            glVertex2f(cx + r1 * cos(theta*M_PI/180),cy + r1 * sin(theta*M_PI/180)); 
        }
    glEnd();   

    glCircle2d(cx,cy,0.03,BLUE);

    for(float theta = 0;theta < 360; theta += 6){
        if( (int)theta % 30 == 0 ) glPointSize(5); 
        else glPointSize(1);
        GLCOLOR(CYAN);
        glBegin(GL_POINTS);
            glVertex2f(cx + (r1-0.01) * cos(theta*M_PI/180),cy + (r1-0.01) * sin(theta*M_PI/180)); 
        glEnd();
    } 

    glLineWidth((GLfloat)5);
    glBegin(GL_LINES); 
        GLCOLOR(CYAN);
        glVertex2d(cx, cy);
        glVertex2d(hline_x,hline_y);
    glEnd();
    glLineWidth(1);
    glBegin(GL_LINES) ;
        glColor3f(0.0f,0.0f,1.0f); 
        glVertex2d(sline_x,sline_y);
        glVertex2d( cx,cy);
    glEnd();
    glLineWidth(3);
    glBegin(GL_LINES) ;
        GLCOLOR(CYAN);
        glVertex2d(cx, cy);
        glVertex2d(mline_x,mline_y);
    glEnd();

    glBegin(GL_LINES) ;
        glColor3f(0.0f,0.0f,1.0f);  
        glVertex2d(cx,cy-r2);
        glVertex2d(cx+pendulumL*cos( theta),cy-r2+pendulumL*sin(theta));
    glEnd();
    glCircle2d(cx+pendulumL*cos( theta),cy-r2+pendulumL*sin(theta),0.04,BLUE); 
}

void displayMe(void){
    glClear(GL_COLOR_BUFFER_BIT);
    
    

    hline_x = cx + hline_len* cos ( (3-hh)*30*M_PI/180 ) ;
    hline_y = cy + hline_len* sin ( (3-hh)*30*M_PI/180 ) ;

    mline_x = cx + mline_len* cos ( (15-mm)*6*M_PI/180 ) ;
    mline_y = cy + mline_len* sin ( (15-mm)*6*M_PI/180 ) ;

    sline_x = cx + sline_len* cos ( (15-ss)*6*M_PI/180 ) ;
    sline_y = cy + sline_len* sin ( (15-ss)*6*M_PI/180 ) ;


    omega = M_PI/1000.0 ;
    theta = theta_max * cos(omega*(double)t) - M_PI/2.0;

    // glCircle2d(cx,cy,r2,1,1,1) ;
    // glCircle2d(cx,cy,r1,0,1,0);

    drawClock(cx,cy);

    glFlush();
}


void timer(int extra)
{ 
    // t = time(0) ;
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    // cout<<ms.count()<<endl;
    // cout<<t<<endl;
    t = ms.count();
    time_t tt = ms.count()/1000;
    tm *ltm = localtime(&tt);
    hh=ltm->tm_hour;
    mm=ltm->tm_min;
    ss=ltm->tm_sec;
    glutPostRedisplay();
    glutTimerFunc(100, timer, 0);
}

/* Callback handler for normal-key event */
void keyboard(unsigned char key, int x, int y) {
    // key is the char pressed, e.g., 'a' or 27 for ESC
    // // (x, y) is the mouse location in Windows' coordinates
    // switch (key) {
    // case 'a':
    //     px -= 0.1;
    //     break;
    // case 'd':
    //     px += 0.1;
    
    // default:
    //     break;
    // }
    // glutPostRedisplay();
}

/* Callback handler for special-key event */
void special(int key, int x, int y) {
    // specialKey: GLUT_KEY_* (* for LEFT, RIGHT, UP, DOWN, HOME, END, PAGE_UP, PAGE_DOWN, F1,...F12). 
    // // (x, y) is the mouse location in Windows' coordinates
    // switch(key) {
    // case GLUT_KEY_LEFT:
    //     mm -= 1;
    //     break;
    // case GLUT_KEY_RIGHT:
    //     mm += 1;
    //     break;
    // }
    // glutPostRedisplay();
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(1024, 1024);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Clock");
    glutDisplayFunc(displayMe);
    glutTimerFunc(0, timer, 0);
    // glutIdleFunc(idle);                
    glutSpecialFunc(special);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}