#pragma once

#include "mat.hpp"

/**
 * Solve lnear system of equations
 
 * @param matrix 	Pointer to augmented matrix of system
 * @particular 		Pointer where particular solution will be stored
 * @param homogen 	Pointer where homogeneous solution will be stored
 *                	if homogenouse solution is null, do nothig
 * @param verbose	If true, function will print triangular matrix
 * @param mulithread	If true, mulithreading is enable
 * @return		Returns true if solution exists

*/
bool solve(mat** matrix, mat** particular, mat** homogen, bool verbose = false, bool mulithread=false);
