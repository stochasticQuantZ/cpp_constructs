-- Create demo keyspace and users table
CREATE KEYSPACE IF NOT EXISTS demo WITH replication = {'class':'SimpleStrategy','replication_factor':1};

CREATE TABLE IF NOT EXISTS demo.users (
  id uuid PRIMARY KEY,
  name text,
  created_at timestamp
);
