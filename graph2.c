 #include <stdio.h>
#include <stdlib.h>
#include "mpi.h" //MPI kutuphanesi

#define MASTER 0

int *create1DArray(int n) {
     int *T = (int *)malloc(n * sizeof(int));
     return T;
}

void printArray(int *T, int n1) {
     int i;
     for (i = 0; i < n1; i++)
          printf("%d ", T[i]); 

     puts("");  
}  

int main(int argc, char *argv[]) {

MPI_Init(NULL, NULL);

int size, rank;

MPI_Comm_size(MPI_COMM_WORLD, &size);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);

int index[8] = {2, 4, 7, 8, 10, 11, 13, 14};
int edges[14] = {1, 2, 0, 3, 0, 4, 5, 1, 2, 6, 2, 4, 7, 6};
int reorder = 1;

int neighborCount = 0;


// com oluşturma
MPI_Comm graphComm;

//graph oluşturma
MPI_Graph_create(MPI_COMM_WORLD, size, index, edges, reorder, &graphComm);

// komşu sayısı bulma
MPI_Graph_neighbors_count(graphComm, rank, &neighborCount);

//printf("Rank = %d\t My neigbour count is %d\n", rank, neighborCount);

int *neighborArray = create1DArray(neighborCount);

MPI_Graph_neighbors(graphComm, rank, neighborCount, neighborArray);

if (rank == 2) {
	printf("I am process %d, my neighbor count is %d\n", rank, neighborCount);
	printArray(neighborArray, neighborCount);
}

// kaç node kaç edge var

int nn, ne;
MPI_Graphdims_get(graphComm, &nn, &ne);
int *ind = create1DArray(nn);
int *edg = create1DArray(ne);
MPI_Graph_get(graphComm, nn, ne, ind, edg);

if (rank == 0) {
	printArray(ind, nn);
	printArray(edg, ne);
}


MPI_Finalize();

return 0;

}
