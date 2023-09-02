#include <bits/stdc++.h>
#include <GL/glut.h>
using namespace std; 

#define GLCOLOR(color) glColor3f( (color).R/255.0,(color).G/255.0,(color).B/255.0)
const double EPSILON = 1e-4;
void showLoadingScreen(int step, int totalSteps, string message = string("Loading...") ) {
        if( step == totalSteps ) {
            std::cout << message << " complete!" << std::endl;
            return;
        }
        
        float progress = float(step) / totalSteps * 100;
        std::cout << message << progress << "%";

        switch (step % 4) {
            case 0: std::cout << " |  "; break;
            case 1: std::cout << " /  "; break;
            case 2: std::cout << " -  "; break;
            case 3: std::cout << " \\  "; break;
        }

        std::cout << "\r";
}


#define DEG2RAD (M_PI/180.0)

struct Vector{
    double X,Y,Z,N;  

    Vector(double x,double y,double z,double n){
        this->X = x; 
        this->Y = y;
        this->Z = z;
        this->N = n;
    }
    Vector(double x,double y,double z):
        Vector(x,y,z,1)
    {
    }
    Vector():
        Vector(0,0,0)
    {
    }
    
    
    Vector(const Vector &rhs){
        this->X = rhs.X;
        this->Y = rhs.Y;
        this->Z = rhs.Z;
        this->N = rhs.N;
    }

    Vector& operator=(const Vector& rhs) {
        if (this != &rhs) {
            this->X = rhs.X;
            this->Y = rhs.Y;
            this->Z = rhs.Z;
            this->N = rhs.N;
        }
        return *this;
    }

    double x(){ return X; }
    double y(){ return Y; }
    double z(){ return Z; }
    double abs(){
        return sqrt(X*X + Y*Y + Z*Z) ;}

