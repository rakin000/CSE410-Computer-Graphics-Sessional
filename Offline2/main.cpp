#include <bits/stdc++.h>
#include "transform.cpp"
#include "bitmap_image.hpp"
using namespace std ;

static unsigned long int g_seed = 1;

namespace my 
{
    inline int random(){
        g_seed = (214013 * g_seed + 2531011);
        return (g_seed >> 16) & 0x7FFF;
    }
}


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

    // view transformation     
    Matrix T(vector<double>({1,0,0,-eye.x(), 
                            0,1,0,-eye.y(), 
                            0,0,1,-eye.z(),
                            0,0,0,1 }),4,4) ;
    Matrix R(vector<double>({r.x(),r.y(),r.z(),0, 
                            u.x(),u.y(),u.z(),0, 
                            -l.x(),-l.y(),-l.z(),0, 
                            0,0,0,1}),4,4);
    Matrix V = R*T ;

    // projection 
    double fovX = fovY * aspectRatio;
    double t = near * tan(DEG2RAD*(fovY/2)) ;
    double r_ = near * tan(DEG2RAD*(fovX/2)) ;

    Matrix P(vector<double> ({near/r_,0,0,0,
                              0,near/t,0,0,
                              0,0,-(far+near)/(far-near), -(2.0*far*near)/(far-near),
                              0,0,-1.0,0}),4,4) ;


    string cmd ;
    transformation_matrix M ; // model transformation

    vector<vector<Vector>> triangles ; // for z buffer  
    while(in>>cmd){
        if(cmd == "triangle"){
            vector<Vector> p(3) ;
            out<<setprecision(7)<<fixed;
            out2<<setprecision(7)<<fixed;
            out3<<setprecision(7)<<fixed;

            for(int i=0;i<3;i++){
                in>>p[i] ;
            }

            // model transformation
            p = M.transform(p) ;
            for(int i=0;i<3;i++){
                out<<p[i];
            }
            out<<endl;
            
            // view transformation  
            p = V*p;
            for(int i=0;i<3;i++){
                out2<<p[i];
            }
            out2<<endl;

            // projection transformation 
            p = P*p;
            for(int i=0;i<3;i++){
                out3<<p[i];
            }

            out3<<endl;
            triangles.push_back(p) ;
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
    out.close();
    out2.close();
    out3.close() ;
    in.close();
    // z -buffer
    fstream config(string(argv[2]),ios_base::in); 
    double screen_height,screen_width; 
    config>>screen_width>>screen_height;
    const double z_max = 1.0;
    vector<vector<double>> z_buffer(screen_width,vector<double> (screen_height,z_max));
    double dx = 2/screen_width ,dy = 2/screen_height ;
    double left_x = dx/2 ;
    double top_y = 1.0-dy/2; 

    bitmap_image image(screen_width,screen_height);
    image.set_all_channels(0,0,0) ;


    for(vector<Vector> triangle: triangles){
        // process triangle  
        double max_x = max({triangle[0].x(),triangle[1].x(),triangle[2].x()});
        double min_x = min({triangle[0].x(),triangle[1].x(),triangle[2].x()});
        double max_y = max({triangle[0].y(),triangle[1].y(),triangle[2].y()});
        double min_y = min({triangle[0].y(),triangle[1].y(),triangle[2].y()});

        int top_y = max_y/dy ;
        int bottom_y = min_y/dy; 
    } 

}