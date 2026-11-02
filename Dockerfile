FROM ubuntu:24.04

RUN apt-get update && apt-get install -y --no-install-recommends \
g++ cmake make git ca-certificates \
&& rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
&& cmake --build build -j"$(nproc)"

RUN ctest --test-dir build --output-on-failure

ENV TERM=xterm-256color
CMD ["./build/tividy"]