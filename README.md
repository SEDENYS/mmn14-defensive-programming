# MMN14 – Defensive Systems Programming

This project implements a **client–server backup system** using **C++ (server)** and **Python (client)**.  
It was developed as part of the Defensive Systems Programming course, focusing on **TCP sockets**, **custom binary protocol**, and **defensive programming techniques**.

---

## 📂 Project Structure

```
mmn14/
├── server/               # C++ server
│   ├── main.cpp
│   ├── handlers.cpp/.hpp
│   ├── tcp_server.cpp/.hpp
│   ├── protocol_utils.cpp/.hpp
│   ├── file_utils.cpp/.hpp
│   └── backupsvr/        # Created automatically per user_id
│
├── client/               # Python client
│   ├── actions.py
│   ├── connection.py
│   ├── protocol.py
│   ├── response_utils.py
│   └── file_utils.py
│
├── mmn14client.py        # Main client – follows assignment sequence
├── full_test.py          # Automated end-to-end test
├── load_test.py          # Stress test with multiple clients
├── backup_only.py        # Simple backup test
├── list_only.py          # Simple list test
└── server.info           # Contains IP and port of the server
```

---

## 🚀 How It Works

### Server (C++, multithreaded)
- Listens on TCP (`127.0.0.1:1234`).
- Creates a **thread per client** (`std::thread` + `detach`).
- Handles operations defined by the assignment:
  - `100` → **BACKUP**: save file into `backupsvr/<user_id>/`.
  - `200` → **RETRIEVE**: send back file contents.
  - `201` → **DELETE**: remove file from user directory.
  - `202` → **LIST**: return list of files for that user.
- Uses **STL**: `std::vector`, `std::string`, `std::fstream`, `std::filesystem`, `std::thread`, `std::mutex`.
- **Defensive programming**:
  - Empty file or recv error → status `1001`.
  - Delete failure → status `1002`.
  - File not found → status `1003`.
  - Protocol version check with `check_version()`.
- All logs are protected with `std::mutex` to avoid mixing between threads.

### Client (Python)
- Reads `server.info` (IP, port) and `backup.info` (files to backup).
- Generates a random `user_id` (32-bit).
- Builds binary requests using `struct.pack`.
- Sends requests via `socket` and parses responses.
- Implements the exact assignment sequence:
  1. LIST (initial)
  2. BACKUP file 1
  3. BACKUP file 2
  4. LIST (after backups)
  5. RETRIEVE file 1
  6. DELETE file 1
  7. RETRIEVE file 1 again (should fail)

---

## 🧪 Tests

- `full_test.py` → Runs the full flow and verifies filesystem state.
- `backup_only.py` → Tests only backup.
- `list_only.py` → Tests only listing.
- `load_test.py` → Stress test that runs multiple clients in parallel (using Python `ThreadPoolExecutor`).

Example server log under load:

```
[SERVER] Listening on 127.0.0.1:1234
[SERVER] user=2140383687 op=100 file=mmn14.pdf
[SERVER] user=743639177 op=100 file=mmn14.pdf
[SERVER] user=1706156343 op=100 file=mmn14.pdf
...
```

Each client gets its own `user_id` and separate directory under `backupsvr/`.

---

## 📑 Example Run

```bash
# Run server
cd server
g++ -std=c++17 main.cpp tcp_server.cpp handlers.cpp file_utils.cpp protocol_utils.cpp -o mmn14server -pthread
./mmn14server

# Run client
cd ..
python3 mmn14client.py
```

Output (simplified):

```
User ID: 2140383687
=== LIST (initial) ===
Files on server: []
=== BACKUP file 1 ===
Backup OK (mmn14.pdf)
=== BACKUP file 2 ===
Backup failed (terminator2.avi)
=== LIST (after backups) ===
Files on server: ['mmn14.pdf', 'terminator2.avi']
=== RETRIEVE file 1 ===
Retrieved: mmn14.pdf (238 KB)
=== DELETE file 1 ===
Delete OK (mmn14.pdf)
=== RETRIEVE file 1 again ===
mmn14.pdf not found on server
```

---

## 🛡️ Defensive Programming Highlights
- Checked protocol version (`version != 1` → warning).
- Mutex-protected logs to avoid interleaved messages.
- Validations: empty file, missing file, invalid ops.
- Consistent error/status codes:
  - `210` → Backup OK
  - `211` → Delete OK
  - `212` → Retrieve/List OK
  - `1001` → General/empty/open error
  - `1002` → Delete failed
  - `1003` → File not found

---

## 👥 Authors
- **Server (C++)**: Multithreaded TCP server, defensive systems programming.
- **Client (Python)**: Protocol implementation, automation tests, stress tests.
