#include <GL/glut.h>
#include <bits/stdc++.h>

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

struct point {
    GLfloat x, y, z;
};

point eye; 
point center ;
point up ;
struct point pos;   // position of the eye
struct point l;     // look/forward direction
struct point r;     // right direction
struct point u;     // up direction
GLdouble side = 1; 
GLdouble rot = 45,delRot = 5; 
GLdouble sphereInscriptionAngle = 30, delSphereInscriptionAngle=1 ;
const int pointsPerRow = 1<<6 ;
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

void generateVertices(){
    double radius = 1.0 / (sin(DEG2RAD*sphereInscriptionAngle)+cos(DEG2RAD*sphereInscriptionAngle)); 
    double n1[3];       
    double n2[3];       
    double v[3];         
    double a1;        
    double a2;  

    // generate X+ vertices 
    for(unsigned int i = 0; i < pointsPerRow; ++i){
        a2 = DEG2RAD * (sphereInscriptionAngle - 2.0*sphereInscriptionAngle * i / (pointsPerRow - 1));
        n2[0] = sin(a2);
        n2[1] = 0;
        n2[2] = cos(a2);
        
        for(unsigned int j = 0; j < pointsPerRow; ++j){
            a1 = DEG2RAD * (sphereInscriptionAngle - 2.0 * sphereInscriptionAngle * j / (pointsPerRow - 1));
            n1[0] = sin(a1);
            n1[1] = cos(a1);
            n1[2] = 0;

            v[0] = n1[1] * n2[2] - n1[2] * n2[1];
            v[1] = n1[2] * n2[0] - n1[0] * n2[2];
            v[2] = n1[0] * n2[1] - n1[1] * n2[0];

            // normalize direction vector
            double scale = radius / sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
            v[0] *= scale;
            v[1] *= scale;
            v[2] *= scale;

            verticesX[i][j][0] = v[0];
            verticesX[i][j][1] = v[1];
            verticesX[i][j][2] = v[2];
        }
    }
    // generate Y+ vertices 
    for(int i=0;i<pointsPerRow;i++){
        a2 = DEG2RAD * (sphereInscriptionAngle - 2.0*sphereInscriptionAngle*i/(pointsPerRow-1.0));
        n2[0] = cos(a2);
        n2[1] = sin(a2);
        n2[2] = 0;           

        for(int j=0;j<pointsPerRow;j++){
            a1 = DEG2RAD * (sphereInscriptionAngle - 2.0*sphereInscriptionAngle*j/(pointsPerRow-1.0));
            n1[0] = 0;
            n1[1] = sin(a1); 
            n1[2] = cos(a1);
            
            v[0] = n1[1]*n2[2] - n1[2]*n2[1];
            v[1] = n1[2]*n2[0] - n1[0]*n2[2];
            v[2] = n1[0]*n2[1] - n1[1]*n2[0];

            // normalize direction vector
            double scale = radius / sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
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
        a2 = DEG2RAD * (sphereInscriptionAngle - 2.0*sphereInscriptionAngle*i/(pointsPerRow-1.0));
        n2[0] = 0; 
        n2[1] = cos(a2);
        n2[2] = sin(a2);
        for(int j=0;j<pointsPerRow;j++){
            a1 = DEG2RAD * (sphereInscriptionAngle - 2.0*sphereInscriptionAngle*j/(pointsPerRow-1.0));
            n1[0] = cos(a1);
            n1[1] = 0;  
            n1[2] = sin(a1);

            v[0] = n1[1]*n2[2] - n1[2]*n2[1];
            v[1] = n1[2]*n2[0] - n1[0]*n2[2];
            v[2] = n1[0]*n2[1] - n1[1]*n2[0];

            // normalize direction vector
            double scale = radius / sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
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

void drawSphereParts(){
    GLCOLOR(RED);
    drawCubeSpherePart(verticesX);
    glPushMatrix();
        glRotatef(180,0,1,0);
        drawCubeSpherePart(verticesX) ;
    glPopMatrix() ;

    GLCOLOR(GREEN);
    drawCubeSpherePart(verticesY);
    glPushMatrix();
        glRotatef(180,1,0,0);
        drawCubeSpherePart(verticesY) ;
    glPopMatrix() ;

    GLCOLOR(BLUE);
    drawCubeSpherePart(verticesZ);
    glPushMatrix();
        glRotatef(180,1,0,0);
        drawCubeSpherePart(verticesZ) ;
    glPopMatrix() ;
}

void drawTriangleParts(){
    for(int i=0;i<8;i++){
        if( i&1 ) GLCOLOR(CYAN);
        else GLCOLOR(PINK) ;
        glPushMatrix();
        glRotatef( (i&3)*90.0,1,0,0);
        if( i&4 ){
            glRotatef(180.0,0,0,1);
        }
        glBegin(GL_TRIANGLES);
            glVertex3d(verticesX[pointsPerRow-1][pointsPerRow-1][0],verticesX[pointsPerRow-1][pointsPerRow-1][1],verticesX[pointsPerRow-1][pointsPerRow-1][2]);
            glVertex3d(verticesY[pointsPerRow-1][pointsPerRow-1][0],verticesY[pointsPerRow-1][pointsPerRow-1][1],verticesY[pointsPerRow-1][pointsPerRow-1][2]);
            glVertex3d(verticesZ[pointsPerRow-1][pointsPerRow-1][0],verticesZ[pointsPerRow-1][pointsPerRow-1][1],verticesZ[pointsPerRow-1][pointsPerRow-1][2]);
        glEnd();
        glPopMatrix();
    }
}

void drawCylinderPart(){
    glBegin(GL_QUAD_STRIP);
    for(int i=0;i<pointsPerRow;i++){
        glVertex3d(verticesY[pointsPerRow-1][i][0],
                verticesY[pointsPerRow-1][i][1],
                verticesY[pointsPerRow-1][i][2]);
        glVertex3d(verticesX[i][pointsPerRow-1][0],
                verticesX[i][pointsPerRow-1][1],
                verticesX[i][pointsPerRow-1][2]);
    }
    glEnd();
}
void drawCylinderParts(){
    GLCOLOR(YELLOW);
    for(int i=0;i<8;i++){
        glPushMatrix();
        glRotatef(90.0*(i&3),1,0,0);
        if( i&4 ){
            glRotatef(180,0,0,1);
        }
        drawCylinderPart();
        glPopMatrix();
    }
    for(int i=0;i<4;i++){
        glPushMatrix();
            glRotatef(90*i,1,0,0);
            glRotatef(90,0,1,0);
            drawCylinderPart();
        glPopMatrix();
    }
}

void displayMe(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);             // To operate on Model-View matrix
    glLoadIdentity();                       // Reset the model-view matrix
    
    gluLookAt(pos.x,pos.y,pos.z,
              pos.x+l.x,pos.y+l.y,pos.z+l.z,
              u.x,u.y,u.z);

    glRotated(rot,0,1,0); 
    // drawAxes();
    generateVertices();
    drawSphereParts();
    drawTriangleParts();
    drawCylinderParts();
    glutSwapBuffers(); 
}


void keyboardListener(unsigned char key, int xx,int yy){
    double rate = 0.005;
	switch(key){

		case '1':
			r.x = r.x*cos(rate)+l.x*sin(rate);
			r.y = r.y*cos(rate)+l.y*sin(rate);
			r.z = r.z*cos(rate)+l.z*sin(rate);

			l.x = l.x*cos(rate)-r.x*sin(rate);
			l.y = l.y*cos(rate)-r.y*sin(rate);
			l.z = l.z*cos(rate)-r.z*sin(rate);
			break;

        case '2':
			r.x = r.x*cos(-rate)+l.x*sin(-rate);
			r.y = r.y*cos(-rate)+l.y*sin(-rate);
			r.z = r.z*cos(-rate)+l.z*sin(-rate);

			l.x = l.x*cos(-rate)-r.x*sin(-rate);
			l.y = l.y*cos(-rate)-r.y*sin(-rate);
			l.z = l.z*cos(-rate)-r.z*sin(-rate);
			break;

        case '3':
			l.x = l.x*cos(rate)+u.x*sin(rate);
			l.y = l.y*cos(rate)+u.y*sin(rate);
			l.z = l.z*cos(rate)+u.z*sin(rate);

			u.x = u.x*cos(rate)-l.x*sin(rate);
			u.y = u.y*cos(rate)-l.y*sin(rate);
			u.z = u.z*cos(rate)-l.z*sin(rate);
			break;

        case '4':
			l.x = l.x*cos(-rate)+u.x*sin(-rate);
			l.y = l.y*cos(-rate)+u.y*sin(-rate);
			l.z = l.z*cos(-rate)+u.z*sin(-rate);

			u.x = u.x*cos(-rate)-l.x*sin(-rate);
			u.y = u.y*cos(-rate)-l.y*sin(-rate);
			u.z = u.z*cos(-rate)-l.z*sin(-rate);
			break;

        case '5':
			u.x = u.x*cos(rate)+r.x*sin(rate);
			u.y = u.y*cos(rate)+r.y*sin(rate);
			u.z = u.z*cos(rate)+r.z*sin(rate);

			r.x = r.x*cos(rate)-u.x*sin(rate);
			r.y = r.y*cos(rate)-u.y*sin(rate);
			r.z = r.z*cos(rate)-u.z*sin(rate);
			break;

        case '6':
			u.x = u.x*cos(-rate)+r.x*sin(-rate);
			u.y = u.y*cos(-rate)+r.y*sin(-rate);
			u.z = u.z*cos(-rate)+r.z*sin(-rate);

			r.x = r.x*cos(-rate)-u.x*sin(-rate);
			r.y = r.y*cos(-rate)-u.y*sin(-rate);
			r.z = r.z*cos(-rate)-u.z*sin(-rate);

			break;
        case 'a': 
            rot -= delRot; 
            break; 
        case 'd':
            rot += delRot ;
            break;   
        case 'w':
            pos.y += rate;
            break;
        case 's':
            pos.y -= rate;
            break;
     
        case ',' :
            sphereInscriptionAngle += delSphereInscriptionAngle;
            sphereInscriptionAngle = (sphereInscriptionAngle > 45.0) ? 45.0:sphereInscriptionAngle ;
            break;
        case '.':
            sphereInscriptionAngle -= delSphereInscriptionAngle;
            sphereInscriptionAngle = (sphereInscriptionAngle <= 0.0) ? 0.0:sphereInscriptionAngle ;
            break;
  
		default:
			break;
	}
	glutPostRedisplay();
}

void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_UP:		//down arrow key
			pos.x+=l.x;
			pos.y+=l.y;
			pos.z+=l.z;
			break;
		case GLUT_KEY_DOWN:		// up arrow key
			pos.x-=l.x;
			pos.y-=l.y;
			pos.z-=l.z;
			break;

		case GLUT_KEY_RIGHT:
			pos.x+=r.x;
			pos.y+=r.y;
			pos.z+=r.z;
			break;
		case GLUT_KEY_LEFT :
			pos.x-=r.x;
			pos.y-=r.y;
			pos.z-=r.z;
			break;

		case GLUT_KEY_PAGE_UP:
		    pos.x+=u.x;
			pos.y+=u.y;
			pos.z+=u.z;
			break;
		case GLUT_KEY_PAGE_DOWN:
            pos.x-=u.x;
			pos.y-=u.y;
			pos.z-=u.z;
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
	glutPostRedisplay();
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
    pos.x=0;pos.y=0;pos.z=10;

    l.x=0;l.y=0;l.z=-1;
    u.x=0;u.y=1;u.z=0;
    r.x=0.25;r.y=0;r.z=0;
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