#include <stdio.h>
#include "mpi.h"

int main(void)
{

    MPI_Init(NULL, NULL);

    int rank, size, i, j;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Comm rowComm;

    int color = rank / 4;
    int key = 0;

    MPI_Comm_split(MPI_COMM_WORLD, color, key, &rowComm);

    int a = rank;
    MPI_Bcast(&a, 1, MPI_INT, 0, rowComm);

    printf("rank = %d\t a = %d\n", rank, a);

    MPI_Finalize();
}
