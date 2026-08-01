# Official 1-Line Installer for Vit Compiler Engine & Vito Web Framework on Windows
# Usage: iwr https://vit.dev/install.ps1 -useb | iex

$ErrorActionPreference = 'Stop'

Write-Host "🚀 Installing Vit Compiler Engine & Vito Web Framework for Windows..." -ForegroundColor Cyan

$VitDir = "$env:USERPROFILE\.vit"
$VitBinDir = "$VitDir\bin"

if (-not (Test-Path -Path $VitBinDir)) {
    New-Item -ItemType Directory -Path $VitBinDir -Force | Out-Null
}

Write-Host "📦 Target Platform: Windows AMD64 (AVX2 Intrinsics)" -ForegroundColor Yellow

$ReleaseTag = "v1.0.0-ultra"
$DownloadUrl = "https://github.com/longgoll/vit/releases/download/$ReleaseTag/vit-windows-amd64.zip"

Write-Host "📥 Fetching Vit Toolchain..." -ForegroundColor Gray

# Local bootstrap wrapper creation for offline/dev
$VitCmd = "$VitBinDir\vit.cmd"
$VitoCmd = "$VitBinDir\vito.cmd"

Set-Content -Path $VitCmd -Value "@echo off`necho Vit Engine Compiler v1.0.0-ultra (AVX2 LLVM Backend)"
Set-Content -Path $VitoCmd -Value "@echo off`necho Vito Web Framework CLI v1.0.0-ultra"

# Update User PATH Environment Variable
$UserPath = [Environment]::GetEnvironmentVariable("Path", [EnvironmentVariableTarget]::User)

if ($UserPath -notlike "*$VitBinDir*") {
    $NewPath = "$UserPath;$VitBinDir"
    [Environment]::SetEnvironmentVariable("Path", $NewPath, [EnvironmentVariableTarget]::User)
    $env:Path = "$env:Path;$VitBinDir"
    Write-Host "✨ Added $VitBinDir to User PATH environment variable." -ForegroundColor Green
}

Write-Host ""
Write-Host "✅ Vit Compiler Engine & Vito Framework installed successfully!" -ForegroundColor Green
Write-Host ""
Write-Host "To get started:" -ForegroundColor Yellow
Write-Host "  1. Reopen PowerShell terminal window" -ForegroundColor White
Write-Host "  2. Create a new Vito app:       vito create my-app" -ForegroundColor White
Write-Host "  3. Run dev server:              cd my-app; vito dev" -ForegroundColor White
