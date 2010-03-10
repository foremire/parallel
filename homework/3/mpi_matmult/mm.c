#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <mpi.h>

//global varialbes and definitions
#define OUTPUT_THRESHOLD 7
#define BUFF_SIZE 16
#define COMM_TAG 16314

#define COMM_TAG_A 18000
#define COMM_TAG_A_2 18001
#define COMM_TAG_B 18002
#define COMM_TAG_B_2 18003
#define COMM_TAG_C 18004
#define COMM_TAG_C_2 18005

#define SAFE_FREE(addr)\
  if(NULL != addr){\
    free(addr);\
    addr = NULL;\
  }

typedef struct _matrix{
  double * data;
  int xDim;
  int yDim;
  int start;
}matrix;

char * usage = "Usage: mm N\n";
char * matrix_size_error = "Invalid matrix size N = %d. It must be greater than 1.\n";
char * matrix_size_info = "Matrix Size N = %d\n";
char * malloc_error = "Malloc ERROR.\n";

char * msg = "Hi";
char * msg_template = "Process number %d writes number %d\n";
char * msg_no_task = "Process %d exit without no task.\n";

void init_matrix(matrix * mat, int xDim, int yDim, int start, int random);
void matrix_mul(matrix matrixA, matrix matrixB, matrix matrixC);
void safe_exit(matrix ma, matrix mb, matrix mc, matrix me);
void print_matrix(matrix * mat);
void ring_print(matrix * mat, char * name, int myrank, int numprocesses);
double get_duration(struct timeval __start);
void validate_result(matrix * matrixA, matrix * matrixB, matrix * matrixC, 
    int myrank, int numprocesses, int matrixSize);

int main( int argc, char *argv[] )
{
  int matrixSize = 0;

  matrix matrixA;
  matrix matrixB;
  matrix matrixC;
  matrix matrixExchange;
  matrix matrixTmp;

  struct timeval __start;
  double Ti = 0.0f;
  double Tc = 0.0f;
  double Tt = 0.0f;

  int cycleI = 0;

  int remainder = 0;
  int num_per_process = 0;
  int range_start = 0;
  int range_end = 0;
  int range_len = 0;

  int myrank = 0;
  int numprocesses = 0;

  if(argc < 2){
    puts(usage);
    exit(-1);
  }
  
  // Init MPI
  MPI_Init( &argc, &argv );
                
  // Get num processes
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

  // Get num of processes
  MPI_Comm_size(MPI_COMM_WORLD, &numprocesses);

  // get the matrix from the command line options list
  matrixSize = atoi(argv[1]);
  if(1 > matrixSize){
    printf(matrix_size_error, matrixSize);
    MPI_Finalize();
    exit(-1);
  }
  if(0 == myrank){
    printf(matrix_size_info, matrixSize);
  }
  
  // Make sure the data is evenly distributed between the processes
  num_per_process = matrixSize / numprocesses;
  remainder = matrixSize % numprocesses;
  if(0 == remainder){
    range_start = myrank * num_per_process;
  }else{
    range_start = myrank * (num_per_process + 1);
    range_start = (myrank > remainder) ? range_start - myrank + remainder : range_start;
  }
  range_end = (myrank > remainder - 1) ? range_start + num_per_process
    : range_start + num_per_process + 1;
  range_len = range_end - range_start;

  //printf("Rank: %d\t Start: %d\t End: %d\n", myrank, range_start, range_end);
  //MPI_Finalize();
  //exit(0);

  // These is no task for the current process, exit gracefully
  //if(0 == range_len){}

  // initilize the matrices
  gettimeofday(&__start, NULL);
  init_matrix(&matrixA, matrixSize, range_len, 0, 1);
  init_matrix(&matrixB, range_len, matrixSize, range_start, 1);
  init_matrix(&matrixC, matrixSize, range_len, 0, 0);
  init_matrix(&matrixExchange, (num_per_process + 1), matrixSize, 0, 0);
  Ti = get_duration(__start);

  // hold the address of the exchange matrix
  matrixTmp.data = matrixExchange.data;
 
  // do the matrix multiplication
  gettimeofday(&__start, NULL);
  //matrix_mul(matrixA, matrixB, matrixC);
  for(cycleI = 0; cycleI < numprocesses; ++ cycleI){
    // it's my turn, broadcast matrix B to other peers
    if(cycleI == myrank){
      matrixExchange = matrixB;
    }
    //otherwise, receive matrix Exchange from other peers

    MPI_Bcast(&matrixExchange, sizeof(matrix), MPI_CHAR, cycleI, MPI_COMM_WORLD);
    if(cycleI == myrank){
      matrixExchange.data = matrixB.data;
    }else{
      matrixExchange.data = matrixTmp.data;
    }
    MPI_Bcast(matrixExchange.data, matrixExchange.xDim * matrixSize, 
        MPI_DOUBLE, cycleI, MPI_COMM_WORLD);
    matrix_mul(matrixA, matrixExchange, matrixC);

    /*
    printf("cycle: %d, myrank %d\n", cycleI, myrank);
    printf("matrixA xDim: %d, yDim: %d, start: %d\n", 
        matrixA.xDim, matrixA.yDim, matrixA.start);
    printf("matrixB xDim: %d, yDim: %d, start: %d\n", 
        matrixExchange.xDim, matrixExchange.yDim, matrixExchange.start);
    printf("MatrixA:\n");
    print_matrix(&matrixA);
    printf("MatrixB:\n");
    print_matrix(&matrixB);
    printf("MatrixExchange:\n");
    print_matrix(&matrixExchange);
    printf("After Mul, MatrixC:\n");
    print_matrix(&matrixC);
    fflush(stdout);
    usleep(100000);
    */
  }

  matrixExchange.data = matrixTmp.data;
  Tc = get_duration(__start);
  
  // if the matrix size is below the threshold, output the result
  if(matrixSize < OUTPUT_THRESHOLD){
    //ring_print(&matrixA, "matrixA", myrank, numprocesses);
    //ring_print(&matrixB, "matrixB", myrank, numprocesses);
    ring_print(&matrixC, "matrixC", myrank, numprocesses);
  }

  matrixA.start = range_start;
  matrixB.start = range_start;
  matrixC.start = range_start;
  validate_result(&matrixA, &matrixB, &matrixC, 
    myrank, numprocesses, matrixSize);

  Tt = Ti + Tc;
  //printf("Ti: %fs\n", Ti);
  //printf("Tc: %fs\n", Tc);
  //printf("Tt: %fs\n", Tt);
  
  safe_exit(matrixA, matrixB, matrixC, matrixExchange);
  return 0;
}

