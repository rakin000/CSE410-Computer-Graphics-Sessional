#include "matrix.cpp" 
#include "point.cpp"

struct transformation_matrix{
    vector<Matrix> M ;
    int i ; 

    transformation_matrix(){
        M.push_back( Matrix::identity(4) ) ;
        i = 0 ;
    }
    void push(Matrix m){
        M.push_back(m);
    }
    void pop(){
        if( M.empty() ) return; 
        i-- ;
        M.pop_back() ;
    }
    void compute(){
        while(i<M.size()){
            M[i] = M[i-1]*M[i] ;
            i++ ;
        }
    }

    Matrix transform(Matrix points){
        compute() ;
        return M[i-1]*points; 
    }
};