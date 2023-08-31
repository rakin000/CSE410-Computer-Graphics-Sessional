#include <GL/glut.h>
#include "transform.cpp"
#include "math.h"

struct Color
{
    GLdouble R, G, B;
} PINK = {235, 52, 186},
  CYAN = {52, 201, 235},
  RED = {255, 0, 0},
  BLUE = {0, 0, 255},
  GREEN = {0, 255, 0},
  YELLOW = {252, 236, 3},
  WHITE = {255, 255, 255},
  BLACK = {0,0,0};

struct point
{
    GLfloat x, y, z;
};
struct SurfaceProperties
{
    double ka;
    double kd;
    double ks;
    double alpha;
};

class Object{
    protected :  
    Vector reference_point;
    double height, width, length;
    Color color;

    SurfaceProperties coefficients;
    int shine;

    public : 
    Object() {
        reference_point = Vector(0, 0, 0);
        height = 0;
        width = 0;
        length = 0;
        color = {0, 0, 0};
        coefficients = {0, 0, 0, 0};
        shine = 0;
    }

    virtual void draw() = 0 ;
    // void setColor(Color color) {}
    // void setCoefficients(SurfaceProperties coefficients) {}
};

class Sphere : public Object{
    public:
    Sphere() : Object() 
        {}
    Sphere(Vector center, double radius, Color color, SurfaceProperties coefficients, int shininess)
    {
        this->reference_point = center;
        this->length = radius;
        this->color = color;
        this->coefficients = coefficients;
        this->shine = shininess;
    }

    virtual void draw(){
        int stacks = 30;
        int slices = 30;

        Vector points[100][100];
        int i, j;
        double h, r;
        for (i = 0; i <= stacks; i++){
            h = length * sin(((double)i / (double)stacks) * (M_PI / 2));
            r = length * cos(((double)i / (double)stacks) * (M_PI / 2));
            for (j = 0; j <= slices; j++){
                points[i][j] = Vector(r * cos(((double)j / (double)slices) * 2 * M_PI),
                                      r * sin(((double)j / (double)slices) * 2 * M_PI),
                                      h);
            }
        }
        for (i = 0; i < stacks; i++){
            glPushMatrix();
            glTranslatef(reference_point.x(), reference_point.y(), reference_point.z());
            glColor3f(color.R, color.G, color.B);
            for (j = 0; j < slices; j++)
            {
                glBegin(GL_QUADS);
                    glVertex3f(points[i][j].x(), points[i][j].y(), points[i][j].z());
                    glVertex3f(points[i][j + 1].x(), points[i][j + 1].y(), points[i][j + 1].z());
                    glVertex3f(points[i + 1][j + 1].x(), points[i + 1][j + 1].y(), points[i + 1][j + 1].z());
                    glVertex3f(points[i + 1][j].x(), points[i + 1][j].y(), points[i + 1][j].z());
                    // lower hemisphere
                    glVertex3f(points[i][j].x(), points[i][j].y(), -points[i][j].z());
                    glVertex3f(points[i][j + 1].x(), points[i][j + 1].y(), -points[i][j + 1].z());
                    glVertex3f(points[i + 1][j + 1].x(), points[i + 1][j + 1].y(), -points[i + 1][j + 1].z());
                    glVertex3f(points[i + 1][j].x(), points[i + 1][j].y(), -points[i + 1][j].z());
                glEnd();
            }
            glPopMatrix();
        }
    }

    friend std::istream &operator>>(std::istream &in, Sphere &sphere)
    {
        in >> sphere.reference_point;
        in >> sphere.length;
        in >> sphere.color.R >> sphere.color.G >> sphere.color.B;
        sphere.color.R *= 255;
        sphere.color.G *= 255;
        sphere.color.B *= 255;
        in >> sphere.coefficients.ka >> sphere.coefficients.kd >> sphere.coefficients.ks >> sphere.coefficients.alpha;
        in >> sphere.shine;
        return in;
    }
};