void init_matrix(matrix * mat, int xDim, int yDim, int start, int random){
  struct timeval tv;
  int cycleI = 0;
  int cycleJ = 0;
  int size = 0;

  size = xDim * yDim;

  mat->data = malloc(size * sizeof(double));
  if(NULL == mat->data){
    puts(malloc_error);
    exit(-1);
  }
  memset(mat->data, 0, size * sizeof(double));
  mat->xDim = xDim;
  mat->yDim = yDim;
  mat->start = start;

  //initilize random generator
  gettimeofday(&tv, NULL);
  srand(tv.tv_sec * tv.tv_usec);

  if(random){
    // initilize the matrix by random numbers between -1.00f and 1.00f
    for(cycleI = 0; cycleI < mat->yDim; ++ cycleI){
      for(cycleJ = 0; cycleJ < mat->xDim; ++ cycleJ){
        mat->data[cycleI * mat->xDim + cycleJ] = 
          (double)rand() / ((double)(RAND_MAX)+ 1.00) * 2.0 - 1.0;
        //mat->data[cycleI * mat->xDim + cycleJ] = cycleJ + start;
        //mat->data[cycleI * mat->xDim + cycleJ] = 1.0;
      }
    }
  }
}

void matrix_combine(matrix * sub, matrix * ref, int horizon){
  int cycleI = 0;
  int cycleJ = 0;
  int start = 0;

  start = ref->start;
  if(horizon){
    if(sub->xDim != ref->xDim){
      printf("matrix sub and matrix ref's dimension do not match\n");
      printf("sub xDim: %d, yDim: %d, start: %d\n", sub->xDim, sub->yDim, sub->start);
      printf("ref xDim: %d, yDim: %d, start: %d\n", ref->xDim, ref->yDim, ref->start);
      return;
    }
    if(sub->yDim + ref->start > ref->yDim){
      printf("matrix ref is full and can not combine with sub matrix\n");
      return;
    }
    for(cycleI = 0; cycleI < sub->yDim; ++ cycleI){
      for(cycleJ = 0; cycleJ < sub->xDim; ++ cycleJ){
        ref->data[(start + cycleI) * ref->xDim + cycleJ] = 
          sub->data[cycleI * sub->xDim + cycleJ];
      }
    }
    ref->start += sub->yDim;
  }else{
    if(sub->yDim != ref->yDim){
      printf("matrix sub and matrix ref's dimension do not match\n");
      printf("sub xDim: %d, yDim: %d, start: %d\n", sub->xDim, sub->yDim, sub->start);
      printf("ref xDim: %d, yDim: %d, start: %d\n", ref->xDim, ref->yDim, ref->start);
      return;
    }
    if(sub->xDim + ref->start > ref->xDim){
      printf("matrix ref is full and can not combine with sub matrix\n");
      return;
    }

    for(cycleI = 0; cycleI < sub->yDim; ++ cycleI){
      for(cycleJ = 0; cycleJ < sub->xDim; ++ cycleJ){
        ref->data[cycleI * ref->xDim + cycleJ + start] = 
          sub->data[cycleI * sub->xDim + cycleJ];
      }
    }
    ref->start += sub->xDim;
  }
}

