#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <omp.h>
#include <papi.h>
#include "mm.h"

extern ftype init_array[4];

/*
 * use transpose, SSE and pipeline( Unrolling + Scheduling )
 * L2 cache: 4096 KB per CPU, 8192 KB total
 * L1 cache: 32 KB per core
 * Intel64: 128 bit SIDM instructions referencing XMM register can access 16
 * XMM registers in 64-bit model
 */
void omp_mat_mul_sse_ppl_asm(matrix matrixA, matrix matrixB, matrix matrixC){
  int dim = matrixA.xDim;
  int thread_id = 0;
  int num_per_thread = dim / PROCESSOR_NUM;
   
  matrix matrixBT;
  init_matrix(&matrixBT, matrixB.xDim, matrixB.yDim, FALSE);

  int cycleI = 0;
  int cycleJ = 0;

  for(cycleI = 0; cycleI < matrixB.yDim; ++ cycleI){
    for(cycleJ = 0; cycleJ < matrixB.xDim; ++ cycleJ){
      matrixBT.data[cycleJ * matrixB.xDim + cycleI] =
        matrixB.data[cycleI * matrixB.xDim + cycleJ];
    }
  }

  omp_set_num_threads(PROCESSOR_NUM);
  //calculate the sum in parallel
  #pragma omp parallel shared (matrixA, matrixBT, matrixC, num_per_thread)\
    private (thread_id)
  {
    thread_id = omp_get_thread_num();
    int start = num_per_thread * thread_id;
    int end = start + num_per_thread;
    //printf("TID: %d, %d - %d\n", thread_id, start, end);
  
    int cycleI = 0;
    int cycleJ = 0;
    int cycleK = 0;

    v4sf acc_0;
    v4sf acc_1;
    v4sf acc_2;
    v4sf acc_3;
    
    v4sf oprand_a;

    v4sf oprand_b_0;
    v4sf oprand_b_1;
    v4sf oprand_b_2;
    v4sf oprand_b_3;

    ftype imd_ret_0[SSE_LENGTH];
    ftype imd_ret_1[SSE_LENGTH];
    ftype imd_ret_2[SSE_LENGTH];
    ftype imd_ret_3[SSE_LENGTH];

    for(cycleI = start; cycleI < end; ++ cycleI){
      for(cycleJ = 0; cycleJ < matrixB.xDim - 3; cycleJ += 4){
        // set acc to 0
        acc_0 = __builtin_ia32_loadups(init_array);
        acc_1 = __builtin_ia32_loadups(init_array);
        acc_2 = __builtin_ia32_loadups(init_array);
        acc_3 = __builtin_ia32_loadups(init_array);
        
        for(cycleK = 0; cycleK < (dim - SSE_LENGTH + 1); cycleK += SSE_LENGTH){
          oprand_a = __builtin_ia32_loadups(
            &(matrixA.data[cycleI * dim + cycleK]));

          oprand_b_0 = __builtin_ia32_loadups(
            &(matrixBT.data[cycleJ * dim + cycleK]));
          oprand_b_1 = __builtin_ia32_loadups(
            &(matrixBT.data[(cycleJ + 1) * dim + cycleK]));
          oprand_b_2 = __builtin_ia32_loadups(
            &(matrixBT.data[(cycleJ + 2) * dim + cycleK]));
          oprand_b_3 = __builtin_ia32_loadups(
            &(matrixBT.data[(cycleJ + 3) * dim + cycleK]));
          
          acc_0 = __builtin_ia32_addps(acc_0, __builtin_ia32_mulps(oprand_a, oprand_b_0));
          acc_1 = __builtin_ia32_addps(acc_1, __builtin_ia32_mulps(oprand_a, oprand_b_1));
          acc_2 = __builtin_ia32_addps(acc_2, __builtin_ia32_mulps(oprand_a, oprand_b_2));
          acc_3 = __builtin_ia32_addps(acc_3, __builtin_ia32_mulps(oprand_a, oprand_b_3));

        }
        __builtin_ia32_storeups(imd_ret_0, acc_0);
        __builtin_ia32_storeups(imd_ret_1, acc_1);
        __builtin_ia32_storeups(imd_ret_2, acc_2);
        __builtin_ia32_storeups(imd_ret_3, acc_3);

        for(cycleK = 1; cycleK < SSE_LENGTH; ++ cycleK){
          imd_ret_0[0] += imd_ret_0[cycleK];
          imd_ret_1[0] += imd_ret_1[cycleK];
          imd_ret_2[0] += imd_ret_2[cycleK];
          imd_ret_3[0] += imd_ret_3[cycleK];
        }
        matrixC.data[cycleI * dim + cycleJ] = imd_ret_0[0];
        matrixC.data[cycleI * dim + cycleJ + 1] = imd_ret_1[0];
        matrixC.data[cycleI * dim + cycleJ + 2] = imd_ret_2[0];
        matrixC.data[cycleI * dim + cycleJ + 3] = imd_ret_3[0];
      }
    }
  }
  
  SAFE_FREE(matrixBT.data);
}
