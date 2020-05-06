#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(void)
{

     MPI_Init(NULL, NULL);

     int rank, size, i, j, even_size, odd_size;
     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
     MPI_Comm_size(MPI_COMM_WORLD, &size);

     MPI_Group fullGroup;
     MPI_Comm_group(MPI_COMM_WORLD, &fullGroup);

     // Even Comm
     even_size = (size + 1) / 2; // cift ranklilarin adedi
     int *even_ranks = (int *)malloc(even_size * sizeof(int));
     j = 0;
     for (i = 0; i < size; i = i + 2)
     {
          even_ranks[j] = i;
          ++j;
     }
     MPI_Group evenGroup;
     MPI_Group_incl(fullGroup, even_size, even_ranks, &evenGroup);

     MPI_Comm evenComm;
     MPI_Comm_create(MPI_COMM_WORLD, evenGroup, &evenComm);

     // Odd Comm
     odd_size = (size) / 2; // tek ranklilarin adedi
     int *odd_ranks = (int *)malloc(even_size * sizeof(int));
     j = 0;
     for (i = 1; i < size; i = i + 2)
     {
          odd_ranks[j] = i;
          ++j;
     }
     MPI_Group oddGroup;
     MPI_Group_incl(fullGroup, odd_size, odd_ranks, &oddGroup);

     MPI_Comm oddComm;
     MPI_Comm_create(MPI_COMM_WORLD, oddGroup, &oddComm);

     // New ranks
     int evenRank, oddRank;
     int evenSum = 0, oddSum = 0;
     if (rank % 2 == 0)
     {
          MPI_Comm_rank(evenComm, &evenRank);
          oddRank = -1;

          MPI_Reduce(&rank, &evenSum, 1, MPI_INT, MPI_SUM, 0, evenComm);
          if (evenRank == 0)
               printf("evenSum = %d\n", evenSum);
     }
     else
     {
          MPI_Comm_rank(oddComm, &oddRank);
          evenRank = -1;
          MPI_Reduce(&rank, &oddSum, 1, MPI_INT, MPI_SUM, 0, oddComm);
          if (oddRank == 0)
               printf("oddSum = %d\n", oddSum);
     }

     printf("Rank = %d\tevenRank = %d\toddRank = %d\n", rank, evenRank, oddRank);

     MPI_Finalize();
}