    Vector cross(Vector rhs){
        return Vector(Y*rhs.Z-Z*rhs.Y, -(X*rhs.Z-Z*rhs.X), X*rhs.Y-Y*rhs.X) ;
    }
    double dot(Vector rhs){
        return X*rhs.X+Y*rhs.Y+Z*rhs.Z ;
    }
    Vector operator+(Vector rhs){
        return Vector(X+rhs.X,Y+rhs.Y,Z+rhs.Z) ;
    }
    Vector operator+(double val){
        return Vector(X+val,Y+val,Z+val) ;
    }
    friend Vector operator+(double val, Vector v){
        return v+val;
    }
    Vector operator-(Vector rhs){
        return Vector(X-rhs.X,Y-rhs.Y,Z-rhs.Z) ;
    }
    Vector operator-(double val){
        return Vector(X-val,Y-val,Z-val) ;
    }
    friend Vector operator-(double val, Vector v){
        return v-val;
    }
    Vector operator-(){
        return Vector(-X,-Y,-Z) ;
    }
    Vector operator*(double val){
        return Vector(val*X,val*Y,val*Z) ;
    }
    Vector operator/(double val){
        return Vector(X/val,Y/val,Z/val) ;
    }
    double angle(Vector rhs){
        return acos( dot(rhs) / (abs() * rhs.abs()) ) ;
    }
    void normalize(){
        double d = X*X + Y*Y + Z*Z ; 
        d = sqrt(d) ;
        X /= d ;
        Y /= d ;
        Z /= d ;
        N = 1; 
    }
    void scaledown(){
        X /= N;
        Y /= N;
        Z /= N;
        N = 1;
    }
    friend double angle(Vector a, Vector b){
        return acos( a.dot(b) / (a.abs() * b.abs()) ) ;
    }
    friend istream& operator>>(istream &in, Vector &p){
        in>>p.X>>p.Y>>p.Z;
        return in;
    }
    friend ostream& operator<<(ostream &out,Vector &p){
        out<<p.X<<" "<<p.Y<<" "<<p.Z<<endl;;
        return out;
    }
};

 
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
    Vector AB, AC ;
    Color color ;

    Triangle(Vector A, Vector B, Vector C, Color color = BLACK){
        this->A = A ;
        this->B = B ;
        this->C = C ; 
        this->AB = B-A ;
        this->AC = C-A ;
        this->normal = (AB).cross(AC) ;
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

    double intersect2(Ray &r){
        Vector pvec = r.dir.cross(AC);

        float det = AB.dot(pvec);

        if (det < 0.000001)
            return -1;

        float invDet = 1.0 / det;
        Vector tvec = r.origin-A;

        float u = tvec.dot(pvec) * invDet;

        if (u < 0 || u > 1)
            return -1;
        Vector qvec = tvec.cross(AB);
        float v = r.dir.dot(qvec) * invDet;

        if (v < 0 || u + v > 1)
            return -1;

        return AC.dot(qvec) * invDet;
    }


    Vector intersectionPoint ;
    double getTwiceArea(Vector &a,Vector &b,Vector &c) {
        return ((b-a).cross(c-a)).abs() ;
    }
    bool isInside(Vector &p) {
        return abs( getTwiceArea(A, B, p) + getTwiceArea(A, C, p) + getTwiceArea(B, C, p) - getTwiceArea(A, B, C)) < EPSILON;
    }
    double intersect(Ray &ray) {
        // Point d = ray.dir;
        Vector normal = this->normal ;// normal = getNormal();
        double dotP = normal.dot(ray.dir);
        if (dotP > EPSILON) normal = normal * -1;
        double y = normal.dot(A);
        if (abs(dotP) < EPSILON) return -1;
        double t = ( y - normal.dot(ray.origin) ) / (dotP);
        intersectionPoint = ray.origin + ray.dir * t;

        if (isInside(intersectionPoint)) 
            return t;
        

        return -1;
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
        //     delete triangle ;
    }
    virtual void draw() = 0 ;
    virtual  double intersect(Ray ray, Color *color, Vector *normal) = 0 ;
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

    virtual double intersect(Ray ray, Color *color, Vector *normal){
        if( color != nullptr ) *color = this->color; 

        //equation  t*t + 2(P0-O).D t + (P0-O).(P0-O)-r*r = 0 ;
        Vector P0_O  = ray.origin - reference_point; // P0-O 
        double a = 1; 
        double b = 2 * (ray.dir.dot(P0_O));
        double c = (P0_O.dot(P0_O)) - (length*length);

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
            if( normal != nullptr ) {
                *normal = intersectionPoint-reference_point; 
                normal->normalize();
            }
            return t1;
        }
        if (t2 > 0){
            Vector intersectionPoint = ray.origin + ray.dir * t2;
            if( normal != nullptr ) {
                *normal = intersectionPoint-reference_point; 
                normal->normalize();
            }
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

    double intersect(Ray ray, Color *color, Vector *normal){
        // return -1; 
        if( color != nullptr ) *color = this->color ;
        double t = 1e18;
        for(Triangle &triangle : triangle_primitives){
            double tt = triangle.intersect(ray) ;
            if( tt < 0 ) continue ;
            if( tt < t ){
                if( normal != nullptr) {
                    *normal = triangle.normal ;
                    normal->normalize() ;
                }
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

    double intersect(Ray ray, Color *color, Vector *normal){
        // return -1; 
        if( color != nullptr ) *color = this->color ;
        double t = 1e18;
        for(Triangle &triangle : triangle_primitives){
            double tt = triangle.intersect(ray) ;
            if( tt < 0 ) continue ;
            if( tt < t ){
                if( normal != nullptr){
                    *normal = triangle.normal ;
                    normal->normalize() ;
                }
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


    double intersect(Ray ray, Color *color, Vector *normal){
        // if( abs(ray.dir.z()) < EPSILON ) return -1 ; // parallel to plane (z=0) 
        if( ray.dir.z() == 0.0) return -1 ;

        double t = -(ray.origin.z()/ray.dir.z()) ;
        if( t < 0.0 ) return -1 ;
        Vector point = ray.origin + ray.dir*t ;
        int x = ceil(point.x()/checkerboard_side);
        int y = ceil(point.y()/checkerboard_side);
        if( color != nullptr ) *color =  getColor(x,y) ;
        if( normal != nullptr ) *normal = Vector(0,0,1) ;
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