#include <GL/glut.h>
#include "transform.cpp"
#include "math.h"



#define GLCOLOR(color) glColor3f( (color).R/255.0,(color).G/255.0,(color).B/255.0)
const double EPSILON = 1e-4;
void showLoadingScreen(int step, int totalSteps, string message = "Loading...") {
        if( step == totalSteps ) {
            std::cout << message << " complete!" << std::endl;
            return;
        }
        
        float progress = float(step) / totalSteps * 100;
        std::cout << message << progress << "%";

        // Print some animation, e.g., spinning wheel
        switch (step % 4) {
            case 0: std::cout << " |  "; break;
            case 1: std::cout << " /  "; break;
            case 2: std::cout << " -  "; break;
            case 3: std::cout << " \\  "; break;
        }

        std::cout << "\r";
        // std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 100 ms delay
    // std::cout << message << " complete!" << std::endl;
}

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
    Color operator*(double k){
        Color temp(this->R * k, this->G * k, this->B * k);
        // temp.fixRange();
        return temp;
    }
    Color operator*(Color c){
        Color temp(this->R * c.R, this->G * c.G, this->B * c.B);
        // temp.fixRange();
        return temp;
    }
    Color operator+(Color c){
        Color temp(this->R + c.R, this->G + c.G, this->B + c.B);
        // temp.fixRange();
        return temp;
    }
    bool operator==(Color rhs){
        return this->R == rhs.R && this->G == rhs.G && this->B == rhs.B;
    }

    void fixRange(){
        R = max(0.0,min(R, 255.0));
        G = max(0.0,min(G, 255.0));
        B = max(0.0,min(B, 255.0));
    }

    friend istream& operator>>(istream &in, Color &c){
        in >> c.R >> c.G >> c.B;
        return in;
    }

    friend ostream& operator<<(ostream &out, Color c){
        out << c.R << " " << c.G << " " << c.B;
        return out;
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

struct SurfaceProperties{
    double ka;
    double kd;
    double ks;
    double reflectance;
};

struct Ray{
    Vector origin, dir;
    
    Ray(Vector origin, Vector dir){
        this->origin = origin;
        this->dir = dir;
        this->dir.normalize() ;
    }

    Ray(const Ray &rhs){
        this->origin = rhs.origin;
        this->dir = rhs.dir;
    }

    Ray& operator=(const Ray &rhs){
        this->origin = rhs.origin;
        this->dir = rhs.dir;
        return *this;
    }

    friend ostream& operator<<(ostream &out, Ray r){
        out << "Origin : " << r.origin << ", Direction : " << r.dir;
        return out;
    }
};

class Triangle {   // primitive 
public:
    Vector A,B,C ;
    Vector normal ;
    Color color ;

    Triangle(Vector A, Vector B, Vector C, Color color = BLACK){
        this->A = A ;
        this->B = B ;
        this->C = C ; 
        this->normal = (B-A).cross(C-A) ;
        this->normal.normalize() ;
        this->color = color;
    }

    Triangle(double ax,double ay,double az,
            double bx,double by,double bz,
            double cx,double cy,double cz) :
                Triangle(Vector(ax,ay,az),Vector(bx,by,bz),Vector(cx,cy,cz))
    {
    }

    Triangle(const Triangle& rhs):
        Triangle(rhs.A,rhs.B,rhs.C,rhs.color)
    {
    }

    Triangle():
        Triangle(Vector(0,0,0),Vector(1,0,0),Vector(0,0,1))
    {
    }

    Triangle& operator=(const Triangle &rhs){
        *this = Triangle(rhs.A,rhs.B,rhs.C,rhs.color) ;
        return *this ;
    }


    void draw(){
        // cout<<color<<endl;
        GLCOLOR(color) ; 
        glBegin(GL_TRIANGLES) ; 
            glVertex3d(A.x(),A.y(),A.z()) ;
            glVertex3d(B.x(),B.y(),B.z()) ;
            glVertex3d(C.x(),C.y(),C.z()) ;
        glEnd();
    }


    double intersect(Ray &ray){
        Vector edge1, edge2, h, s, q;
        double a, f, u, v;
        edge1 = B-A ;
        edge2 = C-A;
        h = ray.dir.cross(edge2);
        a = edge1.dot(h);

        if (a > -EPSILON && a < EPSILON)
            return -1;    // This ray is parallel to this triangle.

        f = 1.0 / a;
        s = ray.origin - A;
        u = f * s.dot(h);

        if (u < 0.0 || u > 1.0)
            return -1;

        q = s.cross(edge1);
        v = f * ray.dir.dot(q);

        if (v < 0.0 || u + v > 1.0)
            return -1;

        // At this stage we can compute t to find out where the intersection point is on the line.
        float t = f * edge2.dot(q);

        if (t > EPSILON){ // ray intersection {
            return t;
        }
        else // This means that there is a line intersection but not a ray intersection.
            return -1;
    }


    double intersect2(
        Ray &ray)
    {
        float t ; 
        Vector vec1 = B - A;
        Vector vec2 = C - A;
        // no need to normalize
        Vector N = vec1.cross(vec2); // N
        float area2 = N.abs();
        // Step 1: finding P
        
        // check if the ray and plane are parallel.
        float NdotRayDirection = N.dot(ray.dir);
        if (fabs(NdotRayDirection) < EPSILON) // almost 0
            return -1; // they are parallel, so they don't intersect! 

        // compute d parameter using equation 2
        float d = -N.dot(A);
        
        t = -(N.dot(ray.origin) + d) / NdotRayDirection;
        
        if (t < 0) return -1; // the triangle is behind
    
        // compute the intersection point using equation 1
        Vector P = ray.origin + ray.dir * t;
    
        Vector nC; // vector perpendicular to triangle's plane
    
        Vector edge0 = B - A; 
        Vector vp0 = P - A;
        nC = edge0.cross(vp0);
        if (N.dot(nC) < 0) return -1; // P is on the right side
    
        // edge 1
        Vector edge1 = C - B; 
        Vector vp1 = P - B;
        nC = edge1.cross(vp1);
        if (N.dot(nC) < 0)  return -1; // P is on the right side
    
        // edge 2
        Vector edge2 = A - C ; 
        Vector vp2 = P - C;
        nC = edge2.cross(vp2);
        if (N.dot(nC) < 0) return -1; // P is on the right side;


        return t; // this ray hits the triangle
    }

};




class Object{
    protected :  
    Vector reference_point;
    double height, width, length;

    vector<Triangle> triangle_primitives ;

    public : 
    SurfaceProperties coefficients;
    Color color;
    int shine;
    Object() {
        reference_point = Vector(0, 0, 0);
        height = 0;
        width = 0;
        length = 0;
        color = {0, 0, 0};
        coefficients = {0, 0, 0, 0};
        shine = 0;
    }
    ~Object(){
        // for(Triangle &triangle : triangle_primitives)
            // delete triangle ;
    }
    virtual void draw() = 0 ;
    virtual  double intersect(Ray ray, Color *color, Vector &normal) = 0 ;
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

    virtual double intersect(Ray ray, Color *color, Vector &normal){
        // return -1; 
        *color = this->color; 

        //equation  t*t + 2(P0-O).D t + (P0-O).(P0-O)-r*r = 0 ;
        ray.origin = ray.origin - reference_point; // P0-O 
        double a = 1; 
        double b = 2 * (ray.dir.dot(ray.origin));
        double c = (ray.origin.dot(ray.origin)) - (length*length);

        double discriminant = b*b - 4 * a * c;
        double t = -1;
        if (discriminant < 0)
            return -1 ; 
        if(fabs(a) < 1e-5){
            t = -c/b;
            return t;
        }

        double t1 = (-b - sqrt(discriminant)) / (2 * a);
        double t2 = (-b + sqrt(discriminant)) / (2 * a);

        if(t2<t1) 
            swap(t1, t2);

        if (t1 > 0){
            Vector intersectionPoint = ray.origin + ray.dir * t1;
            normal = intersectionPoint-reference_point; 
            normal.normalize();
            return t1;
        }
        if (t2 > 0){
            Vector intersectionPoint = ray.origin + ray.dir * t2;
            normal = intersectionPoint-reference_point; 
            normal.normalize();
            return t2;
        }
        
        return -1;
    }

    friend std::istream &operator>>(std::istream &in, Sphere &sphere){
        in >> sphere.reference_point;
        in >> sphere.length;
        in >> sphere.color.R >> sphere.color.G >> sphere.color.B;
        sphere.color.R *= 255.0;
        sphere.color.G *= 255.0;
        sphere.color.B *= 255.0;

        in >> sphere.coefficients.ka >> sphere.coefficients.kd >> sphere.coefficients.ks >> sphere.coefficients.reflectance;
        in >> sphere.shine;

        sphere = Sphere(sphere.reference_point, sphere.length, sphere.color, sphere.coefficients, sphere.shine) ;

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

        Vector apex = lowest_point + Vector(0,0,height) ; 
        Vector A = lowest_point + Vector(-width/2,-width/2,0) ;  
        Vector B = lowest_point + Vector(width/2,-width/2,0) ;
        Vector C = lowest_point + Vector(width/2,width/2,0) ;
        Vector D = lowest_point + Vector(-width/2,width/2,0) ;


        this->triangle_primitives.push_back( Triangle(A,B,apex) ) ;
        this->triangle_primitives.push_back( Triangle(B,C,apex) ) ;
        this->triangle_primitives.push_back( Triangle(C,D,apex) ) ;
        this->triangle_primitives.push_back( Triangle(D,A,apex) ) ;
        this->triangle_primitives.push_back( Triangle(A,C,B) ) ;
        this->triangle_primitives.push_back( Triangle(A,D,C) ) ;
    }

    void draw(){
        for(Triangle &triangle : triangle_primitives){
            triangle.color = this->color ;
            triangle.draw() ;
        }
    }

    double intersect(Ray ray, Color *color, Vector &normal){
        // return -1; 
        *color = this->color ;
        double t = 1e18;
        for(Triangle &triangle : triangle_primitives){
            double tt = triangle.intersect(ray) ;
            if( tt < 0 ) continue ;
            if( tt < t ){
                normal = triangle.normal ;
                normal.normalize() ;
                t = tt ;
            }
        }
        t = (t == 1e18 || t < 0 ) ? -1 : t ;
        return t;  
    }

    friend std::istream &operator>>(std::istream &in, Pyramid &pyramid){
        in >> pyramid.reference_point;
        in >> pyramid.width >> pyramid.height;
        in >> pyramid.color.R >> pyramid.color.G >> pyramid.color.B;
        pyramid.color.R *= 255.0;
        pyramid.color.G *= 255.0;
        pyramid.color.B *= 255.0;
        in >> pyramid.coefficients.ka >> pyramid.coefficients.kd >> pyramid.coefficients.ks >> pyramid.coefficients.reflectance;
        in >> pyramid.shine;

        pyramid = Pyramid(pyramid.reference_point, pyramid.width, pyramid.height, pyramid.color, pyramid.coefficients, pyramid.shine);
        return in;
    }

};

class Cube : public Object {
    public: 
    Cube() : Object() {}

    Cube(Vector bottom_lower_left, double side, Color color, SurfaceProperties coefficients, int shininess){
        this->reference_point = bottom_lower_left;
        this->length = height;
        this->color = color;
        this->coefficients = coefficients;
        this->shine = shininess;

        Vector A = bottom_lower_left + Vector(0,0,0) ;
        Vector B = bottom_lower_left + Vector(side,0,0) ;
        Vector C = bottom_lower_left + Vector(side,0,side) ;
        Vector D = bottom_lower_left + Vector(0,0,side) ;

        Vector E = bottom_lower_left + Vector(0,side,0) ;
        Vector F = bottom_lower_left + Vector(side,side,0) ;
        Vector G = bottom_lower_left + Vector(side,side,side) ;
        Vector H = bottom_lower_left + Vector(0,side,side) ;

        this->triangle_primitives.push_back( Triangle(A,B,C) ) ;
        this->triangle_primitives.push_back( Triangle(A,C,D) ) ;

        this->triangle_primitives.push_back( Triangle(E,G,F) ) ;
        this->triangle_primitives.push_back( Triangle(E,H,G) ) ;
        
        this->triangle_primitives.push_back( Triangle(A,F,B) ) ;
        this->triangle_primitives.push_back( Triangle(A,E,F) ) ;

        this->triangle_primitives.push_back( Triangle(B,G,C) ) ;
        this->triangle_primitives.push_back( Triangle(B,F,G) ) ;

        this->triangle_primitives.push_back( Triangle(C,H,D) ) ;
        this->triangle_primitives.push_back( Triangle(C,G,H) ) ;

        this->triangle_primitives.push_back( Triangle(D,E,A) ) ;
        this->triangle_primitives.push_back( Triangle(D,H,E) ) ;

    }

    void draw(){
        for(Triangle &triangle : triangle_primitives){
            triangle.color = this->color ;
            triangle.draw() ;
        }
    }

    double intersect(Ray ray, Color *color, Vector &normal){
        // return -1; 
        *color = this->color ;
        double t = 1e18;
        for(Triangle &triangle : triangle_primitives){
            double tt = triangle.intersect(ray) ;
            if( tt < 0 ) continue ;
            if( tt < t ){
                normal = triangle.normal ;
                normal.normalize() ;
                t = tt ;
            }
        }
        t = (t == 1e18 || t < 0) ? -1 : t ;
        return t; 
    }


    double intersect2(Ray ray, Color *color, Vector& normal) {
        float tmin, tmax, tymin, tymax, tzmin, tzmax;
        // Calculate the intersection distances on each axis (x, y, z)
        Vector minPoint = reference_point ;
        Vector maxPoint = reference_point + Vector(length,length,length) ;

        if (ray.dir.x() >= 0) {
            tmin = (minPoint.x() - ray.origin.x()) / ray.dir.x();
            tmax = (maxPoint.x() - ray.origin.x()) / ray.dir.x();
        } else {
            tmin = (maxPoint.x() - ray.origin.x()) / ray.dir.x();
            tmax = (minPoint.x() - ray.origin.x()) / ray.dir.x();
        }

        if (ray.dir.y() >= 0) {
            tymin = (minPoint.y() - ray.origin.y()) / ray.dir.y();
            tymax = (maxPoint.y() - ray.origin.y()) / ray.dir.y();
        } else {
            tymin = (maxPoint.y() - ray.origin.y()) / ray.dir.y();
            tymax = (minPoint.y() - ray.origin.y()) / ray.dir.y();
        }

        if ((tmin > tymax) || (tymin > tmax))
            return -1;

        if (tymin > tmin)
            tmin = tymin;
        if (tymax < tmax)
            tmax = tymax;

        if (ray.dir.z() >= 0) {
            tzmin = (minPoint.z() - ray.origin.z()) / ray.dir.z();
            tzmax = (maxPoint.z() - ray.origin.z()) / ray.dir.z();
        } else {
            tzmin = (maxPoint.z() - ray.origin.z()) / ray.dir.z();
            tzmax = (minPoint.z() - ray.origin.z()) / ray.dir.z();
        }

        if ((tmin > tzmax) || (tzmin > tmax))
            return -1;

        if (tzmin > tmin)
            tmin = tzmin;
        if (tzmax < tmax)
            tmax = tzmax;

        if (tmax < 0)
            return -1;

        // Calculate the intersection point and normal
        Vector intersectionPoint = ray.origin + ray.dir * tmin;

        if (tmin == tmax) {
            if (tmin == (minPoint.x() - ray.origin.x()) / ray.dir.x())
                normal = Vector(-1, 0, 0);
            else if (tmin == (maxPoint.x() - ray.origin.x()) / ray.dir.x())
                normal = Vector(1, 0, 0);
            else if (tmin == (minPoint.y() - ray.origin.y()) / ray.dir.y())
                normal = Vector(0, -1, 0);
            else if (tmin == (maxPoint.y() - ray.origin.y()) / ray.dir.y())
                normal = Vector(0, 1, 0);
            else if (tmin == (minPoint.z() - ray.origin.z()) / ray.dir.z())
                normal = Vector(0, 0, -1);
            else if (tmin == (maxPoint.z() - ray.origin.z()) / ray.dir.z())
                normal = Vector(0, 0, 1);
        } else {
            if (tmin == (minPoint.x() - ray.origin.x()) / ray.dir.x())
                normal = Vector(-1, 0, 0);
            else if (tmin == (maxPoint.x() - ray.origin.x()) / ray.dir.x())
                normal = Vector(1, 0, 0);
            else if (tmin == (minPoint.y() - ray.origin.y()) / ray.dir.y())
                normal = Vector(0, -1, 0);
            else if (tmin == (maxPoint.y() - ray.origin.y()) / ray.dir.y())
                normal = Vector(0, 1, 0);
            else if (tmin == (minPoint.z() - ray.origin.z()) / ray.dir.z())
                normal = Vector(0, 0, -1);
            else if (tmin == (maxPoint.z() - ray.origin.z()) / ray.dir.z())
                normal = Vector(0, 0, 1);
        }
        *color = this->color;
        return tmin;
    }
    friend std::istream &operator>>(std::istream &in, Cube &cube){
        in >> cube.reference_point;
        in >> cube.length; 
        in >> cube.color.R >> cube.color.G >> cube.color.B;
        cube.color.R *= 255.0;
        cube.color.G *= 255.0;
        cube.color.B *= 255.0;
        in >> cube.coefficients.ka >> cube.coefficients.kd >> cube.coefficients.ks >> cube.coefficients.reflectance;
        in >> cube.shine;

        cube = Cube(cube.reference_point, cube.length, cube.color, cube.coefficients, cube.shine);
        return in;
    }
};

class Checkerboard : public Object {
    public:
    Vector pos;
    double checkerboard_side;
    double zFar;

    Checkerboard(double checkerboard_side, SurfaceProperties coeff) : Object() {
        this->checkerboard_side = checkerboard_side ;
        this->coefficients = coeff ;
    }
    Checkerboard(Vector &pos, double checkerboard_side, double zFar) : Object() {
        this->pos = pos;
        this->checkerboard_side = checkerboard_side;
        this->zFar
            = zFar;
    }

    Color getColor(int x,int y){
        return (x+y)%2 == 0 ? BLACK : WHITE ;
    }



    void draw(){
    } 


    double intersect(Ray ray, Color *color, Vector &normal){
        // if( abs(ray.dir.z()) < EPSILON ) return -1 ; // parallel to plane (z=0) 
        if( ray.dir.z() == 0.0) return -1 ;

        double t = -(ray.origin.z()/ray.dir.z()) ;
        if( t < 0.0 ) return -1 ;
        Vector point = ray.origin + ray.dir*t ;
        int x = ceil(point.x()/checkerboard_side);
        int y = ceil(point.y()/checkerboard_side);
        *color =  getColor(x,y) ;
        normal = Vector(0,0,1) ;
        // cout<<"color : "<<*color<<endl;
        return t ;
    }
};

struct Light{
    static const int POINT = 0 ;
    static const int SPOTLIGHT = 2 ;
    Vector pos;
    Color color;
    double falloff;
    int type ; 
    Light(){
        type = POINT ; 
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
        type = SPOTLIGHT ;
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


void utiltest(){
    Vector o1(1,2,3) ;
    Vector d1(12,234,-3) ;

    Ray r1(o1,d1) ; 

    cout << r1 << endl; 
    Vector intersectionPoint = r1.origin + r1.dir * 3 ;
    cout << intersectionPoint << endl; 
    Vector normal(0,0,1) ;
    Ray reflectedR1( intersectionPoint, r1.dir - normal * 2.0 * r1.dir.dot(normal) ) ;



    cout << reflectedR1 <<endl ;
    reflectedR1.origin = reflectedR1.origin - reflectedR1.dir * .00012; 
    cout << reflectedR1 <<endl ;

    cout<< d1.dot(normal) << endl; 
    cout<< -d1.dot(normal) << endl; 


}