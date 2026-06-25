<#
.SYNOPSIS
    Creates a historical snapshot of campaign runs with port usage analysis.

.DESCRIPTION
    This script retrieves historical campaign run data from the Cyara API and generates
    a comprehensive report showing port usage trends, peaks, and statistics by campaign.
    Supports both Pulse and Cruncher plan types with appropriate timeframe options.
#>

$ErrorActionPreference = 'Continue'
$API_BASE_URL = "https://cyaraportal.us/cyarawebapi"
$timestamp = Get-Date -Format 'yyyyMMdd_HHmmss'
$OUTPUT_FILE = "campaign_history_snapshot_$timestamp.txt"
$CSV_OUTPUT_FILE = "campaign_history_snapshot_$timestamp.csv"
$HTML_OUTPUT_FILE = "campaign_history_chart_$timestamp.html"

# ─── Banner ───────────────────────────────────────────────────────────────────
function Show-Banner {
    Write-Host "========================================================" -ForegroundColor Cyan
    Write-Host "   Campaign History Snapshot & Port Trending Tool" -ForegroundColor Cyan
    Write-Host "========================================================" -ForegroundColor Cyan
    Write-Host ""
}
Show-Banner

# ─── Credentials ──────────────────────────────────────────────────────────────
if ($env:CYARA_SESSION_ACCOUNT_ID) {
    $AccountId = $env:CYARA_SESSION_ACCOUNT_ID
    Write-Host "Using saved Account ID from this session..." -ForegroundColor Green
} else {
    $AccountId = Read-Host "Enter Account ID"
    $env:CYARA_SESSION_ACCOUNT_ID = $AccountId
}

if ($env:CYARA_SESSION_API_KEY) {
    $ApiKey = $env:CYARA_SESSION_API_KEY
    Write-Host "Using saved API Key from this session..." -ForegroundColor Green
    Write-Host ""
} else {
    $ApiKey = Read-Host "Enter API Key"
    $env:CYARA_SESSION_API_KEY = $ApiKey
    Write-Host ""
}

if ([string]::IsNullOrEmpty($AccountId) -or [string]::IsNullOrEmpty($ApiKey)) {
    Write-Host "Error: Account ID and API Key are required." -ForegroundColor Red
    exit 1
}

$headers = @{ "Authorization" = $ApiKey; "Content-Type" = "application/json" }

# ─── Plan Type ────────────────────────────────────────────────────────────────
Write-Host "Select Plan Type:" -ForegroundColor Cyan
Write-Host "  1. Pulse" -ForegroundColor White
Write-Host "  2. Cruncher" -ForegroundColor White
Write-Host ""
$planChoice = Read-Host "Enter choice (1 or 2)"

switch ($planChoice) {
    "1" { $PlanType = "Pulse" }
    "2" { $PlanType = "Cruncher" }
    default {
        Write-Host "Invalid selection. Defaulting to Pulse." -ForegroundColor Yellow
        $PlanType = "Pulse"
    }
}
Write-Host ""

# ─── Timeframe (split by plan type) ──────────────────────────────────────────
$startDateFilter = $null
$endDateFilter   = $null
$timeframeDescription = ""
$use24HourFetch  = $false  # drives API chunk strategy

