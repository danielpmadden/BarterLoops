
Write-Host "📦 Building containers..."
docker compose up -d --build
Start-Sleep -Seconds 10
Write-Host "🌐 Opening http://localhost"
Start-Process "http://localhost"
