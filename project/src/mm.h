#ifndef _COMMON_H_
#define _COMMON_H_

/*
 * Commen definitions
 *
*/

#define MATRIX_SIZE 256
#define PROCESSOR_NUM 8

// macro
#define TRUE 1
#define FALSE 0

#define SAFE_FREE(addr)\
  if(NULL != addr){\
    free(addr);\
    addr = NULL;\
  }

//global varialbes and definitions
typedef struct _matrix{
  double * data;
  int xDim;
  int yDim;
}matrix;

// function declaration
void init_matrix(matrix * mat, int xDim, int yDim, int random);
void omp_mat_mul_baseline(matrix matrixA, matrix matrixB, matrix matrixC);
void omp_mat_mul(matrix matrixA, matrix matrixB, matrix matrixC);
void serial_mat_mul(matrix matrixA, matrix matrixB, matrix matrixC);
double get_duration(struct timeval __start);
void validate_result(matrix matrixA, matrix matrixB);

#endif