if ($PlanType -eq "Pulse") {
    # ── Pulse timeframe options ──
    Write-Host "Select Timeframe for Analysis (Pulse):" -ForegroundColor Cyan
    Write-Host "  1. Last 24 hours" -ForegroundColor White
    Write-Host "  2. Last 7 days" -ForegroundColor White
    Write-Host "  3. Custom 24-hour period" -ForegroundColor White
    Write-Host ""
    $timeframeChoice = Read-Host "Enter choice (1-3)"

    switch ($timeframeChoice) {
        "1" {
            $startDateFilter  = (Get-Date).AddHours(-24).ToUniversalTime()
            $endDateFilter    = (Get-Date).ToUniversalTime()
            $timeframeDescription = "Last 24 hours"
            $use24HourFetch   = $true
        }
        "2" {
            $startDateFilter  = (Get-Date).AddDays(-7).ToUniversalTime()
            $endDateFilter    = (Get-Date).ToUniversalTime()
            $timeframeDescription = "Last 7 days"
        }
        "3" {
            Write-Host ""
            Write-Host "Enter start date/time for 24-hour period:" -ForegroundColor Cyan
            Write-Host "(Interpreted as local timezone: $([System.TimeZone]::CurrentTimeZone.StandardName))" -ForegroundColor DarkGray
            $startDateInput = Read-Host "Start date/time (MM/DD/YYYY HH:mm or MM/DD/YYYY)"
            try {
                $startDateLocal = [DateTime]::Parse($startDateInput)
                $endDateLocal   = $startDateLocal.AddHours(24)
                $startDateFilter = $startDateLocal.ToUniversalTime()
                $endDateFilter   = $endDateLocal.ToUniversalTime()
                Write-Host ""
                Write-Host "Local : $($startDateLocal.ToString('MM/dd/yyyy HH:mm')) to $($endDateLocal.ToString('MM/dd/yyyy HH:mm'))" -ForegroundColor DarkGray
                Write-Host "UTC   : $($startDateFilter.ToString('MM/dd/yyyy HH:mm')) to $($endDateFilter.ToString('MM/dd/yyyy HH:mm'))" -ForegroundColor DarkGray
                $timeframeDescription = "$($startDateLocal.ToString('MM/dd/yyyy HH:mm')) to $($endDateLocal.ToString('MM/dd/yyyy HH:mm')) (local)"
                $use24HourFetch = $true
            } catch {
                Write-Host "Invalid date format. Defaulting to last 24 hours." -ForegroundColor Yellow
                $startDateFilter  = (Get-Date).AddHours(-24).ToUniversalTime()
                $endDateFilter    = (Get-Date).ToUniversalTime()
                $timeframeDescription = "Last 24 hours (default)"
                $use24HourFetch   = $true
            }
        }
        default {
            Write-Host "Invalid selection. Defaulting to Last 7 days." -ForegroundColor Yellow
            $startDateFilter  = (Get-Date).AddDays(-7).ToUniversalTime()
            $endDateFilter    = (Get-Date).ToUniversalTime()
            $timeframeDescription = "Last 7 days"
        }
    }

} else {
    # ── Cruncher timeframe options ──
    Write-Host "Select Timeframe for Analysis (Cruncher):" -ForegroundColor Cyan
    Write-Host "  1. Last 24 hours" -ForegroundColor White
    Write-Host "  2. Last 7 days" -ForegroundColor White
    Write-Host "  3. Last 30 days" -ForegroundColor White
    Write-Host "  4. Custom time period" -ForegroundColor White
    Write-Host ""
    $timeframeChoice = Read-Host "Enter choice (1-4)"

    switch ($timeframeChoice) {
        "1" {
            $startDateFilter  = (Get-Date).AddHours(-24).ToUniversalTime()
            $endDateFilter    = (Get-Date).ToUniversalTime()
            $timeframeDescription = "Last 24 hours"
            $use24HourFetch   = $true
        }
        "2" {
            $startDateFilter  = (Get-Date).AddDays(-7).ToUniversalTime()
            $endDateFilter    = (Get-Date).ToUniversalTime()
            $timeframeDescription = "Last 7 days"
        }
        "3" {
            $startDateFilter  = (Get-Date).AddDays(-30).ToUniversalTime()
            $endDateFilter    = (Get-Date).ToUniversalTime()
            $timeframeDescription = "Last 30 days"
        }
        "4" {
            Write-Host ""
            Write-Host "Enter custom date range:" -ForegroundColor Cyan
            Write-Host "(Interpreted as local timezone: $([System.TimeZone]::CurrentTimeZone.StandardName))" -ForegroundColor DarkGray
            $startDateInput = Read-Host "Start date/time (MM/DD/YYYY HH:mm or MM/DD/YYYY)"
            $endDateInput   = Read-Host "End date/time   (MM/DD/YYYY HH:mm or MM/DD/YYYY)"
            try {
                $startDateLocal = [DateTime]::Parse($startDateInput)
                $endDateLocal   = [DateTime]::Parse($endDateInput)
                $startDateFilter = $startDateLocal.ToUniversalTime()
                $endDateFilter   = $endDateLocal.ToUniversalTime()
                # If range <= 1 day, use detailed 24-hour fetch strategy
                if (($endDateLocal - $startDateLocal).TotalDays -le 1) { $use24HourFetch = $true }
                Write-Host ""
                Write-Host "Local : $($startDateLocal.ToString('MM/dd/yyyy HH:mm')) to $($endDateLocal.ToString('MM/dd/yyyy HH:mm'))" -ForegroundColor DarkGray
                Write-Host "UTC   : $($startDateFilter.ToString('MM/dd/yyyy HH:mm')) to $($endDateFilter.ToString('MM/dd/yyyy HH:mm'))" -ForegroundColor DarkGray
                $timeframeDescription = "$($startDateLocal.ToString('MM/dd/yyyy HH:mm')) to $($endDateLocal.ToString('MM/dd/yyyy HH:mm')) (local)"
            } catch {
                Write-Host "Invalid date format. Defaulting to Last 7 days." -ForegroundColor Yellow
                $startDateFilter  = (Get-Date).AddDays(-7).ToUniversalTime()
                $endDateFilter    = (Get-Date).ToUniversalTime()
                $timeframeDescription = "Last 7 days (default)"
            }
        }
        default {
            Write-Host "Invalid selection. Defaulting to Last 7 days." -ForegroundColor Yellow
            $startDateFilter  = (Get-Date).AddDays(-7).ToUniversalTime()
            $endDateFilter    = (Get-Date).ToUniversalTime()
            $timeframeDescription = "Last 7 days"
        }
    }
}

Write-Host ""
Write-Host "Plan Type : $PlanType" -ForegroundColor Green
Write-Host "Timeframe : $timeframeDescription" -ForegroundColor Green
Write-Host ""

# ─── Fetch Campaigns ──────────────────────────────────────────────────────────
Write-Host "Fetching campaigns for plan type: $PlanType..." -ForegroundColor Cyan

$allCampaigns = @()
$pageNumber   = 1
$pageSize     = 50   # API caps at 50 per page regardless of pageSize param

