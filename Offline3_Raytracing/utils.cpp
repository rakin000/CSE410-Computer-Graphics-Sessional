#include <GL/glut.h>
#include "transform.cpp"
#include "math.h"



#define GLCOLOR(color) glColor3f( (color).R/255.0,(color).G/255.0,(color).B/255.0)


struct Color{
    double R;
    double G;
    double B;

    Color()
    {
        R = 0;
        G = 0;
        B = 0;
    }

    Color(double R,double G,double B)
    {
        this->R = R;
        this->G = G;
        this->B = B;
    }

    Color operator=(Color c)
    {
        this->R = c.R;
        this->G = c.G;
        this->B = c.B;
        return *this;
    }
};

Color PINK = {235, 52, 186},
  CYAN = {52, 201, 235},
  RED = {255, 0, 0},
  BLUE = {0, 0, 255},
  GREEN = {0, 255, 0},
  YELLOW = {252, 236, 3},
  WHITE = {255, 255, 255},
  BLACK = {0,0,0};

struct SurfaceProperties
{
    double ka;
    double kd;
    double ks;
    double alpha;
};

struct Ray{
    Vector origin, dir;
    
    Ray(Vector origin, Vector dir){
        this->origin = origin;
        dir.normalize();
        this->dir = dir;
    }

    friend ostream& operator<<(ostream &out, Ray r)
    {
        out << "Origin : " << r.origin << ", Direction : " << r.dir;
        return out;
    }
};

class Object{
    protected :  
    Vector reference_point;
    double height, width, length;

    SurfaceProperties coefficients;
    int shine;

    public : 
    Color color;
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
    virtual double intersect(Ray ray,Color *color,int level) = 0 ;
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
            GLCOLOR(color);
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

    double intersect(Ray ray, Color *color, int level){
        // cout << "Before color: "<< this->color.R << " " << this->color.G << " " << this->color.B <<endl; 
        *color = this->color; 
        // color.R = this->color.R ;
        // color.G = this->color.G ;
        // color.B = this->color.B ;

        //equation  t*t + 2(P0-O).D t + (P0-O).(P0-O)-r*r = 0 ;
        ray.origin = ray.origin - reference_point; // P0-O 
        double a = 1; 
        double b = 2 * (ray.dir.dot(ray.origin));
        double c = (ray.origin.dot(ray.origin)) - (length*length);

        double discriminant = b*b - 4 * a * c;
        double t = -1;
        if (discriminant < 0)
            return -1 ; 
        // if(fabs(a) < 1e-5){
            // t = -c/b;
            // return t;
        // }

        double t1 = (-b - sqrt(discriminant)) / (2 * a);
        double t2 = (-b + sqrt(discriminant)) / (2 * a);

        if(t2<t1) 
            swap(t1, t2);

        if (t1 > 0)
            return t1;
        if (t2 > 0)
            return t2;
        
        return -1;
    }

    friend std::istream &operator>>(std::istream &in, Sphere &sphere){
        in >> sphere.reference_point;
        in >> sphere.length;
        in >> sphere.color.R >> sphere.color.G >> sphere.color.B;
        sphere.color.R *= 255.0;
        sphere.color.G *= 255.0;
        sphere.color.B *= 255.0;

        cout << sphere.color.R << " " << sphere.color.G << " " << sphere.color.B <<endl;;
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
        GLCOLOR(color);
        glVertex3f(reference_point.x(), reference_point.y(), reference_point.z());
        glVertex3f(reference_point.x() + width, reference_point.y(), reference_point.z());
        glVertex3f(reference_point.x() + width, reference_point.y() + width, reference_point.z());
        glVertex3f(reference_point.x(), reference_point.y() + width, reference_point.z());
        // back
        GLCOLOR(color);
        glVertex3f(reference_point.x(), reference_point.y(), reference_point.z() + height);
        glVertex3f(reference_point.x() + width, reference_point.y(), reference_point.z() + height);
        glVertex3f(reference_point.x() + width, reference_point.y() + width, reference_point.z() + height);
        glVertex3f(reference_point.x(), reference_point.y() + width, reference_point.z() + height);
        // left
        GLCOLOR(color);
        glVertex3f(reference_point.x(), reference_point.y(), reference_point.z());
        glVertex3f(reference_point.x(), reference_point.y(), reference_point.z() + height);
        glVertex3f(reference_point.x(), reference_point.y() + width, reference_point.z() + height);
        glVertex3f(reference_point.x(), reference_point.y() + width, reference_point.z());
        // right
        GLCOLOR(color);
        glVertex3f(reference_point.x() + width, reference_point.y(), reference_point.z());
        glVertex3f(reference_point.x() + width, reference_point.y(), reference_point.z() + height);
        glVertex3f(reference_point.x() + width, reference_point.y() + width, reference_point.z() + height);
        glVertex3f(reference_point.x() + width, reference_point.y() + width, reference_point.z());
        // top
        GLCOLOR(color);
        glVertex3f(reference_point.x(), reference_point.y() + width, reference_point.z());
        glVertex3f(reference_point.x() + width, reference_point.y() + width, reference_point.z());
        glVertex3f(reference_point.x() + width, reference_point.y() + width, reference_point.z() + height);
        glVertex3f(reference_point.x(), reference_point.y() + width, reference_point.z() + height);
        // bottom
        GLCOLOR(color);
        glVertex3f(reference_point.x(), reference_point.y(), reference_point.z());
        glVertex3f(reference_point.x() + width, reference_point.y(), reference_point.z());
        glVertex3f(reference_point.x() + width, reference_point.y(), reference_point.z() + height);
        glVertex3f(reference_point.x(), reference_point.y(), reference_point.z() + height);

        glEnd();
    }

