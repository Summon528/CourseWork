/*
Student No.: 0611262
Student Name: Chang-Yen Tseng
Email: vanilla.cv06@nctu.edu.tw
SE tag: xnxcxtxuxoxsx
Statement: I am fully aware that this program is not
supposed to be posted to a public server, such as a
public GitHub repository or a public web page.
*/

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

typedef struct arr {
    int len;
    int *data;
} arr_t;

typedef struct shared {
    arr_t arr[16];
    sem_t sem[16];
} shared_t;

typedef struct args {
    shared_t *shared;
    int tidx;
} args_t;

void split(arr_t *source, arr_t *dest1, arr_t *dest2) {
    dest1->len = source->len / 2;
    dest1->data = (int *)malloc(sizeof(int) * dest1->len);
    memmove(dest1->data, source->data, sizeof(int) * dest1->len);

    dest2->len = source->len - dest1->len;
    dest2->data = (int *)malloc(sizeof(int) * dest2->len);
    memmove(dest2->data, source->data + dest1->len, sizeof(int) * dest2->len);

    source->len = 0;
    free(source->data);
}

void merge(arr_t *source1, arr_t *source2, arr_t *dest) {
    dest->len = source1->len + source2->len;
    int *lp = source1->data;
    int *lend = source1->data + source1->len;
    int *rp = source2->data;
    int *rend = source2->data + source2->len;
    int *result = (int *)malloc(sizeof(int) * dest->len);
    dest->data = result;
    for (;;) {
        if (lp == lend) {
            memmove(result, rp, sizeof(int) * (rend - rp));
            break;
        }
        if (rp == rend) {
            memmove(result, lp, sizeof(int) * (lend - lp));
            break;
        }
        *result++ = (*rp < *lp) ? *rp++ : *lp++;
    }
    source1->len = 0;
    source2->len = 0;
    free(source1->data);
    free(source2->data);
}

void bubble_sort(arr_t *arr) {
    for (int i = 0; i < arr->len - 1; i++) {
        for (int j = 0; j < arr->len - i - 1; j++) {
            if (arr->data[j] > arr->data[j + 1]) {
                int tmp = arr->data[j];
                arr->data[j] = arr->data[j + 1];
                arr->data[j + 1] = tmp;
            }
        }
    }
}

void *process(void *args) {
    args_t *data = (args_t *)args;
    shared_t *shared = data->shared;
    int self = data->tidx;
    int left = self * 2;
    int right = self * 2 + 1;
    int parent = self / 2;
    sem_wait(&shared->sem[self]);
    if (self >= 8) {
        bubble_sort(&shared->arr[self]);
    } else {
        split(&shared->arr[self], &shared->arr[left], &shared->arr[right]);
        sem_post(&shared->sem[left]);
        sem_post(&shared->sem[right]);
        sem_wait(&shared->sem[self]);
        sem_wait(&shared->sem[self]);
        merge(&shared->arr[left], &shared->arr[right], &shared->arr[self]);
    }
    free(args);
    sem_post(&shared->sem[parent]);
    pthread_exit(NULL);
}

void merge_sort(arr_t *arr, int d) {
    arr_t left, right;
    if (d >= 3) {
        bubble_sort(arr);
        return;
    }
    split(arr, &left, &right);
    merge_sort(&left, d + 1);
    merge_sort(&right, d + 1);
    merge(&left, &right, arr);
}

int main() {
    pthread_t pthreads[16];
    shared_t *shared = (shared_t *)malloc(sizeof(shared_t));

    for (int i = 0; i < 16; i++) {
        sem_init(&shared->sem[i], 0, 0);
    }

    for (int i = 1; i < 16; i++) {
        args_t *args = (args_t *)malloc(sizeof(args_t));
        args->tidx = i;
        args->shared = shared;
        pthread_create(&pthreads[i], NULL, process, args);
    }

    printf("Enter input file name: ");
    char file_name[100];
    scanf("%s", file_name);
    FILE *fp;
    fp = fopen(file_name, "r");
    int n;
    fscanf(fp, "%d", &n);
    shared->arr[0].data = (int *)malloc(sizeof(int) * n);
    shared->arr[0].len = n;
    for (int i = 0; i < n; i++) {
        fscanf(fp, "%d", &shared->arr[0].data[i]);
    }
    fclose(fp);

    shared->arr[1].data = (int *)malloc(sizeof(int) * n);
    memcpy(shared->arr[1].data, shared->arr[0].data, sizeof(int) * n);
    shared->arr[1].len = n;

    struct timeval st, ed;
    int sec, usec;

    gettimeofday(&st, 0);
    sem_post(&shared->sem[1]);
    sem_wait(&shared->sem[0]);
    gettimeofday(&ed, 0);
    sec = ed.tv_sec - st.tv_sec;
    usec = ed.tv_usec - st.tv_usec;
    printf("MT sorting used %lf secs\n", sec + usec * 1e-6);

    fp = fopen("output1.txt", "w");
    for (int i = 0; i < n; i++) {
        fprintf(fp, "%d ", shared->arr[1].data[i]);
    }
    fclose(fp);

    gettimeofday(&st, 0);
    merge_sort(&shared->arr[0], 0);
    gettimeofday(&ed, 0);
    sec = ed.tv_sec - st.tv_sec;
    usec = ed.tv_usec - st.tv_usec;
    printf("ST sorting used %lf secs\n", sec + usec * 1e-6);
    fp = fopen("output2.txt", "w");
    for (int i = 0; i < n; i++) {
        fprintf(fp, "%d ", shared->arr[0].data[i]);
    }
    fclose(fp);

    free(shared->arr[0].data);
    free(shared->arr[1].data);
    free(shared);
}