try {
    do {
        # API uses 'pageno' param (not 'pageNumber') per pagination links in response
        # planType URL param is ignored by the API - we filter client-side
        $campaignsUrl = "$API_BASE_URL/v3.0/accounts/$AccountId/campaigns?pageno=$pageNumber&pageSize=$pageSize"
        $response = Invoke-RestMethod -Uri $campaignsUrl -Headers $headers -Method Get -TimeoutSec 30

        if ($response.results -and $response.results.Count -gt 0) {
            $allCampaigns += $response.results
        }

        # Use totalResults to know exactly how many pages to fetch - no over-pagination
        if ($pageNumber -eq 1 -and $response.totalResults) {
            $totalPages = [Math]::Ceiling([int]$response.totalResults / $pageSize)
            Write-Host "  Account has $($response.totalResults) total campaigns ($totalPages pages)..." -ForegroundColor DarkGray
        }

        $pageNumber++
        if (-not $response.results -or $response.results.Count -lt $pageSize) { break }
        if ($pageNumber -gt 50) { Write-Host "  Page limit reached." -ForegroundColor Yellow; break }
    } while ($true)
} catch {
    Write-Host "Error fetching campaigns: $_" -ForegroundColor Red
    exit 1
}

# Filter to selected plan type (client-side since API ignores planType param)
$uniqueCampaignList = $allCampaigns | Where-Object { $_.planType -eq $PlanType }

Write-Host "  Retrieved $($allCampaigns.Count) unique campaigns total | $($uniqueCampaignList.Count) are $PlanType" -ForegroundColor DarkGray

# Pre-filter by lastRunDate - skip campaigns whose most recent run predates our window start.
# We can only use lastRunDate as a lower-bound check: if lastRunDate < startDateFilter
# the campaign definitely didn't run in our window. We cannot use it as an upper bound
# because lastRunDate reflects the most recent run ever, not whether it ran in this window.
if ($startDateFilter) {
    $beforeFilter = $uniqueCampaignList.Count
    $uniqueCampaignList = $uniqueCampaignList | Where-Object {
        if ($_.lastRunDate) {
            try {
                $lastRun = [DateTime]::Parse($_.lastRunDate).ToUniversalTime()
                $lastRun -ge $startDateFilter
            } catch {
                $true  # Can't parse date - include to be safe
            }
        } else {
            $false  # Never run - skip
        }
    }
    $skipped = $beforeFilter - $uniqueCampaignList.Count
    Write-Host ""
    if ($skipped -gt 0) {
        Write-Host "  Skipped $skipped $PlanType campaigns with no activity in timeframe." -ForegroundColor DarkGray
    }
    Write-Host "  $($uniqueCampaignList.Count) $PlanType campaigns to analyze." -ForegroundColor Green
} else {
    Write-Host ""
    Write-Host "  $($uniqueCampaignList.Count) $PlanType campaigns to analyze." -ForegroundColor Green
}
Write-Host ""

if ($uniqueCampaignList.Count -eq 0) {
    Write-Host "No $PlanType campaigns found with activity in the selected timeframe." -ForegroundColor Yellow
    exit 0
}

# ─── Fetch Runs Per Campaign ──────────────────────────────────────────────────
Write-Host "Fetching campaign run history..." -ForegroundColor Cyan
Write-Host ""

$campaignStats = @()
$allRuns       = @()
$chartData     = @()
$totalDays     = if ($startDateFilter -and $endDateFilter) { ($endDateFilter - $startDateFilter).TotalDays } else { 7 }

