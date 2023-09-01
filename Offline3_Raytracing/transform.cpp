#include <bits/stdc++.h>
using namespace std ;

#define DEG2RAD (M_PI/180.0)

struct Vector{
    vector<double> v;

    Vector(){
        v = vector<double> (4,0.0) ;
        v[3] = 1.0;
    } 
    Vector(double x,double y,double z):
        Vector()
        {
            v[0] = x; 
            v[1] = y; 
            v[2] = z ;
        }
    Vector(double x,double y,double z,double n):
        Vector(x,y,z)
    {
        v[3] = n;
    }
    Vector(const Vector &rhs){
        v = rhs.v; 
    }

    Vector& operator=(const Vector& rhs) {
        if (this != &rhs) {
            v = rhs.v;
        }
        return *this;
    }

    double x(){ return v[0]; }
    double y(){ return v[1]; }
    double z(){ return v[2]; }
    double abs(){
        return sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]) ;}

    Vector cross(Vector rhs){
        return Vector(v[1]*rhs.v[2]-v[2]*rhs.v[1], -(v[0]*rhs.v[2]-v[2]*rhs.v[0]), v[0]*rhs.v[1]-v[1]*rhs.v[0]) ;
    }
    double dot(Vector rhs){
        return v[0]*rhs.v[0]+v[1]*rhs.v[1]+v[2]*rhs.v[2] ;
    }
    Vector operator+(Vector rhs){
        return Vector(v[0]+rhs.v[0],v[1]+rhs.v[1],v[2]+rhs.v[2]) ;
    }
    Vector operator+(double val){
        return Vector(v[0]+val,v[1]+val,v[2]+val) ;
    }
    friend Vector operator+(double val, Vector v){
        return v+val;
    }
    Vector operator-(Vector rhs){
        return Vector(v[0]-rhs.v[0],v[1]-rhs.v[1],v[2]-rhs.v[2]) ;
    }
    Vector operator-(double val){
        return Vector(v[0]-val,v[1]-val,v[2]-val) ;
    }
    friend Vector operator-(double val, Vector v){
        return v-val;
    }
    Vector operator-(){
        return Vector(-v[0],-v[1],-v[2]) ;
    }
    Vector operator*(double val){
        return Vector(val*v[0],val*v[1],val*v[2]) ;
    }
    Vector operator/(double val){
        return Vector(v[0]/val,v[1]/val,v[2]/val) ;
    }
    double angle(Vector rhs){
        return acos( dot(rhs) / (abs() * rhs.abs()) ) ;
    }
    vector<double> normalize(){
        double d = v[0]*v[0] + v[1]*v[1] + v[2]*v[2] ; 
        d = sqrt(d) ;
        v[0] /= d ;
        v[1] /= d ;
        v[2] /= d ;
        v[3] = 1; 
        return v;
    }
    vector<double> scaledown(){
        v[0] /= v[3];
        v[1] /= v[3];
        v[2] /= v[3];
        v[3] /= v[3];
        return v;
    }
    friend double angle(Vector a, Vector b){
        return acos( a.dot(b) / (a.abs() * b.abs()) ) ;
    }
    friend istream& operator>>(istream &in, Vector &p){
        in>>p.v[0]>>p.v[1]>>p.v[2];
        return in;
    }
    friend ostream& operator<<(ostream &out,Vector &p){
        out<<p.v[0]<<" "<<p.v[1]<<" "<<p.v[2]<<endl;;
        return out;
    }
};

class Matrix{
    public :
    vector<vector<double>> mat; 
    int n,m; 


    Matrix(int n,int m){
        this->n = n ;
        this->m = m ;

        mat = vector<vector<double>> (n,vector<double>(m,0.0)) ;
    }
    Matrix():
        Matrix(4,4) 
    {}
    Matrix(int dim):
        Matrix(dim,dim)
    {}
    Matrix(vector<double> arr, int n,int m){
        if( n*m > arr.size() )
            throw exception();
        this->mat = vector<vector<double>>(n,vector<double>(m)) ;
        this->n = n;
        this->m = m; 
        for(int i=0;i<n;i++){
            for(int j=0;j<m;j++)
               mat[i][j] = arr[i*n+j] ; 
        }
    }
    Matrix(vector<Vector> arr){
        mat = vector<vector<double>> (arr.size(),vector<double>(4)) ;
        n = arr.size();
        m = 4 ;

        for(int i=0;i<n;i++)
            mat[i] = arr[i].v ;
    }
    Matrix(const Matrix &rhs){
        this->mat = rhs.mat; 
        this->n = rhs.n;
        this->m = rhs.m; 
    }

    Matrix operator=(Matrix rhs){
        this->mat = rhs.mat ;
        this->n = rhs.n;
        this->m = rhs.m;
        return *this;
    }

