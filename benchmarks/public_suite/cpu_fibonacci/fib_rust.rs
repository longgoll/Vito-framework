use std::env;
use std::time::Instant;

fn fib(n: u64) -> u64 {
    if n <= 1 { return n; }
    fib(n - 1) + fib(n - 2)
}

fn main() {
    let args: Vec<String> = env::args().collect();
    let n: u64 = if args.len() > 1 {
        args[1].parse().unwrap_or(42)
    } else {
        42
    };

    let t1 = Instant::now();
    let f = fib(n);
    let fib_ms = t1.elapsed().as_secs_f64() * 1000.0;

    println!("{{\"lang\": \"Rust\", \"test\": \"Fibonacci\", \"n\": {}, \"result\": {}, \"time_ms\": {:.3}}}", n, f, fib_ms);
}
