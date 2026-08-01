#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <immintrin.h>

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC optimize("O3,unroll-loops,fast-math")
#endif

#if defined(_MSC_VER) || defined(__MINGW32__)
#include <malloc.h>
static void* alloc_aligned64(size_t size) { return _aligned_malloc(size, 64); }
static void free_aligned64(void* ptr) { _aligned_free(ptr); }
#else
static void* alloc_aligned64(size_t size) {
    void* ptr = NULL;
    if (posix_memalign(&ptr, 64, size) != 0) return NULL;
    return ptr;
}
static void free_aligned64(void* ptr) { free(ptr); }
#endif

#if defined(__GNUC__) || defined(__clang__)
#define RESTRICT __restrict__
#else
#define RESTRICT
#endif

static double matrix_mult_blocked(int n) {
    size_t sz = (size_t)n * n * sizeof(double);
    double* RESTRICT a = (double*)alloc_aligned64(sz);
    double* RESTRICT b = (double*)alloc_aligned64(sz);
    double* RESTRICT c = (double*)alloc_aligned64(sz);

    for (int i = 0; i < n * n; i++) {
        a[i] = 1.0;
        b[i] = 2.0;
        c[i] = 0.0;
    }

    int BLOCK = 32;
    for (int i0 = 0; i0 < n; i0 += BLOCK) {
        int imax = (i0 + BLOCK < n) ? (i0 + BLOCK) : n;
        for (int k0 = 0; k0 < n; k0 += BLOCK) {
            int kmax = (k0 + BLOCK < n) ? (k0 + BLOCK) : n;
            for (int j0 = 0; j0 < n; j0 += BLOCK) {
                int jmax = (j0 + BLOCK < n) ? (j0 + BLOCK) : n;

                for (int i = i0; i < imax; i++) {
                    for (int k = k0; k < kmax; k++) {
                        double aik = a[i * n + k];
                        __m256d v_aik = _mm256_set1_pd(aik);
                        int j = j0;
                        for (; j <= jmax - 8; j += 8) {
                            __m256d v_b0 = _mm256_loadu_pd(&b[k * n + j]);
                            __m256d v_b1 = _mm256_loadu_pd(&b[k * n + j + 4]);

                            __m256d v_c0 = _mm256_loadu_pd(&c[i * n + j]);
                            __m256d v_c1 = _mm256_loadu_pd(&c[i * n + j + 4]);

                            v_c0 = _mm256_fmadd_pd(v_aik, v_b0, v_c0);
                            v_c1 = _mm256_fmadd_pd(v_aik, v_b1, v_c1);

                            _mm256_storeu_pd(&c[i * n + j], v_c0);
                            _mm256_storeu_pd(&c[i * n + j + 4], v_c1);
                        }
                        for (; j < jmax; j++) {
                            c[i * n + j] += aik * b[k * n + j];
                        }
                    }
                }
            }
        }
    }

    double checksum = c[0] + c[n * n - 1];
    free_aligned64(a);
    free_aligned64(b);
    free_aligned64(c);
    return checksum;
}

int main(int argc, char** argv) {
    int n = 500;
    if (argc > 1) n = atoi(argv[1]);

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    double checksum = matrix_mult_blocked(n);
    clock_gettime(CLOCK_MONOTONIC, &end);

    double ms = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_nsec - start.tv_nsec) / 1000000.0;
    printf("{\"lang\": \"Vit (Blocked)\", \"test\": \"Matrix 500x500\", \"checksum\": %.1f, \"time_ms\": %.3f}\n", checksum, ms);
    return 0;
}
