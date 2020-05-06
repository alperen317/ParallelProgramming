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

int chunk = n1 / size;

r = create1DArray(n2);
r_chunk = create1DArray(chunk * n2);

if (rank == MASTER) {
    A = create1DArray(n1 * n2);
    fillArray(A, n1 * n2);
}
    
MPI_Datatype rowType;
MPI_Type_contiguous(n2, MPI_FLOAT, &rowType);
MPI_Type_commit(&rowType);

// P2P communication using rowType
if (rank == MASTER) {
    MPI_Send(&A[2*n2], n2, MPI_FLOAT, MASTER + 1, 666, MPI_COMM_WORLD);
    MPI_Send(&A[2*n2], 1, rowType, MASTER + 2, 777, MPI_COMM_WORLD);
}
else if (rank == MASTER + 1)
    MPI_Recv(r, n2, MPI_FLOAT, MASTER, 666, MPI_COMM_WORLD, &stat);
else if (rank == MASTER + 2)
    MPI_Recv(r, 1, rowType, MASTER, 777, MPI_COMM_WORLD, &stat);

// Collective communication using rowType
// MPI_Scatter(A, chunk, rowType, r_chunk, chunk*n2, MPI_FLOAT, MASTER, MPI_COMM_WORLD);
MPI_Scatter(A, chunk, rowType, r_chunk, chunk, rowType, MASTER, MPI_COMM_WORLD);

if (rank == 0)
    printArray(r_chunk, n2*chunk);




MPI_Finalize();

}

