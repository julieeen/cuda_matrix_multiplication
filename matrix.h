/*
 ============================================================================
 Name        : Matrix.h
 Author      : Julian Fietkau
 Copyright   : Julian Fietkau
 ============================================================================
 */
/*
 * Internal representation of used matrices.
 * We have an array of values and calculate every getValue() and setValue() individually.
 * Matrix is uniquely defined by values, rows and columns
 */
typedef struct Matrix {
	float * values;
	int rows;
	int columns;
}	Matrix;

int equals(Matrix * m1, Matrix * m2);

float getValue(Matrix * Matrix, int x, int y);

int setValue(Matrix * Matrix, float value, int x, int y);

float * getRow( Matrix * Matrix, int row);
float * getColumn( Matrix * Matrix, int column);

void createMatrix(Matrix * m1, int rows, int column);

int destroyMatrix(Matrix * m1);

void printMatrix(Matrix * m1);

void printMatrixRow(Matrix * m1);

int multiplication(Matrix * result, Matrix * m1, Matrix * m2);

int parseMatrix(Matrix * returnMatrix, char * stringData);

int createIdentityMatrix(Matrix * returnMatrix, int n);

int createRandomMatrix(Matrix * returnMatrix, int n, int m);