    virtual double intersect(Ray ray, Color *color, int level){
        *color = this->color ;
        //equation  t*t + 2(P0-O).D t + (P0-O).(P0-O)-r*r = 0 ;
        ray.origin = ray.origin - reference_point; // P0-O 
        double a = 1; 
        double b = 2 * (ray.dir.dot(ray.origin));
        double c = (ray.origin.dot(ray.origin)) - (length*length);

        double discriminant = b*b - 4 * a * c;
        double t = -1;
        if (discriminant < 0)
            return -1 ; 
        // if(fabs(a) < 1e-5){
            // t = -c/b;
            // return t;
        // }

        double t1 = (-b - sqrt(discriminant)) / (2 * a);
        double t2 = (-b + sqrt(discriminant)) / (2 * a);

        if(t2<t1) 
            swap(t1, t2);

        if (t1 > 0)
            return t1;
        if (t2 > 0)
            return t2;
        
        return -1;
    }

    friend std::istream &operator>>(std::istream &in, Pyramid &pyramid)
    {
        in >> pyramid.reference_point;
        in >> pyramid.width >> pyramid.height;
        in >> pyramid.color.R >> pyramid.color.G >> pyramid.color.B;
        pyramid.color.R *= 255.0;
        pyramid.color.G *= 255.0;
        pyramid.color.B *= 255.0;
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
        GLCOLOR(color);
        glVertex3f(reference_point.x(), reference_point.y(), reference_point.z());
        glVertex3f(reference_point.x() + length, reference_point.y(), reference_point.z());
        glVertex3f(reference_point.x() + length, reference_point.y() + length, reference_point.z());
        glVertex3f(reference_point.x(), reference_point.y() + length, reference_point.z());
        // back
        GLCOLOR(color);
        glVertex3f(reference_point.x(), reference_point.y(), reference_point.z() + length);
        glVertex3f(reference_point.x() + length, reference_point.y(), reference_point.z() + length);
        glVertex3f(reference_point.x() + length, reference_point.y() + length, reference_point.z() + length);
        glVertex3f(reference_point.x(), reference_point.y() + length, reference_point.z() + length);
        // left
        GLCOLOR(color);
        glVertex3f(reference_point.x(), reference_point.y(), reference_point.z());
        glVertex3f(reference_point.x(), reference_point.y(), reference_point.z() + length);
        glVertex3f(reference_point.x(), reference_point.y() + length, reference_point.z() + length);
        glVertex3f(reference_point.x(), reference_point.y() + length, reference_point.z());
        // right
        GLCOLOR(color);
        glVertex3f(reference_point.x() + length, reference_point.y(), reference_point.z());
        glVertex3f(reference_point.x() + length, reference_point.y(), reference_point.z() + length);
        glVertex3f(reference_point.x() + length, reference_point.y() + length, reference_point.z() + length);
        glVertex3f(reference_point.x() + length, reference_point.y() + length, reference_point.z());
        // top
        GLCOLOR(color);
        glVertex3f(reference_point.x(), reference_point.y() + length, reference_point.z());
        glVertex3f(reference_point.x() + length, reference_point.y() + length, reference_point.z());
        glVertex3f(reference_point.x() + length, reference_point.y() + length, reference_point.z() + length);
        glVertex3f(reference_point.x(), reference_point.y() + length, reference_point.z() + length);
        // bottom
        GLCOLOR(color);
        glVertex3f(reference_point.x(), reference_point.y(), reference_point.z());
        glVertex3f(reference_point.x() + length, reference_point.y(), reference_point.z());
        glVertex3f(reference_point.x() + length, reference_point.y(), reference_point.z() + length);
        glVertex3f(reference_point.x(), reference_point.y(), reference_point.z() + length);

        glEnd() ;
    }

    virtual  double intersect(Ray ray, Color *color, int level){
        *color = this->color ;
        //equation  t*t + 2(P0-O).D t + (P0-O).(P0-O)-r*r = 0 ;
        ray.origin = ray.origin - reference_point; // P0-O 
        double a = 1; 
        double b = 2 * (ray.dir.dot(ray.origin));
        double c = (ray.origin.dot(ray.origin)) - (length*length);

        double discriminant = b*b - 4 * a * c;
        double t = -1;
        if (discriminant < 0)
            return -1 ; 
        // if(fabs(a) < 1e-5){
            // t = -c/b;
            // return t;
        // }

        double t1 = (-b - sqrt(discriminant)) / (2 * a);
        double t2 = (-b + sqrt(discriminant)) / (2 * a);

        if(t2<t1) 
            swap(t1, t2);

        if (t1 > 0)
            return t1;
        if (t2 > 0)
            return t2;
        
        return -1;
    }

    friend std::istream &operator>>(std::istream &in, Cube &cube)
    {
        in >> cube.reference_point;
        in >> cube.length; 
        in >> cube.color.R >> cube.color.G >> cube.color.B;
        cube.color.R *= 255.0;
        cube.color.G *= 255.0;
        cube.color.B *= 255.0;
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
