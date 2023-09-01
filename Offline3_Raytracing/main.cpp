#include <bits/stdc++.h>
#include <GL/glut.h>
#include "utils.cpp"
#include "bitmap_image.hpp" 


using namespace std ;
    
#define TO_RADIAN(deg) ((deg)*(M_PI/180.0))
#define DEG2RAD (M_PI/180.0) 

double window_height = 1024, window_width = 1024; 
Vector pos(200,0,10);   // position of the eye
Vector l(-1/sqrt(2), -1/sqrt(2),0 );     // look/forward direction
Vector r(-1/sqrt(2), 1/sqrt(2), 0);     // right direction
Vector u = r.cross(l);     // up direction
bool showAxes = 1;



double zNear, zFar ; 
double fovY,fovX; 
double aspect ;
int recursion_level; 
int image_height; 
int image_width; 
double checkerboard_side, checkerboard_ka, checkerboard_kd, checkerboard_ks, checkerboard_alpha ;
vector<Object*> objects ;
vector<Light*> lights; 
vector<Spotlight*> spotlights; 


void input(string filename){
    fstream in(filename, ios_base::in) ;

    in>>zNear>>zFar; 
    in>>fovY;     
    in>>aspect ;
    in>>recursion_level ;
    in>>image_height ;
    image_width = image_height ;
    in>>checkerboard_side ;
    in>>checkerboard_ka>>checkerboard_kd>>checkerboard_ks; 
    
    int num_obj; 
    in>>num_obj ;
    
    objects.resize(num_obj) ;
    for(int i=0;i<num_obj;i++){
        string obj_type ;
        in>>obj_type ;

        if( obj_type == "cube") {
            objects[i] = new Cube() ; 
            in>>*((Cube*)objects[i]); 
        }
        else if( obj_type == "pyramid") {
            objects[i] = new Pyramid() ; 
            in>>*((Pyramid*)objects[i]); 
        }
        else if( obj_type == "sphere") {
            objects[i] = new Sphere() ; 
            in>>*((Sphere*)objects[i]); 
            // cout << objects[i]->color.R << " " << objects[i]->color.G << " " << objects[i]->color.B << endl ;
        }
    }
    // objects.push_back(new Checkerboard(pos,checkerboard_side,zFar)); 
    objects.push_back(new Checkerboard(checkerboard_side,{checkerboard_ka,checkerboard_kd,checkerboard_ks,checkerboard_alpha})); 

    int num_light;
	in >> num_light;

	for(int i=0;i<num_light;i++){
		Light *light = new Light();
		in >> *light;
		lights.push_back(light);
	}

	int num_spotlights;
	in >> num_spotlights;

	for(int i=0;i<num_spotlights;i++){
		Spotlight *spotlight = new Spotlight();
		in >> *spotlight;
		lights.push_back(spotlight);
	}
}


