# ---------------------------------------------------------------------------
#  build.ps1 — Build Snake Engine RGB
#
#  Usage:  .\scripts\build.ps1
#  Output: dist\snake_game.exe (statically linked)
# ---------------------------------------------------------------------------

$ErrorActionPreference = "Stop"

# Compiler
$GCC = "C:\msys64\ucrt64\bin\g++.exe"

if (-not (Test-Path $GCC)) {
    Write-Host "ERROR: g++ not found at $GCC" -ForegroundColor Red
    Write-Host "Install MSYS2 UCRT64 toolchain: pacman -S mingw-w64-ucrt-x86_64-gcc"
    exit 1
}

# Paths (relative to repo root)
$ROOT = Split-Path -Parent $PSScriptRoot
$SRC  = "$ROOT\src"
$SDK  = "$ROOT\OpenRGB-cppSDK"
$DIST = "$ROOT\dist"

# Ensure dist directory exists
if (-not (Test-Path $DIST)) {
    New-Item -ItemType Directory -Path $DIST | Out-Null
}

# Include paths
$Includes = @(
    "-I$SDK\include",
    "-I$SDK\external",
    "-I$SDK\external\CppUtils-Essential",
    "-I$SDK\external\CppUtils-Network",
    "-I$SRC"
)

# Source files
$Sources = @(
    "$SRC\main.cpp",
    "$SRC\game.cpp",
    "$SRC\server.cpp",
    "$SRC\input.cpp",
    "$SDK\src\Client.cpp",
    "$SDK\src\Color.cpp",
    "$SDK\src\DeviceInfo.cpp",
    "$SDK\src\Exceptions.cpp",
    "$SDK\src\MiscUtils.cpp",
    "$SDK\src\ProtocolCommon.cpp",
    "$SDK\src\ProtocolMessages.cpp",
    "$SDK\external\CppUtils-Essential\BinaryStream.cpp",
    "$SDK\external\CppUtils-Essential\ContainerUtils.cpp",
    "$SDK\external\CppUtils-Essential\CriticalError.cpp",
    "$SDK\external\CppUtils-Essential\LangUtils.cpp",
    "$SDK\external\CppUtils-Essential\StreamUtils.cpp",
    "$SDK\external\CppUtils-Essential\StringUtils.cpp",
    "$SDK\external\CppUtils-Network\HostInfo.cpp",
    "$SDK\external\CppUtils-Network\NetAddress.cpp",
    "$SDK\external\CppUtils-Network\Socket.cpp",
    "$SDK\external\CppUtils-Network\SystemErrorInfo.cpp"
)

# Output
$Output = "$DIST\snake_game.exe"

# Build flags
$Flags = @(
    "-g",
    "-o", $Output,
    "-static-libgcc",
    "-static-libstdc++",
    "-static",
    "-lws2_32"
)

Write-Host ""
Write-Host "=== Building Snake Engine RGB ===" -ForegroundColor Cyan
Write-Host "Compiler : $GCC"
Write-Host "Output   : $Output"
Write-Host ""

$AllArgs = $Includes + $Sources + $Flags
& $GCC @AllArgs

if ($LASTEXITCODE -eq 0) {
    $size = (Get-Item $Output).Length / 1MB
    Write-Host ""
    Write-Host "BUILD SUCCESSFUL" -ForegroundColor Green
    Write-Host "Output: $Output ($([math]::Round($size, 2)) MB)"
    Write-Host ""
} else {
    Write-Host ""
    Write-Host "BUILD FAILED (exit code: $LASTEXITCODE)" -ForegroundColor Red
    exit $LASTEXITCODE
}
