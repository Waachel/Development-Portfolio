@echo off
setlocal

:: Campaign History Snapshot & Port Trending Tool
:: Double-click this file to launch the tool

powershell.exe -ExecutionPolicy Bypass -File "%~dp0CampaignHistorySnapshotV3.ps1"

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo Script encountered an error. Press any key to exit.
    pause > nul
)
