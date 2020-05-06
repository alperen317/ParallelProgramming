#include <stdio.h>
#include <stdlib.h>
#include "mpi.h" //MPI kutuphanesi

#define MASTER 0

int *create1DArray(int n)
{
     int *T = (int *)malloc(n * sizeof(int));
     return T;
}

void printArray(int *T, int n1)
{
     int i;
     for (i = 0; i < n1; i++)
          printf("%d ", T[i]);

     puts("");
}

int main(int argc, char *argv[])
{

     MPI_Init(NULL, NULL);

     int neighborCount = 0;
     int size, rank;

     MPI_Comm_size(MPI_COMM_WORLD, &size);
     MPI_Comm_rank(MPI_COMM_WORLD, &rank);

     int index[7] = {2, 5, 9, 10, 11, 12};
     int edges[12] = {1, 2, 0, 3, 4, 0, 5, 6, 1, 1, 2, 2};
     int reoder = 1;

     MPI_Comm treeGraph;

     // Graph Create
     // 1. mpı com world --- 2. node sayısı --3.  index --4. edges
     MPI_Graph_create(MPI_COMM_WORLD, size, index, edges, reoder, &treeGraph);

     // Nodes komşu sayısı
     // komşu sayısı   ---- 1. com --- 2. rank istenilen rank yazılır --- 3.
     MPI_Graph_neighbors_count(treeGraph, rank, &neighborCount);

     // komşu sayısını yazdırma
     // printf("Rank = %d \t My neighborCount %d \n", rank, neighborCount );

     int *neighborArray = create1DArray(neighborCount);

     MPI_Graph_neighbors(treeGraph, 0, neighborCount, neighborArray);
     /*Komşuları bastırma
if(rank == 0)
	printArray(neighborArray, neighborCount);
*/

     int topoType;
     MPI_Topo_test(treeGraph, &topoType);
     if (rank == 0)
          printf("%d", topoType);

     MPI_Finalize();

     return 0;
}