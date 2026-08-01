#include <iostream>
#include <chrono>
#include <cstdlib>

long long fib(long long n) {
    if (n <= 1) return n;
    return fib(n - 1) + fib(n - 2);
}

int main(int argc, char** argv) {
    int n = 42;
    if (argc > 1) {
        n = std::atoi(argv[1]);
    }

    auto t1 = std::chrono::high_resolution_clock::now();
    long long f = fib(n);
    auto t2 = std::chrono::high_resolution_clock::now();
    double fib_ms = std::chrono::duration<double, std::milli>(t2 - t1).count();

    std::cout << "{\"lang\": \"C++20\", \"test\": \"Fibonacci\", \"n\": " << n << ", \"result\": " << f << ", \"time_ms\": " << fib_ms << "}\n";
    return 0;
}
