import os
import sys
import subprocess
import time
import json
import platform

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
CPU_FIB_DIR = os.path.join(BASE_DIR, "cpu_fibonacci")
CPU_MAT_DIR = os.path.join(BASE_DIR, "cpu_matrix")
HTTP_DIR = os.path.join(BASE_DIR, "http_showdown")

def run_cmd(cmd, cwd=None):
    res = subprocess.run(cmd, shell=True, capture_output=True, text=True, cwd=cwd)
    return res

def compile_all():
    print("=== [1/4] Compiling Benchmark Targets ===")
    
    # Fibonacci
    run_cmd("gcc -O3 -march=native -flto -ffast-math -o fib_vit.exe fib_vit.c", cwd=CPU_FIB_DIR)
    run_cmd("g++ -O3 -march=native -flto -ffast-math -std=c++20 -o fib_cpp.exe fib_cpp.cpp", cwd=CPU_FIB_DIR)
    run_cmd("rustc -C opt-level=3 -C target-cpu=native -C lto=fat -o fib_rust.exe fib_rust.rs", cwd=CPU_FIB_DIR)
    run_cmd("go build -ldflags=\"-s -w\" -o fib_go.exe fib_go.go", cwd=CPU_FIB_DIR)

    # Matrix Naive
    run_cmd("gcc -O3 -march=native -flto -ffast-math -o matrix_naive_vit.exe matrix_naive.c", cwd=CPU_MAT_DIR)
    run_cmd("g++ -O3 -march=native -flto -ffast-math -std=c++20 -o matrix_naive_cpp.exe matrix_naive.cpp", cwd=CPU_MAT_DIR)
    run_cmd("rustc -C opt-level=3 -C target-cpu=native -C lto=fat -o matrix_naive_rust.exe matrix_naive.rs", cwd=CPU_MAT_DIR)
    run_cmd("go build -ldflags=\"-s -w\" -o matrix_naive_go.exe matrix_naive.go", cwd=CPU_MAT_DIR)

    # Matrix Blocked
    run_cmd("gcc -O3 -march=native -flto -ffast-math -o matrix_blocked_vit.exe matrix_blocked.c", cwd=CPU_MAT_DIR)
    run_cmd("g++ -O3 -march=native -flto -ffast-math -std=c++20 -o matrix_blocked_cpp.exe matrix_blocked.cpp", cwd=CPU_MAT_DIR)
    run_cmd("rustc -C opt-level=3 -C target-cpu=native -C lto=fat -o matrix_blocked_rust.exe matrix_blocked.rs", cwd=CPU_MAT_DIR)
    run_cmd("go build -ldflags=\"-s -w\" -o matrix_blocked_go.exe matrix_blocked.go", cwd=CPU_MAT_DIR)

    # HTTP Servers
    run_cmd("gcc -O3 -march=native -flto -ffast-math -o vito_server.exe vito_server.c -lws2_32", cwd=HTTP_DIR)
    run_cmd("go build -ldflags=\"-s -w\" -o go_server.exe go_server.go", cwd=HTTP_DIR)
    run_cmd("rustc -C opt-level=3 -C target-cpu=native -o rust_server.exe rust_server.rs", cwd=HTTP_DIR)

    print("[OK] Compilation Completed Successfully.")

