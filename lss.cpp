#include <iostream>
#include <iomanip>
#include <string>
#include "mat.hpp"
#include "gaussian_elimination.hpp"
#include <chrono>
#include <fstream> 

#define RET_OK 0
#define FAILTURE_INVALID_ARG 1
#define FAILTURE_INVALID_INPUT 2
#define FAILTURE_NO_SOLUTION 3
#define FAILTURE_COULD_NOT_OPEN_INFILE 4

template <typename TimePoint>
std::chrono::milliseconds to_ms(TimePoint tp) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(tp);
}

void printHelp(){
    std::cout << "Usage: lss [options]" << std::endl;
    std::cout << "Program can read matrix from standard input or from file (using -f)" << std::endl;
    std::cout << "Options" << std::endl;
    std::cout << std::setw(18) << std::left 
        << " --help" << "Display help" << std::endl
              << std::setw(18) << std::left
        << " -f [file_path]" << "Path to file containnig extended matrix" << std::endl
              << std::setw(18) << std::left
        << " --multithread" << "Use multithreading" << std::endl
              << std::setw(18) << std::left
        << " --verbose" << "Print loaded and triangular matrix" << std::endl;
}

void processArguments(int argc, char** argv, bool* help, bool* multyThread, bool* verbose,  std::ifstream* input){
    for(int i = 1; i < argc; i++){
        if(std::string(argv[i]) == "--help"){
            (*help) = true;
            return;
        }else if(std::string(argv[i]) == "--multithread"){
            (*multyThread) = true;
        }else if(std::string(argv[i]) == "--verbose"){
            (*verbose) = true;
        }else if(std::string(argv[i]) == "-f"){
            if(i == argc - 1){
                std::cerr << "Invalid argument syntax, must be called like this : .\\lss -f [filename]" << std::endl;
                std::cerr << " see --help for more info." << std::endl;
                exit(FAILTURE_INVALID_ARG);
            }
            i++;
            std::string file_address = std::string(argv[i]);
            
            input->open(argv[i]);
            if (!*input){
                std::cerr << "Invalid argument: Cannot open file \'" << argv[i] << "\'" << std::endl;
                exit(FAILTURE_COULD_NOT_OPEN_INFILE);
            }
            std::cin.rdbuf(input->rdbuf());
            
        }else{
            std::cerr << "Invalid argument \'" << argv[i] << "\'!" << std::endl;
            std::cerr << " see --help for more info." << std::endl;
            exit(FAILTURE_INVALID_ARG);
            return;
        }
    }
}

void readMatrixDimension(bool prompt, int* rows, int* cols){
    if(prompt){
            std::cout << "Type number of rows: ";
        }
        std::cin >> *rows;
        if(!std::cin.good() || *rows <= 0){
            std::cerr << "Invalid input: Number of rows must be an integer greater than 0" << std::endl;
            exit(FAILTURE_INVALID_INPUT);
        }    
        if(prompt){
            std::cout << "Type number of columns: ";
        }
        std::cin >> *cols;
        if(!std::cin.good() || *cols <= 0){
            std::cerr << "Invalid input: Number of cols must be an integer greater than 0" << std::endl;
            exit(FAILTURE_INVALID_INPUT);
        } 
}


void readMatrix(bool prompt, mat** mat_out1, bool verbose=true){
    int rows, cols;
    
    readMatrixDimension(prompt, &rows, &cols);
    mat* matrix = new mat(rows, cols);
    (*mat_out1) = matrix;
    for(int j = 0; j < rows; j++){
        mat_row* row = (*matrix)[j];
        for(int i = 0; i < cols; i++){
            std::cin >> (*row)[i];
            if(!std::cin.good()){
std::cout << j << ", " << i << "\n";
                std::cerr << "Invalid input: input must be double" << std::endl;
                exit(FAILTURE_INVALID_INPUT);      
            }
        }
    }
    if(verbose){
        std::cout << "Mat loaded rows " << rows << " cols " << cols << std::endl;
        std::cout << *matrix << std::endl;
    }
}


int main(int argc, char** argv){
    bool pHelp = false;
    bool multi_thread = false;
    bool verbose = false;
    int retCode = RET_OK;
    std::ifstream input_file_stream;

    auto start = std::chrono::high_resolution_clock::now();

    processArguments(argc, argv, &pHelp, &multi_thread, &verbose, &input_file_stream);

    if(pHelp){
	    printHelp();
	    return retCode;
    }
    //load matrix
    mat* matrix = nullptr;
    readMatrix(verbose, &matrix, verbose);
    
    mat* particular = nullptr;
    mat* homogen = nullptr;
    bool res = solve(&matrix, &particular, &homogen, verbose, multi_thread);
    if(!res){
        std::cerr << "System has no solution\n";
        retCode = FAILTURE_NO_SOLUTION;
    }else{
        std::cout << "Particular solution: " << std::endl;
        std::cout << *particular << std::endl;
        if(homogen != nullptr){  
            std::cout << "Homogeneous solution: " << std::endl;
            std::cout << *homogen << std::endl;
            delete homogen;
        }     
        delete particular;
    }
    delete matrix;

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Needed " << to_ms(end - start).count() << " ms to finish.\n";
	return retCode;
}

