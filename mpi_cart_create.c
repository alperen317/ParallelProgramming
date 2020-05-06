#include <mpi.h>
#include <stdio.h>
/* A two-dimensional torus of 12 processes in a 4x3 grid */
int main(int argc, char *argv[])
{
    int rank, size;
    MPI_Comm comm;
    // 2 boyutlu dim --- 2 boyutlu yönle --- tekrar sıralama
    int dim[2], period[2], reorder;
    // proces coord
    int coord[2], id;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size != 12)
    {
        printf("Please run with 12 processes.\n");
        fflush(stdout);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    dim[0] = 4;
    dim[1] = 3;
    period[0] = 1;
    period[1] = 0;
    reorder = 1;
    // eski com -- 2 boyutlu - 4x 3 lük - message yön - tekrar sınırlandırma - yeni com
    MPI_Cart_create(MPI_COMM_WORLD, 2, dim, period, reorder, &comm);
    if (rank == 5)
    {
        // coord verip rank bulma
        // yeni com - rank - 2 boyutlu - coord
        MPI_Cart_coords(comm, rank, 2, coord);
        printf("Rank %d coordinates are %d %d\n", rank, coord[0], coord[1]);
        fflush(stdout);
    }
    if (rank == 0)
    {
        coord[0] = 3;
        coord[1] = 1;
        // kordinat verip rank bulma
        MPI_Cart_rank(comm, coord, &id);
        printf("The processor at position (%d, %d) has rank %d\n", coord[0], coord[1], id);
        fflush(stdout);
    }
    MPI_Finalize();
    return 0;
}