class Pyramid : public Object {
    public: 
    Pyramid() : Object() {}

    Pyramid(Vector lowest_point, double width, double height, Color color, SurfaceProperties coefficients, int shininess)
    {
        reference_point = lowest_point;
        this->width = width;
        this->height = height;
        this->color = color;
        this->coefficients = coefficients;
        this->shine = shininess;
    }

    void draw(){
        glBegin(GL_QUADS);
        // front
        glColor3f(color.R, color.G, color.B);
        glVertex3f(reference_point.x(), reference_point.y(), reference_point.z());
        glVertex3f(reference_point.x() + width, reference_point.y(), reference_point.z());
        glVertex3f(reference_point.x() + width, reference_point.y() + width, reference_point.z());
        glVertex3f(reference_point.x(), reference_point.y() + width, reference_point.z());
        // back
        glColor3f(color.R, color.G, color.B);
        glVertex3f(reference_point.x(), reference_point.y(), reference_point.z() + height);
        glVertex3f(reference_point.x() + width, reference_point.y(), reference_point.z() + height);
        glVertex3f(reference_point.x() + width, reference_point.y() + width, reference_point.z() + height);
        glVertex3f(reference_point.x(), reference_point.y() + width, reference_point.z() + height);
        // left
        glColor3f(color.R, color.G, color.B);
        glVertex3f(reference_point.x(), reference_point.y(), reference_point.z());
        glVertex3f(reference_point.x(), reference_point.y(), reference_point.z() + height);
        glVertex3f(reference_point.x(), reference_point.y() + width, reference_point.z() + height);
        glVertex3f(reference_point.x(), reference_point.y() + width, reference_point.z());
        // right
        glColor3f(color.R, color.G, color.B);
        glVertex3f(reference_point.x() + width, reference_point.y(), reference_point.z());
        glVertex3f(reference_point.x() + width, reference_point.y(), reference_point.z() + height);
        glVertex3f(reference_point.x() + width, reference_point.y() + width, reference_point.z() + height);
        glVertex3f(reference_point.x() + width, reference_point.y() + width, reference_point.z());
        // top
        glColor3f(color.R, color.G, color.B);
        glVertex3f(reference_point.x(), reference_point.y() + width, reference_point.z());
        glVertex3f(reference_point.x() + width, reference_point.y() + width, reference_point.z());
        glVertex3f(reference_point.x() + width, reference_point.y() + width, reference_point.z() + height);
        glVertex3f(reference_point.x(), reference_point.y() + width, reference_point.z() + height);
        // bottom
        glColor3f(color.R, color.G, color.B);
        glVertex3f(reference_point.x(), reference_point.y(), reference_point.z());
        glVertex3f(reference_point.x() + width, reference_point.y(), reference_point.z());
        glVertex3f(reference_point.x() + width, reference_point.y(), reference_point.z() + height);
        glVertex3f(reference_point.x(), reference_point.y(), reference_point.z() + height);

        glEnd();
    }

    friend std::istream &operator>>(std::istream &in, Pyramid &pyramid)
    {
        in >> pyramid.reference_point;
        in >> pyramid.width >> pyramid.height;
        in >> pyramid.color.R >> pyramid.color.G >> pyramid.color.B;
        pyramid.color.R *= 255;
        pyramid.color.G *= 255;
        pyramid.color.B *= 255;
        in >> pyramid.coefficients.ka >> pyramid.coefficients.kd >> pyramid.coefficients.ks >> pyramid.coefficients.alpha;
        in >> pyramid.shine;
        return in;
    }

};

class Cube : public Object {
    public: 
    Cube() : Object() {}

    Cube(Vector bottom_lower_left, double side, Color color, SurfaceProperties coefficients, int shininess)
    {
        this->reference_point = bottom_lower_left;
        this->length = height;
        this->color = color;
        this->coefficients = coefficients;
        this->shine = shininess;
    }

