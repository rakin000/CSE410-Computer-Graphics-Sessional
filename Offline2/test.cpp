#include "transform.cpp"


int main(){
    Vector a(0,0,1);
    Matrix m = Matrix::rotate(Vector(0,0,1),90) ;
    Vector rotz = Matrix::R(Vector(0,0,1),a,90) ;

    double dt = a.dot(a);
    double mul = (1.0-cos(DEG2RAD*90));
    
    cout<<dt<<endl;
    cout<<mul<<endl;
    mul = dt*mul;
    a = a*mul; 
    cout<<a<<endl;
    // cout<<a.cross(a)<<endl;
    cout<<rotz<<endl;
    cout<<m<<endl;
}