#include <bits/stdc++.h>
#include "matrix.cpp" 
#include "point.cpp"
using namespace std ;

Point eye,look,up; 
double fovY ;

int main(int argc,char **argv){
    if( argc < 2 ){
        exit(-1) ;
    }
    fstream in(argv[1],ios_base::in) ;

    in>>eye.x>>eye.y>>eye.z ;
    in>>look.x>>look.y>>look.z ;
    in>>up.x>>up.y>>up.z ;
    in >> fovY ;

    string cmd ;
    stack<Matrix> M ;
    
    while(in>>cmd){
        if(cmd == "triangle"){

        }
        else if(cmd == "translate"){}
        else if(cmd == "scale"){}
        else if(cmd == "rotate"){}
        else if(cmd == "push"){}
        else if(cmd == "pop"){
        }
        else {
            break; 
        }
    }

}