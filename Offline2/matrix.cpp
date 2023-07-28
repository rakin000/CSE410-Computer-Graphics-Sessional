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
    Matrix identity(){
        
    }    
    tuple<int,int> shape(){
        return make_tuple(n,m) ;
    }

    int rows(){
        return n; 
    }
    int cols(){
        return m ;
    }
    int get(int i,int j){
        return mat[i][j] ;
    }
    Matrix operator*(Matrix b){
        if( m != b.rows() )
            throw exception("Matmul: dimension error") ;

        Matrix res(n,b.cols()) ;

        for(int i=0;i<n;i++){
            for(int j=0;j<b.cols();j++){
                res.mat[i][j] = 0 ;
                for(int k=0;k<m;k++)
                    res.mat[i][j] += mat[i][k]*b.get(k,j) ;
            }
        }
    }
};