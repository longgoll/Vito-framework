#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC optimize("O3,unroll-loops,fast-math")
#define RESTRICT __restrict__
#else
#define RESTRICT
#endif

double matrix_mult_naive(int n) {
    double** RESTRICT a = (double**)malloc(n * sizeof(double*));
    double** RESTRICT b = (double**)malloc(n * sizeof(double*));
    double** RESTRICT c = (double**)malloc(n * sizeof(double*));
    for (int i = 0; i < n; i++) {
        a[i] = (double*)malloc(n * sizeof(double));
        b[i] = (double*)malloc(n * sizeof(double));
        c[i] = (double*)calloc(n, sizeof(double));
        for (int j = 0; j < n; j++) {
            a[i][j] = 1.0;
            b[i][j] = 2.0;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            double aik = a[i][k];
            double* RESTRICT ci = c[i];
            double* RESTRICT bk = b[k];
            for (int j = 0; j < n; j++) {
                ci[j] += aik * bk[j];
            }
        }
    }

    double checksum = c[0][0] + c[n-1][n-1];
    for (int i = 0; i < n; i++) {
        free(a[i]);
        free(b[i]);
        free(c[i]);
    }
    free(a);
    free(b);
    free(c);
    return checksum;
}

int main(int argc, char** argv) {
    int n = 500;
    if (argc > 1) n = atoi(argv[1]);

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    double checksum = matrix_mult_naive(n);
    clock_gettime(CLOCK_MONOTONIC, &end);

    double ms = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_nsec - start.tv_nsec) / 1000000.0;
    printf("{\"lang\": \"Vit (Naive)\", \"test\": \"Matrix 500x500\", \"checksum\": %.1f, \"time_ms\": %.3f}\n", checksum, ms);
    return 0;
}
