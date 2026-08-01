package main

import (
	"fmt"
	"os"
	"strconv"
	"time"
)

func fib(n uint64) uint64 {
	if n <= 1 {
		return n
	}
	return fib(n-1) + fib(n-2)
}

func main() {
	n := uint64(42)
	if len(os.Args) > 1 {
		if val, err := strconv.ParseUint(os.Args[1], 10, 64); err == nil {
			n = val
		}
	}

	t1 := time.Now()
	f := fib(n)
	fibMs := float64(time.Since(t1).Nanoseconds()) / 1e6

	fmt.Printf("{\"lang\": \"Go\", \"test\": \"Fibonacci\", \"n\": %d, \"result\": %d, \"time_ms\": %.3f}\n", n, f, fibMs)
}
