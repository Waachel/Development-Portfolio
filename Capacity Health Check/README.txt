CAMPAIGN OVERLAP ANALYSIS TOOL
===============================

QUICK START:
-----------
1. Extract all files from this zip to a folder
2. Double-click "PortUsagePulseReport.bat" to run
3. Enter your Account ID and API Key when prompted
4. Follow the on-screen instructions

FILES INCLUDED:
--------------
- PortUsagePulseReport.bat    : Main launcher (double-click to run)
- analyzeOverlaps.ps1          : PowerShell script (called by the .bat file)
- README.txt                   : This file

WHAT IT DOES:
------------
- Fetches your Pulse Voice campaigns from Cyara API
- Analyzes scheduled run times for today
- Identifies port usage overlaps
- Reports CRITICAL overlaps that exceed your plan's port limit
- Allows testing hypothetical campaigns before deployment

FEATURES:
--------
✓ Automatically fetches max ports from your Pulse plan
✓ Saves credentials during session (no re-entry needed)
✓ Test campaign mode with start time configuration
✓ Only reports critical overlaps for test campaigns
✓ Generates detailed and summary reports
✓ Restart or exit after each analysis

OUTPUT FILES:
------------
- campaign_overlaps_summary.txt  : Quick overview of overlaps
- campaign_overlaps_detailed.txt : Detailed breakdown with times

REQUIREMENTS:
------------
- Windows PowerShell (comes with Windows)
- Cyara API access (Account ID + API Key)
- Internet connection to reach Cyara API

TROUBLESHOOTING:
---------------
If you see "scripts are disabled", run this in PowerShell as Administrator:
  Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser

Then try running the tool again.

SUPPORT:
-------
For issues or questions, reach out to Cyara Support
