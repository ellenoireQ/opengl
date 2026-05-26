param()

$ErrorActionPreference = 'Stop'

$root = Split-Path -Parent $MyInvocation.MyCommand.Path
$buildDir = Join-Path $root 'build'

Write-Host '=== OpenGL Chapter Builder (Windows) ===' -ForegroundColor Cyan
Write-Host ''

$chapters = Get-ChildItem -Path $root -Directory | Where-Object { $_.Name -like 'gs_*' } | Sort-Object Name

if (-not $chapters -or $chapters.Count -eq 0) {
    Write-Host 'No chapters found (gs_* folders)' -ForegroundColor Red
    exit 1
}

Write-Host 'Select chapter to build:' -ForegroundColor Yellow
for ($i = 0; $i -lt $chapters.Count; $i++) {
    Write-Host ("  {0}. {1}" -f ($i + 1), $chapters[$i].Name)
}
Write-Host '  0. Build all chapters'
Write-Host ''

$choiceInput = Read-Host ("Enter choice (0-{0})" -f $chapters.Count)

if ($choiceInput -notmatch '^[0-9]+$') {
    Write-Host 'Invalid choice!' -ForegroundColor Red
    exit 1
}

$choice = [int]$choiceInput
if ($choice -lt 0 -or $choice -gt $chapters.Count) {
    Write-Host 'Invalid choice!' -ForegroundColor Red
    exit 1
}

function Get-BuiltExecutable {
    param(
        [string]$SearchRoot
    )

    $candidates = @('my_app.exe', 'my_app')
    foreach ($candidate in $candidates) {
        $found = Get-ChildItem -Path $SearchRoot -File -Filter $candidate -Recurse -ErrorAction SilentlyContinue |
            Sort-Object LastWriteTime -Descending |
            Select-Object -First 1
        if ($found) {
            return $found
        }
    }

    return $null
}

function Build-Chapter {
    param(
        [System.IO.DirectoryInfo]$Chapter,
        [bool]$ShouldRun
    )

    function Write-ErrorSummary {
        param(
            [Parameter(Mandatory = $true)]
            [object[]]$Output,
            [Parameter(Mandatory = $true)]
            [string]$Header
        )

        Write-Host $Header -ForegroundColor Red

        $lines = $Output | ForEach-Object { $_.ToString() }
        $errorLines = $lines | Select-String -Pattern 'CMake Error|\berror\b|\bfatal\b|\bfailed\b|undefined reference|LNK\d+' -CaseSensitive:$false | ForEach-Object { $_.Line }

        if ($errorLines -and $errorLines.Count -gt 0) {
            $errorLines | ForEach-Object { Write-Host $_ }
            return
        }

        Write-Host 'No filtered error lines found. Showing full command output:' -ForegroundColor Yellow
        $lines | ForEach-Object { Write-Host $_ }
    }

    $chapterName = $Chapter.Name
    Write-Host ''
    Write-Host ("Building {0}..." -f $chapterName) -ForegroundColor Green

    $cppFile = Get-ChildItem -Path $Chapter.FullName -File -Filter '*.cpp' | Select-Object -First 1
    if (-not $cppFile) {
        Write-Host ("No .cpp file found in {0}" -f $chapterName) -ForegroundColor Red
        return $false
    }

    New-Item -ItemType Directory -Path $buildDir -Force | Out-Null

    Write-Host 'Configuring CMake...' -ForegroundColor Cyan
    $cmakeOutput = & cmake -G Ninja -S $root -B $buildDir "-DTARGET=$($cppFile.FullName)" 2>&1
    $cmakeExit = $LASTEXITCODE

    if ($cmakeExit -ne 0) {
        Write-Host ("CMake configuration failed for {0}" -f $chapterName) -ForegroundColor Red
        Write-ErrorSummary -Output $cmakeOutput -Header 'Configuration output:'
        if ($LASTEXITCODE -ne 0) {
            $null = $LASTEXITCODE
        }
        return $false
    }

    Write-Host 'Compiling...' -ForegroundColor Cyan
    $buildOutput = & cmake --build $buildDir 2>&1
    $buildExit = $LASTEXITCODE

    if ($buildExit -ne 0) {
        Write-Host ("Build failed for {0}" -f $chapterName) -ForegroundColor Red
        Write-ErrorSummary -Output $buildOutput -Header 'Build output:'
        if ($LASTEXITCODE -ne 0) {
            $null = $LASTEXITCODE
        }
        return $false
    }

    $builtExe = Get-BuiltExecutable -SearchRoot $buildDir
    if ($builtExe) {
        $targetName = if ($builtExe.Extension -ieq '.exe') { "$chapterName.exe" } else { $chapterName }
        $targetPath = Join-Path $builtExe.DirectoryName $targetName

        if (Test-Path $targetPath) {
            Remove-Item $targetPath -Force
        }

        Move-Item -Path $builtExe.FullName -Destination $targetPath -Force

        Write-Host ("OK {0} built successfully" -f $chapterName) -ForegroundColor Green

        if ($ShouldRun) {
            Write-Host ("Running {0}..." -f $chapterName) -ForegroundColor Cyan
            Write-Host ''
            & $targetPath
            $exitCode = $LASTEXITCODE
            Write-Host ''
            Write-Host ("Program exited with code: {0}" -f $exitCode) -ForegroundColor Cyan
        }
    }
    else {
        Write-Host ("Build succeeded, but executable my_app(.exe) was not found for {0}" -f $chapterName) -ForegroundColor Yellow
    }

    return $true
}

if ($choice -eq 0) {
    Write-Host ''
    Write-Host 'Building all chapters...' -ForegroundColor Cyan

    $success = 0
    $failed = 0

    foreach ($chapter in $chapters) {
        if (Build-Chapter -Chapter $chapter -ShouldRun:$false) {
            $success++
        }
        else {
            $failed++
        }
    }

    Write-Host ''
    Write-Host '=== Build Results ===' -ForegroundColor Cyan
    Write-Host ("Success: {0}" -f $success) -ForegroundColor Green
    Write-Host ("Failed: {0}" -f $failed) -ForegroundColor Red
    Write-Host ''
    Write-Host 'Executables are in build/ folder' -ForegroundColor Cyan
}
else {
    $selectedChapter = $chapters[$choice - 1]
    $null = Build-Chapter -Chapter $selectedChapter -ShouldRun:$true
}
