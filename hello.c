#include <stdio.h>
#include "mpi.h"

int main(void) {

    MPI_Init(NULL, NULL);

    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    //printf("rank = %d\tMerhaba Dünya\n", rank);
    printf("\n");

    MPI_Barrier(MPI_COMM_WORLD);  // synchronization
    printf("-------------------------------\n");
    if (rank == 1 || rank == 2)
        printf("rank = %d\tMerhaba Dünya\n", rank);

    MPI_Finalize();

    return 0;
}