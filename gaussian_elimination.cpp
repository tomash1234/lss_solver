#include "gaussian_elimination.hpp"
#include <thread>
#include <iostream>

double abs(double a){
    if(a >= 0){
        return a;
    }
    return -a;
}

size_t find_pivot(mat** matrix, int r){
    size_t argmax = r;
    mat_row* row = (**matrix)[r];
    double max_val = abs((*row)[r]);
    size_t rows = (*matrix)->get_rows();
    for(size_t i = r + 1; i < rows; i++){
        mat_row* rowi = (**matrix)[i];
        double v = abs((*rowi)[r]);        
        if(v > max_val){
            argmax = i;
            max_val = v;
        }   
    }
    return argmax;
}

mat* find_particular(mat** matrix){
    mat* A = *matrix;
    int n = static_cast<int>(A->get_cols());
    int m =static_cast<int>(A->get_rows());
    mat* x = new mat(n - 1, 1, false);
    for(int i = n - 2; i >= 0; i--){
        if(i >= m){
            ((*x)[i])->set(0, 0);
            continue;
        }
        double s = 0;
        for(int j = n - 2; j > i; j--){
            s += (*(*A)[i])[j] * (*(*x)[j])[0];
        }
        double v = ((*(*A)[i])[n-1] - s) / (*(*A)[i])[i];
        ((*x)[i])->set(0, v); 
    }
    return x;
}

void find_vector_homogeneous(mat** matrix, mat** homogen, int index){
    mat* A = *matrix;
    int n = static_cast<int>(A->get_cols());
    int m = static_cast<int>(A->get_rows());
    mat* x = *homogen;
    for(int i = n - 2; i >= 0; i--){
        if(i >= m){
            ((*x)[i])->set(index, n - 2 - index == i ? 1 : 0);
            continue;
        }
        double s = 0;
        for(int j = n - 2; j > i; j--){
            s += (*(*A)[i])[j] * (*(*x)[j])[index];
        }
        double v =  -s / (*(*A)[i])[i];
        ((*x)[i])->set(index, v); 
    }
}

void make_column_zero(mat* A, double pivot, size_t r, size_t start, size_t end){
    mat_row* cur_row = (*A)[r];
    for (size_t i = start; i < end; i++) {
        double v = (*(*A)[i])[r];
        if(is_zero(v)){
            (*A)[i]->set(r, 0);
            continue;
        }
        double alpha = -v / pivot;
        mat_row new_row = (*cur_row) * alpha;
        (*A)[i]->add(new_row); 
        (*A)[i]->set(r, 0);
        
    }
}

void make_upper_triangular_parallel(mat** matrix){
    int num_threads =  std::thread::hardware_concurrency();

    mat* A = *matrix;
    size_t rows = A->get_rows();
    size_t cols = A->get_cols();
	for(size_t r = 0; r < rows && r < cols - 1; r++){
        size_t pivot_row = find_pivot(matrix, r);
        if(pivot_row != r){
            A->swap(pivot_row, r);
        }        
        double pivot = (*((*A)[r]))[r];

        int chunk_size = (rows-r-1) / num_threads;
        if(chunk_size < 2){
            //when the chunk size is too small, run code in single thread
            make_column_zero(A, pivot, r, r+1, rows);
            continue;
        }

        std::thread* threads = new std::thread[num_threads];
        for (int i = 0; i < num_threads; ++i) {
            size_t start = r+1 + i* chunk_size;
            size_t end = r+1 + (i+1)* chunk_size;
            if(end > rows || i == num_threads - 1){
                end = rows;
            }    
            threads[i] = std::thread(make_column_zero, A, pivot, r, start, end);
        }

        for (int i = 0; i < num_threads; ++i) {
            threads[i].join();
        }
        delete [] threads;
    }
}

void make_upper_triangular(mat** matrix){
    mat* A = *matrix;
    size_t rows = A->get_rows();
    size_t cols = A->get_cols();
	for(size_t r = 0; r < rows && r < cols - 1; r++){
        size_t pivot_row = find_pivot(matrix, r);
        if(pivot_row != r){
            A->swap(pivot_row, r);
        }        

        double pivot = (*((*A)[r]))[r];
        mat_row* cur_row = (*A)[r];
    
        for (size_t i = r + 1; i < rows; i++) {
            double v = (*(*A)[i])[r];
            if(is_zero(v)){
                (*A)[i]->set(r, 0);
                continue;
            }
            double alpha = -v / pivot;
            mat_row new_row = (*cur_row) * alpha;
            (*A)[i]->add(new_row); 
            (*A)[i]->set(r, 0);
            
        }
    }
}

void find_solution(mat** matrix, int nullity, mat** particular, mat** homogen){
    mat* A = *matrix;
    size_t n = A->get_cols();
    *particular = find_particular(matrix);
    if(nullity > 0){
        *homogen = new mat(n - 1, nullity, false);
        for(int i = 0; i < nullity; i++){
            find_vector_homogeneous(matrix, homogen, i);
        }
    }
}

bool solve(mat** matrix, mat** particular, mat** homogen, bool verbose, bool multithread){
    if(multithread){
        make_upper_triangular_parallel(matrix);
    }else{
        make_upper_triangular(matrix);
    }
    if(verbose){
        std::cout << "Upper triangular form:" << std::endl;
        std::cout << **matrix << std::endl;
    }

    //check if system has a solution
    mat* A = *matrix;
    size_t rank = A->count_non_zero_rows(false);
    size_t rankAdj = A->count_non_zero_rows(true);
    if(rank < rankAdj){
	    //there is no solution
        return false;
    }

    int nullity = A->get_cols() - 1 - rank;
    find_solution(matrix, nullity, particular, homogen);
    return true;
}
