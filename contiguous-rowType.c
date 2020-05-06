#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define MASTER 0

float *create1DArray(int n) {
    float *T = (float *)malloc(n * sizeof(float));
    return T;
}

void fillArray(float *T, int n) {
    int i;
    for (i = 0; i < n; i++)
        T[i] = (float)i + 1.0;
}

void printArray(float *T, int n) {
    int i;
    for (i = 0; i < n; i++)
        printf("%.2f ", T[i]);
    puts("");
}

int main(int argc, char *argv[]) {

int n1 = atoi(argv[1]);
int n2 = atoi(argv[2]);

int rank, size, i;

MPI_Init(NULL, NULL);

MPI_Comm_size(MPI_COMM_WORLD, &size);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);

MPI_Status stat;

float *A, *r, *r_chunk;

int chunk = n1 / size;  // row count per process

r = create1DArray(n2);
r_chunk = create1DArray(chunk * n2);

if (rank == MASTER) {
    A = create1DArray(n1 * n2);
    fillArray(A, n1 * n2);
}

MPI_Datatype rowType;
MPI_Type_contiguous(n2, MPI_FLOAT, &rowType);
MPI_Type_commit(&rowType);

// P2P communication using rowType (only for processes 0 and 1)
/*
if (rank == MASTER)
    MPI_Send(&A[2*n2], 1, rowType, 1, 666, MPI_COMM_WORLD);
else 
    MPI_Recv(r, n2, MPI_FLOAT, MASTER, 666, MPI_COMM_WORLD, &stat);
    //MPI_Recv(r, 1, rowType, MASTER, 666, MPI_COMM_WORLD, &stat);

if (rank == 1)
    printArray(r, n2);

*/
// Scatter columns of A equally
// MPI_Scatter(A, chunk, rowType, r_chunk, n2*chunk, MPI_FLOAT, MASTER, MPI_COMM_WORLD);
MPI_Scatter(A, chunk, rowType, r_chunk, chunk, rowType, MASTER, MPI_COMM_WORLD);

if (rank == 3)
    printArray(r_chunk, n2*chunk);
/*

// Define colType
MPI_Datatype colType;
int count = n1, blocklength = 1, stride = n2;
MPI_Type_vector(count, blocklength, stride, MPI_FLOAT, &colType);
MPI_Type_commit(&colType);

float *col = create1DArray(n1);

if (rank == MASTER)
    MPI_Send(&A[1], 1, colType, 3, 666, MPI_COMM_WORLD);
else if (rank == 3)
    MPI_Recv(col, 1, colType, MASTER, 666, MPI_COMM_WORLD, &stat);

if (rank == 3)
    printArray(col, n1);
*/

MPI_Finalize();

}

