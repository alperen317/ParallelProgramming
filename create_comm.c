#include "mpi.h"
#include <stdio.h>
#define NPROCS 16

int main(void)
{
    int rank, size;
    const int ranks1[4] = {0, 1}, ranks2[4] = {10, 11, 14, 15};
    const int ranks3[2] = {2, 3};

    MPI_Init(NULL, NULL);

    MPI_Group orig_group, group1, group2, group3;
    MPI_Comm comm1, comm2, comm3;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Comm_group(MPI_COMM_WORLD, &orig_group);

    MPI_Group_incl(orig_group, 2, ranks1, &group1);
    MPI_Group_incl(orig_group, 4, ranks2, &group2);
    MPI_Group_incl(orig_group, 2, ranks3, &group3);

    MPI_Comm_create(MPI_COMM_WORLD, group1, &comm1);
    MPI_Comm_create(MPI_COMM_WORLD, group2, &comm2);
    MPI_Comm_create(MPI_COMM_WORLD, group3, &comm3);

    int newRank, newSize;
    MPI_Comm_rank(comm1, &newRank);
    MPI_Comm_size(comm1, &newSize);
    /*
MPI_Comm_rank(comm2, &newRank);
MPI_Comm_size(comm2, &newSize);

MPI_Comm_rank(comm3, &newRank);
MPI_Comm_size(comm3, &newSize);

printf("Old rank = %d of %d\t New rank = %d of %d\n", rank, size, newRank, newSize);
*/

    MPI_Comm_free(&comm1);
    MPI_Comm_free(&comm2);
    MPI_Comm_free(&comm3);
    MPI_Group_free(&group1);
    MPI_Group_free(&group2);
    MPI_Group_free(&group3);
    MPI_Finalize();
}