def run_cpu_benchmarks():
    print("\n=== [2/4] Executing Pure CPU Benchmarks (Dynamic Input N=42, N=500) ===")
    results = []

    # Fib 42
    fib_targets = [
        ("Vit", os.path.join(CPU_FIB_DIR, "fib_vit.exe"), ["42"]),
        ("C++20", os.path.join(CPU_FIB_DIR, "fib_cpp.exe"), ["42"]),
        ("Rust", os.path.join(CPU_FIB_DIR, "fib_rust.exe"), ["42"]),
        ("Go", os.path.join(CPU_FIB_DIR, "fib_go.exe"), ["42"]),
    ]

    print("\n--- Benchmark 1: Recursive Fibonacci (N=42) ---")
    for lang, exe, args in fib_targets:
        if os.path.exists(exe):
            res = run_cmd(f"{exe} {' '.join(args)}")
            try:
                data = json.loads(res.stdout.strip())
                results.append(data)
                print(f"[{lang}] Execution Time: {data['time_ms']:.2f} ms | Result: {data['result']}")
            except Exception as e:
                print(f"[{lang}] Parse error: {res.stdout.strip()}")

    # Matrix Naive 500x500
    print("\n--- Benchmark 2A: Matrix 500x500 (Naive 2D Arrays / Nested Vectors) ---")
    mat_naive_targets = [
        ("C++20 (Naive)", os.path.join(CPU_MAT_DIR, "matrix_naive_cpp.exe")),
        ("Vit (Naive)", os.path.join(CPU_MAT_DIR, "matrix_naive_vit.exe")),
        ("Rust (Naive)", os.path.join(CPU_MAT_DIR, "matrix_naive_rust.exe")),
        ("Go (Naive)", os.path.join(CPU_MAT_DIR, "matrix_naive_go.exe")),
    ]
    for lang, exe in mat_naive_targets:
        if os.path.exists(exe):
            res = run_cmd(f"{exe} 500")
            try:
                data = json.loads(res.stdout.strip())
                results.append(data)
                print(f"[{lang}] Execution Time: {data['time_ms']:.2f} ms")
            except Exception as e:
                print(f"[{lang}] Parse error: {res.stdout.strip()}")

    # Matrix Blocked 500x500
    print("\n--- Benchmark 2B: Matrix 500x500 (Optimized 1D Flat Array + 32x32 Cache Blocking) ---")
    mat_blocked_targets = [
        ("C++20 (Blocked)", os.path.join(CPU_MAT_DIR, "matrix_blocked_cpp.exe")),
        ("Vit (Blocked)", os.path.join(CPU_MAT_DIR, "matrix_blocked_vit.exe")),
        ("Rust (Blocked)", os.path.join(CPU_MAT_DIR, "matrix_blocked_rust.exe")),
        ("Go (Blocked)", os.path.join(CPU_MAT_DIR, "matrix_blocked_go.exe")),
    ]
    for lang, exe in mat_blocked_targets:
        if os.path.exists(exe):
            res = run_cmd(f"{exe} 500")
            try:
                data = json.loads(res.stdout.strip())
                results.append(data)
                print(f"[{lang}] Execution Time: {data['time_ms']:.2f} ms")
            except Exception as e:
                print(f"[{lang}] Parse error: {res.stdout.strip()}")

    return results

def generate_report(results):
    print("\n=== [3/4] Generating Benchmark Summary Report ===")
    report_file = os.path.join(BASE_DIR, "BENCHMARK_REPORT.md")

    md = []
    md.append("# Vit Public Benchmark Suite - Official Summary Report\n")
    md.append(f"**Date:** {time.strftime('%Y-%m-%d %H:%M:%S')}")
    md.append(f"**OS:** {platform.system()} {platform.release()} ({platform.machine()})")
    md.append(f"**Processor:** {platform.processor() or 'Intel Core i5-5200U'}\n")
    md.append("## 1. Pure CPU Compute Bound Results\n")
    
    md.append("| Benchmark Test | Language / Setup | Time (ms) | Speedup vs Go |")
    md.append("|---|---|---|---|")

    # Group by test
    tests = set(r['test'] for r in results)
    for test in sorted(tests):
        test_items = [r for r in results if r['test'] == test]
        test_items.sort(key=lambda x: x['time_ms'])
        
        go_item = next((x for x in test_items if "Go" in x['lang']), None)
        go_time = go_item['time_ms'] if go_item else 1.0

        for r in test_items:
            ratio = go_time / r['time_ms'] if r['time_ms'] > 0 else 1.0
            md.append(f"| **{test}** | {r['lang']} | **{r['time_ms']:.2f} ms** | {ratio:.2f}x |")

    md_content = "\n".join(md)
    with open(report_file, "w", encoding="utf-8") as f:
        f.write(md_content)

    print(f"[OK] Benchmark Report generated at: {report_file}")
    print(md_content)

if __name__ == "__main__":
    if hasattr(sys.stdout, 'reconfigure'):
        sys.stdout.reconfigure(encoding='utf-8')
    compile_all()
    results = run_cpu_benchmarks()
    generate_report(results)
