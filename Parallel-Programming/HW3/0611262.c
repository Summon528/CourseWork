#pragma GCC optimize("O3")
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>
#include "mpi.h"

#ifndef W
#define W 20  // Width
#endif

typedef struct {
    int st;
    int ed;
    int *temp;
    int *next;
    int balance;
    float d;
} args_t;

int get_L(int L, int rank, int size) {
    int LL = L / size;
    if (rank == size - 1) LL += L % size;
    return LL;
}

void *calc(void *data) {
    args_t *args = (args_t *)data;
    int balance = 1;
    int *temp = args->temp;
    int *next = args->next;
    float d = args->d;
    for (int i = args->st; i < args->ed; i++) {
        for (int j = 0; j < W; j++) {
            float t = temp[i * W + j] / d;
            t += temp[i * W + j] * -4;
            t += temp[(i - 1) * W + j];
            t += temp[(i + 1) * W + j];
            t += temp[i * W + (j - 1 < 0 ? 0 : j - 1)];
            t += temp[i * W + (j + 1 >= W ? j : j + 1)];
            t *= d;
            next[i * W + j] = t;
            if (next[i * W + j] != temp[i * W + j]) {
                balance = 0;
            }
        }
    }
    args->balance = balance;
}

int run(int LL, int iteration, int *temp, float d, int *result_it) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int *next = (int *)malloc((LL + 2) * W * sizeof(int));
    int it;
    int prev_rank = rank - 1 < 0 ? -1 : rank - 1;
    int next_rank = rank + 1 >= size ? -1 : rank + 1;
    int balance = 1;
    MPI_Request req_s[2], req_r[2], req_b[size];
    int local_size = 1;
    char *local_size_str = getenv("OMPI_COMM_WORLD_LOCAL_SIZE");
    if (local_size_str != NULL) {
        sscanf(local_size_str, "%d", &local_size);
    }

    int nthreads = (get_nprocs() + local_size - 1) / local_size;
    pthread_t threads[nthreads];
    args_t args[nthreads];

    for (it = 0; it < iteration; it++) {
        if (it != 0) {
            int tmp_balance;

            for (int i = 0; i < size; i++) {
                if (i == rank) continue;
                MPI_Isend(&balance, 1, MPI_INT, i, 4, MPI_COMM_WORLD,
                          &req_b[i]);
            }

            for (int i = 0; i < size; i++) {
                if (i == rank) continue;
                MPI_Recv(&tmp_balance, 1, MPI_INT, i, 4, MPI_COMM_WORLD,
                         MPI_STATUS_IGNORE);
                if (tmp_balance == 0) balance = 0;
            }

            for (int i = 0; i < size; i++) {
                if (i == rank) continue;
                MPI_Wait(&req_b[i], MPI_STATUS_IGNORE);
            }
            if (balance) break;

            if (prev_rank == -1) {
                memcpy(temp, temp + W, W * sizeof(int));
            } else {
                MPI_Isend(temp + W, W, MPI_INT, prev_rank, 2, MPI_COMM_WORLD,
                          &req_s[0]);
                MPI_Irecv(temp, W, MPI_INT, prev_rank, 2, MPI_COMM_WORLD,
                          &req_r[0]);
            }
            if (next_rank == -1) {
                memcpy(temp + (LL + 1) * W, temp + LL * W, W * sizeof(int));
            } else {
                MPI_Isend(temp + LL * W, W, MPI_INT, next_rank, 2,
                          MPI_COMM_WORLD, &req_s[1]);
                MPI_Irecv(temp + (LL + 1) * W, W, MPI_INT, next_rank, 2,
                          MPI_COMM_WORLD, &req_r[1]);
            }
            if (prev_rank != -1) {
                MPI_Wait(&req_s[0], MPI_STATUS_IGNORE);
                MPI_Wait(&req_r[0], MPI_STATUS_IGNORE);
            }
            if (next_rank != -1) {
                MPI_Wait(&req_s[1], MPI_STATUS_IGNORE);
                MPI_Wait(&req_r[1], MPI_STATUS_IGNORE);
            }
        }

        int LLL = LL / nthreads;
        for (int i = 0; i < nthreads; i++) {
            args[i].temp = temp;
            args[i].next = next;
            args[i].d = d;
            args[i].st = 1 + LLL * i;
            args[i].ed = args[i].st + LLL;
            args[i].balance = 1;
            if (i == nthreads - 1) {
                args[i].ed = args[i].ed + LL % nthreads;
            }
            pthread_create(&threads[i], NULL, calc, &args[i]);
        }

        balance = 1;
        void *tmp_balance;
        for (int i = 0; i < nthreads; i++) {
            pthread_join(threads[i], NULL);
            if (args[i].balance == 0) balance = 0;
        }

        int *tmp = temp;
        temp = next;
        next = tmp;
    }

    int min = temp[W];
    for (int i = 1; i <= LL; i++) {
        for (int j = 0; j < W; j++) {
            if (temp[i * W + j] < min) min = temp[i * W + j];
        }
    }

    if (rank != 0) {
        MPI_Send(&min, 1, MPI_INT, 0, 3, MPI_COMM_WORLD);
    }

    if (result_it != NULL) {
        *result_it = it;
    }

    return min;
}

int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int L = atoi(argv[1]);          // Length
    int iteration = atoi(argv[2]);  // Iteration
    if (rank == 0) {
        srand(atoi(argv[3]));                        // Seed
        float d = (float)random() / RAND_MAX * 0.2;  // Diffusivity

        for (int i = 0; i < size; i++) {
            MPI_Send(&d, 1, MPI_FLOAT, i, 1, MPI_COMM_WORLD);
        }
        int *temp = malloc((L + 2) * W * sizeof(int));  // Current temperature

        for (int i = 1; i <= L; i++) {
            for (int j = 0; j < W; j++) {
                temp[i * W + j] = random() >> 3;
            }
        }
        memcpy(temp, temp + W, W * sizeof(int));
        memcpy(temp + (L + 1) * W, temp + L * W, W * sizeof(int));

        int part = L / size * W;
        for (int i = 1; i < size; i++) {
            int send_chunck = (get_L(L, i, size) + 2) * W;
            MPI_Send(temp + part * i, send_chunck, MPI_INT, i, 0,
                     MPI_COMM_WORLD);
        }

        int cnt;
        int min = run(L / size, iteration, temp, d, &cnt);
        int tmp_min;
        for (int i = 1; i < size; i++) {
            MPI_Recv(&tmp_min, 1, MPI_INT, i, 3, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);
            if (tmp_min < min) min = tmp_min;
        }

        printf("Size: %d*%d, Iteration: %d, Min Temp: %d\n", L, W, cnt, min);
    } else {
        float d;
        int LL = get_L(L, rank, size);
        int recv_chunk = (LL + 2) * W;
        int *temp = (int *)malloc((LL + 2) * W * sizeof(int));
        MPI_Recv(&d, 1, MPI_FLOAT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(temp, (LL + 2) * W, MPI_INT, 0, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        run(LL, iteration, temp, d, NULL);
    }
    MPI_Finalize();
    return 0;
}
