#ifndef _COMMON_H_
#define _COMMON_H_

/*
 * Commen definitions
 *
*/

#define MATRIX_SIZE 512
#define PROCESSOR_NUM 32
#define SERIAL_TIME 19.0

// macro
#define TRUE 1
#define FALSE 0

#define SAFE_FREE(addr)\
  if(NULL != addr){\
    free(addr);\
    addr = NULL;\
  }

typedef float ftype;

// global varialbes and definitions
typedef struct _matrix{
  ftype * data;
  int xDim;
  int yDim;
}matrix;

// SSE
typedef float v4sf __attribute__ ((vector_size(16)));
#define SSE_LENGTH 4

#define CORRECT_THRESHOLD 0.0001

// function declaration
void init_matrix(matrix * mat, int xDim, int yDim, int random);

void omp_mat_mul_baseline(matrix matrixA, matrix matrixB, matrix matrixC);
void omp_mat_mul_div(matrix matrixA, matrix matrixB, matrix matrixC);
void omp_mat_mul_transpose(matrix matrixA, matrix matrixB, matrix matrixC);
void omp_mat_mul_sse(matrix matrixA, matrix matrixB, matrix matrixC);
void omp_mat_mul_sse_ppl(matrix matrixA, matrix matrixB, matrix matrixC);
void omp_mat_mul_sse_ppl2(matrix matrixA, matrix matrixB, matrix matrixC);
void omp_mat_mul_sse_ppl_asm(matrix matrixA, matrix matrixB, matrix matrixC);

void serial_mat_mul(matrix matrixA, matrix matrixB, matrix matrixC);

double get_duration(struct timeval __start);
void validate_result(matrix matrixA, matrix matrixB);
void report_result();

#endif
