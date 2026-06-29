Cassandra demo

This directory shows a minimal demo of running Apache Cassandra locally with Docker and interacting with it using Python.

Files:
- `docker-compose.yml`: starts a single-node Cassandra cluster (exposes `9042`).
- `schema.cql`: CQL statements to create the `demo` keyspace and `users` table.
- `demo.py`: Python script that loads `schema.cql`, inserts a row and reads it back.
- `requirements.txt`: Python dependency (`cassandra-driver`).

Run the demo
1. Start Cassandra with Docker Compose:

```bash
cd cpp_constructs/test/Cassandra
docker-compose up -d
```

2. Wait for Cassandra to be ready (can take 30–60s). You can check logs:

```bash
docker-compose logs -f cassandra
```

3. Install Python dependency (preferably in a venv):

```bash
python3 -m pip install -r requirements.txt
```

4. Run the demo script:

```bash
python3 demo.py
```

You should see output showing the inserted row.

Tear down:

```bash
docker-compose down
```
