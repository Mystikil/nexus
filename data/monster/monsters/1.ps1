# run-server.ps1
Write-Host "🔄 Starting 1.exe..."
Start-Sleep -Milliseconds 500
try {
    .\1.exe
} catch {
    Write-Host "❌ Error: $($_.Exception.Message)"
}
Read-Host "✅ Server exited. Press Enter to close"