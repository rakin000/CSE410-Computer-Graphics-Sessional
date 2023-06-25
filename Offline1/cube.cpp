#include <GL/glut.h>
#include <math.h>
#include <ctime>
#include <chrono>
#include <iostream> 
#include <bits/stdc++.h>

using namespace std ;

struct Color{
    GLfloat R,G,B ;
}   PINK = { 235/255.0, 52/255.0, 186/255.0},
    CYAN = {52/255.0, 201/255.0, 235/255.0},
    RED = {1,0,0},
    BLUE = {0,0,1},
    GREEN = {0,1,0} ;

    
#define GLCOLOR(color) glColor3f(color.R,color.G,color.B)
#define TO_RADIAN(deg) ((deg)*(M_PI/180.0))
#define DEG2RAD (M_PI/180.0) 

// Global variables
GLfloat eyex = 4, eyey = 4, eyez = 4;
GLfloat centerx = 0, centery = 0, centerz = 0;
GLfloat upx = 0, upy = 1, upz = 0;
bool isAxes = true, isCube = false, isPyramid = false;
GLdouble side = 1; 
GLdouble rot = 1; 
GLdouble growSphere = 30 ;
const int pointsPerRow = 1<<4 ;
// vector<vector<vector<GLdouble> > > verticesX(pointsPerRow, vector<vector<GLdouble>> (pointsPerRow, vector<GLdouble> (3))),
//                                    verticesY(pointsPerRow, vector<vector<GLdouble>> (pointsPerRow, vector<GLdouble> (3))),
//                                    verticesZ(pointsPerRow, vector<vector<GLdouble>> (pointsPerRow, vector<GLdouble> (3))) ;
GLdouble verticesX[pointsPerRow][pointsPerRow][3];
GLdouble verticesY[pointsPerRow][pointsPerRow][3];
GLdouble verticesZ[pointsPerRow][pointsPerRow][3];

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

void drawSphere(GLdouble radius,GLdouble cx,GLdouble cy,GLdouble cz){
    GLdouble theta,phi ;
    GLdouble x,y,z; 
    z = cz+radius * cos( TO_RADIAN(phi) ) ;
    y = cy + radius * sin( TO_RADIAN(phi) ) * sin( TO_RADIAN(theta) ) ;
    x = cx + radius * sin( TO_RADIAN(phi) ) * cos( TO_RADIAN(theta) ) ;
    for(phi=180;phi>0;phi-=5){
        glBegin(GL_QUAD_STRIP);
        for(theta=0;theta<=360;theta+=5){
            GLCOLOR(CYAN);
            z = cz+radius * cos( TO_RADIAN(phi) ) ;
            y = cy+radius * sin( TO_RADIAN(phi) ) * sin( TO_RADIAN(theta) ) ;
            x = cx+radius * sin( TO_RADIAN(phi) ) * cos( TO_RADIAN(theta) ) ;
            glVertex3d(x,y,z) ;
            
            GLCOLOR(PINK);
            z = cz+radius * cos( TO_RADIAN(phi-5) ) ;
            y = cy + radius * sin( TO_RADIAN(phi-5) ) * sin( TO_RADIAN(theta) ) ;
            x = cx + radius * sin( TO_RADIAN(phi-5) ) * cos( TO_RADIAN(theta) ) ;
            glVertex3d(x,y,z) ;
        }
        glEnd();
    }
}

void drawAxes() {
    glLineWidth(3);
    glBegin(GL_LINES);
        glColor3f(1,0,0);   // Red
        // X axis
        glVertex3f(0,0,0);
        glVertex3f(2,0,0);

        glColor3f(0,1,0);   // Green
        // Y axis
        glVertex3f(0,0,0);        glVertex3f(0,1,0);

        glColor3f(0,0,2);   // Blue
        // Z axis
        glVertex3f(0,0,0);
        glVertex3f(0,0,2);
    glEnd();
}

