package main

import (
	"fmt"
	"net/http"
	"os"
	"runtime"
)

func handler(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json")
	w.Write([]byte(`{"message":"Hello, World!"}`))
}

func main() {
	runtime.GOMAXPROCS(runtime.NumCPU())
	port := "8082"
	if len(os.Args) > 1 {
		port = os.Args[1]
	}

	http.HandleFunc("/", handler)
	fmt.Printf("Go Server listening on port %s...\n", port)
	if err := http.ListenAndServe(":"+port, nil); err != nil {
		panic(err)
	}
}
