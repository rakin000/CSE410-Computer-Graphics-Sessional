#include <bits/stdc++.h>
#include "transform.cpp"
#include "bitmap_image.hpp"
using namespace std ;


namespace my 
{
    static unsigned long int g_seed = 1;
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


    // view transformation     
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


    //z -buffer
    fstream config(string(argv[2]),ios_base::in); 
    double screen_height,screen_width; 
    config>>screen_width>>screen_height;
    config.close();

    const double z_max = 1.0;
    const double z_min = -1.0;
    const double y_max = 1.0;
    const double y_min = -1.0;
    const double x_max = 1.0;
    const double x_min = -1.0;
    vector<vector<double>> z_buffer(screen_width,vector<double> (screen_height,z_max));

    double dx = (x_max-x_min)/screen_width ,dy = (y_max-y_min)/screen_height ;
    double left_x = x_min+dx/2 ;
    double right_x = x_max-dx/2 ;
    double top_y = y_max-dy/2 ;
    double bottom_y = y_min+dy/2 ;

    bitmap_image image(screen_width,screen_height);
    image.set_all_channels(0,0,0) ;

    out3 = fstream("stage3.txt",ios_base::in); 
    triangles = vector<vector<Vector>>();
    while(!out3.eof()){
        vector<Vector> p(3) ;
        out3>>p[0]>>p[1]>>p[2] ;
        triangles.push_back(p);
    }

    for(vector<Vector> triangle: triangles){
        rgb_t col = {my::random(),my::random(),my::random()} ;

        // for(int i=0;i<3;i++){ 
            // triangle[i] = triangle[i] + 1.0 ;
            // cout<<triangle[i]; 
        // } 
        // cout<<endl ;

        double triangle_max_x = max({triangle[0].x(),triangle[1].x(),triangle[2].x()});
        double triangle_min_x = min({triangle[0].x(),triangle[1].x(),triangle[2].x()});
        double triangle_max_y = max({triangle[0].y(),triangle[1].y(),triangle[2].y()});
        double triangle_min_y = min({triangle[0].y(),triangle[1].y(),triangle[2].y()});

        triangle_max_x = min(right_x,triangle_max_x);
        triangle_min_x = max(left_x,triangle_min_x);
        triangle_max_y = min(top_y,triangle_max_y);
        triangle_min_y = max(bottom_y,triangle_min_y);

        int topline_y = round((triangle_max_y-bottom_y)/dy);
        int bottomline_y = round((triangle_min_y-bottom_y)/dy) ;
        topline_y = min((int)screen_height-1,topline_y);
        bottomline_y = max(0,bottomline_y) ;

        for(unsigned int y = topline_y;y>=bottomline_y;y--){
            Vector left(triangle_max_x, y*dy, z_max) ; 
            Vector right(triangle_min_x, y*dy, z_max) ; 

            for(int i=0;i<3;i++){
                int j = (i+1)%3; 
                if( abs(triangle[j].y()-triangle[i].y()) <= 1e-16 ){
                }
                else {
                    double tt = ( (y*dy+bottom_y)-triangle[i].y())/(triangle[j].y()-triangle[i].y()) ;
                    if( tt >= 0.0 && tt <= 1.0) {
                        Vector point = triangle[i] + (triangle[j]-triangle[i])*tt ;

                        if( point.x() <= left.x() )
                            left = point ;
                        if( point.x() >= right.x() )
                            right = point ;
                    }

                }
            }

            
            int leftline_x = round((left.x()-left_x)/dx); 
            int rightline_x = round((right.x()-left_x)/dx); 
            leftline_x = max(0,leftline_x) ;
            rightline_x = min((int)screen_width-1,rightline_x) ;

            for(int x = leftline_x;x<=rightline_x;x++){
                double z = left.z() + ((right.z()-left.z())/(right.x()-left.x()))*((x*dx+left_x)-left.x()) ;
                if( z < z_buffer[x][screen_height-1-y] && z > z_min ){
                    z_buffer[x][screen_height-1-y] = z; 
                    image.set_pixel(x,(int)screen_height-y,col) ;
                }
            }
        } 
    }


    fstream zout("z_buffer.txt",ios_base::out) ;
    zout<<setprecision(6)<<fixed ;
    for(int y=0;y<screen_height;y++){
        for(int x=0;x<screen_width;x++){
            if( z_buffer[x][y]<z_max){
                zout<<z_buffer[x][y]<< "\t" ;
            }
        }
        zout<<endl;
    }

    zout.close();

    image.save_image("out.bmp"); 

    return 0;
}