Color ray_tracing(Ray ray, int level, bool reflect = 0){
    if(level <= 0) return BLACK;


    // find the nearest object that intersects with the ray 
    Object *nearestObject = nullptr ;
    double t,tMin;
    tMin = -1;
    Color color,minColor;
    Vector normal,minNormal; 
    for(int k=0;k<(int)objects.size();k++){
        t = objects[k]->intersect(ray,&color,normal) ;
        if(t>0 && (nearestObject== nullptr || t<tMin) )
            tMin = t, nearestObject=objects[k], minColor = color, minNormal = normal;
    }
    minColor.fixRange();
    color = minColor ;
    normal = minNormal; 
    if( nearestObject == nullptr || tMin < 0 ) return BLACK; 

    // if an intersection is found, then shade the pixel; 
    
    vector<Ray> reflectedRays ;
    Vector intersection_point = ray.origin + ray.dir * tMin;
    Color tempColor ;
    Vector tempNormal ;
    Color diffuse_multiplier  ;
    Color specular_multiplier  ;

    for(int i=0;i<(int)lights.size();i++){
        Ray lightray(lights[i]->pos,intersection_point-lights[i]->pos);
        double tLightray = nearestObject->intersect(lightray,&tempColor,tempNormal) ;
        
        if( lights[i]->type == Light::SPOTLIGHT ){
            double theta = angle(lightray.dir,((Spotlight*)lights[i])->dir);
            if( theta > ((Spotlight*)lights[i])->cutoffAngle ) continue;
        }

        bool isObscured = 0; 
        for(int k=0;k<(int)objects.size();k++){
            t = objects[k]->intersect(lightray,&tempColor,tempNormal) ;
            if(t>0 && t<tLightray){
                isObscured = 1; 
                // cout<<"Found and obscuring object - "<<k <<", t = "<< t << ", tMIn = "<< tMin<<endl;
                // cout << "Ray = " <<lightray.origin << "--->" << lightray.dir<<endl;
                break; 
            }
        }

        if(!isObscured){
            double diffuse_multiplier_t = max(0.0,-lightray.dir.dot(normal));
            Ray reflection = Ray(intersection_point, lightray.dir - normal * (2 * lightray.dir.dot(normal)) ); 
            reflectedRays.push_back(reflection) ;
            double specular_multiplier_t = pow(max(0.0, -ray.dir.dot(reflection.dir)), nearestObject->shine);

            diffuse_multiplier = diffuse_multiplier +  lights[i]->color * diffuse_multiplier_t * nearestObject->coefficients.kd ;
            specular_multiplier = specular_multiplier +  lights[i]->color * specular_multiplier_t * nearestObject->coefficients.ks ;
        }
    }

    color = color * nearestObject->coefficients.ka + color * diffuse_multiplier + color * specular_multiplier ; 
    for(Ray &ray: reflectedRays) 
        color = color + ray_tracing(ray,level-1,true);
    color = (reflect) ? color * nearestObject->coefficients.alpha : color; 
    return color ;
}


int imageCount = 1;
void save_frame(){
    cout << "Capturing frame" <<endl;
    bitmap_image image(image_width,image_height);
    image.set_all_channels(0,0,0) ;

    double distance = (window_height/2.0) / tan(DEG2RAD * fovY / 2.0) ;

    Vector top_left = pos + (l*distance) + (u * (window_height/2.0) - (r*(window_width/2.0)) );  

    double dx = window_width/ image_width ; 
    double dy = window_height/image_height; 

    top_left = top_left + (r * (dx / 2.0)) - (u * (dy/2.0))  ;   

    int nearestObjectIndex = -1;
	double t,tMin;

	for(int i=0;i<image_width;i++){
		for(int j=0;j<image_height;j++){
			Vector pixel = top_left + (r* dx * i) - (u * dy * j);
			
            Ray ray(pos,pixel-pos);
			
            Color color = ray_tracing(ray,2) ;         
			image.set_pixel(i, j, color.R, color.G, color.B);
		}

        showLoadingScreen(i,image_width,"Saving image----") ;
	}

	// image.save_image("Output_1"+to_string(imageCount)+".bmp");
	image.save_image("out.bmp");
	imageCount++;
	cout<<"Saving Image"<<endl;		
}


void drawAxes() {
    glLineWidth(3);
    glBegin(GL_LINES);
        glColor3f(1,0,0);   // Red
        // X axis
        glVertex3f(0,0,0);  glVertex3f(zFar,0,0);

        glColor3f(0,1,0);   // Green
        // Y axis
        glVertex3f(0,0,0); glVertex3f(0,zFar,0);

        glColor3f(0,0,2);   // Blue
        // Z axis
        glVertex3f(0,0,0); glVertex3f(0,0,zFar);
    glEnd();
}


