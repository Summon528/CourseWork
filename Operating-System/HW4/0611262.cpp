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
#include <queue>
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

enum WorkType { BUBBLE, MERGE };

typedef struct arr {
    int len;
    int *data;
} arr_t;

typedef struct {
    int idx;
    int level;
    arr_t arr1, arr2;
    WorkType type;
} args_t;

pthread_mutex_t dispatch_mutex, complete_mutex;
sem_t dispatch_event, complete_event;
std::queue<args_t *> dispatch_q;
std::queue<args_t *> complete_q;

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

void *worker(void *data) {
    for (;;) {
        sem_wait(&dispatch_event);
        pthread_mutex_lock(&dispatch_mutex);
        args_t *args = dispatch_q.front();
        dispatch_q.pop();
        pthread_mutex_unlock(&dispatch_mutex);
        switch (args->type) {
            case BUBBLE:
                bubble_sort(&args->arr1);
                break;
            case MERGE: {
                int merge_len = args->arr1.len + args->arr2.len;
                arr_t tmp;
                merge(&args->arr1, &args->arr2, &tmp);
                args->arr1 = tmp;
                break;
            }
            default:
                break;
        }
        pthread_mutex_lock(&complete_mutex);
        complete_q.push(args);
        pthread_mutex_unlock(&complete_mutex);
        sem_post(&complete_event);
    }
}

void split(arr_t *source, arr_t *dest1, arr_t *dest2, int free_mem) {
    dest1->len = source->len / 2;
    dest1->data = (int *)malloc(sizeof(int) * dest1->len);
    memmove(dest1->data, source->data, sizeof(int) * dest1->len);

    dest2->len = source->len - dest1->len;
    dest2->data = (int *)malloc(sizeof(int) * dest2->len);
    memmove(dest2->data, source->data + dest1->len, sizeof(int) * dest2->len);

    if (free_mem) {
        source->len = 0;
        free(source->data);
    }
}

int main() {
    FILE *fp;
    fp = fopen("input.txt", "r");
    int n;
    fscanf(fp, "%d", &n);
    arr_t arr = {.len = n, .data = (int *)malloc(sizeof(int) * n)};
    for (int i = 0; i < n; i++) {
        fscanf(fp, "%d", &arr.data[i]);
    }
    fclose(fp);

    for (int thread_cnt = 1; thread_cnt <= 8; thread_cnt++) {
        struct timeval st, ed;
        gettimeofday(&st, 0);
        pthread_t pthreads[thread_cnt];
        sem_init(&dispatch_event, 0, 0);
        sem_init(&complete_event, 0, 0);
        pthread_mutex_init(&dispatch_mutex, NULL);
        pthread_mutex_init(&complete_mutex, NULL);
        for (int i = 0; i < thread_cnt; i++) {
            pthread_create(&pthreads[i], NULL, worker, NULL);
        }

        int job_cnt = 8;
        int part = n / job_cnt;

        args_t *args[job_cnt];
        int cur_idx = 0;
        for (int i = 0; i < job_cnt; i++) {
            args[i] = (args_t *)malloc(sizeof(args_t));
            int len = cur_idx >= n ? 0 : part;
            if (i == job_cnt - 1) len += n % job_cnt;
            args[i]->arr1.data = (int *)malloc(sizeof(int) * len);
            memcpy(args[i]->arr1.data, arr.data + cur_idx, sizeof(int) * len);
            args[i]->arr1.len = len;
            args[i]->idx = i;
            args[i]->type = BUBBLE;
            args[i]->level = 0;
            cur_idx += len;
            pthread_mutex_lock(&dispatch_mutex);
            dispatch_q.push(args[i]);
            pthread_mutex_unlock(&dispatch_mutex);
            sem_post(&dispatch_event);
        }

        args_t *ready[job_cnt];
        for (int i = 0; i < job_cnt; i++) ready[i] = NULL;
        for (;;) {
            sem_wait(&complete_event);
            pthread_mutex_lock(&complete_mutex);
            args_t *args = complete_q.front();
            complete_q.pop();
            pthread_mutex_unlock(&complete_mutex);
            int pair = (1 << args->level);
            ready[args->idx] = args;
            if (args->arr1.len == n && args->idx == 0) break;
            if ((args->idx >> args->level) & 1) {
                pair = args->idx - pair;
            } else {
                pair = args->idx + pair;
            }
            if (pair >= job_cnt ||
                (ready[pair] != NULL && ready[pair]->level == args->level)) {
                int min_idx = MIN(args->idx, pair);
                int max_idx = MAX(args->idx, pair);
                args_t *new_args = ready[min_idx];
                if (max_idx < job_cnt) {
                    new_args->arr2 = ready[max_idx]->arr1;
                } else {
                    new_args->arr2.data = NULL;
                    new_args->arr2.len = 0;
                }
                new_args->level = new_args->level + 1;
                new_args->type = MERGE;
                pthread_mutex_lock(&dispatch_mutex);
                dispatch_q.push(new_args);
                pthread_mutex_unlock(&dispatch_mutex);
                sem_post(&dispatch_event);
                if (max_idx < job_cnt) {
                    free(ready[max_idx]);
                    ready[max_idx] = NULL;
                }
                ready[min_idx] = NULL;
            }
        }

        for (int i = 0; i < thread_cnt; i++) {
            pthread_cancel(pthreads[i]);
        }

        gettimeofday(&ed, 0);
        int sec = ed.tv_sec - st.tv_sec;
        int usec = ed.tv_usec - st.tv_usec;
        printf("Sorting using %d worker thread(s) took %lf secs\n", thread_cnt,
               sec + usec * 1e-6);
        char s[20];
        sprintf(s, "output%d.txt", thread_cnt);
        fp = fopen(s, "w");
        for (int i = 0; i < n; i++) {
            fprintf(fp, "%d ", ready[0]->arr1.data[i]);
        }
        fclose(fp);

        free(ready[0]);
        free(ready[0]->arr1.data);
    }
    free(arr.data);
}