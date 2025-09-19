@echo off
echo 📦 Building containers...
docker compose up -d --build
timeout /t 10
echo 🌐 Opening http://localhost
start http://localhost
