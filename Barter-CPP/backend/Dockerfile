FROM ubuntu:22.04 as build
RUN apt-get update && apt-get install -y build-essential cmake
WORKDIR /app
COPY CMakeLists.txt ./
COPY src ./src
COPY include ./include
RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
RUN cmake --build build --parallel

FROM debian:stable-slim
RUN apt-get update && apt-get install -y libstdc++6 && rm -rf /var/lib/apt/lists/*
WORKDIR /app
# ✅ make sure uploads folder exists
RUN mkdir -p /app/uploads
COPY --from=build /app/build/barterloops_engine /app/barterloops_engine
EXPOSE 8081
CMD ["./barterloops_engine"]
