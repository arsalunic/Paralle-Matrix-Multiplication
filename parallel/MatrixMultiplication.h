#ifndef MATRIX_MULTIPLICATION_H
#define MATRIX_MULTIPLICATION_H

#include <iostream>
#include <string>
#include <climits>
#include <cstdlib>
#include <cilk/cilk.h>

class MatrixMultiplication{
	
	public:
		MatrixMultiplication(int, bool, bool);
		~MatrixMultiplication();
		
		void updatingMatrixRow();
		void updatingMatrixZOrder();

		int** getMatrixRow();
		void printMatrixRow();
		int* getMatrixZOrder();
		void printMatrixZOrder();

	private:
		void convertToRowMatrix(unsigned int, unsigned int);
		void convertToZorder(unsigned int, unsigned int, unsigned int);
				
		int** matrixRow;
		int matrixRowSize;
		int* matrixZOrder; 
		int matrixZOrderSize;	
		

		
};

#endif
