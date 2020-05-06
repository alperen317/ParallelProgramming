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

float *A, *upp;

if (rank == MASTER) {
    A = create1DArray(n1 * n2);
    fillArray(A, n1 * n2);
}

/*
// declare upperType as an indexed datatype
MPI_Datatype upperType;
int count = n1, blen[n1], indices[n1];
for (i = 0; i < n1; i++) {
    blen[i] = n1 - i;
    indices[i] = i * (n1 + 1);
}

MPI_Type_indexed(count, blen, indices, MPI_FLOAT, &upperType);
MPI_Type_commit(&upperType);

if (rank == MASTER)
    MPI_Send(A, 1, upperType, 1, 666, MPI_COMM_WORLD);
else if (rank == 1) {
    upp = create1DArray((int)(n1*(n1+1)/2));
    MPI_Recv(upp, (int)(n1*(n1+1)/2), MPI_FLOAT, MASTER, 666, MPI_COMM_WORLD, &stat);
}

if (rank == 1)
    printArray(upp, (int)(n1*(n1+1)/2));
*/

int sendcounts[size];
int displs[size];  

for (i = 0; i < size; i++) {
    sendcounts[i] = 2 * n1 - 1 - 4 * i;
    displs[i] = 2 * ( n1 + 1 ) * i;
}

float *recvbuf = create1DArray(sendcounts[rank]);

MPI_Scatterv(A, sendcounts, displs, MPI_FLOAT, recvbuf, sendcounts[rank], MPI_FLOAT, MASTER, MPI_COMM_WORLD);

if (rank == 2)
    printArray(recvbuf, sendcounts[rank]);

MPI_Finalize();

}