void draw2(){
    GLdouble theta,phi ;
    GLdouble x,y,z; 
    GLdouble cz=0,cy=0,cx=0,radius=1;
    z = cz+radius * cos( TO_RADIAN(phi) ) ;
    y = cy + radius * sin( TO_RADIAN(phi) ) * sin( TO_RADIAN(theta) ) ;
    x = cx + radius * sin( TO_RADIAN(phi) ) * cos( TO_RADIAN(theta) ) ;
    // for(phi=180;phi>180-growSphere;phi-=5){
    //     glBegin(GL_QUAD_STRIP);
    //     for(theta=0;theta<=360;theta+=5){
    //         // GLCOLOR(CYAN);
    //         z = cz+radius * cos( TO_RADIAN(phi) ) ;
    //         y = cy+radius * sin( TO_RADIAN(phi) ) * sin( TO_RADIAN(theta) ) ;
    //         x = cx+radius * sin( TO_RADIAN(phi) ) * cos( TO_RADIAN(theta) ) ;
    //         glVertex3d(x,y,z) ;
            
    //         // GLCOLOR(PINK);
    //         z = cz+radius * cos( TO_RADIAN(phi-5) ) ;
    //         y = cy + radius * sin( TO_RADIAN(phi-5) ) * sin( TO_RADIAN(theta) ) ;
    //         x = cx + radius * sin( TO_RADIAN(phi-5) ) * cos( TO_RADIAN(theta) ) ;
    //         glVertex3d(x,y,z) ;
    //     }
    //     glEnd();
    // }    
    // for(phi=0;phi<growSphere;phi+=5){
    //     glBegin(GL_QUAD_STRIP);
    //     for(theta=0;theta<=360;theta+=5){        
    //         // GLCOLOR(CYAN);
    //         z = cz+radius * cos( TO_RADIAN(phi) ) ;
    //         y = cy+radius * sin( TO_RADIAN(phi) ) * sin( TO_RADIAN(theta) ) ;
    //         x = cx+radius * sin( TO_RADIAN(phi) ) * cos( TO_RADIAN(theta) ) ;
    //         glVertex3d(x,y,z) ;
            
    //         // GLCOLOR(PINK);
    //         z = cz+radius * cos( TO_RADIAN(phi+5) ) ;
    //         y = cy + radius * sin( TO_RADIAN(phi+5) ) * sin( TO_RADIAN(theta) ) ;
    //         x = cx + radius * sin( TO_RADIAN(phi+5) ) * cos( TO_RADIAN(theta) ) ;
    //         glVertex3d(x,y,z) ;

    //     }
    //     glEnd();
    // }

    // GLdouble dPhi = 5;; 
    // GLdouble dTheta = 5 ;
    // for(phi=-growSphere;phi<growSphere;phi+=dPhi ){
    //     glBegin(GL_QUAD_STRIP);
    //     for(theta=-growSphere;theta<growSphere;theta+=dTheta){
    //         z = cz+radius * cos( TO_RADIAN(phi) ) ;
    //         y = cy+radius * sin( TO_RADIAN(phi) ) * sin( TO_RADIAN(theta) ) ;
    //         x = cx+radius * sin( TO_RADIAN(phi) ) * cos( TO_RADIAN(theta) ) ;
    //         glVertex3d(x,y,z) ;
            
    //         // GLCOLOR(PINK);
    //         z = cz+radius * cos( TO_RADIAN(phi+5) ) ;
    //         y = cy + radius * sin( TO_RADIAN(phi+5) ) * sin( TO_RADIAN(theta) ) ;
    //         x = cx + radius * sin( TO_RADIAN(phi+5) ) * cos( TO_RADIAN(theta) ) ;
    //         glVertex3d(x,y,z) ;

 
    //     }
    //     glEnd();
    // }

    GLdouble dx = 0.1,dy,dz;
    GLCOLOR(RED);
    for(dx=0.1,x=-growSphere;x<growSphere;x+=dx){
        glBegin(GL_QUAD_STRIP);
        for(dy=0.1,y=-growSphere;y<=growSphere;y+=dy){
            z = sqrt(radius*radius-x*x-y*y) ;
            glVertex3d(x,y,z) ;
            x += dx; 
            z = sqrt(radius*radius-x*x-y*y) ;
            glVertex3d(x,y,z) ;
            x -= dx ;
        }
        glEnd();

        glBegin(GL_QUAD_STRIP);
        for(dy=0.1,y=-growSphere;y<=growSphere;y+=dy){
            z = -sqrt(radius*radius-x*x-y*y) ;
            glVertex3d(x,y,z) ;
            x += dx; 
            z = -sqrt(radius*radius-x*x-y*y) ;
            glVertex3d(x,y,z) ;
            x -= dx ;
        }
        glEnd();


        
    }

    GLCOLOR(GREEN);
    for(dx=0.1,x=-growSphere;x<growSphere;x+=dx){
        glBegin(GL_QUAD_STRIP);
        for(dz=0.1,z=-growSphere;z<=growSphere;z+=dz){
            y = sqrt(radius*radius-x*x-z*z) ;
            glVertex3d(x,y,z) ;
            x += dx; 
            y = sqrt(radius*radius-x*x-z*z) ;
            glVertex3d(x,y,z) ;
            x -= dx ;
        }
        glEnd();
        
        glBegin(GL_QUAD_STRIP);
        for(dz=0.1,z=-growSphere;z<=growSphere;z+=dz){
            y = -sqrt(radius*radius-x*x-z*z) ;
            glVertex3d(x,y,z) ;
            x += dx; 
            y = -sqrt(radius*radius-x*x-z*z) ;
            glVertex3d(x,y,z) ;
            x -= dx ;
        }
        glEnd();
    }

    GLCOLOR(BLUE);
    for(dy=0.1,y=-growSphere;y<growSphere;y+=dy){
        glBegin(GL_QUAD_STRIP);
        for(dz=0.1,z=-growSphere;z<=growSphere;z+=dz){
            x = sqrt(radius*radius-y*y-z*z) ;
            glVertex3d(x,y,z) ;
            y += dy; 
            x = sqrt(radius*radius-y*y-z*z) ;
            glVertex3d(x,y,z) ;
            y -= dy ;
        }
        glEnd();

        glBegin(GL_QUAD_STRIP);
        for(dz=0.1,z=-growSphere;z<=growSphere;z+=dz){
            x = -sqrt(radius*radius-y*y-z*z) ;
            glVertex3d(x,y,z) ;
            y += dy; 
            x = -sqrt(radius*radius-y*y-z*z) ;
            glVertex3d(x,y,z) ;
            y -= dy ;
        }
        glEnd();

    }
}