$campaignIndex = 0
foreach ($campaign in $uniqueCampaignList) {
    $campaignIndex++
    Write-Host "[$campaignIndex/$($uniqueCampaignList.Count)] $($campaign.name)" -ForegroundColor White

    # Fetch campaign detail to get concurrency - not included in the campaigns list response
    $campaignPorts = 1
    try {
        $detailUrl = "$API_BASE_URL/v3.0/accounts/$AccountId/campaigns/$($campaign.campaignId)"
        $campaignDetail = Invoke-RestMethod -Uri $detailUrl -Headers $headers -Method Get -TimeoutSec 10
        if ($campaignDetail.concurrency) { $campaignPorts = [int]$campaignDetail.concurrency }
        Write-Host "  Concurrency: $campaignPorts" -ForegroundColor DarkGray
    } catch {
        Write-Host "  Warning: Could not fetch campaign detail, defaulting concurrency to 1" -ForegroundColor Yellow
    }

    $runs = @()

    if ($startDateFilter -and $endDateFilter) {
        if ($use24HourFetch) {
            # ── 24-hour strategy: 1-hour chunks, up to 20 pages each ──
            $chunkHours  = 1
            $maxPages    = 20
            $currentStart = $startDateFilter
            $chunkNum    = 1

            while ($currentStart -lt $endDateFilter) {
                $currentEnd = $currentStart.AddHours($chunkHours)
                if ($currentEnd -gt $endDateFilter) { $currentEnd = $endDateFilter }

                Write-Host "  Chunk $chunkNum ($($currentStart.ToString('HH:mm'))-$($currentEnd.ToString('HH:mm')) UTC)..." -ForegroundColor DarkGray

                $fromStr = [System.Web.HttpUtility]::UrlEncode($currentStart.ToString("yyyy-MM-ddTHH:mm:ssZ"))
                $toStr   = [System.Web.HttpUtility]::UrlEncode($currentEnd.ToString("yyyy-MM-ddTHH:mm:ssZ"))
                $baseUrl = "$API_BASE_URL/v3.0/accounts/$AccountId/campaigns/$($campaign.campaignId)/runs?fromDate=$fromStr&toDate=$toStr&pageSize=100"

                $pageNum = 1
                do {
                    try {
                        $resp = Invoke-RestMethod -Uri "$baseUrl&pageNumber=$pageNum" -Headers $headers -Method Get -TimeoutSec 30
                        if ($resp.results -and $resp.results.Count -gt 0) {
                            $runs += $resp.results
                            if ($resp.results.Count -lt 100) { break }
                        } else { break }
                    } catch { Write-Host "    Error: $_" -ForegroundColor Red; break }
                    $pageNum++
                    if ($pageNum -gt $maxPages) { Write-Host "    Page limit reached for this chunk." -ForegroundColor Yellow; break }
                } while ($true)

                $currentStart = $currentEnd
                $chunkNum++
            }
        } else {
            # ── Multi-day strategy: 1-day chunks ──
            # Fetches first 2 pages (run starts) AND last page (run ends) per day
            # so we capture both when runs began and when they ended even for
            # high-volume campaigns with thousands of calls per day.
            $chunkDays    = 1
            $currentStart = $startDateFilter
            $chunkNum     = 1

            while ($currentStart -lt $endDateFilter) {
                $currentEnd = $currentStart.AddDays($chunkDays)
                if ($currentEnd -gt $endDateFilter) { $currentEnd = $endDateFilter }

                Write-Host "  Day $chunkNum ($($currentStart.ToString('MM/dd')) UTC)..." -ForegroundColor DarkGray

                $fromStr = [System.Web.HttpUtility]::UrlEncode($currentStart.ToString("yyyy-MM-ddTHH:mm:ssZ"))
                $toStr   = [System.Web.HttpUtility]::UrlEncode($currentEnd.ToString("yyyy-MM-ddTHH:mm:ssZ"))
                $baseUrl = "$API_BASE_URL/v3.0/accounts/$AccountId/campaigns/$($campaign.campaignId)/runs?fromDate=$fromStr&toDate=$toStr&pageSize=100"

                # Fetch first 2 pages to get run start times
                $pageNum = 1
                $totalPagesInChunk = $null
                do {
                    try {
                        $resp = Invoke-RestMethod -Uri "$baseUrl&pageNumber=$pageNum" -Headers $headers -Method Get -TimeoutSec 30
                        if ($resp.results -and $resp.results.Count -gt 0) {
                            $runs += $resp.results
                            # Capture total pages from first response
                            if ($null -eq $totalPagesInChunk -and $resp.totalResults) {
                                $totalPagesInChunk = [Math]::Ceiling([int]$resp.totalResults / 100)
                            }
                            if ($resp.results.Count -lt 100) { break }
                        } else { break }
                    } catch { Write-Host "    Error: $_" -ForegroundColor Red; break }
                    $pageNum++
                    if ($pageNum -gt 2) { break }
                } while ($true)

                # Also fetch the last page to capture run end times
                # (high-volume runs have thousands of calls; end times are on the last page)
                if ($totalPagesInChunk -and $totalPagesInChunk -gt 2) {
                    try {
                        $lastResp = Invoke-RestMethod -Uri "$baseUrl&pageNumber=$totalPagesInChunk" -Headers $headers -Method Get -TimeoutSec 30
                        if ($lastResp.results -and $lastResp.results.Count -gt 0) {
                            $runs += $lastResp.results
                        }
                    } catch { Write-Host "    Error fetching last page: $_" -ForegroundColor Red }
                }

                $currentStart = $currentEnd
                $chunkNum++
            }
        }
    } else {
        # No date filter - single page sample
        $runsUrl = "$API_BASE_URL/v3.0/accounts/$AccountId/campaigns/$($campaign.campaignId)/runs?pageSize=100&pageNumber=1"
        try {
            $resp = Invoke-RestMethod -Uri $runsUrl -Headers $headers -Method Get -TimeoutSec 30
            if ($resp.results) { $runs += $resp.results }
        } catch { Write-Host "  Error fetching runs: $_" -ForegroundColor Red; continue }
    }

    Write-Host "  Fetched $($runs.Count) call records" -ForegroundColor DarkGray

    if ($runs.Count -eq 0) { continue }

    # Group individual calls into campaign runs by campaignRunId
    $runGroups = $runs | Group-Object -Property { $_.campaignRunId }

    $runSummaries = @()
    foreach ($group in $runGroups) {
        $calls = $group.Group
        $ports = $campaignPorts

        if ($PlanType -eq "Pulse") {
            # ── Pulse: original logic - use Measure-Object, skip runs with no endDate ──
            try {
                $runStartTime = ($calls | ForEach-Object {
                    if ($_.startDate) { [DateTime]::Parse($_.startDate) }
                } | Measure-Object -Minimum).Minimum

                $runEndTime = ($calls | ForEach-Object {
                    if ($_.endDate) { [DateTime]::Parse($_.endDate) }
                } | Measure-Object -Maximum).Maximum

                if ($runStartTime -and $runEndTime) {
                    $runSummaries += [PSCustomObject]@{
                        CampaignRunId  = $group.Name
                        CampaignName   = $campaign.name
                        CampaignId     = $campaign.campaignId
                        StartTimeUTC   = $runStartTime.ToUniversalTime()
                        EndTimeUTC     = $runEndTime.ToUniversalTime()
                        StartTimeLocal = $runStartTime
                        EndTimeLocal   = $runEndTime
                        Ports          = $ports
                        CallCount      = $calls.Count
                    }

                    $chartData += [PSCustomObject]@{
                        CampaignName = $campaign.name
                        StartTime    = $runStartTime
                        EndTime      = $runEndTime
                        Ports        = $ports
                    }
                }
            } catch {
                # Skip run groups with unparseable dates
            }

        } else {
            # ── Cruncher: get run start/end times ──
            # Strategy 1: use numeric runId from call record to fetch run summary endpoint
            # Strategy 2: fall back to call-level min startDate / max endDate
            # Strategy 3: if no endDates available, cap at endDateFilter (still running)
            $runStartUTC = $null
            $runEndUTC   = $null

            # Try run summary endpoint using numeric runId from call records
            $numericRunId = ($calls | Select-Object -First 1).runId
            if ($numericRunId) {
                $runSummaryUrl = "$API_BASE_URL/v3.0/accounts/$AccountId/campaigns/$($campaign.campaignId)/runs/$numericRunId"
                try {
                    $runSummary = Invoke-RestMethod -Uri $runSummaryUrl -Headers $headers -Method Get -TimeoutSec 15
                    if ($runSummary.startDate) {
                        $runStartUTC = [DateTime]::Parse($runSummary.startDate).ToUniversalTime()
                    }
                    if ($runSummary.endDate) {
                        $runEndUTC = [DateTime]::Parse($runSummary.endDate).ToUniversalTime()
                        if ($runEndUTC -gt $endDateFilter) { $runEndUTC = $endDateFilter }
                    } else {
                        $runEndUTC = $endDateFilter  # Still running
                    }
                } catch {}
            }

            # Fall back to call-level dates if run summary unavailable or runId missing
            if (-not $runStartUTC) {
                $parsedStarts = @()
                $parsedEnds   = @()
                foreach ($c in $calls) {
                    if ($c.startDate) { try { $parsedStarts += [DateTime]::Parse($c.startDate).ToUniversalTime() } catch {} }
                    if ($c.endDate)   { try { $parsedEnds   += [DateTime]::Parse($c.endDate).ToUniversalTime()   } catch {} }
                }
                if ($parsedStarts.Count -eq 0) { continue }
                $runStartUTC = ($parsedStarts | Measure-Object -Minimum).Minimum
                if ($parsedEnds.Count -gt 0) {
                    $runEndUTC = ($parsedEnds | Measure-Object -Maximum).Maximum
                    if ($runEndUTC -gt $endDateFilter) { $runEndUTC = $endDateFilter }
                } else {
                    $runEndUTC = $endDateFilter  # No end dates - still running
                }
            }

            if (-not $runStartUTC) { continue }

            $startUTC = $runStartUTC
            $endUTC   = $runEndUTC

            $startLocal = $startUTC.ToLocalTime()
            $endLocal   = $endUTC.ToLocalTime()

            Write-Host "    Run $runId : $($startLocal.ToString('MM/dd HH:mm')) -> $($endLocal.ToString('MM/dd HH:mm')) local | Ports: $ports" -ForegroundColor DarkCyan

            $runSummaries += [PSCustomObject]@{
                CampaignRunId  = $group.Name
                CampaignName   = $campaign.name
                CampaignId     = $campaign.campaignId
                StartTimeUTC   = $startUTC
                EndTimeUTC     = $endUTC
                StartTimeLocal = $startLocal
                EndTimeLocal   = $endLocal
                Ports          = $ports
                CallCount      = $calls.Count
            }

            $chartData += [PSCustomObject]@{
                CampaignName = $campaign.name
                StartTime    = $startLocal
                EndTime      = $endLocal
                Ports        = $ports
            }
        }
    }

    $allRuns += $runSummaries

    if ($runSummaries.Count -gt 0) {
        $maxPorts  = ($runSummaries | Measure-Object -Property Ports -Maximum).Maximum
        $minPorts  = ($runSummaries | Measure-Object -Property Ports -Minimum).Minimum
        $avgPorts  = [Math]::Round(($runSummaries | Measure-Object -Property Ports -Average).Average, 1)
        $firstRun  = ($runSummaries | Sort-Object StartTimeLocal | Select-Object -First 1).StartTimeLocal
        $lastRun   = ($runSummaries | Sort-Object StartTimeLocal | Select-Object -Last 1).StartTimeLocal

        $campaignStats += [PSCustomObject]@{
            CampaignName  = $campaign.name
            CampaignId    = $campaign.campaignId
            TotalRuns     = $runSummaries.Count
            MaxPorts      = $maxPorts
            MinPorts      = $minPorts
            AvgPorts      = $avgPorts
            FirstRun      = $firstRun
            LastRun       = $lastRun
        }

        Write-Host "  Runs: $($runSummaries.Count) | Ports: $maxPorts max" -ForegroundColor DarkGray
    }
}

