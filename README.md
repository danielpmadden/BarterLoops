# BarterLoops

**AI-assisted barter marketplace prototype. Built twice.**

## Overview

BarterLoops is an experimental project exploring how AI can power a free global barter marketplace.

The idea: instead of simple one-to-one trades, the system discovers multi-party loops (e.g. eggs → hat → rocking chair → eggs) so everyone gets what they want.

This repo contains two MVP scaffolds, both built in about 3.5 hours each using custom GPT coding assistants:

- **Python stack**: FastAPI + Postgres + Nginx
- **C++ stack**: C++17 JSON API + Tailwind frontend + Nginx

Both are fully Dockerized and orchestrated with Compose, running identically on Windows, macOS, and Linux with a single command.

## Project Goals

- **Feasibility**: Prove that a barter loop marketplace can be scaffolded rapidly with AI help
- **Portability**: Containerized services that spin up anywhere
- **Credibility**: Even minimal frontends connect correctly with working backends
- **Exploration**: Compare Python (velocity) vs. C++ (performance) approaches

## Current State

✅ **Complete**
- Backend services containerized and exposed via Nginx
- Frontends boot and connect to backends (placeholder UIs)
- Cross-platform Docker Compose orchestration

🚧 **Todo**
- User accounts & authentication
- Offer creation & inventory management
- Barter loop matcher service
- Persistence for C++ service
- Image uploads for listings
- Trade loop visualization UI

## Architecture

**Python track**: Browser → Nginx → FastAPI → Postgres  
**C++ track**: Browser → Nginx → C++ JSON API (stateless for now)

Both stacks are managed with `docker compose` for simple, consistent launches.

## Getting Started

### Prerequisites
- Docker and Docker Compose installed
- Git

### Run (Python stack)
```bash
cd python/
docker compose up --build
```

### Run (C++ stack)
```bash
cd cpp/
docker compose up --build
```

### Access
- **Frontend**: http://localhost:8080
- **Backend health endpoint**: http://localhost:8000/health

## Future Directions

- Graph-based barter matcher (cycle detection, fairness weighting)
- Accounts, authentication, and reputation systems
- Persistent storage, media uploads, and trade history
- Graph visualization for trade loops
- Cloud deployment and observability

## Meta

This project is not a polished product — it is a proof of velocity. Two credible MVP foundations stood up in hours show how AI-assisted development can turn ideas into running systems fast.