void generateVertices(){
    float n1[3];        // normal of longitudinal plane rotating along Y-axis
    float n2[3];        // normal of latitudinal plane rotating along Z-axis
    float v[3];         // direction vector intersecting 2 planes, n1 x n2
    float a1;           // longitudinal angle along Y-axis
    float a2;  
    // rotate latitudinal plane from 45 to -45 degrees along Z-axis (top-to-bottom)
    for(unsigned int i = 0; i < pointsPerRow; ++i){
        a2 = DEG2RAD * (growSphere - 2.0*growSphere * i / (pointsPerRow - 1));
        n2[0] = -sin(a2);
        n2[1] = cos(a2);
        n2[2] = 0;
        // rotate longitudinal plane from -45 to 45 along Y-axis (left-to-right)
        for(unsigned int j = 0; j < pointsPerRow; ++j){
            a1 = DEG2RAD * (-growSphere + 2.0 * growSphere * j / (pointsPerRow - 1));
            n1[0] = -sin(a1);
            n1[1] = 0;
            n1[2] = -cos(a1);

            // find direction vector of intersected line, n1 x n2
            v[0] = n1[1] * n2[2] - n1[2] * n2[1];
            v[1] = n1[2] * n2[0] - n1[0] * n2[2];
            v[2] = n1[0] * n2[1] - n1[1] * n2[0];

            // normalize direction vector
            double scale = 1 / sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
            v[0] *= scale;
            v[1] *= scale;
            v[2] *= scale;

            // add a vertex into array
            verticesX[i][j][0] = v[0];
            verticesX[i][j][1] = v[1];
            verticesX[i][j][2] = v[2];
        }
    }
    // cerr<<"great success"; 
    // generate Y+ vertices 
    for(int i=0;i<pointsPerRow;i++){
        a2 = DEG2RAD * (growSphere - 2.0*growSphere*i/(pointsPerRow-1.0));
        n2[0] = cos(a2);
        n2[1] = sin(a2);
        n2[2] = 0;
        for(int j=0;j<pointsPerRow;j++){
            a1 = DEG2RAD * (growSphere - 2.0*growSphere*j/(pointsPerRow-1.0));
            n1[0] = 0;
            n1[1] = sin(a1); 
            n1[2] = cos(a1);

            v[0] = n1[1]*n2[2] - n1[2]*n2[1];
            v[1] = n1[2]*n2[0] - n1[0]*n2[2];
            v[2] = n1[0]*n2[1] - n1[1]*n2[0];

            // normalize direction vector
            double scale = 1 / sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
            v[0] *= scale;
            v[1] *= scale;
            v[2] *= scale;

            // add a vertex into array
            verticesY[i][j][0] = v[0];
            verticesY[i][j][1] = v[1];
            verticesY[i][j][2] = v[2];
        }
    }

    // generate Z+ vertices 
    for(int i=0;i<pointsPerRow;i++){
        a2 = DEG2RAD * (growSphere - 2.0*growSphere*i/(pointsPerRow-1.0));
        n2[0] = 0; 
        n2[1] = cos(a2);
        n2[2] = sin(a2);
        for(int j=0;j<pointsPerRow;j++){
            a1 = DEG2RAD * (growSphere - 2.0*growSphere*j/(pointsPerRow-1.0));
            n1[0] = cos(a1);
            n1[1] = 0;  
            n1[2] = sin(a1);

            v[0] = n1[1]*n2[2] - n1[2]*n2[1];
            v[1] = n1[2]*n2[0] - n1[0]*n2[2];
            v[2] = n1[0]*n2[1] - n1[1]*n2[0];

            // normalize direction vector
            double scale = 1 / sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
            v[0] *= scale;
            v[1] *= scale;
            v[2] *= scale;

            // add a vertex into array
            verticesZ[i][j][0] = v[0];
            verticesZ[i][j][1] = v[1];
            verticesZ[i][j][2] = v[2];
        }
    }
}