void matrix_mul(matrix matrixA, matrix matrixB, matrix matrixC){
  int cycleI = 0;
  int cycleJ = 0;
  int cycleK = 0;
  int dim = 0;

  // check whether the dimension of the three matrices match 
  // with each other to conduct the following multiplication
  if(matrixA.xDim != matrixB.yDim){
    printf("matrix A and matrix B's dimension do not match\n");
    return;
  }
  dim = matrixA.xDim;

  if((matrixA.xDim != matrixC.xDim) || (matrixA.yDim != matrixC.yDim)){
    printf("matrix A and matrix C's dimension do not match\n");
    return;
  }

  for(cycleI = 0; cycleI < matrixA.yDim; ++ cycleI){
    for(cycleJ = 0; cycleJ < matrixB.xDim; ++ cycleJ){
      
      matrixC.data[cycleI * dim + cycleJ + matrixB.start] = 0.0;

      for(cycleK = 0; cycleK < dim; ++ cycleK){
        matrixC.data[cycleI * dim + cycleJ + matrixB.start] += 
          matrixA.data[cycleI * matrixA.xDim + cycleK] *
          matrixB.data[cycleJ + cycleK * matrixB.xDim];
      }
    }
  }
}

void safe_exit(matrix ma, matrix mb, matrix mc, matrix me){
  SAFE_FREE(ma.data);
  SAFE_FREE(mb.data);
  SAFE_FREE(mc.data);
  SAFE_FREE(me.data);

  // Finalize MPI
  MPI_Finalize();
}

void print_matrix(matrix * mat){
  int cycleI = 0;
  int cycleJ = 0;
  double val = 0.0;

  for(cycleI = 0; cycleI < mat->yDim; ++ cycleI){
    for(cycleJ = 0; cycleJ < mat->xDim; ++ cycleJ){
      val = mat->data[cycleI * mat->xDim + cycleJ];
      if(val >= 0.0){
        printf("+%f\t", val);
      }else{
        printf("%f\t", val);
      }
    }
    printf("\n");
  }
  fflush(stdout);
  usleep(1000);
}

void ring_print(matrix * mat, char * name, int myrank, int numprocesses){
  int source = 0;
  int dest = 0;
  char buffer[BUFF_SIZE];
  MPI_Status status;

  // start message relay ring from process 0
  if(0 == myrank){
    source = 0;
    dest = source + 1;
    MPI_Send(msg, strlen(msg) + 1, MPI_CHAR, dest, COMM_TAG, MPI_COMM_WORLD);
    puts(name);
    print_matrix(mat);
  }else{
    // or, it's a intermediate relay
    source = myrank - 1;
    dest = myrank + 1;
    // receive the message from the previus process
    MPI_Recv(buffer, BUFF_SIZE, MPI_CHAR, source, COMM_TAG, MPI_COMM_WORLD, &status);
    print_matrix(mat);

    //relay the message to the next receiver
    if(dest < numprocesses){
      MPI_Send(msg, strlen(msg) + 1, MPI_CHAR, dest, COMM_TAG, MPI_COMM_WORLD);
    }
  }
  usleep(100000);
}

