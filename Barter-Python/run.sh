#!/bin/bash
echo "📦 Building containers..."
docker compose up -d --build
sleep 10
echo "🌐 Opening http://localhost"
xdg-open http://localhost || open http://localhost
