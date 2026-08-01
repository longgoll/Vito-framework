#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib>

double matrix_mult_naive(int n) {
    std::vector<std::vector<double>> a(n, std::vector<double>(n, 1.0));
    std::vector<std::vector<double>> b(n, std::vector<double>(n, 2.0));
    std::vector<std::vector<double>> c(n, std::vector<double>(n, 0.0));

    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            double aik = a[i][k];
            for (int j = 0; j < n; j++) {
                c[i][j] += aik * b[k][j];
            }
        }
    }
    return c[0][0] + c[n-1][n-1];
}

int main(int argc, char** argv) {
    int n = 500;
    if (argc > 1) n = std::atoi(argv[1]);

    auto t1 = std::chrono::high_resolution_clock::now();
    double checksum = matrix_mult_naive(n);
    auto t2 = std::chrono::high_resolution_clock::now();
    double ms = std::chrono::duration<double, std::milli>(t2 - t1).count();

    std::cout << "{\"lang\": \"C++20 (Naive)\", \"test\": \"Matrix 500x500\", \"checksum\": " << checksum << ", \"time_ms\": " << ms << "}\n";
    return 0;
}
