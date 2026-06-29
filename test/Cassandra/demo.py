#!/usr/bin/env python3
"""Simple Cassandra demo: create keyspace/table, insert and select a row."""
import os
from cassandra.cluster import Cluster
from cassandra.query import SimpleStatement
import uuid
import time

KEYSPACE = "demo"

def main():
    host = os.environ.get('CASSANDRA_HOST', '127.0.0.1')
    cluster = Cluster([host])
    session = cluster.connect()

    # ensure schema exists
    with open('schema.cql', 'r') as f:
        schema = f.read()
    for stmt in [s.strip() for s in schema.split(';') if s.strip()]:
        session.execute(stmt)

    # Insert a user
    user_id = uuid.uuid4()
    insert = session.prepare("INSERT INTO demo.users (id, name, created_at) VALUES (?, ?, toTimestamp(now()))")
    session.execute(insert, (user_id, 'alice'))

    # Query back
    stmt = SimpleStatement("SELECT id, name, created_at FROM demo.users WHERE id = %s")
    rows = session.execute(stmt, (user_id,))
    for row in rows:
        print(f"Got row: id={row.id}, name={row.name}, created_at={row.created_at}")

    cluster.shutdown()

if __name__ == '__main__':
    main()
