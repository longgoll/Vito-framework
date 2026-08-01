#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static unsigned long long fib(unsigned long long n) {
    if (n <= 1) return n;
    return fib(n - 1) + fib(n - 2);
}

int main(int argc, char** argv) {
    int n = 42;
    if (argc > 1) {
        n = atoi(argv[1]);
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    unsigned long long f = fib((unsigned long long)n);
    clock_gettime(CLOCK_MONOTONIC, &end);

    double fib_ms = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_nsec - start.tv_nsec) / 1000000.0;

    printf("{\"lang\": \"Vit\", \"test\": \"Fibonacci\", \"n\": %d, \"result\": %llu, \"time_ms\": %.3f}\n", n, f, fib_ms);
    return 0;
}
