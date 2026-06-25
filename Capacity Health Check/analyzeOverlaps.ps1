<#
.SYNOPSIS
    Analyzes Pulse campaign overlaps and port usage.

.DESCRIPTION
    This script retrieves Pulse campaign data from the Cyara API, analyzes scheduled run times,
    and identifies overlaps that may exceed port capacity limits.

.PARAMETER AccountId
    The Cyara account ID.

.PARAMETER ApiKey
    The Cyara API key in format: ApiKey key:secret

.EXAMPLE
    .\campaignOverlapAnalysis.ps1
    Runs interactively with prompts for credentials and optional test campaign.
#>

param(
    [Parameter(Mandatory=$false)]
    [string]$AccountId,
    
    [Parameter(Mandatory=$false)]
    [string]$ApiKey
)

$ErrorActionPreference = 'Continue'
$API_BASE_URL = "https://cyaraportal.us/cyarawebapi"
$OUTPUT_FILE_VERBOSE = "campaign_overlaps_detailed.txt"
$OUTPUT_FILE_SUMMARY = "campaign_overlaps_summary.txt"
$CAMPAIGNS_JSON = "temp_campaigns.json"
$DEFAULT_CALL_DURATION_MINUTES = 0.5  # 30 seconds default
$MAX_PORTS = 9  # Maximum allowed ports

# Prompt for credentials if not provided
if ([string]::IsNullOrEmpty($AccountId)) {
    # Check if credentials are stored in environment variables from previous run
    if ($env:CYARA_SESSION_ACCOUNT_ID) {
        $AccountId = $env:CYARA_SESSION_ACCOUNT_ID
        Write-Host "================================================" -ForegroundColor Cyan
        Write-Host "   Campaign Port Overlap Analysis Tool" -ForegroundColor Cyan
        Write-Host "================================================" -ForegroundColor Cyan
        Write-Host ""
        Write-Host "Using saved credentials from this session..." -ForegroundColor Green
        Write-Host ""
    } else {
        Write-Host "================================================" -ForegroundColor Cyan
        Write-Host "   Campaign Port Overlap Analysis Tool" -ForegroundColor Cyan
        Write-Host "================================================" -ForegroundColor Cyan
        Write-Host ""
        $AccountId = Read-Host "Enter Account ID"
    }
}

if ([string]::IsNullOrEmpty($ApiKey)) {
    # Check if credentials are stored in environment variables from previous run
    if ($env:CYARA_SESSION_API_KEY) {
        $ApiKey = $env:CYARA_SESSION_API_KEY
    } else {
        if ([string]::IsNullOrEmpty($AccountId) -and -not $env:CYARA_SESSION_ACCOUNT_ID) {
            Write-Host "================================================" -ForegroundColor Cyan
            Write-Host "   Campaign Port Overlap Analysis Tool" -ForegroundColor Cyan
            Write-Host "================================================" -ForegroundColor Cyan
            Write-Host ""
        }
        $ApiKey = Read-Host "API Key"
        Write-Host ""
    }
}

if ([string]::IsNullOrEmpty($AccountId)) {
    Write-Host "Error: Account ID is required" -ForegroundColor Red
    exit 1
}

if ([string]::IsNullOrEmpty($ApiKey)) {
    Write-Host "Error: API Key is required" -ForegroundColor Red
    exit 1
}

# Store credentials in environment variables for this session
$env:CYARA_SESSION_ACCOUNT_ID = $AccountId
$env:CYARA_SESSION_API_KEY = $ApiKey

# Fetch max ports from the API based on plan concurrency
Write-Host "Fetching plan information..." -ForegroundColor Cyan
$headers = @{
    'accept' = 'application/json'
    'Authorization' = $ApiKey
}

try {
    # Get all plans
    $plansUrl = "$API_BASE_URL/v3.0/accounts/$AccountId/plans?pageSize=100"
    $plansResponse = Invoke-RestMethod -Uri $plansUrl -Headers $headers -Method Get -TimeoutSec 10
    
    # Find Pulse plans (not PulseOutbound) with Voice channel
    $pulsePlans = @()
    foreach ($plan in $plansResponse.results) {
        if ($plan.type -eq 'Pulse' -and $plan.channel -eq 'Voice') {
            $pulsePlans += $plan
        }
    }
    
    if ($pulsePlans.Count -gt 0) {
        # Get details for each Pulse plan to find max concurrency
        $maxConcurrency = 0
        foreach ($plan in $pulsePlans) {
            try {
                $planDetailUrl = "$API_BASE_URL/v3.0/accounts/$AccountId/plans/$($plan.planId)"
                $planDetail = Invoke-RestMethod -Uri $planDetailUrl -Headers $headers -Method Get -TimeoutSec 10
                
                if ($planDetail.concurrency -gt $maxConcurrency) {
                    $maxConcurrency = $planDetail.concurrency
                }
            } catch {
                Write-Host "Warning: Could not fetch details for plan $($plan.name)" -ForegroundColor Yellow
            }
        }
        
        if ($maxConcurrency -gt 0) {
            $MAX_PORTS = $maxConcurrency
            Write-Host "Max ports set to $MAX_PORTS (from plan concurrency)" -ForegroundColor Green
        } else {
            Write-Host "Error: Could not determine max ports from API." -ForegroundColor Red
            Write-Host "Unable to proceed without plan concurrency information." -ForegroundColor Red
            exit 1
        }
    } else {
        Write-Host "Error: No Pulse plans with Voice channel found." -ForegroundColor Red
        Write-Host "Unable to proceed without plan information." -ForegroundColor Red
        exit 1
    }
} catch {
    Write-Host "Error: Could not fetch plan information." -ForegroundColor Red
    Write-Host "Unable to proceed without plan concurrency data." -ForegroundColor Red
    Write-Host "Error details: $_" -ForegroundColor Red
    exit 1
}

