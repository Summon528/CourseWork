#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef unsigned long long ull;

void *toss(void *) __attribute__((optimize("-ffast-math")));

void *toss(void *number_of_tosses) {
    unsigned int seed = (time(NULL) * pthread_self());
    ull number_in_circle = 0;
    double x, y;
    for (ull toss = 0; toss < (ull)number_of_tosses; toss++) {
        x = (double)seed / 0xFFFFFFFF;
        seed = 48271 * seed;
        y = (double)seed / 0xFFFFFFFF;
        seed = 48271 * seed;
        if (x * x + y * y <= 1) number_in_circle++;
    }
    return (void *)number_in_circle;
}

int main(int argc, char **argv) {
    double pi_estimate;
    ull number_of_cpu, number_of_tosses, number_in_circle;
    number_of_cpu = atoi(argv[1]);
    number_of_tosses = atoi(argv[2]);

    pthread_t threads[number_of_cpu];
    ull slice = number_of_tosses / number_of_cpu;
    for (unsigned int i = 0; i < number_of_cpu; i++) {
        pthread_create(&threads[i], NULL, toss, (void *)slice);
    }

    number_in_circle = 0;
    for (unsigned int i = 0; i < number_of_cpu; i++) {
        void *re;
        pthread_join(threads[i], &re);
        number_in_circle += (ull)re;
    }

    pi_estimate = 4 * number_in_circle / ((double)number_of_tosses);

    printf("%f\n", pi_estimate);
    return 0;
}
