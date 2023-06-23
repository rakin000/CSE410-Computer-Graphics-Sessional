#include <GL/glut.h>
#include <math.h>
#include <ctime>
#include <chrono>
#include <iostream> 

using namespace std ;

struct Color{
    GLfloat R,G,B ;
} PINK = { 235/255.0, 52/255.0, 186/255.0},CYAN = {52/255.0, 201/255.0, 235/255.0};
#define GLCOLOR(color) glColor3f(color.R,color.G,color.B);

// Global variables
GLfloat eyex = 4, eyey = 4, eyez = 4;
GLfloat centerx = 0, centery = 0, centerz = 0;
GLfloat upx = 0, upy = 1, upz = 0;
bool isAxes = true, isCube = false, isPyramid = false;
GLdouble side = 1; 
GLdouble rot = 1; 

/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // Black and opaque
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
}

void glCircle2d(GLdouble cx,GLdouble cy,GLdouble radius,double r=1,double g=0,double b=0){
    // glBegin(GL_LINE_LOOP);  // All vertices form a single loop of single pixel width
    glBegin(GL_POLYGON);  // All vertices form a single loop of single pixel width
        glColor3f(r,g,b);  // Light-blue
        for (float theta = 0; theta < 360; theta += 10) {
            float x = cx + radius * cos(theta*M_PI/180);
            float y = cy + radius * sin(theta*M_PI/180);
            glVertex2f(x,y);
        }
    glEnd();
}



void drawOctahedron(){
    GLCOLOR(PINK);
    glBegin(GL_TRIANGLES);                      
        glVertex3d(0.0f, 0.0f, side );         
        glVertex3d(0.0f, side, 0.0f );         
        glVertex3d(side , 0.0f, 0.0f );         
    glEnd(); 
    
    GLCOLOR(CYAN) ; 
    glBegin(GL_TRIANGLES);                      
        glVertex3d( 0.0f, 0.0f, side );         
        glVertex3d( 0.0f, side, 0.0f );         
        glVertex3d(-side , 0.0f, 0.0f );         
    glEnd(); 
    
    GLCOLOR(PINK) ; 
    glBegin(GL_TRIANGLES);                      
        glVertex3d( 0.0f, 0.0f, -side );         
        glVertex3d( 0.0f, side, 0.0f );         
        glVertex3d(-side , 0.0f, 0.0f );         
    glEnd(); 
    
    
    GLCOLOR(CYAN) ; 
    glBegin(GL_TRIANGLES);                      
        glVertex3d( 0.0f, 0.0f, -side );         
        glVertex3d( 0.0f, side, 0.0f );         
        glVertex3d(side , 0.0f, 0.0f );         
    glEnd(); 




    GLCOLOR(CYAN);
    glBegin(GL_TRIANGLES);                      
        glVertex3d(0.0f, 0.0f, side );         
        glVertex3d(0.0f, -side, 0.0f );         
        glVertex3d(side , 0.0f, 0.0f );         
    glEnd(); 
    
    GLCOLOR(PINK) ; 
    glBegin(GL_TRIANGLES);                      
        glVertex3d( 0.0f, 0.0f, side );         
        glVertex3d( 0.0f, -side, 0.0f );         
        glVertex3d(-side , 0.0f, 0.0f );         
    glEnd(); 
    
    GLCOLOR(CYAN) ; 
    glBegin(GL_TRIANGLES);                      
        glVertex3d( 0.0f, 0.0f, -side );         
        glVertex3d( 0.0f, -side, 0.0f );         
        glVertex3d(-side , 0.0f, 0.0f );         
    glEnd(); 
    
    
    GLCOLOR(PINK) ; 
    glBegin(GL_TRIANGLES);                      
        glVertex3d( 0.0f, 0.0f, -side );         
        glVertex3d( 0.0f, -side, 0.0f );         
        glVertex3d(side , 0.0f, 0.0f );         
    glEnd(); 
}



