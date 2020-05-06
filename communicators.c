#include <stdio.h>
#include "mpi.h"

#define Q 4
#define NPROCS 4 * 4

int main(void)
{

     MPI_Init(NULL, NULL);

     int rank, size, i, j;
     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
     MPI_Comm_size(MPI_COMM_WORLD, &size);

     MPI_Group fullGroup, rowG1, rowG2, rowG3, rowG4;
     MPI_Comm rowComm1, rowComm2, rowComm3, rowComm4;

     MPI_Comm_group(MPI_COMM_WORLD, &fullGroup);

     int gRanks[Q] = {0};

     // First row communicator
     for (i = 0; i < Q; i++)
          gRanks[i] = i;

     // full gruptan q tane 0 1 2 3 ranklarında rowG1 oluştur
     MPI_Group_incl(fullGroup, Q, gRanks, &rowG1);
     // row com oluştur
     MPI_Comm_create(MPI_COMM_WORLD, rowG1, &rowComm1);

     // Second row communicator
     for (i = 0; i < Q; i++)
          gRanks[i] = Q + i;

     MPI_Group_incl(fullGroup, Q, gRanks, &rowG2);
     MPI_Comm_create(MPI_COMM_WORLD, rowG2, &rowComm2);

     // Third row communicator
     for (i = 0; i < Q; i++)
          gRanks[i] = 2 * Q + i;

     MPI_Group_incl(fullGroup, Q, gRanks, &rowG3);
     MPI_Comm_create(MPI_COMM_WORLD, rowG3, &rowComm3);

     // Fourth row communicator
     for (i = 0; i < Q; i++)
          gRanks[i] = 3 * Q + i;

     MPI_Group_incl(fullGroup, Q, gRanks, &rowG4);
     MPI_Comm_create(MPI_COMM_WORLD, rowG4, &rowComm4);

     int rank1, rank2, rank3, rank4;
     int a = rank;

     MPI_Bcast(&a, 1, MPI_INT, 0, rowComm1);

     MPI_Bcast(&a, 1, MPI_INT, 0, rowComm2);

     MPI_Bcast(&a, 1, MPI_INT, 0, rowComm3);

     MPI_Bcast(&a, 1, MPI_INT, 0, rowComm4);

     printf("rank = %d\t a = %d\n", rank, a);

     MPI_Finalize();
}
