#include <bits/stdc++.h>
#include "transform.cpp"
using namespace std ;

Vector eye,look,up; 
double fovY,aspectRatio,near,far ;

int main(int argc,char **argv){
    if( argc < 2 ){
        exit(-1) ;
    }
    fstream in(string(argv[1]),ios_base::in) ;
    fstream out("out.txt",ios_base::out);
    in>>eye;
    in>>look;
    in>>up;
    in>>fovY>>aspectRatio>>near>>far ;

    string cmd ;
    transformation_matrix M ;

    while(in>>cmd){
        if(cmd == "triangle"){
            vector<Vector> p(3) ;
            out<<setprecision(7)<<fixed; 

            for(int i=0;i<3;i++){
                in>>p[i] ;
            }
            p = M.transform(p) ;
            for(int i=0;i<3;i++){
                out<<p[i];
            }
            out<<endl;
        }
        else if(cmd == "translate"){
            double dx,dy,dz;
            in>>dx>>dy>>dz; 
            M.add(Matrix::translate(vector<double>({dx,dy,dz}))) ;
        }
        else if(cmd == "scale"){
            double sx,sy,sz;
            in>>sx>>sy>>sz;
            M.add(Matrix::scale(vector<double>({sx,sy,sz})) );
        }
        else if(cmd == "rotate"){
            double theta ;
            in>>theta ;
            Vector a;
            in>>a; 
            M.add(Matrix::rotate(a,theta)) ;
        }
        else if(cmd == "push"){
            M.push();
        }
        else if(cmd == "pop"){
            M.pop();
        }
        else if(cmd == "end"){
            break; 
        }
    }

}