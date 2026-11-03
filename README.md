# Mini-RocksDB----LSM-Tree-Key-Value-Database-Engine.

A small key-value database engine written in C++, inspired by RocksDB. It stores data using the idea of a Log-Structured Merge-Tree (LSM-Tree): fast writes in memory, sorted files on disk, and a quick check before reading from disk.

Project-Based Learning (PBL) project, Department of Computer Science & Engineering, Graphic Era (Deemed to be University), Dehradun. Session 2026-27.

- **Team ID:** DSCPP-III-2026-T146
- **Mentor:** Dr. Siddhant Thapliyal

## Team Members
- Tanvi Kangar
- Kripa Bargali
- Yashika Rajwar
- Kushi Malik

## Problem

Normal data structures like hash maps and binary trees are fast only while all the data fits in RAM. RAM is expensive, and writing directly to disk is slow. Modern applications need to write a huge number of records quickly and still store more data than memory can hold.

## Our Approach

1. A key-value pair arrives (for example a user ID and a message).
2. It is written instantly into the **MemTable** in RAM, which is a Skip List.
3. Keys are added to a **Bloom Filter**, which can tell us quickly if a key is definitely not present.
4. When the MemTable is full, its data is saved to disk as a sorted file called an **SSTable**. A Min-Heap manages the order of flushing.
5. On a read, we check the MemTable first, and the disk is read only if the Bloom Filter says the key might exist.

## Modules

| Module | Folder | Files | What it does |
|---|---|---|---|
| Shared type | `Shared type` | `common.h` | `Entry` struct: key, value, deleted flag |
| MemTable | `MemTable` | `memtable.h`, `memtable.cpp` | Skip List in RAM with put, get, remove |
| Bloom Filter | `Bloom Filter` | `bloom_filter.h`, `bloom_filter.cpp` | Says a key is "definitely absent" or "might be present" |
| SSTable | `SSTable` | `sstable.h`, `sstable.cpp` | Writes and reads sorted data files |
| Flush Manager | `FlushManager` | `flush_manager.h`, `flush_manager.cpp` | Min-Heap of flush jobs (Phase 3) |
| Core Engine | `Core Engine` | `database_engine.h`, `database_engine.cpp`, `engine_read.cpp`, `engine_load.cpp`, `cli.cpp` | Connects all modules, command-line shell |
| Tests | `Test files` | `test_*.cpp` | One test program per module |

## Team

| Member | Role | Main work |
|---|---|---|
| Tanvi Kangar | Team Lead | Core Engine, Flush Manager, repo, documentation |
| Kripa Bargali | Developer | Bloom Filter and the read path |
| Yashika Rajwar | Developer | SSTable and file loading |
| Kushi Malik | Developer, Testing | MemTable, tests, benchmark |

## Technology

- Language: C++
- Storage: custom file-based SSTables
- Tools: C++ Standard Library, VS Code, Git/GitHub

## Build and Run

Needs a C++ compiler (g++). Run the commands from the project root folder.

**Phase 2 command-line shell:**

```
g++ -I "Shared type" -I MemTable -I SSTable -I "Core Engine" "Core Engine/cli.cpp" MemTable/memtable.cpp SSTable/sstable.cpp -o cli
./cli
```

(On Windows use `.\cli`.)

**Phase 3 (full engine):**
```
g++ -I "Shared type" -I "Bloom Filter" -I MemTable -I SSTable -I FlushManager -I "Core Engine" "Core Engine/cli.cpp" "Core Engine/database_engine.cpp" "Core Engine/engine_read.cpp" "Core Engine/engine_load.cpp" FlushManager/flush_manager.cpp MemTable/memtable.cpp SSTable/sstable.cpp "Bloom Filter/bloom_filter.cpp" -o cli

./cli
```

## Using the shell

| Command | What it does |
|---|---|
| `put <key> <value>` | Store a key and its value |
| `get <key>` | Read a value |
| `delete <key>` | Delete a key |
| `flush` | Save the MemTable to a disk file now |
| `stats` | Show files created and Bloom Filter savings (Phase 3) |
| `help` | List the commands |
| `exit` | Quit |

Example:

```
minirocks> put banana yellow
OK
minirocks> put apple red
OK
minirocks> delete banana
OK
minirocks> get apple
red
minirocks> get banana
(not found)
```

## Project Status

| Phase | Status |
|---|---|

| Phase 1: Proposal and design | Done |
| Phase 2: Individual modules (MemTable, Bloom Filter, SSTable, basic engine, command-line shell) | Done |
| Phase 3: Min-Heap flush manager, automatic flush, Bloom Filter in the read path, reading across several files, speed measurement | In progress |


## Running the tests
Each member tests their own module. Example for the MemTable:

```
g++ -I "Shared type" -I MemTable "Test files/test_memtable.cpp" MemTable/memtable.cpp -o tm
./tm
```

Other tests: `test_bloom.cpp`, `test_sstable.cpp`, and in Phase 3 `test_flush.cpp` and `test_engine.cpp`.

## Known limits

- SSTables are plain text files, one line per entry, and a lookup scans the file line by line.
- Keys and values cannot contain the `|` character.
- Flushing happens one job at a time, with no background threads yet.
- The Bloom Filter has a fixed size, so the MemTable limit stays around 1000 keys.
- No compaction, so old files and delete markers pile up.

## Future Work

Write-Ahead Log for crash recovery, compaction to merge old files, multi-threading, an index inside each SSTable, and a binary file format.

## References

- Bonomi et al. (2006), An Improved Construction for Counting Bloom Filters
- M. Kleppmann, Designing Data-Intensive Applications
- RocksDB documentation and cppreference.com