/* Draw a pyramid centered at the origin */
void drawPyramid() {
    glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles
        // Front
        glColor3f(1.0f, 0.0f, 0.0f);     // Red
        glVertex3f( 0.0f, 1.0f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f);     // Green
        glVertex3f(-1.0f, -1.0f, 1.0f);
        glColor3f(0.0f, 0.0f, 1.0f);     // Blue
        glVertex3f(1.0f, -1.0f, 1.0f);

        // Right
        glColor3f(1.0f, 0.0f, 0.0f);     // Red
        glVertex3f(0.0f, 1.0f, 0.0f);
        glColor3f(0.0f, 0.0f, 1.0f);     // Blue
        glVertex3f(1.0f, -1.0f, 1.0f);
        glColor3f(0.0f, 1.0f, 0.0f);     // Green
        glVertex3f(1.0f, -1.0f, -1.0f);

        // Back
        glColor3f(1.0f, 0.0f, 0.0f);     // Red
        glVertex3f(0.0f, 1.0f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f);     // Green
        glVertex3f(1.0f, -1.0f, -1.0f);
        glColor3f(0.0f, 0.0f, 1.0f);     // Blue
        glVertex3f(-1.0f, -1.0f, -1.0f);

        // Left
        glColor3f(1.0f,0.0f,0.0f);       // Red
        glVertex3f( 0.0f, 1.0f, 0.0f);
        glColor3f(0.0f,0.0f,1.0f);       // Blue
        glVertex3f(-1.0f,-1.0f,-1.0f);
        glColor3f(0.0f,1.0f,0.0f);       // Green
        glVertex3f(-1.0f,-1.0f, 1.0f);
    glEnd();   // Done drawing the pyramid
}


void displayMe(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);             // To operate on Model-View matrix
    glLoadIdentity();                       // Reset the model-view matrix
    // glRotatef(rot,0.0,1.0,0.0);
    gluLookAt(eyex,eyey,eyez,
              centerx,centery,centerz,
              upx,upy,upz);
    drawOctahedron();   
    // drawPyramid();
    glutSwapBuffers(); 
}


/* Callback handler for normal-key event */
void keyboardListener(unsigned char key, int x, int y) {
    float v = 0.1;
    switch (key) {
    // Control eye (location of the eye)
    // control eyex
    case '1':
        eyex += v;
        break;
    case '2':
        eyex -= v;
        break;
    // control eyey
    case '3':
        eyey += v;
        break;
    case '4':
        eyey -= v;
        break;
    // control eyez
    case '5':
        eyez += v;
        break;
    case '6':
        eyez -= v;
        break;

    // Control center (location where the eye is looking at)
    // control centerx
    case 'q':
        centerx += v;
        break;
    case 'w':
        centerx -= v;
        break;
    // control centery
    case 'e':
        centery += v;
        break;
    case 'r':
        centery -= v;
        break;
    // control centerz
    case 't':
        centerz += v;
        break;
    case 'y':
        centerz -= v;
        break;

    // Control what is shown
    case 'a':
        isAxes = !isAxes;   // show/hide Axes if 'a' is pressed
        break;
    case 'c':
        isCube = !isCube;   // show/hide Cube if 'c' is pressed
        break;
    case 'p':
        isPyramid = !isPyramid; // show/hide Pyramid if 'p' is pressed
        break;

    // Control exit
    case 27:    // ESC key
        exit(0);    // Exit window
        break;
    }
    glutPostRedisplay();    // Post a paint request to activate display()
}

/* Callback handler for special-key event */
void specialKeyListener(int key, int x,int y) {
    double v = 0.25;
    double lx = centerx - eyex;
    double lz = centerz - eyez;
    double s;
    switch (key) {
    case GLUT_KEY_LEFT:
        eyex += v * (upy*lz);
        eyez += v * (-lx*upy);
        s = sqrt(eyex*eyex + eyez*eyez) / (4 * sqrt(2));
        eyex /= s;
        eyez /= s;
        break;
    case GLUT_KEY_RIGHT:
        eyex += v * (-upy*lz);
        eyez += v * (lx*upy);
        s = sqrt(eyex*eyex + eyez*eyez) / (4 * sqrt(2));
        eyex /= s;
        eyez /= s;
        break;
    case GLUT_KEY_UP:
        eyey += v;
        break;
    case GLUT_KEY_DOWN:
        eyey -= v;
        break;
    
    default:
        return;
    }
    glutPostRedisplay();    // Post a paint request to activate display()
}

void reshapeListener(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0) height = 1;                // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping area to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset the projection matrix
    /*if (width >= height) {
        // aspect >= 1, set the height from -1 to 1, with larger width
        gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
    } else {
        // aspect < 1, set the width to -1 to 1, with larger height
        gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
    }*/
    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    // glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(1024, 1024);
    glutInitWindowPosition(100, 100);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color
    glutCreateWindow("Cube");
    glutDisplayFunc(displayMe);
    glutReshapeFunc(reshapeListener);           // Register callback handler for window re-shape
    glutSpecialFunc(specialKeyListener);
    glutKeyboardFunc(keyboardListener);
    initGL();
    glutMainLoop();
    return 0;
}