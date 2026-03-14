# Localhost REST API Test Output

## GET combinations

## Run the test: ./rest_server
```bash
curl "http://127.0.0.1:8080/combinations?input=abc"
```

```json
{
    "combinations": [
        "a",
        "b",
        "c",
        "ab",
        "ac",
        "bc",
        "abc"
    ],
    "count": 7,
    "input": "abc"
}
```

## POST create file

```bash
curl -X POST http://127.0.0.1:8080/files \
  -H "Content-Type: application/json" \
  -d '{"name":"sample2.txt"}'
```

```json
{
    "message": "File created successfully",
    "path": "test/created_files/sample2.txt"
}
```

## Verify created file

```bash
cat cpp_constructs/build/test/created_files/sample2.txt
```

```text
Dummy file created by POST /files
```
