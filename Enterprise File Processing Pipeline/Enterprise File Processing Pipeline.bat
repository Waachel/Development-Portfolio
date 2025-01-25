@echo off
setlocal EnableDelayedExpansion

echo Debug: Script started >> "%~dp0debug.txt"

rem OneDrive paths
set "sourcePath=C:\Users\OneDrive - Company\File Migration"
set "destPath=C:\Users\OneDrive - Company\Usage"
set "archivePath=C:\Users\OneDrive - Company\File Migration\Archive"
set "templatePath=C:\Users\OneDrive - Company\File Migration\Template"

rem Template file name
set "templateFile=Customer_File.xlsx"

rem Calculate date 7 days in the future using PowerShell
for /f "delims=" %%a in ('powershell -Command "(Get-Date).AddDays(7).ToString('yyyy-MM-dd')"') do set "futureDate=%%a"
echo Debug: Future date is %futureDate% >> "%~dp0debug.txt"

rem Define file names
set "sourceXlsx=Customer_File_*.xlsx"
set "newFileName=Customer_File_%futureDate%.xlsx"

rem Create log file
set "logfile=%~dp0operations_log.txt"
echo Starting process at %date% %time% > "%logfile%"

rem Create PowerShell script with Excel cleanup and correct date handling
set "psfile=%~dp0excel2csv.ps1"
echo $ErrorActionPreference = 'Stop' > "%psfile%"
echo Write-Host "Cleaning up existing Excel processes..." >> "%psfile%"
echo Get-Process excel -ErrorAction SilentlyContinue ^| Stop-Process -Force >> "%psfile%"
echo Start-Sleep -Seconds 2 >> "%psfile%"
echo Write-Host "Starting Excel conversion..." >> "%psfile%"
echo try { >> "%psfile%"
echo     $excelFile = Get-ChildItem -Path '%sourcePath%' -Filter 'Customer_File_*.xlsx' ^| Sort-Object LastWriteTime -Descending ^| Select-Object -First 1 >> "%psfile%"
echo     if ($null -eq $excelFile) { >> "%psfile%"
echo         Write-Host "No Excel files found in source directory" >> "%psfile%"
echo         exit 1 >> "%psfile%"
echo     } >> "%psfile%"
echo     Write-Host "Found file: $($excelFile.FullName)" >> "%psfile%"
echo     # Extract the date from the original filename >> "%psfile%"
echo     $originalDate = $excelFile.Name -replace 'Customer_File_(.+)\.xlsx','$1' >> "%psfile%"
echo     $csvFileName = "Customer_File_${originalDate}.csv" >> "%psfile%"
echo     Write-Host "Using original date for CSV: $csvFileName" >> "%psfile%"
echo     $excel = New-Object -ComObject Excel.Application >> "%psfile%"
echo     $excel.Visible = $false >> "%psfile%"
echo     $excel.DisplayAlerts = $false >> "%psfile%"
echo     Write-Host "Excel application created" >> "%psfile%"
echo     $workbook = $excel.Workbooks.Open($excelFile.FullName) >> "%psfile%"
echo     Write-Host "Workbook opened" >> "%psfile%"
echo     $csvPath = Join-Path '%destPath%' $csvFileName >> "%psfile%"
echo     Write-Host "Saving as CSV to: $csvPath" >> "%psfile%"
echo     $workbook.SaveAs($csvPath, 6) >> "%psfile%"
echo     Write-Host "Save completed" >> "%psfile%"
echo     $workbook.Close() >> "%psfile%"
echo     $excel.Quit() >> "%psfile%"
echo     [System.Runtime.Interopservices.Marshal]::ReleaseComObject($excel) >> "%psfile%"
echo     [System.GC]::Collect() >> "%psfile%"
echo     [System.GC]::WaitForPendingFinalizers() >> "%psfile%"
echo     Write-Host "Excel cleanup completed" >> "%psfile%"
echo } catch { >> "%psfile%"
echo     Write-Host "Error: $_" >> "%psfile%"
echo     if ($null -ne $excel) { >> "%psfile%"
echo         $excel.Quit() >> "%psfile%"
echo         [System.Runtime.Interopservices.Marshal]::ReleaseComObject($excel) >> "%psfile%"
echo     } >> "%psfile%"
echo     Get-Process excel -ErrorAction SilentlyContinue ^| Stop-Process -Force >> "%psfile%"
echo     exit 1 >> "%psfile%"
echo } finally { >> "%psfile%"
echo     Get-Process excel -ErrorAction SilentlyContinue ^| Stop-Process -Force >> "%psfile%"
echo } >> "%psfile%"

echo Debug: Running Excel conversion >> "%~dp0debug.txt"
powershell -ExecutionPolicy Bypass -File "%psfile%" >> "%~dp0debug.txt" 2>&1
set "CONVERTERROR=%ERRORLEVEL%"
echo Debug: Conversion complete with error level %CONVERTERROR% >> "%~dp0debug.txt"

if %CONVERTERROR% NEQ 0 (
    set "notifTitle=Error: Excel Conversion Failed"
    set "notifText=Failed to convert Excel file to CSV."
    goto shownotification
)

echo Debug: Moving Excel file to archive >> "%~dp0debug.txt"
robocopy "%sourcePath%" "%archivePath%" "%sourceXlsx%" /MOV /R:3 /W:5 /LOG+:"%logfile%" /TEE /NP
set "MOVEERROR=%ERRORLEVEL%"
echo Debug: Move complete with error level %MOVEERROR% >> "%~dp0debug.txt"

echo Debug: Checking template file exists >> "%~dp0debug.txt"
if not exist "%templatePath%\%templateFile%" (
    set "notifTitle=Error: Template Not Found"
    set "notifText=Template file could not be found."
    goto shownotification
)

echo Debug: Copying template to source folder >> "%~dp0debug.txt"
copy "%templatePath%\%templateFile%" "%sourcePath%\%newFileName%" /Y
if errorlevel 1 (
    set "notifTitle=Error: Template Copy Failed"
    set "notifText=Failed to copy template file."
    goto shownotification
)
echo Debug: Template copied successfully >> "%~dp0debug.txt"

rem Set success notification
set "notifTitle=Success: File Processing Complete"
set "notifText=All operations completed successfully.!lineBreak!!lineBreak!- Excel file converted to CSV!lineBreak!- Original file moved to archive!lineBreak!- New template created: %newFileName%"

:shownotification
echo Debug: Creating notification script >> "%~dp0debug.txt"
set "lineBreak=$([char]0x000A)"
set "notifyScript=%~dp0notify.ps1"

(
echo Add-Type -AssemblyName System.Windows.Forms
echo $notification = New-Object System.Windows.Forms.NotifyIcon
echo $notification.Icon = [System.Drawing.SystemIcons]::Information
echo $notification.BalloonTipIcon = [System.Windows.Forms.ToolTipIcon]::Info
echo $notification.BalloonTipTitle = "%notifTitle%"
echo $notification.BalloonTipText = "%notifText%"
echo $notification.Visible = $true
echo $notification.ShowBalloonTip(5000^)
echo Start-Sleep -Seconds 6
echo $notification.Visible = $false
echo $notification.Dispose(^)
) > "%notifyScript%"

echo Debug: Showing notification >> "%~dp0debug.txt"
powershell -ExecutionPolicy Bypass -File "%notifyScript%" >> "%~dp0debug.txt" 2>&1

rem Clean up
if exist "%psfile%" del "%psfile%"
if exist "%notifyScript%" del "%notifyScript%"

echo Process completed at %date% %time% >> "%logfile%"
echo Debug: Script completed >> "%~dp0debug.txt"

exit /b 0