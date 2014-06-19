/*
 ============================================================================
 Name        : test.c
 Author      : Julian Fietkau
 Copyright   : Julian Fietkau
 Description : a small test-case 4 developing
 ============================================================================
 */

#include <stdio.h>
#include <string.h>
#include "matrix.h"
#include "help.h"
#include "matrixMult.h"

/*
 * a small test-case 4 developing
 */
int test() {
	printf("RUNNING Matrix MULTIPLICATION TESTCASE: ");
	printf(" CUDA version \n");

	char m1string[] = "[1,2;3,4;5,6]";
	char m2string[] = "[1,2,3;4,5,6]";
	char identityMatrix3[] = "[1,0,0;0,1,0;0,0,1]";
	char m1m2string[] = "[9,12,15;19,26,33;29,40,51]";	// m1 * m2
	char m2m1string[] = "[22,28;49,64]";					// m2 * m1

	Matrix Matrix1;
	Matrix Matrix2;
	Matrix m1m2;
	Matrix m2m1;
	Matrix identity3;
	Matrix identityBig;

	parseMatrix(&Matrix1, m1string);
	parseMatrix(&Matrix2, m2string);
	parseMatrix(&m1m2, m1m2string);
	parseMatrix(&m2m1, m2m1string);
	parseMatrix(&identity3, identityMatrix3);

	Matrix result;

	/*
	 * CASE 1	A*B
	 */
	printf("Testing A (3x2) * B (2x3) = C (3x3) :");
	runMultiplication(Matrix1, Matrix2, result);
	if (!equals(&result, &m1m2)) {
		printf(" ... calculated and pre-calculatet results are different!\n");
	}
	else{printf(" ... ok \n");}
	// printMatrix(&result);

	/*
	 * CASE 2	B*A
	 */
	printf("Testing B (2x3) * A (3x2) = C (2x2) :");
	runMultiplication(Matrix2, Matrix1, result);
	if (!equals(&result, &m2m1)) {
		perror("calculated and pre-calculatet results are different!");
	}
	else{printf(" ... ok \n");}
	//printMatrix(&result);

	/*
	 * CASE 3	E*E
	 */
	printf("Testing E (2x3) * E (3x2) = C (2x2) :");
	runMultiplication(identity3, identity3, result);
	if (!equals(&result, &identity3)) {
		perror("calculated and pre-calculatet results are different!");
	}
	else{printf(" ... ok \n");}
	//printMatrix(&result);

	int big = 100;
	createIdentityMatrix(&identityBig, big);
	/*
	 * CASE 4	E.big*E.big
	 */
	printf("Testing E (%dx%d) * E (%dx%d) = C (%dx%d) :",big,big,big,big,big,big);
		runMultiplication(identityBig, identityBig, result);
	if (!equals(&result, &identityBig)) {
		perror("calculated and pre-calculatet results are different!");
	}
	else{printf(" ... ok \n");}
	//printMatrix(&result);
	
	
	destroyMatrix(&Matrix1);
	destroyMatrix(&Matrix2);
	destroyMatrix(&m1m2);
	destroyMatrix(&identity3);
	destroyMatrix(&identityBig);

	return 0;
}


/*
 * greater testcase 4 benchmark calculation power
 * 	n : 1 for sequential
 * 	n : 2 for p_threads implementation
 * 	n : 3 for opemMP
 * 	n : 4 for ...
 */
void bench(Matrix m1) {
	Matrix result;

	printf("Benchmarking sequential program with n = %d ",m1.rows);
	run_timer();
	runMultiplication(m1,m1,result);
	double last_measured_time = stop_timer();
	
	printf("-> %.3f seconds for benchmark\n", last_measured_time);
	printf("\n");
	destroyMatrix(&result);
}
