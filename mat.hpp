#pragma once
#include <iosfwd>

#define EPSILON .00001

//Represents the matrix row
class mat_row
{
    public:
        mat_row(int col);
	    void add(const mat_row& r);
        void set(size_t index, double d);
	    double& operator[](int index) const; 
        size_t length() const;
        ~mat_row();

    private:
        size_t cols = 0;
        double* row = nullptr;
};

class mat
{
    public:
        mat(int rows, int cols, bool aug=true);
	    mat_row* operator[](int index);
        size_t get_rows() const;
        size_t get_cols() const;
        void swap(int i, int j);
	    size_t count_non_zero_rows(bool adj);
        bool is_aug();
        ~mat();
        
    private:
        size_t rows = 0;
        size_t cols = 0;
        //is matrix augmented or not
        bool aug = true;
        mat_row** mat_rows = nullptr;
};

mat_row operator * (const mat_row& r1, double d);

std::ostream& operator<<(std::ostream& out, mat_row const& row);
std::ostream& operator<<(std::ostream& out, mat& mat);

/**
 * Checks if number can be consider as zero. See EPSILON defined upper. 
 * @param d Number to check
 * @return Returns true if number is almost zero otherwise false
*/
bool is_zero(double d);