Write-Host ""
Write-Host "Processing complete. Building reports..." -ForegroundColor Cyan
Write-Host ""

# ─── Summary Stats ────────────────────────────────────────────────────────────
$totalCampaigns   = $campaignStats.Count
$peakConcurrent   = 0
$campaignStats    = $campaignStats | Sort-Object -Property MaxPorts -Descending

# ─── Text Report ─────────────────────────────────────────────────────────────
$report = @"
========================================================
   Campaign History Snapshot & Port Trending Tool
========================================================
Generated  : $(Get-Date -Format 'MM/dd/yyyy HH:mm:ss') (Local)
Account ID : $AccountId
Plan Type  : $PlanType
Timeframe  : $timeframeDescription
Campaigns  : $totalCampaigns analyzed

CAMPAIGN STATISTICS (sorted by max ports desc)
--------------------------------------------------------
"@

foreach ($stat in $campaignStats) {
    $report += @"

Campaign   : $($stat.CampaignName)
ID         : $($stat.CampaignId)
Runs       : $($stat.TotalRuns)
Ports      : Max=$($stat.MaxPorts)  Min=$($stat.MinPorts)  Avg=$($stat.AvgPorts)
First Run  : $($stat.FirstRun.ToString('MM/dd/yyyy HH:mm'))
Last Run   : $($stat.LastRun.ToString('MM/dd/yyyy HH:mm'))
"@
}