void drawCubeSpherePart(GLdouble vertices[pointsPerRow][pointsPerRow][3]){
    glBegin(GL_QUAD_STRIP);
    for(int i=0;i<pointsPerRow-1;i++){
        for(int j=0;j<pointsPerRow;j++){
            glVertex3d(vertices[i][j][0],vertices[i][j][1],vertices[i][j][2]);
            glVertex3d(vertices[i+1][j][0],vertices[i+1][j][1],vertices[i+1][j][2]);
        }
    }
    glEnd();
}

void draw(){
    GLdouble theta,phi ;
    GLdouble x,y,z; 
    GLdouble cz=0,cy=0,cx=0,radius=1;
    z = cz+radius * cos( TO_RADIAN(phi) ) ;
    y = cy + radius * sin( TO_RADIAN(phi) ) * sin( TO_RADIAN(theta) ) ;
    x = cx + radius * sin( TO_RADIAN(phi) ) * cos( TO_RADIAN(theta) ) ;



    std::vector<float> vertices;
    float n1[3];        // normal of longitudinal plane rotating along Y-axis
    float n2[3];        // normal of latitudinal plane rotating along Z-axis
    float v[3];         // direction vector intersecting 2 planes, n1 x n2
    float a1;           // longitudinal angle along Y-axis
    float a2;  

    // int pointsPerRow = (int)pow(2, subdivision) + 1;

    int pointsPerRow = (int)pow(2, 5) + 1;
    // rotate latitudinal plane from 45 to -45 degrees along Z-axis (top-to-bottom)
    for(unsigned int i = 0; i < pointsPerRow; ++i)
    {
        // normal for latitudinal plane
        // if latitude angle is 0, then normal vector of latitude plane is n2=(0,1,0)
        // therefore, it is rotating (0,1,0) vector by latitude angle a2
        // a2 = DEG2RAD * (45.0f - 90.0f * i / (pointsPerRow - 1));
        a2 = DEG2RAD * (growSphere - 2.0*growSphere * i / (pointsPerRow - 1));
        n2[0] = -sin(a2);
        n2[1] = cos(a2);
        n2[2] = 0;

        // rotate longitudinal plane from -45 to 45 along Y-axis (left-to-right)
        for(unsigned int j = 0; j < pointsPerRow; ++j)
        {
            // normal for longitudinal plane
            // if longitude angle is 0, then normal vector of longitude is n1=(0,0,-1)
            // therefore, it is rotating (0,0,-1) vector by longitude angle a1
            // a1 = DEG2RAD * (-45.0f + 90.0f * j / (pointsPerRow - 1));
            a1 = DEG2RAD * (-growSphere + 2.0 * growSphere * j / (pointsPerRow - 1));
            n1[0] = -sin(a1);
            n1[1] = 0;
            n1[2] = -cos(a1);

            // find direction vector of intersected line, n1 x n2
            v[0] = n1[1] * n2[2] - n1[2] * n2[1];
            v[1] = n1[2] * n2[0] - n1[0] * n2[2];
            v[2] = n1[0] * n2[1] - n1[1] * n2[0];

            // normalize direction vector
            float scale = 1 / sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
            v[0] *= scale;
            v[1] *= scale;
            v[2] *= scale;

            // add a vertex into array
            vertices.push_back(v[0]);
            vertices.push_back(v[1]);
            vertices.push_back(v[2]);
        }
    }


    // GLCOLOR(RED);
    glBegin(GL_QUAD_STRIP);
    for(int i=0;i< pointsPerRow -1  ;i++){
        for(int j=0;j<pointsPerRow;j++){
            int id = i*pointsPerRow+j ;
            id*=3; 
            glVertex3d(vertices[id],vertices[id+1],vertices[id+2]);
            id = (i+1)*pointsPerRow+j;
            id*=3;
            glVertex3d(vertices[id],vertices[id+1],vertices[id+2]);
        }
    }
    glEnd();

}

void drawdouble(){
    draw();
    glPushMatrix();
        glRotatef(180,0,1,0);
        draw();
    glPopMatrix();
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
    // drawOctahedron();   
    // drawPyramid();
    // drawSphere(1.0,0.0,0.0,0.0);
    // drawAxes();
    GLCOLOR(RED) ;
    // drawdouble();
    // glPushMatrix();
    //     glRotatef(90,0,1,0);
    //     GLCOLOR(GREEN) ;
    //     drawdouble();
    // glPopMatrix();
    // glPushMatrix();
    //     glRotatef(90,0,0,1);
    //     GLCOLOR(BLUE);
    //     drawdouble();
    // glPopMatrix();
    generateVertices();
    // cerr<<"success\n";
    GLCOLOR(RED);
    drawCubeSpherePart(verticesX);
   
    GLCOLOR(GREEN);
    drawCubeSpherePart(verticesY);

    GLCOLOR(BLUE);
    drawCubeSpherePart(verticesZ);
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
    case '0' :
        growSphere += 5;
        growSphere = (growSphere > 45) ? 45:growSphere ;
        break;
    case '9':
        growSphere -= 5;
        growSphere = (growSphere <= 0.0) ? 0:growSphere ;
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