Write-Host ""


# Ask if user wants to test a new campaign
$testMode = Read-Host "Do you want to test a new campaign? (Y/N)"
$isTestMode = ($testMode -eq "Y" -or $testMode -eq "y")

$TestCampaignName = ""
$TestCampaignPorts = 0
$TestCampaignStartTime = ""
$TestCampaignScheduleStart = ""
$TestCampaignScheduleEnd = ""
$TestCampaignFrequencyMinutes = 0
$TestCampaignRuntimeMinutes = 0

if ($isTestMode) {
    Write-Host ""
    Write-Host "TEST MODE: Configure hypothetical campaign" -ForegroundColor Cyan
    Write-Host "==========================================" -ForegroundColor Cyan
    Write-Host ""
    
    $TestCampaignName = "Test Campaign"
    $TestCampaignPorts = [int](Read-Host "Number of Ports")
    
    $startChoice = Read-Host "When do you want to start this campaign? (I)mmediately or (S)cheduled time?"
    $TestCampaignStartTime = ""
    if ($startChoice -eq "S" -or $startChoice -eq "s") {
        $TestCampaignStartTime = Read-Host "Start Time (HH:mm, e.g., 14:30)"
    } else {
        $TestCampaignStartTime = [DateTime]::Now.ToString("HH:mm")
        Write-Host "Starting immediately at $TestCampaignStartTime" -ForegroundColor Yellow
    }
    
    $scheduleChoice = Read-Host "Does the campaign have a schedule? (Y/N)"
    if ($scheduleChoice -eq "Y" -or $scheduleChoice -eq "y") {
        $TestCampaignScheduleStart = Read-Host "Schedule Start Time (HH:mm, e.g., 09:00)"
        $TestCampaignScheduleEnd = Read-Host "Schedule End Time (HH:mm, e.g., 17:00)"
    } else {
        Write-Host "Campaign will run 24/7" -ForegroundColor Yellow
    }
    
    $TestCampaignFrequencyMinutes = [int](Read-Host "Frequency in Minutes (e.g., 60)")
    $TestCampaignRuntimeMinutes = [int](Read-Host "Estimated Runtime in Minutes (e.g., 5)")
    
    Write-Host ""
}

function Format-TimeSpan($timeString) {
    try {
        if ($timeString -match '^(\d{2}):(\d{2}):(\d{2})') {
            $hours = [int]$matches[1]
            $minutes = [int]$matches[2]
            $seconds = [int]$matches[3]
            $totalMinutes = ($hours * 60) + $minutes + ($seconds / 60)
            return $totalMinutes
        }
        $ts = [TimeSpan]::Parse($timeString)
        return $ts.TotalMinutes
    } catch {
        return 0
    }
}

function Get-AverageCallDuration($accountId, $campaignId, $headers) {
    try {
        $runsUrl = "$API_BASE_URL/v3.0/accounts/$accountId/campaigns/$campaignId/runs?pageSize=2&sortField=StartDate&sortAsc=false"
        $runs = Invoke-RestMethod -Uri $runsUrl -Headers $headers -Method Get -ErrorAction Stop -TimeoutSec 10
        
        if (-not $runs.results -or $runs.results.Count -eq 0) {
            return $null
        }
        
        $durations = @()
        $totalCallsNeeded = 20
        
        foreach ($run in $runs.results) {
            if ($durations.Count -ge $totalCallsNeeded) { break }
            
            try {
                $resultsUrl = "$API_BASE_URL/v3.0/accounts/$accountId/report/campaigns/$campaignId/runs/$($run.runId)/results?pageSize=30"
                $results = Invoke-RestMethod -Uri $resultsUrl -Headers $headers -Method Get -ErrorAction Stop -TimeoutSec 10
                
                if ($results.results) {
                    foreach ($result in $results.results) {
                        if ($durations.Count -ge $totalCallsNeeded) { break }
                        
                        if ($result.duration) {
                            $durationMinutes = Format-TimeSpan $result.duration
                            if ($durationMinutes -gt 0 -and $durationMinutes -lt 60) {
                                $durations += $durationMinutes
                            }
                        }
                    }
                }
                
                if ($durations.Count -ge $totalCallsNeeded) { break }
            } catch {
                continue
            }
        }
        
        if ($durations.Count -ge 3) {
            $avg = ($durations | Measure-Object -Average).Average
            return @{
                Average = $avg
                Count = $durations.Count
            }
        }
        
        return $null
    } catch {
        return $null
    }
}

