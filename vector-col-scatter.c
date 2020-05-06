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

float *A, *c, *c_chunk, *cc;

int chunk = n2 / size;

c = create1DArray(n1);
cc = create1DArray(chunk*n1);

if (rank == MASTER) {
    A = create1DArray(n1 * n2);
    fillArray(A, n1 * n2);
}
    
MPI_Datatype colType, newColType;

int blocklength = 1, stride = n2, count = n1;
MPI_Type_vector(count, blocklength, stride, MPI_FLOAT, &colType);
MPI_Type_commit(&colType);

MPI_Type_create_resized(colType, 0, 1*sizeof(float), &newColType);
MPI_Type_commit(&newColType);

// P2P communication using colType
//if (rank == MASTER)
//    MPI_Send(&A[0], 1, colType, MASTER + 1, 666, MPI_COMM_WORLD);    
//else if (rank == MASTER + 1)
//    MPI_Recv(c, n1, MPI_FLOAT, MASTER, 666, MPI_COMM_WORLD, &stat);

// Collective communication using colType
MPI_Scatter(A, chunk, newColType, cc, chunk*n1, MPI_FLOAT, MASTER, MPI_COMM_WORLD);

if (rank == 1)
    printArray(cc, chunk*n1);


MPI_Finalize();

}

