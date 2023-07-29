#include <bits/stdc++.h>
using namespace std ; 

class Matrix{
    public :
    vector<vector<double>> mat; 
    int n,m; 


    Matrix(){
        n=m=4; 
        mat = vector<vector<double>> (n,vector<double>(m,0.0)) ;
    }

    Matrix(int n,int m){
        this->n = n ;
        this->m = m ;

        mat = vector<vector<double>> (n,vector<double>(m,0.0)) ;
    }
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
    Matrix(Matrix &rhs){
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
        Matrix S(sc.size()+1) ;
        for(int i=0;i<sc.size();i++)
            S[i][i] = sc[i];
        S[S.n-1][S.m-1] = 1;
        return S ;
    }
    static Matrix translate(vector<double> t){
        Matrix T(t.size()) ;
        for(int i=0;i<t.size();i++)
            T[i][T.m-1] = t[i];
        T[T.n-1][T.m-1] = 1;
        return T ;
    }
    static Matrix rotate(vector<double> rot, double angle){
        return identity(4) ;
    }
    tuple<int,int> shape(){
        return make_tuple(n,m) ;
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
    Matrix operator*(Matrix b){
        if( m != b.rows() )
            throw exception();//"Matmul: dimension error") ;

        Matrix res(n,b.cols()) ;

        for(int i=0;i<n;i++){
            for(int j=0;j<b.cols();j++){
                res.mat[i][j] = 0 ;
                for(int k=0;k<m;k++)
                    res[i][j] += mat[i][k]*b[k][j];
            }
        }
        return res ;
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


int main(){
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

    cout<<d*e<<endl;
    cout<<e*d<<endl;
    cout<<d+e<<endl;
}