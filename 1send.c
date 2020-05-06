#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define MASTER 0
#define N 60

#define TAG 777

void printArray(int *d, int n) {
    for (int j = 0; j < n; j++)
        printf("%d\t", d[j]);
    printf("\n");
}

int main(int argc, char *argv[]) {

    MPI_Init(&argc, &argv);

    int size, rank;
    MPI_Status *status;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int chunk = (int)N / size;
    int *altdizi = malloc(chunk * sizeof(int));
    for (int i = 0; i < chunk; i++)
            altdizi[i] = 0;

    if (rank == MASTER) {
        int *dizi = malloc(N * sizeof(int));
        for (int i = 0; i < N; i++)
            dizi[i] = i + 1;

        //MPI_Send( &dizi[15], 15, MPI_INT, 1, 777, MPI_COMM_WORLD );
        //MPI_Send( &dizi[30], 15, MPI_INT, 2, 778, MPI_COMM_WORLD );
        //MPI_Send( &dizi[45], 15, MPI_INT, 3, 779, MPI_COMM_WORLD );

        for (int m = 1; m <= 3; m++)
            MPI_Send( &dizi[chunk*m], chunk, MPI_INT, m, TAG+m-1, MPI_COMM_WORLD );

        for (int j = 0; j < 15; j++)
            altdizi[j] = dizi[j];
    }
    
    if (rank == 1) {
        MPI_Recv( altdizi, 15, MPI_INT, MASTER, 777, MPI_COMM_WORLD, status );
        // printArray(altdizi, 15);
    }
    else if (rank == 2) {    
        MPI_Recv( altdizi, 15, MPI_INT, MASTER, 778, MPI_COMM_WORLD, status );  
        printArray(altdizi, 15);   
    } 
    else if (rank == 3) {       
        MPI_Recv( altdizi, 15, MPI_INT, MASTER, 779, MPI_COMM_WORLD, status );  
        //printArray(altdizi, 15);
    }

    MPI_Finalize();

    return 0;
}