double get_duration(struct timeval __start){
  struct timeval  __end;

  double t_start = 0.0;
  double t_end = 0.0;
  double duration = 0.0;
  
  gettimeofday(&__end, NULL);
  t_start = (__start.tv_sec + (__start.tv_usec/1000000.0));
  t_end = (__end.tv_sec + (__end.tv_usec/1000000.0));
  duration= t_end - t_start;

  return duration;
}

void validate_result(matrix * matrixA, matrix * matrixB, matrix * matrixC, 
    int myrank, int numprocesses, int matrixSize){

  char buffer[BUFF_SIZE];
  MPI_Status status;
  int cycleI = 0;
  int cycleJ = 0;
  int cycleK = 0;

  int diff = 0;
  int match = 0;
  
  matrix matrixARef;
  matrix matrixBRef;
  matrix matrixCRef;
  matrix matrixCSerial;
  matrix matrixExchange;
  matrix matrixTmp;
  
  if(0 == myrank){
    // intialize reference matrix on process #0
    init_matrix(&matrixARef, matrixSize, matrixSize, 0, 0);
    init_matrix(&matrixBRef, matrixSize, matrixSize, 0, 0);
    init_matrix(&matrixCRef, matrixSize, matrixSize, 0, 0);
    init_matrix(&matrixCSerial, matrixSize, matrixSize, 0, 0);
    init_matrix(&matrixExchange, matrixSize, matrixSize, 0, 0);
  }else{
    matrixARef.data = NULL;
    matrixBRef.data = NULL;
    matrixCRef.data = NULL;
    matrixCSerial.data = NULL;
    matrixExchange.data = NULL;
    matrixTmp.data = NULL;
  }

  if(0 == myrank){
    matrixTmp = matrixExchange;

    matrix_combine(matrixA, &matrixARef, 1);
    matrix_combine(matrixB, &matrixBRef, 0);
    matrix_combine(matrixC, &matrixCRef, 1);

    // collect data from other peers
    printf("Collecting the result...\n");
    for(cycleI = 1; cycleI < numprocesses; ++ cycleI){

      MPI_Send(msg, strlen(msg) + 1, MPI_CHAR, cycleI, COMM_TAG, MPI_COMM_WORLD);

      MPI_Recv(&matrixExchange, sizeof(matrix), MPI_CHAR, cycleI, 
          COMM_TAG_A, MPI_COMM_WORLD, &status);
      matrixExchange.data = matrixTmp.data;
      MPI_Recv(matrixExchange.data,  matrixSize * matrixSize, 
          MPI_DOUBLE, cycleI,  COMM_TAG_A_2, MPI_COMM_WORLD, &status);
      matrix_combine(&matrixExchange, &matrixARef, 1);

      //printf("Recv A %d\n", cycleI);
      //fflush(stdout);
      //usleep(100000);

      MPI_Recv(&matrixExchange, sizeof(matrix), MPI_CHAR, cycleI, 
          COMM_TAG_B, MPI_COMM_WORLD, &status);
      matrixExchange.data = matrixTmp.data;
      MPI_Recv(matrixExchange.data, matrixSize * matrixSize, 
          MPI_DOUBLE, cycleI,  COMM_TAG_B_2, MPI_COMM_WORLD, &status);
      matrix_combine(&matrixExchange, &matrixBRef, 0);
      
      //printf("Recv B %d\n", cycleI);
      //print_matrix(&matrixExchange);
      //fflush(stdout);
      //usleep(100000);

      MPI_Recv(&matrixExchange, sizeof(matrix), MPI_CHAR, cycleI, 
          COMM_TAG_C, MPI_COMM_WORLD, &status);
      matrixExchange.data = matrixTmp.data;
      MPI_Recv(matrixExchange.data,  matrixSize * matrixSize, 
          MPI_DOUBLE, cycleI,  COMM_TAG_C_2, MPI_COMM_WORLD, &status);
      matrix_combine(&matrixExchange, &matrixCRef, 1);

      //printf("Recv C %d\n", cycleI);
      //fflush(stdout);
      //usleep(100000);
    }
  }else{
    // receive the message from the previus process
    MPI_Recv(buffer, BUFF_SIZE, MPI_CHAR, myrank - 1, COMM_TAG, MPI_COMM_WORLD, &status);

    // send the data to process #0
    MPI_Send(matrixA, sizeof(matrix), MPI_CHAR, 0, COMM_TAG_A, MPI_COMM_WORLD);
    MPI_Send(matrixA->data, matrixA->xDim * matrixA->yDim, 
        MPI_DOUBLE, 0, COMM_TAG_A_2, MPI_COMM_WORLD);
      
    //printf("Sent A %d\n", myrank);
    //printf("xDim: %d, yDim: %d, start: %d\n\n", 
    //    matrixA->xDim, matrixA->yDim, matrixA->start);
    //fflush(stdout);
    //usleep(100000);
    
    MPI_Send(matrixB, sizeof(matrix), MPI_CHAR, 0, COMM_TAG_B, MPI_COMM_WORLD);
    MPI_Send(matrixB->data, matrixB->xDim * matrixB->yDim, 
        MPI_DOUBLE, 0, COMM_TAG_B_2, MPI_COMM_WORLD);
    
    //printf("Sent B %d\n", myrank);
    //printf("xDim: %d, yDim: %d, start: %d\n\n", 
    //    matrixB->xDim, matrixB->yDim, matrixB->start);
    //print_matrix(matrixB);
    //fflush(stdout);
    //usleep(100000);
   
    MPI_Send(matrixC, sizeof(matrix), MPI_CHAR, 0, COMM_TAG_C, MPI_COMM_WORLD);
    MPI_Send(matrixC->data, matrixC->xDim * matrixC->yDim, 
        MPI_DOUBLE, 0, COMM_TAG_C_2, MPI_COMM_WORLD);
    
    //printf("Sent C %d\n", myrank);
    //printf("xDim: %d, yDim: %d, start: %d\n\n", 
    //    matrixC->xDim, matrixC->yDim, matrixC->start);
    //fflush(stdout);
    //usleep(100000);
    
    //relay the message to the next receiver
    if(myrank + 1< numprocesses){
      MPI_Send(msg, strlen(msg) + 1, MPI_CHAR, myrank + 1, COMM_TAG, MPI_COMM_WORLD);
    }
  }
  
  if(0 == myrank){
    // do the matrix multiplication
    for(cycleI = 0; cycleI < matrixSize; ++ cycleI){
      for(cycleJ = 0; cycleJ < matrixSize; ++ cycleJ){
        matrixCSerial.data[cycleI * matrixSize + cycleJ] = 0.0;
        for(cycleK = 0; cycleK < matrixSize; ++ cycleK){
          matrixCSerial.data[cycleI * matrixSize + cycleJ] += 
            matrixARef.data[cycleI * matrixSize + cycleK] *
            matrixBRef.data[cycleK * matrixSize + cycleJ];
        }
      }
    }
  
    if(matrixSize < OUTPUT_THRESHOLD){
      printf("MatrixARef:\n");
      print_matrix(&matrixARef);
      printf("MatrixBRef:\n");
      print_matrix(&matrixBRef);
      printf("MatrixCRef:\n");
      print_matrix(&matrixCRef);
      printf("MatrixCSerial:\n");
      print_matrix(&matrixCSerial);
    }

    // compare the serial result and parallel result
    for(cycleI = 0; cycleI < matrixSize; ++ cycleI){
      for(cycleJ = 0; cycleJ < matrixSize; ++ cycleJ){
        if(matrixCRef.data[cycleI * matrixSize + cycleJ]
            == matrixCSerial.data[cycleI * matrixSize + cycleJ]){
          ++match;
        }else{
          ++diff;
        }
      }
    }

    printf("\n====\nValidation Result: %d match(es), %d diff(s)\n\n", match, diff);
  }

  SAFE_FREE(matrixARef.data);
  SAFE_FREE(matrixBRef.data);
  SAFE_FREE(matrixCRef.data);
  SAFE_FREE(matrixCSerial.data);
  SAFE_FREE(matrixTmp.data);
}
