#include "mat.hpp"
#include <iostream>
#include <iomanip>

mat_row::mat_row(int col){
    cols = col;
	row = new double[col];
}

double& mat_row::operator[](int index) const{
    return row[index];
} 

void mat_row::set(size_t index, double d){
    row[index] = d;
}

size_t mat_row::length() const{
    return cols;
}

mat_row::~mat_row(){
    delete[] row;
}

mat::mat(int nrows, int ncols, bool aug){
    this->aug = aug;
    rows = nrows;
    cols = ncols;
    mat_rows = new mat_row*[nrows];
    for(int i = 0; i < nrows; i++){
        mat_row* row = new mat_row(ncols);
        mat_rows[i] = row;
    }    
}

mat_row* mat::operator[](int index){
    return mat_rows[index];
} 

mat::~mat(){
    for(size_t i = 0; i < rows; i++){
        delete mat_rows[i];
        mat_rows[i] = nullptr;
    }
    delete[] mat_rows;
}


void mat_row::add(const mat_row& r){
    size_t len = r.length();
    for(size_t i = 0; i < len; i++){
        row[i] += r[i]; 
    }
}

void mat::swap(int i, int j){
	mat_row* tmp = mat_rows[i];
    mat_rows[i] = mat_rows[j];
    mat_rows[j] = tmp;
}

size_t mat::get_rows() const{
    return rows;
}

size_t mat::get_cols() const{
    return cols;
}

bool mat::is_aug(){
    return aug;
}

size_t mat::count_non_zero_rows(bool aug){
    size_t rank = 0;
    size_t m = aug ? 0 : 1;
    for(size_t i = 0; i < rows; i++){
        bool zero_row = true;        
        for(size_t j = i; j < cols-m; j++){
            if(!is_zero((*mat_rows[i])[j])){
                zero_row = false;                
                break;
            }
        }
        if(!zero_row){
            rank++;
        }
    }
    return rank;
}

std::ostream& operator<<(std::ostream& out, mat_row const& mat_row){
    for(size_t i = 0; i < mat_row.length() - 1; i++){
        out << std::setw(12) << mat_row[i];
    }
    out  << std::setw(4) << '|';
    out  << std::setw(12) << mat_row[mat_row.length() - 1];
    return out;
}

void print_simple_mat(std::ostream& out, mat_row const& mat_row){
    for(size_t i = 0; i < mat_row.length(); i++){
        out << std::setw(12)  << mat_row[i];
    }
}

std::ostream& operator<<(std::ostream& out, mat& mat){
    if(mat.is_aug()){
        for(size_t i = 0; i < mat.get_rows(); i++){
            out << *mat[i] << std::endl;
        }
    }else{
        for(size_t i = 0; i < mat.get_rows(); i++){
            print_simple_mat(out, *mat[i]);
            out << std::endl;
        }
    }
    return out;
}

mat_row operator * (const mat_row& r1, double d){
    size_t len = r1.length();     
    mat_row r(len);
    for(size_t i = 0; i < len; i++){
        r[i] = r1[i]*d; 
    }
    return r;
}

bool is_zero(double d){
    return d > -EPSILON && d < EPSILON;
}