    void draw(){
        glBegin(GL_QUADS);
        // front
        glColor3f(color.R, color.G, color.B);
        glVertex3f(reference_point.x(), reference_point.y(), reference_point.z());
        glVertex3f(reference_point.x() + length, reference_point.y(), reference_point.z());
        glVertex3f(reference_point.x() + length, reference_point.y() + length, reference_point.z());
        glVertex3f(reference_point.x(), reference_point.y() + length, reference_point.z());
        // back
        glColor3f(color.R, color.G, color.B);
        glVertex3f(reference_point.x(), reference_point.y(), reference_point.z() + length);
        glVertex3f(reference_point.x() + length, reference_point.y(), reference_point.z() + length);
        glVertex3f(reference_point.x() + length, reference_point.y() + length, reference_point.z() + length);
        glVertex3f(reference_point.x(), reference_point.y() + length, reference_point.z() + length);
        // left
        glColor3f(color.R, color.G, color.B);
        glVertex3f(reference_point.x(), reference_point.y(), reference_point.z());
        glVertex3f(reference_point.x(), reference_point.y(), reference_point.z() + length);
        glVertex3f(reference_point.x(), reference_point.y() + length, reference_point.z() + length);
        glVertex3f(reference_point.x(), reference_point.y() + length, reference_point.z());
        // right
        glColor3f(color.R, color.G, color.B);
        glVertex3f(reference_point.x() + length, reference_point.y(), reference_point.z());
        glVertex3f(reference_point.x() + length, reference_point.y(), reference_point.z() + length);
        glVertex3f(reference_point.x() + length, reference_point.y() + length, reference_point.z() + length);
        glVertex3f(reference_point.x() + length, reference_point.y() + length, reference_point.z());
        // top
        glColor3f(color.R, color.G, color.B);
        glVertex3f(reference_point.x(), reference_point.y() + length, reference_point.z());
        glVertex3f(reference_point.x() + length, reference_point.y() + length, reference_point.z());
        glVertex3f(reference_point.x() + length, reference_point.y() + length, reference_point.z() + length);
        glVertex3f(reference_point.x(), reference_point.y() + length, reference_point.z() + length);
        // bottom
        glColor3f(color.R, color.G, color.B);
        glVertex3f(reference_point.x(), reference_point.y(), reference_point.z());
        glVertex3f(reference_point.x() + length, reference_point.y(), reference_point.z());
        glVertex3f(reference_point.x() + length, reference_point.y(), reference_point.z() + length);
        glVertex3f(reference_point.x(), reference_point.y(), reference_point.z() + length);

        glEnd() ;
    }

    friend std::istream &operator>>(std::istream &in, Cube &cube)
    {
        in >> cube.reference_point;
        in >> cube.length; 
        in >> cube.color.R >> cube.color.G >> cube.color.B;
        cube.color.R *= 255;
        cube.color.G *= 255;
        cube.color.B *= 255;
        in >> cube.coefficients.ka >> cube.coefficients.kd >> cube.coefficients.ks >> cube.coefficients.alpha;
        in >> cube.shine;
        return in;
    }
};

struct Light{
    Vector pos;
    Color color;
    double falloff;
    Light(){
        color = WHITE ;
    }
    void draw() {
        glPointSize(5);
        glBegin(GL_POINTS);
        glColor3f(color.R, color.G, color.G);
        glVertex3f(pos.x(), pos.y(), pos.z());
        glEnd();
    }

    friend istream& operator>>(istream &in, Light &l){
        in >> l.pos;  
        in >> l.falloff; 
        return in;
    }

};

struct Spotlight : Light{
    Vector dir;
    double cutoffAngle; 
    Spotlight(): Light() 
    {
    }
    void draw(){
        glPointSize(15);
        glBegin(GL_POINTS);
        glColor3f(color.R, color.G, color.B);
        glVertex3f(pos.x(), pos.y(), pos.z());
        glEnd();
    }

    friend istream& operator>>(istream &in, Spotlight &l){
        in >> l.pos ;
        in >> l.falloff ;
        in >> l.dir ;
        in >> l.cutoffAngle;
        return in;
    }

};
