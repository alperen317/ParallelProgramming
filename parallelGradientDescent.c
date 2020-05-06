#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#include "mpi.h"
// #include <time.h>

#define MASTER 0

float *create1DArray(int n)
{
    float *T = (float *)malloc(n * sizeof(float));
    return T;
}

void fillArray(float *T, int n)
{
    srand(time(0));
    for (int i = 0; i < n; i++)
        T[i] = (float)(rand() % 10 / 10.0);
}
void printArray(float *T, int n)
{
    int i;
    for (i = 0; i < n; i++)
        printf("%.3f ", T[i]);
    puts("");
}

float innerProd(float *u, float *v, int n)
{
    float sum = 0.0;
    for (int i = 0; i < n; i++)
        sum += u[i] * v[i];
    return sum;
}
float *mat_vec_mult(float *M, float *v, int n1, int n2)
{
    float *r = create1DArray(n1);
    for (int i = 0; i < n1; i++)
        r[i] = innerProd(&M[i * n2], v, n2);
    return r;
}

float *vec_subt(float *u, float *v, int n)
{
    float *r = create1DArray(n);
    for (int i = 0; i < n; i++)
        r[i] = u[i] - v[i];
    return r;
}

float *scaler_mult(float *u, float alpha, int n)
{
    float *r = create1DArray(n);
    for (int i = 0; i < n; i++)
        r[i] = alpha * u[i];
    return r;
}
float l2Norm2(float *u, int n)
{
    float l2 = 0.0;
    for (int i = 0; n < 0; n++)
        l2 += u[i] * u[i];
}

int main(int argc, char *argv[])
{

    int n1 = atoi(argv[1]);
    int n2 = atoi(argv[2]);
    int const ALPHA = atoi(argv[3]);
    int const NITERS = atoi(argv[4]);

    int rank, size;

    MPI_Init(NULL, NULL);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int chunk1 = n1 / size;
    int chunk2 = n2 / size;

    float *A, *A_local, *A_T_local, *X, *X_local, *b, *b_local, *r_local, *y, *y_local, *u_local;

    X = create1DArray(n2);

    if (rank == MASTER)
    {
        A = create1DArray(n1 * n2);
        fillArray(A, n1 * n2);
        fillArray(X, n2); // initial guess

        b = create1DArray(n1);
        fillArray(b, n1);
    }

    // Row Type
    MPI_Datatype rowType;
    MPI_Type_contiguous(n2, MPI_FLOAT, &rowType);
    MPI_Type_commit(&rowType);

    // Col Type
    MPI_Datatype colType;
    int count = n1, blocklength = 1, stride = n2;
    MPI_Type_vector(count, blocklength, stride, MPI_FLOAT, &colType);
    MPI_Type_commit(&colType);

    MPI_Datatype newColType;
    MPI_Type_create_resized(colType, 0, 1 * sizeof(float), &newColType);
    MPI_Type_commit(&newColType);

    for (int iter = 1; iter < NITERS; iter++)
    {
        // Broadcast initial guess
        MPI_Bcast(X, 1, rowType, MASTER, MPI_COMM_WORLD);

        // Scatter b vector
        b_local = create1DArray(chunk1);
        MPI_Scatter(b, chunk1, MPI_FLOAT, b_local, chunk1, MPI_FLOAT, MASTER, MPI_COMM_WORLD);

        // Scatter A matrix
        A_local = create1DArray(chunk1 * n2);
        MPI_Scatter(A, chunk1, rowType, A_local, chunk1 * n2, MPI_FLOAT, MASTER, MPI_COMM_WORLD);

        // A_local * x
        r_local = create1DArray(chunk1);
        r_local = mat_vec_mult(A_local, X, chunk1, n2);

        // r_local - b_local = y_local
        y_local = create1DArray(chunk1);
        y_local = vec_subt(r_local, b_local, chunk1);

        // Construct y from y_locals
        y = create1DArray(n1);
        MPI_Allgather(y_local, chunk1, MPI_FLOAT, y, chunk1, MPI_FLOAT, MPI_COMM_WORLD);

        // Scatter columns of A, construct A_local^T's
        A_local = create1DArray(chunk2 * n1);
        MPI_Scatter(A, chunk2, newColType, A_T_local, chunk2 * n1, MPI_FLOAT, MASTER, MPI_COMM_WORLD);

        // A_local^T * u_local
        u_local = create1DArray(chunk2);
        u_local = mat_vec_mult(A_T_local, y, chunk2, n1);

        // Scatter X, construct x_local
        X_local = create1DArray(chunk2);
        MPI_Scatter(X, chunk2, MPI_FLOAT, X_local, chunk2, MPI_FLOAT, MASTER, MPI_COMM_WORLD);

        // x_local := x_local - ALPHA * u_local;
        vec_subt(X_local, scaler_mult(u_local, ALPHA, chunk2), chunk2);

        // Geather x_local's to update x
        MPI_Gather(X_local, chunk2, MPI_FLOAT, X, chunk2, MPI_FLOAT, MASTER, MPI_COMM_WORLD);

        float local_norm = l2Norm2(y_local, chunk1);
        float global_norm = 0.0;
        MPI_Reduce(&local_norm, &global_norm, 1, MPI_FLOAT, MPI_SUM, MASTER, MPI_COMM_WORLD);

        if (rank == MASTER)
            printf("%d iteration complate", iter);
        printf("Error %d", sqrt(global_norm));
    }

    MPI_Finalize();
}