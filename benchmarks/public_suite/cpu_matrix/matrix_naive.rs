use std::env;
use std::time::Instant;

fn matrix_mult_naive(n: usize) -> f64 {
    let a = vec![vec![1.0f64; n]; n];
    let b = vec![vec![2.0f64; n]; n];
    let mut c = vec![vec![0.0f64; n]; n];

    for i in 0..n {
        for k in 0..n {
            let aik = a[i][k];
            for j in 0..n {
                c[i][j] += aik * b[k][j];
            }
        }
    }
    c[0][0] + c[n - 1][n - 1]
}

fn main() {
    let args: Vec<String> = env::args().collect();
    let n: usize = if args.len() > 1 { args[1].parse().unwrap_or(500) } else { 500 };

    let t1 = Instant::now();
    let checksum = matrix_mult_naive(n);
    let ms = t1.elapsed().as_secs_f64() * 1000.0;

    println!("{{\"lang\": \"Rust (Naive)\", \"test\": \"Matrix 500x500\", \"checksum\": {}, \"time_ms\": {:.3}}}", checksum, ms);
}
