/*
Student No.: 0611262
Student Name: Chang-Yen Tseng
Email: vanilla.cv06@nctu.edu.tw
SE tag: xnxcxtxuxoxsx
Statement: I am fully aware that this program is not
supposed to be posted to a public server, such as a
public GitHub repository or a public web page.
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

void calc(int matrix_id, int ans_id, int from, int to, int n) {
    if (from >= to) return;
    uint32_t(*matrix)[n] = (uint32_t(*)[n])shmat(matrix_id, NULL, 0);
    uint32_t(*ans)[n] = (uint32_t(*)[n])shmat(ans_id, NULL, 0);
    for (int i = from; i < to; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                ans[i][j] += matrix[i][k] * matrix[k][j];
            }
        }
    }
}

void start_and_wait(int procs, int matrix_id, int ans_id, int n) {
    int from = 0;
    for (int i = 0; i < procs; i++) {
        int to = (i < n % procs ? 1 : 0) + n / procs + from;
        pid_t pid = fork();
        if (pid == 0) {
            calc(matrix_id, ans_id, from, to, n);
            exit(0);
        } else if (pid > 0) {
            from = to;
        }
    }
    for (int i = 0; i < procs; i++) {
        wait(NULL);
    }
}

int main() {
    int n;
    printf("%s", "Input the matrix dimension: ");
    scanf("%d", &n);
    puts("");
    int matrix_id = shmget(IPC_PRIVATE, sizeof(uint32_t) * n * n,
                           IPC_CREAT | S_IRUSR | S_IWUSR);
    int ans_id = shmget(IPC_PRIVATE, sizeof(uint32_t) * n * n,
                        IPC_CREAT | S_IRUSR | S_IWUSR);

    uint32_t(*matrix)[n] = (uint32_t(*)[n])shmat(matrix_id, NULL, 0);

    for (int i = 0; i < n * n; i++) {
        matrix[0][i] = i;
    }

    for (int i = 1; i <= 16; i++) {
        printf("Multiplying matrices using %d process%s\n", i,
               i == 1 ? "" : "es");
        struct timeval st, ed;
        gettimeofday(&st, 0);
        start_and_wait(i, matrix_id, ans_id, n);
        gettimeofday(&ed, 0);
        int sec = ed.tv_sec - st.tv_sec;
        int usec = ed.tv_usec - st.tv_usec;
        printf("Elapsed time: %lf sec, ", sec + usec * 1e-6);

        uint32_t(*ans)[n] = (uint32_t(*)[n])shmat(ans_id, NULL, 0);
        uint32_t checksum = 0;
        for (int i = 0; i < n * n; i++) {
            checksum += ans[0][i];
            ans[0][i] = 0;
        }
        printf("Checksum: %lu\n", (unsigned long)checksum);
    }

    shmctl(matrix_id, IPC_RMID, NULL);
    shmctl(ans_id, IPC_RMID, NULL);
}
