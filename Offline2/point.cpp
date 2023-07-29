#include <vector> 
#include <math.h>
using namespace std; 

struct Point{
    vector<double> vec(4); 

    vector<double> normalize(){
        double d = vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2] ; 
        d = sqrt(d) ;
        vec[0] /= d ;
        vec[1] /= d ;
        vec[2] /= d ;
        vec[3] /= d ; 
        return vec;
    }
    friend istream& operator>>(istream &in, Point &p){
        in>>vec[0]>>vec[1]>>vec[2];
        return in;
    }
};