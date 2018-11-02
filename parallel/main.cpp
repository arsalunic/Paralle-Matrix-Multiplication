#include <iostream> 
#include <cstdlib>
#include <fstream>
#include <climits>
#include "hwtimer.h"
#include <cilk/cilk.h>
#include "MatrixMultiplication.h"

using namespace std;


/*
 *MultA : this method does matrix multiplication on z-order curved matrices
 */
void MultA(int* c, int* a, int* b, unsigned int n){
	
	//base case
	if(n < 4){
		
		// Multiplying the matrix elements
		*c += *a * *b;
	
	}else{
		
		// z-curve matrix representation
		cilk_spawn MultA(c, a, b, n / 4);
		cilk_spawn MultA(c + (n / 4), a, b + (n / 4), n / 4);
		cilk_spawn MultA(c + ((n / 4) * 3), a + ((n / 4) * 2), b + (n / 4), n / 4);
		cilk_spawn MultA(c + ((n / 4) * 2), a + ((n / 4) * 2), b, n / 4);
		
		cilk_sync;
		
		cilk_spawn MultA(c + ((n / 4) * 2), a + ((n / 4) * 3), b + ((n / 4) * 2), n / 4);
		cilk_spawn MultA(c + ((n / 4) * 3), a + ((n / 4) * 3), b + ((n / 4) * 3), n / 4);
		cilk_spawn MultA(c + (n / 4), a + (n / 4), b + ((n / 4) * 3), n / 4);
		cilk_spawn MultA(c, a + (n / 4), b + ((n / 4) * 2), n / 4);
		
		cilk_sync;
	}
	
}

//printMatrix 
void printMatrix(int** matrix, int size, const string& outputFilename){
	
	//output file stream
	ofstream outFile;
	outFile.open(outputFilename.c_str(), std::fstream::app);
	
	if(!outFile){	
		cout << "Fatal:> Output file error";
		exit(1);
	
	}
	
	// printing the matrix to an output file
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			outFile << matrix[i][j];
			
			if(j != size - 1)
				outFile << " ";
		}
		
		outFile << endl;
	}
	
	outFile << endl << "\n*************************\n" << endl;	
	outFile.close();
}

int main(int argc, char* argv[]){
	
	if (argc != 3) {
		cout << "Please check your command, and enter the right amount of arguments!! " << endl;
		return 1;
	}
	
	hwtimer_t timer;
	initTimer(&timer);
	
	int parameter = atoi(argv[1]);
	
	// Creating an empty file
	ofstream outFile;
	outFile.open(argv[2], std::ofstream::out | std::ofstream::trunc);
	outFile.close();
	
	// Creating matrix A B and C
	MatrixMultiplication a(parameter, false, false);
	a.updatingMatrixZOrder();

	MatrixMultiplication b(parameter, false, true);
	b.updatingMatrixZOrder();
		
	MatrixMultiplication c(parameter, true, false);
	c.updatingMatrixZOrder();
	
	//starting the timer
	startTimer(&timer);
	
	// Performimg matrix multiplication
	MultA(	c.getMatrixZOrder(), 
			a.getMatrixZOrder(), 
			b.getMatrixZOrder(), 
			parameter * parameter
		);
	
	//stopping the timer	
	stopTimer(&timer);
	
	//displaying the runtime 	
	int matrixMultTime = getTimerNs(&timer);
	
	cout << "Total time: " << matrixMultTime << "ns" << endl;
	
	//converting the result matrix to a regular square matrix
	c.updatingMatrixRow();	
		
	//as required by the assignment, printing matrices a, b and c seperately to an output file
	printMatrix(a.getMatrixRow(), parameter, argv[2]);
	printMatrix(b.getMatrixRow(), parameter, argv[2]);
	printMatrix(c.getMatrixRow(), parameter, argv[2]);
		
	return 0;
};
