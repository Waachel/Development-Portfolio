@echo off
setlocal

:: Campaign Port Overlap Analysis Tool
:: This script analyzes Pulse campaign overlaps and identifies port capacity issues

powershell.exe -ExecutionPolicy Bypass -File "%~dp0analyzeOverlaps.ps1"

if errorlevel 1 (
    echo.
    echo Script encountered an error.
    pause
    exit /b 1
)
