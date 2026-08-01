package main

import (
	"fmt"
	"os"
	"strconv"
	"time"
)

func matrixMultNaive(n int) float64 {
	a := make([][]float64, n)
	b := make([][]float64, n)
	c := make([][]float64, n)
	for i := 0; i < n; i++ {
		a[i] = make([]float64, n)
		b[i] = make([]float64, n)
		c[i] = make([]float64, n)
		for j := 0; j < n; j++ {
			a[i][j] = 1.0
			b[i][j] = 2.0
		}
	}

	for i := 0; i < n; i++ {
		for k := 0; k < n; k++ {
			aik := a[i][k]
			for j := 0; j < n; j++ {
				c[i][j] += aik * b[k][j]
			}
		}
	}
	return c[0][0] + c[n-1][n-1]
}

func main() {
	n := 500
	if len(os.Args) > 1 {
		if val, err := strconv.Atoi(os.Args[1]); err == nil {
			n = val
		}
	}

	t1 := time.Now()
	checksum := matrixMultNaive(n)
	ms := float64(time.Since(t1).Nanoseconds()) / 1e6

	fmt.Printf("{\"lang\": \"Go (Naive)\", \"test\": \"Matrix 500x500\", \"checksum\": %.1f, \"time_ms\": %.3f}\n", checksum, ms)
}