    static Matrix identity(int dim){
        Matrix I(dim,dim) ;

        for(int i=0;i<dim;i++) 
            I.mat[i][i] = 1.0 ;
        return I ;
    }    
    static Matrix scale(vector<double> sc){
        Matrix S = Matrix::identity(4);
        for(int i=0;i<sc.size();i++)
            S[i][i] = sc[i];
        return S ;
    }
    static Matrix translate(vector<double> t){
        Matrix T = Matrix::identity(4);
        for(int i=0;i<t.size();i++)
            T[i][T.m-1] = t[i];
        return T ;
    }
    static Vector R(Vector x,Vector a, double theta){
        return x*cos(DEG2RAD*theta) + a*((1.0-cos(DEG2RAD*theta))*(a.dot(x))) + a.cross(x)*sin(DEG2RAD*theta) ; 
    }
    static Matrix rotate(Vector a, double angle){
        a.normalize() ;

        Vector c1 = R(Vector(1,0,0),a,angle);
        Vector c2 = R(Vector(0,1,0),a,angle);
        Vector c3 = R(Vector(0,0,1),a,angle);

        Matrix Rot(4) ;

        Rot[0] = c1.v; Rot[0][3] = 0;
        Rot[1] = c2.v; Rot[1][3] = 0;
        Rot[2] = c3.v; Rot[2][3] = 0;
        Rot[3][3] = 1; 

        // cout<<Rot<<endl; 
        // cout<<Rot.transpose()<<endl;
        Rot.transpose();   
        return Rot;
    }
    vector<double>& operator[](int i){
        return mat[i] ;
    }
    int rows(){
        return n; 
    }
    int cols(){
        return m ;
    }

    Matrix transpose(){
        Matrix res(m,n) ;

        for(int i=0;i<m;i++)
            for(int j=0;j<n;j++)
                res[i][j] = mat[j][i];
        *this = Matrix(res); 
        return res; 
    }
    Matrix operator*(Matrix b){
        if( m != b.rows() )
            throw exception();//"Matmul: dimension error") ;

        Matrix res(n,b.cols()) ;

        for(int i=0;i<n;i++){
            for(int j=0;j<b.cols();j++){
                res.mat[i][j] = 0.0 ;
                for(int k=0;k<m;k++)
                    res[i][j] += mat[i][k]*b[k][j];
            }
        }
        return res ;
    }
    vector<Vector> operator*(vector<Vector> points){
        Matrix res = (*this)*Matrix(points).transpose(); 
        res.transpose() ;
        for(int i=0;i<points.size();i++){
            points[i] = Vector(res[i][0],res[i][1],res[i][2],res[i][3]);
            points[i].scaledown();
        }
        return points; 
    }
    Matrix operator+(Matrix b){
        if( n != b.rows() )
            throw exception() ;
        if( m != b.cols() )
            throw exception();//"Matmul: dimension error") ;

        Matrix res(m,n) ;

        for(int i=0;i<n;i++){
            for(int j=0;j<b.cols();j++){
                res[i][j]=mat[i][j] + b[i][j] ;
            }
        }
        return res; 
    }
    bool operator==(Matrix rhs){
        bool res = 1; 
        res &= (n==rhs.n) && (m == rhs.m) ;

        for(int i=0;i<n && res;i++){
            for(int j=0;j<m && res;j++){
                res &= (mat[i][j] == rhs[i][j]) ;
            }
        }
        return res ;
    }
    friend ostream& operator<<(ostream& out,Matrix mat){
        out<<"[\n" ;
        for(int i=0;i<mat.rows();i++){
            out<<"[" ;
            out<<mat[i][0];
            for(int j=1;j<mat.cols();j++)
                out<<","<<mat[i][j] ;
            out<<"]\n";
        }
        out<<"]"<<endl;
        return out; 
    }
    friend istream& operator>>(istream& in,Matrix &mat){
        cout<<"Dimension: "<<mat.n<<" x "<<mat.m<<endl;
        for(int i=0;i<mat.rows();i++){
            for(int j=0;j<mat.cols();j++)
                in>>mat[i][j] ; 
        }
        return in; 
    }
};


struct transformation_matrix{
    stack<Matrix> M ;

    transformation_matrix(){
        M.push( Matrix::identity(4) ) ;
    }
    void add(Matrix m){
        M.top() = M.top()*m;
    }
    void push(){
        M.push(M.top());
    }
    void pop(){
        if( !M.empty() )  M.pop() ;
        if( M.empty() )   M.push(Matrix::identity(4));
    }

    Matrix transform(Matrix points){
        return M.top()*points; 
    }
    vector<Vector> transform(vector<Vector> points){
        // Matrix res = M.top()*Matrix(points).transpose(); 
        // res.transpose() ;
        // for(int i=0;i<points.size();i++){
        //     points[i] = Vector(res[i][0],res[i][1],res[i][2]);
        // }
        // return points; 
        return M.top()*points;
    }
};


void test(){
    Matrix a ;

    cout<<a<<endl; 

    Matrix b,c;
    c = Matrix::identity(4);
    b[1][3] = 5 ;
    
    cout<<b ;
    cout<<c ;
    cout<<b*c<<endl;


    Matrix d(vector<double>({2,3,1,1,1,2,3,1,1,1,2,3,3,1,1,2}),4,4) ;
    Matrix e(vector<double>({5,4,3,2,1,3,7,5,3,5,7,2,1,2,3,4}),4,4) ;
    cout<<d<<endl<<e<<endl;

    // cout<<d*e<<endl;
    // cout<<e*d<<endl;
    // cout<<d+e<<endl;

    e.transpose() ;

    cout<<e<<endl;
}