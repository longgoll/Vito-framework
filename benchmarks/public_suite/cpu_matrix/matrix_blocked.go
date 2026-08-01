package main

import (
	"fmt"
	"os"
	"strconv"
	"time"
)

func min(a, b int) int {
	if a < b {
		return a
	}
	return b
}

func matrixMultBlocked(n int) float64 {
	a := make([]float64, n*n)
	b := make([]float64, n*n)
	c := make([]float64, n*n)
	for i := 0; i < n*n; i++ {
		a[i] = 1.0
		b[i] = 2.0
	}

	block := 32
	for i0 := 0; i0 < n; i0 += block {
		imax := min(i0+block, n)
		for k0 := 0; k0 < n; k0 += block {
			kmax := min(k0+block, n)
			for j0 := 0; j0 < n; j0 += block {
				jmax := min(j0+block, n)

				for i := i0; i < imax; i++ {
					for k := k0; k < kmax; k++ {
						aik := a[i*n+k]
						for j := j0; j < jmax; j++ {
							c[i*n+j] += aik * b[k*n+j]
						}
					}
				}
			}
		}
	}

	return c[0] + c[n*n-1]
}

func main() {
	n := 500
	if len(os.Args) > 1 {
		if val, err := strconv.Atoi(os.Args[1]); err == nil {
			n = val
		}
	}

	t1 := time.Now()
	checksum := matrixMultBlocked(n)
	ms := float64(time.Since(t1).Nanoseconds()) / 1e6

	fmt.Printf("{\"lang\": \"Go (Blocked)\", \"test\": \"Matrix 500x500\", \"checksum\": %.1f, \"time_ms\": %.3f}\n", checksum, ms)
}
