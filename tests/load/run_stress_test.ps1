# Stress test runner script for Windows PowerShell
Write-Host "=========================================" -ForegroundColor Cyan
Write-Host " Vito Load & Stress Testing Suite " -ForegroundColor Cyan
Write-Host "=========================================" -ForegroundColor Cyan

if (Get-Command k6 -ErrorAction SilentlyContinue) {
    Write-Host "[1/2] Running Plaintext Load Test..." -ForegroundColor Green
    k6 run tests/load/k6_plaintext.js

    Write-Host "[2/2] Running JSON Serialization Load Test..." -ForegroundColor Green
    k6 run tests/load/k6_json.js
} else {
    Write-Host "k6 is not installed. Please install k6 or run with wrk." -ForegroundColor Yellow
}
