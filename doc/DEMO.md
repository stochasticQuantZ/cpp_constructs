# Running Demos (LCM gRPC + Cassandra)

This document describes how to run the two demos added under `test/`:

- LCM gRPC server/client (C++ built with CMake)
- Cassandra demo (single-node Cassandra + Python client) using Podman/Docker

Prerequisites
- CMake and a C++ toolchain (clang/clang++)
- gRPC and Protobuf available in CMake (project already finds them)
- Podman (or Docker) for the Cassandra demo
- Python 3.11 (or use the provided container method)

1) Build the project (LCM gRPC)

From workspace root:
```bash
cd cpp_constructs/build
cmake ..
make -j$(sysctl -n hw.ncpu)
```

2) Run the LCM gRPC server and client

Start the server in one terminal (foreground):
```bash
cd cpp_constructs/build
./test/test_lcm_server
```

Run the client (separate terminal):
```bash
cd cpp_constructs/build
./test/test_lcm_client 12 18
# Expected: LCM(12, 18) = 36
```

3) Cassandra demo (Podman recommended on macOS)

From `cpp_constructs/test/Cassandra` start a single-node Cassandra:

Using Podman Compose (recommended on macOS):
```bash
cd cpp_constructs/test/Cassandra
podman compose up -d
# or if using podman-compose python package:
podman-compose up -d
```

If `podman compose`/`podman-compose` is not available, use Docker Compose:
```bash
docker compose up -d
```

Check container status and logs:
```bash
podman ps --filter name=cassandra_demo
podman logs -f cassandra_demo
```

4) Run the Python demo inside the Cassandra pod (reliable method)

On macOS Podman runs containers inside a pod; run a transient Python 3.11 container attached to the Cassandra pod and execute the demo there:

```bash
cd cpp_constructs/test/Cassandra
# find the pod id of the running cassandra container
POD=$(podman ps --filter name=cassandra_demo --format "{{.Pod}}")
# get Cassandra container IP inside the pod network
IP=$(podman inspect cassandra_demo --format '{{range .NetworkSettings.Networks}}{{.IPAddress}}{{end}}')

# run the demo inside same pod (installs driver and runs demo.py)
podman run --rm --pod $POD -e CASSANDRA_HOST=$IP -v "$PWD":/app -w /app docker.io/library/python:3.11-slim \
  bash -c "pip install --upgrade pip setuptools wheel && pip install cassandra-driver && python demo.py"
```

Alternatively run on host Python 3.11 (if available):
```bash
python3.11 -m venv .venv311
. .venv311/bin/activate
pip install --upgrade pip setuptools wheel
pip install -r requirements.txt
python demo.py
```

5) Tear down

Stop the Cassandra composition:
```bash
podman compose down
# or
docker compose down
```

Notes and troubleshooting
- If `podman compose` complains about missing providers, install `podman-compose` or Docker Compose plugin.
- If the Python `cassandra-driver` fails to build on host Python (>=3.14), use the container method above or use Python 3.11.
- The `demo.py` script accepts `CASSANDRA_HOST` env var (default `127.0.0.1`).

Files touched
- `test/LCM/lcm_server.cpp`, `test/LCM/lcm_client.cpp` — gRPC server/client
- `protobuffs/lcm.proto` — proto definition
- `test/Cassandra/*` — compose, demo and schema
