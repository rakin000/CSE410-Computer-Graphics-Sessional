#include <GL/glut.h>
#include "transform.cpp" 


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
struct SurfaceProperties {
    double ka;
    double kd; 
    double ks; 
    double alpha ; 
}

class Object {
    Vector reference_point; 
    double height, width, length; 
    Color color ;

    SurfaceProperties coefficients ; 
    int shine ;
    Object() ;

    virtual void draw() ;
    void setColor(Color color) {} 
    void setCoefficients(SurfaceProperties coefficients ){ }
} ;

class Sphere : Object {

    Sphere(Vector center,double  radius, Color color, SurfaceProperties coefficients, int shininess){ 
        reference_poiont = center ;
        length = radius ;
        this->color = color; 
        this-> coefficients = coefficients ;
        this-> shine = shininess;
    }
     
    void draw() {

    }
} ;

class Pyramid : Object { 

    Pyramid(Vector lowest_point, double width, double height, Color color, SurfaceProperties coefficients, int shininess) {
        reference_point = lowest_point ;
        this->width = width ;
        this->height = height ;
        this->color= color ;
        this->coefficients = coefficients ;
        this->shine = shininess; 
    }

    void draw() {

    }
};


class Cube: Object {
    Cube(Vector bottom_lower_left, double side, Color color, SurfaceProperties coefficients, int shininess) {
        reference_point = lowest_point ;
        this->length = this->width = this->length = side ;
        this->color= color ;
        this->coefficients = coefficients ;
        this->shine = shininess; 
    }

    void draw() {

    }
}