void drawGrid(){
    int i = 0; 
    int x = ceil(pos.x()/checkerboard_side)*checkerboard_side;
    int y = ceil(pos.y()/checkerboard_side)*checkerboard_side;
    int xL = x-zFar;
    int yB = y-zFar;
    int xR = x+zFar;
    int yT = y+zFar;
    for(x=xL;x<=xR;x+=checkerboard_side)
        for(y=yB;y<=yT;y+=checkerboard_side){
            if( ((int)(x/checkerboard_side) + (int)(y/checkerboard_side)) % 2 == 0 ) 
                GLCOLOR(BLACK);
            else GLCOLOR(WHITE);
            glBegin(GL_QUADS);
                glVertex3d(x,y,0);
                glVertex3d(x+checkerboard_side,y,0);
                glVertex3d(x+checkerboard_side,y+checkerboard_side,0);
                glVertex3d(x,y+checkerboard_side,0);
            glEnd();
        }
    
}

void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);             // To operate on Model-View matrix
    glLoadIdentity();                       // Reset the model-view matrix
    
    gluLookAt(pos.x(),pos.y(),pos.z(),
              pos.x()+l.x(),pos.y()+l.y(),pos.z()+l.z(),
              u.x(),u.y(),u.z());

    if( showAxes) drawAxes();
    drawGrid();

    for(int i=0;i<objects.size();i++)
        objects[i]->draw();
    for(int i=0;i<lights.size();i++)
        lights[i]->draw();
    for(int i=0;i<spotlights.size();i++)
        spotlights[i]->draw();
    glutSwapBuffers(); 
}



void rotate(Vector &vec,Vector &axis,double angle){
    vec = Matrix::R(vec,axis,angle);
}


void keyboardListener(unsigned char key, int x,int y){
    double ROT_ANG = 15 ;
	switch(key){
		case '0':
			// capture();
            save_frame() ;
			break;
		case '1':
			// rotate LEFT 
			rotate(r,u,ROT_ANG);
			rotate(l,u,ROT_ANG);
			break;
		case '2':
			// rotate right
			rotate(r,u,-ROT_ANG);
			rotate(l,u,-ROT_ANG);
			break;
		case '3':
			// rotate UP
			rotate(u,r,ROT_ANG);
			rotate(l,r,ROT_ANG);
			break;
		case '4':
			// rotate DOWN
			rotate(u,r,-ROT_ANG);
			rotate(l,r,-ROT_ANG);
			break;
		case '5':
			// tilt CLKWISE
			rotate(r,l,ROT_ANG);
			rotate(u,l,ROT_ANG);
			break;
		case '6':
			// tilt COUNTER CLKWISE
			rotate(r,l,-ROT_ANG);
			rotate(u,l,-ROT_ANG);
			break;
		default:
			break;
	}
    glutPostRedisplay();
}


void specialKeyListener(int key, int x,int y){
    double del = 3;
    // cout<<pos<<endl; 
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			pos = pos - l * del;
			break;
		case GLUT_KEY_UP:		// up arrow key
			pos = pos + l * del;
			break;
		case GLUT_KEY_RIGHT:
			pos = pos + r * del;
			break;
		case GLUT_KEY_LEFT:
			pos = pos - r * del ; 
			break;
		case GLUT_KEY_PAGE_UP:
			pos = pos + u * del;
			break;
		case GLUT_KEY_PAGE_DOWN:
			pos = pos - u * del ;
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


void mouseListener(int button, int state, int x, int y){	
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		
				showAxes=1-showAxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
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
    gluPerspective(fovY, aspect, zNear, zFar);
}

void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // Black and opaque
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovY, aspect, zNear, zFar);
}


int main(int argc, char** argv){
    if( argc > 1 )
        input(argv[1]) ;
    else input("description.txt") ;

    glutInit(&argc, argv);
    // glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(window_width, window_height);
    glutInitWindowPosition(100, 100);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color
    glutCreateWindow("Ray Tracing");
    glutDisplayFunc(display);
    glutReshapeFunc(reshapeListener);           // Register callback handler for window re-shape
    glutSpecialFunc(specialKeyListener);
    glutKeyboardFunc(keyboardListener);
    glutMouseFunc(mouseListener);
    initGL();
    glutMainLoop();

    objects.clear();
    return 0;
}