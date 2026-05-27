# Redis C++ Clone

A high-performance, multithreaded Redis-like in-memory key-value data store implemented in modern C++23.

---

## Features

- **RESP protocol support**: Custom parser and serializer for the Redis Serialization Protocol (RESP).
- **In-memory cache**: Thread-safe cache implementation utilizing priority queues for lazy and background key-expiration cleanup.
- **Multithreaded network engine**: POSIX socket server utilizing standard threads to handle concurrent client connections.
- **Supported Redis Commands**:
  - `PING` — Ping-pong healthcheck.
  - `ECHO` — Echoes back the provided bulk string.
  - `SET` — Stores key-value pair, with support for expiration options (`EX` in seconds and `PX` in milliseconds).
  - `GET` — Retrieves value of a key.
  - `INCR` — Increments the integer value of a key by 1.
  - `DECR` — Decrements the integer value of a key by 1.
  - `INCRBY` — Increments the integer value of a key by a specified integer value.
  - `MULTI` — Starts a transaction block where subsequent commands are queued.
  - `EXEC` — Executes all queued commands in the transaction block.
  - `DISCARD` — Discards all queued commands in the transaction block.
- **Append-Only File (AOF) Logging**: Saves client request buffers to `logs.txt` for auditing and history tracking.

### TTL (Time-To-Live) Specification

The database supports key expiration using the `SET` command:
- **Options**:
  - `EX` — Set the specified expire time, in **seconds**.
  - `PX` — Set the specified expire time, in **milliseconds**.
- **Rules**:
  - Expiration values must be non-negative integers.
  - Specifying multiple expiration arguments (e.g., both `EX` and `PX` in the same command) is invalid and will return an error response.
- **Mechanisms**:
  - **Lazy Deletion**: Expired keys are deleted from the database storage immediately when a client tries to access them.
  - **Active Deletion**: A background thread runs periodically to clean up expired keys using a priority queue.

### Transaction Specification

The database supports basic transaction operations via queuing:
- **MULTI**: Switches the client connection to a transaction state. All subsequent commands (except `EXEC`, `DISCARD`, and nested `MULTI` calls) are queued into a transaction buffer and respond with `+QUEUED`.
- **EXEC**: Executes all queued commands sequentially in the order they were received, returns an array of responses, and restores the client connection back to normal state.
- **DISCARD**: Discards all queued commands in the transaction buffer and restores the connection back to normal state.

---

## Directory Structure

The project follows a clean header-source separation structure:

```
├── CMakeLists.txt          # CMake Build Configuration
├── logs.txt                # Append-Only command log file (auto-generated)
├── include/                # Header Files
│   ├── cache/
│   │   ├── Cache.hpp       # Thread-safe Cache class template declaration
│   │   └── Cache.tpp       # Cache class template method implementations
│   ├── commands/
│   │   └── db_commands.hpp # DB Command function signatures (SET/GET/INCR/etc.)
│   ├── resp/
│   │   ├── types.hpp       # RESP data structures and enums
│   │   ├── serialiser.hpp  # RESP serializer interface
│   │   ├── deserialiser.hpp# RESP deserializer & parser interface
│   │   └── handler.hpp     # Request dispatcher interface
│   └── utils/
│       ├── string_helpers.hpp # General string utilities
│       └── aof.hpp         # Append-only file logger declaration
└── src/                    # Source Files
    ├── main.cpp            # Server socket loop and main entrypoint
    ├── commands/
    │   └── db_commands.cpp # Database operations implementation
    ├── resp/
    │   ├── handler.cpp     # Command dispatch logic
    │   ├── serialiser.cpp  # RESP serialization logic
    │   └── deserialiser.cpp# RESP protocol parsing logic
    └── utils/
        ├── string_helpers.cpp # String utility implementations
        └── aof.cpp         # Command logging logic
```

---

## Prerequisites

To build and run this project, make sure you have:
- A compiler supporting **C++23** (e.g., Apple Clang 15+, GCC 13+, or Clang 16+)
- **CMake** version 3.13 or higher
- **Pthreads** support (standard on macOS and Linux)

---

## Building the Project

Run the following commands from the root directory:

```bash
# Generate the build system files
cmake -B build

# Compile and build the executable
cmake --build build
```

This compiles all files and outputs a `redis` executable under `build/`.

---

## Running the Server

Start the server using:

```bash
./build/redis
```

The server binds to port `6379` by default and waits for client connections.

---

## Testing the Server

You can connect and send commands using any standard TCP client or the official `redis-cli`:

### Using `redis-cli`
```bash
redis-cli -p 6379
127.0.0.1:6379> PING
PONG
127.0.0.1:6379> SET name codecrafters EX 10
OK
127.0.0.1:6379> GET name
"codecrafters"
# (Wait 10 seconds)
127.0.0.1:6379> GET name
(nil)
127.0.0.1:6379> SET counter 10
OK
127.0.0.1:6379> INCR counter
(integer) 11
127.0.0.1:6379> INCRBY counter 5
(integer) 16
```

### Transactions with `redis-cli`
```bash
127.0.0.1:6379> MULTI
OK
127.0.0.1:6379> SET user_age 25
QUEUED
127.0.0.1:6379> INCR user_age
QUEUED
127.0.0.1:6379> EXEC
1) OK
2) (integer) 26
```

### Using netcat (`nc`)
```bash
echo -e "*1\r\n\$4\r\nPING\r\n" | nc localhost 6379
# Output: +PONG
```

---

*Note: This README was generated by AI.*
