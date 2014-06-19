/*
 ============================================================================
 Name        : matrixMult.cu
 Author      : Julian Fietkau
 Copyright   : Julian Fietkau

 Implementation of matrix multiplication with CUDA
 BUILD & TESTED ONLY ON OSX 10.9 MAVERICKS with NVIDIA GTX 760 (Hackintosh)
 --> Install nvcc
  * Download NVIDA DEV TOOLKIT
  * export nvcc filepath:
      export PATH=/Developer/NVIDIA/CUDA-5.5/bin:$PATH
      export DYLD_LIBRARY_PATH=/Developer/NVIDIA/CUDA-5.5/lib:$DYLD_LIBRARY_PATH
  * after cd, running 'make run clean' on terminal
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "main.h"
#include "help.h"
#include "test.h"

// max. Threadsblockgroeße, depends on your GPU 
const int BLOCK_SIZE = 32; 

// Kernel function, multiplication, ! GPU CODE
__global__ void matMulKernel(const Matrix a, const Matrix b, Matrix c)
{
  float cvalue = 0.0f; // Zwischensumme
  // Threadblock * 32 + Subthread im Threadblock
  int row = (blockIdx.y * blockDim.y) + threadIdx.y; // Zeilenindex
  int col = (blockIdx.x * blockDim.x) + threadIdx.x; // Spaltenindex
 
  // only calulate if current thread is in place of C
  // waste some power, but is very easy solution for max BLOCK_SIZE problems
  if((row < c.rows) && (col < c.columns)){
    for (int e = 0; e < a.columns; ++e){
      cvalue += a.values[(row * a.columns) + e] * b.values[(e * b.columns) + col];
    }
    c.values[(row * c.columns) + col] = cvalue;
  }
}

// prepare GPU for computaion
void runMultiplication(const Matrix &m1, const Matrix &m2, Matrix &m3){
  
  int size;
  
  // TODO Check if multiplication is possible
  createMatrix(&m3, m1.rows, m2.columns);

  Matrix A; // represents matrix copy on device
    A.rows = m1.rows;
    A.columns = m1.columns;
    // calc size of matrix 1
    size = m1.rows * m1.columns * sizeof(float);
    // allocate memory for values
    cudaMalloc((void**) &A.values, size);
    // copy given data into memory
    cudaMemcpy( A.values, m1.values, size, cudaMemcpyHostToDevice);

  Matrix B; // represents matrix copy on device
    B.rows = m2.rows;
    B.columns = m2.columns;
    // calc size of matrix 2
    size = m2.rows * m2.columns * sizeof(float);
    cudaMalloc((void**) &B.values, size); // allocate mem for values
    // copy given data into memory
    cudaMemcpy( B.values, m2.values, size, cudaMemcpyHostToDevice);
     
  Matrix C; // Device-Matrix C (ErgebnisMatrix)
    C.rows = m1.rows;
    C.columns = m2.columns;
    // calc size of result matrix
    size = m3.rows * m3.columns * sizeof(float);
    // allocate mem for result values
    cudaMalloc((void**) &C.values, size);
  
  // can only multiply multiples from BLOCK_SIZE, depends on your GPU
  // dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
  dim3 dimBlock( BLOCK_SIZE, BLOCK_SIZE);

  // amount of threadblocks we need (Zeilen der Matrix / verfuegbare Zeilen pro Threadblock)
  // zB. 100 x 100 Matrix --> 100 / 32 = 4 Threadbloecke noetig
  // +1 to round up every time ( 5 / 32 = 0, but we need 1 )
  dim3 dimGrid( (m3.rows / dimBlock.x) +1, (m3.columns / dimBlock.y) +1);

  // printf("%d / %d -> GridSpalten = %d\n", m3.rows, dimBlock.x, dimGrid.x );

  // ohne Shared Memory, Kernel starten
  matMulKernel<<<dimGrid, dimBlock>>>(A, B, C);
    
  // copy calculated result from device to host
  cudaMemcpy( m3.values, C.values, size, cudaMemcpyDeviceToHost);

  // free allocated mem
  cudaFree(A.values);
  cudaFree(B.values);
  cudaFree(C.values);
}

int main()  {
  test();

  Matrix Matrix1;
  Matrix Matrix2;
  Matrix Matrix3;
  Matrix Matrix4;

  createIdentityMatrix(&Matrix1, 500);
  bench(Matrix1);
  destroyMatrix(&Matrix1);

  createIdentityMatrix(&Matrix2, 750);
  bench(Matrix2);
  destroyMatrix(&Matrix2);


  createIdentityMatrix(&Matrix3, 2000);
  bench(Matrix3);
  destroyMatrix(&Matrix3);


  // 10.000 * 10.000 * 4 ... / 1024 / 1024 => 380 MB 
  createIdentityMatrix(&Matrix4, 10000);
  bench(Matrix4);
  destroyMatrix(&Matrix4);
  
  // root( (8GB * 1024^3) / 4 ) = 46340
  // Testsystem with 8 GB RAM, matrix > 46340 can't allocate 
  // ! wrong computation i guess, but no error
  // Matrix Matrix5;
  // createIdentityMatrix(&Matrix5, 46340); 
  // bench(Matrix5);
  // destroyMatrix(&Matrix5);


}

