#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <algorithm>

double matrix_mult_blocked(int n) {
    std::vector<double> a(n * n, 1.0);
    std::vector<double> b(n * n, 2.0);
    std::vector<double> c(n * n, 0.0);

    const double* __restrict pa = a.data();
    const double* __restrict pb = b.data();
    double* __restrict pc = c.data();

    int BLOCK = 32;
    for (int i0 = 0; i0 < n; i0 += BLOCK) {
        int imax = std::min(i0 + BLOCK, n);
        for (int k0 = 0; k0 < n; k0 += BLOCK) {
            int kmax = std::min(k0 + BLOCK, n);
            for (int j0 = 0; j0 < n; j0 += BLOCK) {
                int jmax = std::min(j0 + BLOCK, n);

                for (int i = i0; i < imax; i++) {
                    for (int k = k0; k < kmax; k++) {
                        double aik = pa[i * n + k];
                        for (int j = j0; j < jmax; j++) {
                            pc[i * n + j] += aik * pb[k * n + j];
                        }
                    }
                }
            }
        }
    }

    return c[0] + c[n * n - 1];
}

int main(int argc, char** argv) {
    int n = 500;
    if (argc > 1) n = std::atoi(argv[1]);

    auto t1 = std::chrono::high_resolution_clock::now();
    double checksum = matrix_mult_blocked(n);
    auto t2 = std::chrono::high_resolution_clock::now();
    double ms = std::chrono::duration<double, std::milli>(t2 - t1).count();

    std::cout << "{\"lang\": \"C++20 (Blocked)\", \"test\": \"Matrix 500x500\", \"checksum\": " << checksum << ", \"time_ms\": " << ms << "}\n";
    return 0;
}
