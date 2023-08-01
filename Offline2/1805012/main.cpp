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
    fstream out("stage1.txt",ios_base::out);
    fstream out2("stage2.txt",ios_base::out);
    fstream out3("stage3.txt",ios_base::out);
    in>>eye;
    in>>look;
    in>>up;
    in>>fovY>>aspectRatio>>near>>far ;


    Vector l = look-eye ;
    l.normalize();
    Vector r = l.cross(up) ;
    r.normalize();
    Vector u = r.cross(l) ;

    Matrix T(vector<double>({1,0,0,-eye.x(), 
                            0,1,0,-eye.y(), 
                            0,0,1,-eye.z(),
                            0,0,0,1 }),4,4) ;
    Matrix R(vector<double>({r.x(),r.y(),r.z(),0, 
                            u.x(),u.y(),u.z(),0, 
                            -l.x(),-l.y(),-l.z(),0, 
                            0,0,0,1}),4,4);
    Matrix V = R*T ;


    double fovX = fovY * aspectRatio;
    double t = near * tan(DEG2RAD*(fovY/2)) ;
    double r_ = near * tan(DEG2RAD*(fovX/2)) ;

    Matrix P(vector<double> ({near/r_,0,0,0,
                              0,near/t,0,0,
                              0,0,-(far+near)/(far-near), -(2.0*far*near)/(far-near),
                              0,0,-1.0,0}),4,4) ;


    string cmd ;
    transformation_matrix M ;
    while(in>>cmd){
        if(cmd == "triangle"){
            vector<Vector> p(3) ;
            out<<setprecision(7)<<fixed;
            out2<<setprecision(7)<<fixed;
            out3<<setprecision(7)<<fixed;

            for(int i=0;i<3;i++){
                in>>p[i] ;
            }
            p = M.transform(p) ;
            for(int i=0;i<3;i++){
                out<<p[i];
            }
            out<<endl;

            p = V*p;
            for(int i=0;i<3;i++){
                out2<<p[i];
            }
            out2<<endl;

            p = P*p;
            for(int i=0;i<3;i++){
                out3<<p[i];
            }
            out3<<endl;

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