function Get-ScheduledRunTimes($campaign, $frequencyMinutes, $estimatedRuntimeMinutes, $hasCallData) {
    $runTimes = @()
    
    # If no schedules defined, it runs 24/7
    if (-not $campaign.schedules -or $campaign.schedules.Count -eq 0) {
        if ($campaign.requestedRunDate -and $frequencyMinutes -gt 0) {
            $nextRun = [DateTime]::Parse($campaign.requestedRunDate)
            $endOfPeriod = $nextRun.AddHours(24)
            
            $currentRunTime = $nextRun
            
            # Generate all runs for the next 24 hours
            while ($currentRunTime -lt $endOfPeriod) {
                $runTimes += [PSCustomObject]@{
                    StartTime = $currentRunTime
                    EndTime = $currentRunTime.AddMinutes($estimatedRuntimeMinutes)
                    HasDurationData = $hasCallData
                }
                
                $currentRunTime = $currentRunTime.AddMinutes($frequencyMinutes)
            }
        } elseif ($campaign.requestedRunDate) {
            # If no frequency, just use the single next run
            $nextRun = [DateTime]::Parse($campaign.requestedRunDate)
            $runTimes += [PSCustomObject]@{
                StartTime = $nextRun
                EndTime = $nextRun.AddMinutes($estimatedRuntimeMinutes)
                HasDurationData = $hasCallData
            }
        }
        return $runTimes
    }
    
    # Get today's date
    $today = Get-Date
    $currentDayOfWeek = $today.DayOfWeek.ToString()
    
    # Find today's schedule
    $todaySchedule = $campaign.schedules | Where-Object { $_.day -eq $currentDayOfWeek } | Select-Object -First 1
    
    if (-not $todaySchedule) {
        # No schedule for today, return empty
        return $runTimes
    }
    
    # Parse schedule times
    $scheduleStart = [DateTime]::Parse($todaySchedule.startTime)
    $scheduleEnd = [DateTime]::Parse($todaySchedule.endTime)
    
    # Create DateTime objects for today with the scheduled times
    $windowStart = Get-Date -Year $today.Year -Month $today.Month -Day $today.Day -Hour $scheduleStart.Hour -Minute $scheduleStart.Minute -Second 0
    $windowEnd = Get-Date -Year $today.Year -Month $today.Month -Day $today.Day -Hour $scheduleEnd.Hour -Minute $scheduleEnd.Minute -Second 0
    
    # Generate all run times within the schedule window
    if ($frequencyMinutes -gt 0) {
        $currentRunTime = $windowStart
        
        while ($currentRunTime -le $windowEnd) {
            $runEndTime = $currentRunTime.AddMinutes($estimatedRuntimeMinutes)
            
            # Only include if the run would start before or at the schedule end
            if ($currentRunTime -le $windowEnd) {
                $runTimes += [PSCustomObject]@{
                    StartTime = $currentRunTime
                    EndTime = $runEndTime
                    HasDurationData = $hasCallData
                }
            }
            
            $currentRunTime = $currentRunTime.AddMinutes($frequencyMinutes)
        }
    }
    
    return $runTimes
}

function Get-TestCampaignRunTimes($startTimeStr, $scheduleStart, $scheduleEnd, $frequencyMinutes, $runtimeMinutes) {
    $runTimes = @()
    $today = Get-Date
    
    # Parse the campaign start time
    $startParts = $startTimeStr.Split(':')
    $campaignStartTime = Get-Date -Year $today.Year -Month $today.Month -Day $today.Day -Hour ([int]$startParts[0]) -Minute ([int]$startParts[1]) -Second 0
    
    # If no schedule provided, assume 24/7 from start time
    if ([string]::IsNullOrEmpty($scheduleStart)) {
        $startTime = $campaignStartTime
        $endTime = $campaignStartTime.AddHours(24)
    } else {
        # Parse schedule times for today
        $schedStartParts = $scheduleStart.Split(':')
        $schedEndParts = $scheduleEnd.Split(':')
        
        $scheduleStartTime = Get-Date -Year $today.Year -Month $today.Month -Day $today.Day -Hour ([int]$schedStartParts[0]) -Minute ([int]$schedStartParts[1]) -Second 0
        $scheduleEndTime = Get-Date -Year $today.Year -Month $today.Month -Day $today.Day -Hour ([int]$schedEndParts[0]) -Minute ([int]$schedEndParts[1]) -Second 0
        
        # Use the later of campaign start time or schedule start time
        $startTime = if ($campaignStartTime -gt $scheduleStartTime) { $campaignStartTime } else { $scheduleStartTime }
        $endTime = $scheduleEndTime
    }
    
    $currentRunTime = $startTime
    
    while ($currentRunTime -le $endTime) {
        $runTimes += [PSCustomObject]@{
            StartTime = $currentRunTime
            EndTime = $currentRunTime.AddMinutes($runtimeMinutes)
            HasDurationData = $true
        }
        
        $currentRunTime = $currentRunTime.AddMinutes($frequencyMinutes)
    }
    
    return $runTimes
}

