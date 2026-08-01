use std::env;
use std::time::Instant;

fn matrix_mult_blocked(n: usize) -> f64 {
    let a = vec![1.0f64; n * n];
    let b = vec![2.0f64; n * n];
    let mut c = vec![0.0f64; n * n];

    let block = 32;
    for i0 in (0..n).step_by(block) {
        let imax = (i0 + block).min(n);
        for k0 in (0..n).step_by(block) {
            let kmax = (k0 + block).min(n);
            for j0 in (0..n).step_by(block) {
                let jmax = (j0 + block).min(n);

                for i in i0..imax {
                    for k in k0..kmax {
                        let aik = a[i * n + k];
                        for j in j0..jmax {
                            c[i * n + j] += aik * b[k * n + j];
                        }
                    }
                }
            }
        }
    }

    c[0] + c[n * n - 1]
}

fn main() {
    let args: Vec<String> = env::args().collect();
    let n: usize = if args.len() > 1 { args[1].parse().unwrap_or(500) } else { 500 };

    let t1 = Instant::now();
    let checksum = matrix_mult_blocked(n);
    let ms = t1.elapsed().as_secs_f64() * 1000.0;

    println!("{{\"lang\": \"Rust (Blocked)\", \"test\": \"Matrix 500x500\", \"checksum\": {}, \"time_ms\": {:.3}}}", checksum, ms);
}