$report | Out-File -FilePath $OUTPUT_FILE -Encoding UTF8
Write-Host "Text report : $OUTPUT_FILE" -ForegroundColor Green

# ─── CSV Export ───────────────────────────────────────────────────────────────
$campaignStats | Export-Csv -Path $CSV_OUTPUT_FILE -NoTypeInformation
Write-Host "CSV export  : $CSV_OUTPUT_FILE" -ForegroundColor Green

# ─── Chart Data ───────────────────────────────────────────────────────────────
Write-Host "Building interactive chart..." -ForegroundColor Cyan

$timelineJson = "{}"

if ($chartData.Count -gt 0) {

    $colors = @('#667eea','#f56565','#48bb78','#ed8936','#4299e1',
                 '#9f7aea','#f687b3','#38b2ac','#ecc94b','#fc8181',
                 '#68d391','#f6ad55','#63b3ed','#b794f4','#f9a8d4')
    $dashPatterns = @(@(5,5),@(10,5),@(15,5),@(5,10),@(10,10))
    $colorIndex = 0
    $datasets   = @()

    if ($PlanType -eq "Pulse") {
        # ── Pulse: original chart logic ──────────────────────────────────────
        $minTime = ($chartData | Measure-Object -Property StartTime -Minimum).Minimum
        $maxTime = ($chartData | Measure-Object -Property EndTime   -Maximum).Maximum
        $timeSpan = $maxTime - $minTime

        if ($timeSpan.TotalHours -le 24) {
            $intervalMinutes = 15
            $startMinute = [Math]::Floor($minTime.Minute / 15) * 15
            $currentTime = Get-Date -Year $minTime.Year -Month $minTime.Month -Day $minTime.Day -Hour $minTime.Hour -Minute $startMinute -Second 0 -Millisecond 0
        } elseif ($timeSpan.TotalHours -le 48) {
            $intervalMinutes = 60
            $currentTime = Get-Date -Year $minTime.Year -Month $minTime.Month -Day $minTime.Day -Hour $minTime.Hour -Minute 0 -Second 0 -Millisecond 0
        } else {
            $intervalMinutes = 1440
            $currentTime = Get-Date -Year $minTime.Year -Month $minTime.Month -Day $minTime.Day -Hour 0 -Minute 0 -Second 0 -Millisecond 0
        }

        $sampleTimes = @()
        while ($currentTime -le $maxTime) {
            if ($currentTime -ge $minTime) { $sampleTimes += $currentTime }
            $currentTime = $currentTime.AddMinutes($intervalMinutes)
        }

        $uniqueCampaigns = $chartData | Select-Object -ExpandProperty CampaignName -Unique | Sort-Object

        foreach ($camp in $uniqueCampaigns) {
            $campRuns = $chartData | Where-Object { $_.CampaignName -eq $camp }
            $color    = $colors[$colorIndex % $colors.Count]
            $dash     = $dashPatterns[$colorIndex % $dashPatterns.Count]
            $colorIndex++

            $dataPoints = @()
            for ($i = 0; $i -lt $sampleTimes.Count; $i++) {
                $currentSample = $sampleTimes[$i]
                $nextSample = if ($i -lt $sampleTimes.Count - 1) { $sampleTimes[$i+1] } else { $maxTime.AddMinutes(1) }
                $maxPortsInPeriod = 0
                foreach ($run in $campRuns) {
                    if ($run.StartTime -lt $nextSample -and $run.EndTime -gt $currentSample) {
                        if ($run.Ports -gt $maxPortsInPeriod) { $maxPortsInPeriod = $run.Ports }
                    }
                }
                $dataPoints += $maxPortsInPeriod
            }

            $datasets += @{
                label           = $camp
                data            = $dataPoints
                borderColor     = $color
                backgroundColor = "transparent"
                borderWidth     = 2
                borderDash      = $dash
                pointRadius     = 2
                tension         = 0
                stepped         = 'before'
            }
        }

        # Pulse concurrent total: check peak at every moment within each sample period
        $concurrentDataPoints = @()
        for ($i = 0; $i -lt $sampleTimes.Count; $i++) {
            $currentSample = $sampleTimes[$i]
            $nextSample = if ($i -lt $sampleTimes.Count - 1) { $sampleTimes[$i+1] } else { $maxTime.AddMinutes(1) }

            $momentsInPeriod = @($currentSample)
            foreach ($run in $chartData) {
                if ($run.StartTime -ge $currentSample -and $run.StartTime -lt $nextSample) { $momentsInPeriod += $run.StartTime }
                if ($run.EndTime   -ge $currentSample -and $run.EndTime   -lt $nextSample) { $momentsInPeriod += $run.EndTime }
            }

            $maxConcurrentInPeriod = 0
            foreach ($moment in $momentsInPeriod) {
                $concurrentAtMoment = 0
                foreach ($run in $chartData) {
                    if ($moment -ge $run.StartTime -and $moment -le $run.EndTime) { $concurrentAtMoment += $run.Ports }
                }
                if ($concurrentAtMoment -gt $maxConcurrentInPeriod) { $maxConcurrentInPeriod = $concurrentAtMoment }
            }
            $concurrentDataPoints += $maxConcurrentInPeriod
            if ($maxConcurrentInPeriod -gt $peakConcurrent) { $peakConcurrent = $maxConcurrentInPeriod }
        }

        if ($intervalMinutes -ge 1440) {
            $timeLabels = $sampleTimes | ForEach-Object { $_.ToString('MM/dd/yyyy') }
        } else {
            $timeLabels = $sampleTimes | ForEach-Object { $_.ToString('MM/dd HH:mm') }
        }

    } else {
        # ── Cruncher: per-campaign dataset sums, no double-counting ──────────
        # Always anchor to the full query window so the x-axis reflects the
        # requested timeframe, not just when runs happened.
        if ($use24HourFetch) {
            $sampleMinutes = 15   # 15-min for 24h = 96 data points, matches Pulse
        } elseif ($totalDays -le 7) {
            $sampleMinutes = 60
        } else {
            $sampleMinutes = 360
        }

        # Use query window boundaries, not chart data boundaries
        $chartStart = $startDateFilter.ToLocalTime()
        $chartEnd   = $endDateFilter.ToLocalTime()

        # Round start down to nearest interval boundary for clean labels
        $startMinute = [Math]::Floor($chartStart.Minute / $sampleMinutes) * $sampleMinutes
        $alignedStart = Get-Date -Year $chartStart.Year -Month $chartStart.Month -Day $chartStart.Day `
                                  -Hour $chartStart.Hour -Minute $startMinute -Second 0 -Millisecond 0

        $sampleTimes = @()
        $t = $alignedStart
        while ($t -le $chartEnd) { $sampleTimes += $t; $t = $t.AddMinutes($sampleMinutes) }

        $uniqueCampaigns = $chartData | Select-Object -ExpandProperty CampaignName -Unique

        foreach ($camp in $uniqueCampaigns) {
            $campRuns = $chartData | Where-Object { $_.CampaignName -eq $camp }
            $color    = $colors[$colorIndex % $colors.Count]
            $colorIndex++

            $dataPoints = @()
            for ($i = 0; $i -lt $sampleTimes.Count; $i++) {
                $currentSample = $sampleTimes[$i]
                $nextSample = if ($i -lt $sampleTimes.Count - 1) { $sampleTimes[$i+1] } else { $currentSample.AddMinutes($sampleMinutes) }
                $portsAtTime = 0
                foreach ($run in $campRuns) {
                    # Run overlaps this period if it starts before the period ends AND ends after the period starts
                    if ($run.StartTime -lt $nextSample -and $run.EndTime -gt $currentSample) {
                        $portsAtTime = $run.Ports; break
                    }
                }
                $dataPoints += $portsAtTime
            }

            $datasets += @{
                label           = $camp
                data            = $dataPoints
                borderColor     = $color
                backgroundColor = "transparent"
                borderWidth     = 2
                pointRadius     = 4
                pointHoverRadius = 6
                tension         = 0
                stepped         = 'before'
            }
        }

        # Cruncher concurrent total: sum per-campaign values (not raw runs)
        $concurrentDataPoints = @()
        for ($i = 0; $i -lt $sampleTimes.Count; $i++) {
            $total = 0
            foreach ($ds in $datasets) { $total += $ds.data[$i] }
            $concurrentDataPoints += $total
            if ($total -gt $peakConcurrent) { $peakConcurrent = $total }
        }

        if ($sampleMinutes -ge 1440) {
            $timeLabels = $sampleTimes | ForEach-Object { $_.ToString('MM/dd/yyyy') }
        } else {
            $timeLabels = $sampleTimes | ForEach-Object { $_.ToString('MM/dd HH:mm') }
        }
    }

    $datasets += @{
        label           = "Total Concurrent Ports"
        data            = $concurrentDataPoints
        borderColor     = "#000000"
        backgroundColor = "rgba(0,0,0,0.05)"
        borderWidth     = 3
        pointRadius     = 2
        tension         = 0
        stepped         = 'before'
        fill            = $false
    }

    $timelineJson = @{ labels = $timeLabels; datasets = $datasets } | ConvertTo-Json -Depth 10 -Compress
}

# ─── HTML Report ─────────────────────────────────────────────────────────────
$statsTableRows = ""
foreach ($stat in $campaignStats) {
    $statsTableRows += "<tr><td>$($stat.CampaignName)</td><td>$($stat.TotalRuns)</td><td>$($stat.MaxPorts)</td><td>$($stat.MinPorts)</td><td>$($stat.AvgPorts)</td><td>$($stat.LastRun.ToString('MM/dd HH:mm'))</td></tr>`n"
}

$html = @"
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<title>Campaign History - $PlanType - $timeframeDescription</title>
<script src="https://cdn.jsdelivr.net/npm/chart.js@4.4.0/dist/chart.umd.min.js"></script>
<style>
  body { font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', sans-serif; margin: 0; background: #f7f8fc; color: #333; }
  .header { background: linear-gradient(135deg,#667eea,#764ba2); color: #fff; padding: 24px 32px; }
  .header h1 { margin: 0 0 6px; font-size: 1.6rem; }
  .header p  { margin: 0; opacity: .85; font-size: .95rem; }
  .meta { display: flex; gap: 24px; margin-top: 14px; flex-wrap: wrap; }
  .meta span { background: rgba(255,255,255,.2); border-radius: 6px; padding: 4px 12px; font-size: .85rem; }
  .content { padding: 24px 32px; }
  .card { background: #fff; border-radius: 10px; box-shadow: 0 2px 8px rgba(0,0,0,.08); margin-bottom: 24px; padding: 20px; }
  .card h2 { margin: 0 0 16px; font-size: 1.1rem; color: #555; border-bottom: 1px solid #eee; padding-bottom: 10px; }
  canvas { max-height: 420px; }
  table { width: 100%; border-collapse: collapse; font-size: .88rem; }
  th { background: #f0f0f5; padding: 8px 12px; text-align: left; font-weight: 600; }
  td { padding: 7px 12px; border-bottom: 1px solid #f0f0f0; }
  tr:hover td { background: #f9f9ff; }
</style>
</head>
<body>
<div class="header">
  <h1>Campaign History Snapshot</h1>
  <p>Port usage over time for $PlanType campaigns</p>
  <div class="meta">
    <span>Plan: $PlanType</span>
    <span>Timeframe: $timeframeDescription</span>
    <span>Campaigns: $totalCampaigns</span>
    <span>Peak Concurrent Ports: $peakConcurrent</span>
    <span>Generated: $(Get-Date -Format 'MM/dd/yyyy HH:mm')</span>
  </div>
</div>
<div class="content">
  <div class="card">
    <h2>Port Usage Timeline</h2>
    <canvas id="timelineChart"></canvas>
  </div>
  <div class="card">
    <h2>Campaign Statistics</h2>
    <table>
      <thead><tr><th>Campaign</th><th>Runs</th><th>Max Ports</th><th>Min Ports</th><th>Avg Ports</th><th>Last Run</th></tr></thead>
      <tbody>$statsTableRows</tbody>
    </table>
  </div>
</div>
<script>
const data = $timelineJson;
if (data && data.labels) {
  new Chart(document.getElementById('timelineChart'), {
    type: 'line',
    data: data,
    options: {
      responsive: true,
      animation: false,
      interaction: { mode: 'index', intersect: false },
      plugins: {
        legend: { position: 'bottom', labels: { boxWidth: 12, font: { size: 11 } } },
        tooltip: { callbacks: { title: ctx => ctx[0].label } }
      },
      scales: {
        x: { ticks: { maxTicksLimit: 24, font: { size: 10 } }, grid: { display: false } },
        y: { beginAtZero: true, title: { display: true, text: 'Concurrent Ports' } }
      }
    }
  });
}
</script>
</body>
</html>
"@

$html | Out-File -FilePath $HTML_OUTPUT_FILE -Encoding UTF8
Write-Host "HTML chart  : $HTML_OUTPUT_FILE" -ForegroundColor Green
Write-Host ""

# ─── Console Summary ──────────────────────────────────────────────────────────
Write-Host "========================================================" -ForegroundColor Cyan
Write-Host "  SUMMARY" -ForegroundColor Cyan
Write-Host "========================================================" -ForegroundColor Cyan
Write-Host "  Plan Type            : $PlanType" -ForegroundColor White
Write-Host "  Timeframe            : $timeframeDescription" -ForegroundColor White
Write-Host "  Campaigns Analyzed   : $totalCampaigns" -ForegroundColor White
Write-Host "  Peak Concurrent Ports: $peakConcurrent" -ForegroundColor White
Write-Host ""
Write-Host "  Top Campaigns by Port Usage:" -ForegroundColor Cyan
$campaignStats | Select-Object -First 10 | ForEach-Object {
    Write-Host ("    {0,-40} Max:{1,4}  Runs:{2,5}" -f $_.CampaignName, $_.MaxPorts, $_.TotalRuns) -ForegroundColor White
}
Write-Host ""
Write-Host "Output files written to current directory." -ForegroundColor Green
Write-Host ""

# ─── Run Another Analysis ─────────────────────────────────────────────────────
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Run another analysis?" -ForegroundColor Cyan
Write-Host "  1. Same account" -ForegroundColor White
Write-Host "  2. Different account" -ForegroundColor White
Write-Host "  3. Exit" -ForegroundColor White
Write-Host ""
$restartChoice = Read-Host "Enter choice (1-3)"

switch ($restartChoice) {
    "1" {
        Write-Host ""
        Write-Host "Restarting with same account..." -ForegroundColor Green
        Write-Host ""
        & $PSCommandPath
        exit 0
    }
    "2" {
        Write-Host ""
        Write-Host "Clearing account credentials..." -ForegroundColor Yellow
        $env:CYARA_SESSION_ACCOUNT_ID = $null
        $env:CYARA_SESSION_API_KEY = $null
        Write-Host "Restarting..." -ForegroundColor Green
        Write-Host ""
        & $PSCommandPath
        exit 0
    }
    default {
        Write-Host ""
        Write-Host "Thank you!" -ForegroundColor Green
    }
}