try {
    Write-Host ""
    Write-Host "Fetching Pulse campaigns..." -ForegroundColor Cyan
    Write-Host ""
    
    # Get all campaigns
    $headers = @{
        'accept' = 'application/json'
        'Authorization' = $ApiKey
    }
    
    $campaignsUrl = "$API_BASE_URL/v3.0/accounts/$AccountId/campaigns?pageSize=1000"
    
    try {
        $campaigns = Invoke-RestMethod -Uri $campaignsUrl -Headers $headers -Method Get
        $campaigns | ConvertTo-Json -Depth 10 | Out-File $CAMPAIGNS_JSON
    } catch {
        Write-Host "Error fetching campaigns: $_" -ForegroundColor Red
        Write-Host "Please check your Account ID and API Key." -ForegroundColor Red
        exit 1
    }
    
    $pulseCampaigns = $campaigns.results | Where-Object {
        $_.planType -eq 'Pulse' -and
        ($_.status -eq 'Running' -or $_.status -eq 'Queued' -or $_.status -eq 'Scheduled')
    }
    
    if ($pulseCampaigns.Count -eq 0) {
        Write-Host 'No active Pulse campaigns found.' -ForegroundColor Yellow
        "No active Pulse campaigns found." | Out-File $OUTPUT_FILE_VERBOSE
        "No active Pulse campaigns found." | Out-File $OUTPUT_FILE_SUMMARY
        exit
    }
    
    Write-Host "Found $($pulseCampaigns.Count) Pulse campaign(s). Processing..."
    Write-Host ""
    
    # Initialize output files
    if ($isTestMode) {
        "Campaign Port Overlap Analysis Report - TEST MODE (Detailed)" | Out-File $OUTPUT_FILE_VERBOSE
    } else {
        "Campaign Port Overlap Analysis Report (Detailed)" | Out-File $OUTPUT_FILE_VERBOSE
    }
    "Generated: $(Get-Date)" | Out-File $OUTPUT_FILE_VERBOSE -Append
    "Account ID: $AccountId" | Out-File $OUTPUT_FILE_VERBOSE -Append
    "================================================" | Out-File $OUTPUT_FILE_VERBOSE -Append
    "" | Out-File $OUTPUT_FILE_VERBOSE -Append
    
    if ($isTestMode) {
        "Campaign Port Overlap Analysis - TEST MODE - Summary" | Out-File $OUTPUT_FILE_SUMMARY
        "Testing: $TestCampaignName" | Out-File $OUTPUT_FILE_SUMMARY -Append
    } else {
        "Campaign Port Overlap Analysis - Summary" | Out-File $OUTPUT_FILE_SUMMARY
    }
    "Generated: $(Get-Date)" | Out-File $OUTPUT_FILE_SUMMARY -Append
    "Account ID: $AccountId" | Out-File $OUTPUT_FILE_SUMMARY -Append
    "Maximum Port Limit: $MAX_PORTS ports" | Out-File $OUTPUT_FILE_SUMMARY -Append
    "================================================" | Out-File $OUTPUT_FILE_SUMMARY -Append
    "" | Out-File $OUTPUT_FILE_SUMMARY -Append
    
    $campaignData = @()
    $progressCount = 0
    
    foreach ($campaign in $pulseCampaigns) {
        $progressCount++
        $progressMsg = "[$progressCount/$($pulseCampaigns.Count)] Processing: $($campaign.name)"
        
        $detailUrl = "$API_BASE_URL/v3.0/accounts/$AccountId/campaigns/$($campaign.campaignId)"
        
        try {
            $detail = Invoke-RestMethod -Uri $detailUrl -Headers $headers -Method Get -TimeoutSec 10
            
            $ports = if ($detail.concurrency) { $detail.concurrency } else { 1 }
            $testCaseCount = if ($detail.testCases) { $detail.testCases.Count } else { 0 }
            
            $frequencyMinutes = 0
            if ($detail.frequency) {
                $frequencyMinutes = [Math]::Round((Format-TimeSpan $detail.frequency), 0)
            }
            
            $avgDuration = Get-AverageCallDuration $AccountId $campaign.campaignId $headers
            
            $avgCallMinutes = 0
            $callDataAvailable = $false
            $callsAnalyzed = 0
            
            if ($avgDuration) {
                $avgCallMinutes = [Math]::Round($avgDuration.Average, 2)
                $callsAnalyzed = $avgDuration.Count
                $callDataAvailable = $true
            } else {
                # Use default if no data available
                $avgCallMinutes = $DEFAULT_CALL_DURATION_MINUTES
            }
            
            # Calculate estimated runtime (use actual or default call duration)
            $estimatedRuntimeMinutes = 0
            if ($testCaseCount -gt 0) {
                if ($ports -gt 1) {
                    $estimatedRuntimeMinutes = [Math]::Ceiling(($testCaseCount / $ports) * $avgCallMinutes)
                } else {
                    $estimatedRuntimeMinutes = [Math]::Ceiling($testCaseCount * $avgCallMinutes)
                }
            }
            
            $scheduleInfo = ''
            if ($detail.schedules -and $detail.schedules.Count -gt 0) {
                $days = ($detail.schedules | Select-Object -ExpandProperty day) -join ', '
                $firstSchedule = $detail.schedules[0]
                $startTime = ([DateTime]::Parse($firstSchedule.startTime)).ToString('HH:mm')
                $endTime = ([DateTime]::Parse($firstSchedule.endTime)).ToString('HH:mm')
                $scheduleInfo = "$days ($startTime-$endTime)"
            } else {
                $scheduleInfo = 'Always (24/7)'
            }
            
            # Generate all scheduled run times
            $runTimes = Get-ScheduledRunTimes $detail $frequencyMinutes $estimatedRuntimeMinutes $callDataAvailable
            
            $obj = [PSCustomObject]@{
                CampaignId = $campaign.campaignId
                Name = $campaign.name
                Ports = $ports
                TestCases = $testCaseCount
                FrequencyMin = $frequencyMinutes
                AvgCallDuration = $avgCallMinutes
                EstimatedRuntime = $estimatedRuntimeMinutes
                CallsAnalyzed = $callsAnalyzed
                HasCallData = $callDataAvailable
                Schedule = $scheduleInfo
                RunTimes = $runTimes
                Status = $campaign.status
                PlanName = $detail.plan.name
                IsTestCampaign = $false
            }
            
            $campaignData += $obj
            
            # Write to VERBOSE file only
            if ($callDataAvailable) {
                "$progressMsg - Checking call history... [OK]" | Out-File $OUTPUT_FILE_VERBOSE -Append
            } else {
                "$progressMsg - Checking call history... [WARNING] No data" | Out-File $OUTPUT_FILE_VERBOSE -Append
            }
        } catch {
            "$progressMsg - Checking call history... [ERROR]: $_" | Out-File $OUTPUT_FILE_VERBOSE -Append
        }
    }
    
    # Add test campaign if in test mode
    if ($isTestMode) {
        Write-Host "Adding test campaign to analysis..." -ForegroundColor Cyan
        
        $testScheduleInfo = if ([string]::IsNullOrEmpty($TestCampaignScheduleStart)) {
            "Always (24/7)"
        } else {
            "$TestCampaignScheduleStart-$TestCampaignScheduleEnd"
        }
        
        $testRunTimes = Get-TestCampaignRunTimes $TestCampaignStartTime $TestCampaignScheduleStart $TestCampaignScheduleEnd $TestCampaignFrequencyMinutes $TestCampaignRuntimeMinutes
        
        $testCampaignObj = [PSCustomObject]@{
            CampaignId = 999999
            Name = "$TestCampaignName [TEST]"
            Ports = $TestCampaignPorts
            TestCases = 0
            FrequencyMin = $TestCampaignFrequencyMinutes
            AvgCallDuration = 0
            EstimatedRuntime = $TestCampaignRuntimeMinutes
            CallsAnalyzed = 0
            HasCallData = $true
            Schedule = $testScheduleInfo
            RunTimes = $testRunTimes
            Status = "Test"
            PlanName = "Test Campaign"
            IsTestCampaign = $true
        }
        
        $campaignData += $testCampaignObj
        
        "[TEST CAMPAIGN ADDED] $TestCampaignName - $TestCampaignPorts ports, runs every $TestCampaignFrequencyMinutes min" | Out-File $OUTPUT_FILE_VERBOSE -Append
    }
    
    "" | Out-File $OUTPUT_FILE_VERBOSE -Append
    
    # Write campaign summary to VERBOSE file only
    "CAMPAIGN SUMMARY:" | Out-File $OUTPUT_FILE_VERBOSE -Append
    "================" | Out-File $OUTPUT_FILE_VERBOSE -Append
    "" | Out-File $OUTPUT_FILE_VERBOSE -Append
    
    $campaignData | Format-Table CampaignId, Name, Ports, TestCases, AvgCallDuration, EstimatedRuntime, Schedule,
        @{Label='Runs Today';Expression={$_.RunTimes.Count}}, Status -AutoSize |
        Out-File $OUTPUT_FILE_VERBOSE -Append -Width 300
    
    Write-Host ""    
    Write-Host "Analyzing for runtime overlaps..."
    Write-Host "========================================`n"
    
    "`n`nRUNTIME OVERLAP ANALYSIS:" | Out-File $OUTPUT_FILE_VERBOSE -Append
    "=========================" | Out-File $OUTPUT_FILE_VERBOSE -Append
    "" | Out-File $OUTPUT_FILE_VERBOSE -Append
    
    "OVERLAP ANALYSIS:" | Out-File $OUTPUT_FILE_SUMMARY -Append
    "================" | Out-File $OUTPUT_FILE_SUMMARY -Append
    "" | Out-File $OUTPUT_FILE_SUMMARY -Append
    
    $overlapFound = $false
    $testCampaignOverlapFound = $false
    $criticalOverlapFound = $false
    $campaignsToCheck = $campaignData | Where-Object { $_.RunTimes.Count -gt 0 }
    
    # First pass: collect all overlaps for counting
    $totalOverlaps = 0
    $overlapsByPair = @{}
    
    foreach ($c1 in $campaignsToCheck) {
        foreach ($c2 in $campaignsToCheck) {
            if ($c1.CampaignId -ge $c2.CampaignId) { continue }
            
            $pairKey = "$($c1.CampaignId)-$($c2.CampaignId)"
            $overlapTimes = @()
            
            # Check each run time of c1 against each run time of c2
            foreach ($run1 in $c1.RunTimes) {
                foreach ($run2 in $c2.RunTimes) {
                    $c1Start = $run1.StartTime
                    $c1End = $run1.EndTime
                    $c2Start = $run2.StartTime
                    $c2End = $run2.EndTime
                    
                    # Check if the time windows overlap
                    $hasOverlap = ($c1Start -lt $c2End) -and ($c2Start -lt $c1End)
                    
                    if ($hasOverlap) {
                        $totalOverlaps++
                        $overlapStart = if ($c1Start -gt $c2Start) { $c1Start } else { $c2Start }
                        $overlapEnd = if ($c1End -lt $c2End) { $c1End } else { $c2End }
                        
                        $overlapTimes += [PSCustomObject]@{
                            Run1Start = $c1Start
                            Run1End = $c1End
                            Run2Start = $c2Start
                            Run2End = $c2End
                            OverlapStart = $overlapStart
                            OverlapEnd = $overlapEnd
                            OverlapMinutes = [Math]::Round(($overlapEnd - $overlapStart).TotalMinutes, 1)
                            HasDurationData1 = $run1.HasDurationData
                            HasDurationData2 = $run2.HasDurationData
                        }
                    }
                }
            }
            
            if ($overlapTimes.Count -gt 0) {
                $overlapsByPair[$pairKey] = @{
                    Campaign1 = $c1
                    Campaign2 = $c2
                    Overlaps = $overlapTimes
                }
            }
        }
    }
    
    # Calculate peak port usage across all overlaps
    $peakPorts = 0
    foreach ($pairKey in $overlapsByPair.Keys) {
        $pairData = $overlapsByPair[$pairKey]
        $pairPorts = $pairData.Campaign1.Ports + $pairData.Campaign2.Ports
        if ($pairPorts -gt $peakPorts) {
            $peakPorts = $pairPorts
        }
    }
    

# Second pass: report overlaps
    foreach ($pairKey in $overlapsByPair.Keys | Sort-Object) {
        $pairData = $overlapsByPair[$pairKey]
        $c1 = $pairData.Campaign1
        $c2 = $pairData.Campaign2
        $firstOverlap = $pairData.Overlaps[0]
        $overlapCount = $pairData.Overlaps.Count
        
        $totalPorts = $c1.Ports + $c2.Ports
        $exceedsMaxPorts = $totalPorts -gt $MAX_PORTS
        
        # Check if this involves the test campaign
        $involvesTestCampaign = $c1.IsTestCampaign -or $c2.IsTestCampaign
        
        if ($exceedsMaxPorts) {
            $criticalOverlapFound = $true
        }
        
        
        # Only mark test campaign overlap as found if it's critical
        if ($involvesTestCampaign -and $exceedsMaxPorts) {
            $testCampaignOverlapFound = $true
        }
        
        # Skip reporting non-critical test campaign overlaps
        if ($involvesTestCampaign -and -not $exceedsMaxPorts) {
            continue
        }
        
        # SUMMARY FILE OUTPUT
        $summaryOutput = ""
        if ($involvesTestCampaign) {
            $summaryOutput += "[TEST] "
        }
        if ($exceedsMaxPorts) {
            $summaryOutput += "[CRITICAL] "
        }
        $summaryOutput += "$($c1.Name) + $($c2.Name) = $totalPorts ports"
        if ($exceedsMaxPorts) {
            $summaryOutput += " (EXCEEDS LIMIT!)"
        }
        
        $summaryOutput | Out-File $OUTPUT_FILE_SUMMARY -Append
        
        # VERBOSE OUTPUT (Detailed File Only)
        $verboseOutput = "`n"
        if ($involvesTestCampaign) {
            $verboseOutput += "=== TEST CAMPAIGN OVERLAP ===`n"
        }
        if ($exceedsMaxPorts) {
            $verboseOutput += "!!! CRITICAL: PORT LIMIT EXCEEDED !!!`n"
        }
        $verboseOutput += "*** RUNTIME OVERLAP DETECTED ***`n"
        $verboseOutput += "   Campaign 1: '$($c1.Name)' (ID: $($c1.CampaignId))`n"
        $verboseOutput += "      - Ports: $($c1.Ports) | Test Cases: $($c1.TestCases)`n"
        
        if ($c1.HasCallData -and -not $c1.IsTestCampaign) {
            $verboseOutput += "      - Avg Call: $($c1.AvgCallDuration) min (from $($c1.CallsAnalyzed) calls)`n"
        } elseif ($c1.IsTestCampaign) {
            $verboseOutput += "      - Test Campaign`n"
        } else {
            $verboseOutput += "      - Avg Call: 0.5 min (default - no historical data)`n"
        }
        
        $verboseOutput += "      - Est Runtime: $($c1.EstimatedRuntime) min`n"
        $verboseOutput += "      - Example Run: $($firstOverlap.Run1Start.ToString('HH:mm')) to $($firstOverlap.Run1End.ToString('HH:mm'))`n"
        $verboseOutput += "`n   Campaign 2: '$($c2.Name)' (ID: $($c2.CampaignId))`n"
        $verboseOutput += "      - Ports: $($c2.Ports) | Test Cases: $($c2.TestCases)`n"
        
        if ($c2.HasCallData -and -not $c2.IsTestCampaign) {
            $verboseOutput += "      - Avg Call: $($c2.AvgCallDuration) min (from $($c2.CallsAnalyzed) calls)`n"
        } elseif ($c2.IsTestCampaign) {
            $verboseOutput += "      - Test Campaign`n"
        } else {
            $verboseOutput += "      - Avg Call: 0.5 min (default - no historical data)`n"
        }
        
        $verboseOutput += "      - Est Runtime: $($c2.EstimatedRuntime) min`n"
        $verboseOutput += "      - Example Run: $($firstOverlap.Run2Start.ToString('HH:mm')) to $($firstOverlap.Run2End.ToString('HH:mm'))`n"
        $verboseOutput += "`n   >> Example Overlap Window: $($firstOverlap.OverlapStart.ToString('HH:mm')) to $($firstOverlap.OverlapEnd.ToString('HH:mm')) ($($firstOverlap.OverlapMinutes) min)`n"
        
        if ($exceedsMaxPorts) {
            $verboseOutput += "   >> CRITICAL: Peak Port Usage: $totalPorts ports (EXCEEDS MAXIMUM OF $MAX_PORTS PORTS!)`n"
        } else {
            $verboseOutput += "   >> Peak Port Usage: $totalPorts ports (within limit of $MAX_PORTS)`n"
        }
        
        $verboseOutput += "   >> Total Overlapping Runs Today: $overlapCount`n"
        
        if ((-not $firstOverlap.HasDurationData1 -or -not $firstOverlap.HasDurationData2) -and -not ($c1.IsTestCampaign -or $c2.IsTestCampaign)) {
            $verboseOutput += "   >> NOTE: Using default 30 sec call duration for campaign(s) without historical data`n"
        }
        
        if ($c1.PlanName -eq $c2.PlanName -and -not ($c1.IsTestCampaign -or $c2.IsTestCampaign)) {
            $verboseOutput += "   >> WARNING: Both campaigns use the same plan: '$($c1.PlanName)'`n"
        }
        
        $verboseOutput += "   " + ('-' * 70)
        
        $verboseOutput | Out-File $OUTPUT_FILE_VERBOSE -Append
        $overlapFound = $true
    }
    
    if (-not $overlapFound) {
        $noOverlap = '[OK] No runtime overlaps detected.'
        $noOverlap | Out-File $OUTPUT_FILE_VERBOSE -Append
        $noOverlap | Out-File $OUTPUT_FILE_SUMMARY -Append
        Write-Host "$noOverlap" -ForegroundColor Green
    } elseif ($isTestMode -and -not $testCampaignOverlapFound) {
        $noTestOverlap = "[OK] Test campaign would NOT cause any overlaps."
        $noTestOverlap | Out-File $OUTPUT_FILE_VERBOSE -Append
        $noTestOverlap | Out-File $OUTPUT_FILE_SUMMARY -Append
        Write-Host "$noTestOverlap" -ForegroundColor Green
    }
    
    Write-Host ""

    
    # Write campaigns without data to VERBOSE file only
    $campaignsWithoutData = $campaignData | Where-Object { -not $_.HasCallData -and -not $_.IsTestCampaign }
    if ($campaignsWithoutData.Count -gt 0) {
        $warning = "`n[NOTE] $($campaignsWithoutData.Count) campaign(s) have no historical data (using 30 sec default call duration):"
        $warning | Out-File $OUTPUT_FILE_VERBOSE -Append
        
        foreach ($c in $campaignsWithoutData) {
            $msg = "   - $($c.Name) (ID: $($c.CampaignId)) - Est Runtime: $($c.EstimatedRuntime) min for $($c.TestCases) test case(s)"
            $msg | Out-File $OUTPUT_FILE_VERBOSE -Append
        }
    }
    
    $totalPorts = ($campaignData | Where-Object { -not $_.IsTestCampaign } | Measure-Object -Property Ports -Sum).Sum
    $totalTestCases = ($campaignData | Where-Object { -not $_.IsTestCampaign } | Measure-Object -Property TestCases -Sum).Sum
    $totalCalls = ($campaignData | Where-Object { -not $_.IsTestCampaign } | Measure-Object -Property CallsAnalyzed -Sum).Sum
    $totalScheduledRuns = ($campaignData | Where-Object { -not $_.IsTestCampaign } | ForEach-Object { $_.RunTimes.Count } | Measure-Object -Sum).Sum
    $uniquePairs = $overlapsByPair.Keys.Count
    
    # Count critical overlaps
    $criticalPairs = 0
    foreach ($pairKey in $overlapsByPair.Keys) {
        $pairData = $overlapsByPair[$pairKey]
        $totalPorts = $pairData.Campaign1.Ports + $pairData.Campaign2.Ports
        if ($totalPorts -gt $MAX_PORTS) {
            $criticalPairs++
        }
    }
    
    # VERBOSE SUMMARY
    "`n`n========================================" | Out-File $OUTPUT_FILE_VERBOSE -Append
    "SUMMARY:" | Out-File $OUTPUT_FILE_VERBOSE -Append
    "========================================" | Out-File $OUTPUT_FILE_VERBOSE -Append
    if ($isTestMode) {
        "TEST MODE: Analysis includes hypothetical campaign '$TestCampaignName'" | Out-File $OUTPUT_FILE_VERBOSE -Append
        "" | Out-File $OUTPUT_FILE_VERBOSE -Append
    }
    "Active Campaigns: $(($campaignData | Where-Object { -not $_.IsTestCampaign }).Count) | With Call Data: $(($campaignData | Where-Object { $_.HasCallData -and -not $_.IsTestCampaign }).Count)" | Out-File $OUTPUT_FILE_VERBOSE -Append
    "Total Scheduled Runs Today: $totalScheduledRuns" | Out-File $OUTPUT_FILE_VERBOSE -Append
    "Campaign Pairs with Overlaps: $uniquePairs" | Out-File $OUTPUT_FILE_VERBOSE -Append
    "CRITICAL Overlaps (Exceeding $MAX_PORTS ports): $criticalPairs" | Out-File $OUTPUT_FILE_VERBOSE -Append
    "Peak Ports (highest overlap): $peakPorts ports" | Out-File $OUTPUT_FILE_VERBOSE -Append
    "Total Overlapping Run Instances: $totalOverlaps" | Out-File $OUTPUT_FILE_VERBOSE -Append
    "Total Ports: $totalPorts | Test Cases: $totalTestCases" | Out-File $OUTPUT_FILE_VERBOSE -Append
    "Historical Calls Analyzed: $totalCalls" | Out-File $OUTPUT_FILE_VERBOSE -Append
    "Default Call Duration: 30 seconds (for campaigns without data)" | Out-File $OUTPUT_FILE_VERBOSE -Append
    "Maximum Port Limit: $MAX_PORTS ports" | Out-File $OUTPUT_FILE_VERBOSE -Append
    
    # SUMMARY FILE - SUMMARY
    "`n========================================" | Out-File $OUTPUT_FILE_SUMMARY -Append
    "SUMMARY:" | Out-File $OUTPUT_FILE_SUMMARY -Append
    "========================================" | Out-File $OUTPUT_FILE_SUMMARY -Append
    if ($isTestMode) {
        "TEST MODE: Analysis includes hypothetical campaign '$TestCampaignName'" | Out-File $OUTPUT_FILE_SUMMARY -Append
        "Test Campaign: $TestCampaignPorts ports, runs every $TestCampaignFrequencyMinutes min" | Out-File $OUTPUT_FILE_SUMMARY -Append
        "" | Out-File $OUTPUT_FILE_SUMMARY -Append
    }
    "Active Campaigns: $(($campaignData | Where-Object { -not $_.IsTestCampaign }).Count)" | Out-File $OUTPUT_FILE_SUMMARY -Append
    "Campaign Pairs with Overlaps: $uniquePairs" | Out-File $OUTPUT_FILE_SUMMARY -Append
    "CRITICAL Overlaps (Exceeding $MAX_PORTS ports): $criticalPairs" | Out-File $OUTPUT_FILE_SUMMARY -Append
    "Peak Ports (highest overlap): $peakPorts ports" | Out-File $OUTPUT_FILE_SUMMARY -Append
    "Total Overlapping Run Instances: $totalOverlaps" | Out-File $OUTPUT_FILE_SUMMARY -Append
    
# CONSOLE SUMMARY
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host "SUMMARY" -ForegroundColor Cyan
    Write-Host "========================================" -ForegroundColor Cyan
    
    if ($isTestMode) {
        Write-Host "TEST MODE: '$TestCampaignName' ($TestCampaignPorts ports)" -ForegroundColor Cyan
        Write-Host ""
    }
    
    Write-Host "Active Campaigns: $(($campaignData | Where-Object { -not $_.IsTestCampaign }).Count)"
    
    if ($criticalPairs -gt 0) {
        Write-Host "CRITICAL Overlaps (Exceeding $MAX_PORTS ports): $criticalPairs" -ForegroundColor Red
    } else {
        Write-Host "CRITICAL Overlaps (Exceeding $MAX_PORTS ports): 0" -ForegroundColor Green
    }
    
    if ($peakPorts -gt $MAX_PORTS) {
        Write-Host "Peak Ports (highest overlap): $peakPorts ports" -ForegroundColor Red
    } elseif ($peakPorts -gt 0) {
        Write-Host "Peak Ports (highest overlap): $peakPorts ports" -ForegroundColor Yellow
    } else {
        Write-Host "Peak Ports (highest overlap): 0 ports" -ForegroundColor Green
    }
    
    Write-Host ""
    Write-Host "For details on overlaps, see:" -ForegroundColor Cyan
    Write-Host "  Summary: $OUTPUT_FILE_SUMMARY" -ForegroundColor White
    Write-Host "  Detailed: $OUTPUT_FILE_VERBOSE" -ForegroundColor White
    
    if ($isTestMode) {
        Write-Host ""
        if ($testCampaignOverlapFound) {
            Write-Host "TEST RESULT: Adding this campaign WOULD cause overlaps!" -ForegroundColor Red
        } else {
            Write-Host "TEST RESULT: Adding this campaign would NOT cause overlaps." -ForegroundColor Green
        }
    }
    Write-Host ""
    
    # Prompt to restart or exit
    Write-Host "========================================" -ForegroundColor Cyan
    $restartChoice = Read-Host "Would you like to run another analysis? (Y/N)"
    if ($restartChoice -eq "Y" -or $restartChoice -eq "y") {
        Write-Host ""
        Write-Host "Restarting script..." -ForegroundColor Green
        Write-Host ""
        & $PSCommandPath
        exit 0
    } else {
        Write-Host ""
        Write-Host "Exiting. Thank you!" -ForegroundColor Green
    }
    
} catch {
    Write-Host ""
    Write-Host "Error: $_" -ForegroundColor Red
    Write-Host $_.ScriptStackTrace
    Write-Host ""
    Read-Host "Press Enter to exit"
} finally {
    # Clean up temporary files
    if (Test-Path $CAMPAIGNS_JSON) {
        Remove-Item $CAMPAIGNS_JSON
    }
}
