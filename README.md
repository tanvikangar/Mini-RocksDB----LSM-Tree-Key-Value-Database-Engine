# Mini-RocksDB----LSM-Tree-Key-Value-Database-Engine.
## Team Members
- Tanvi Kangar
- Kripa Bargali
- Yashika Rajwar
- Kushi Malik

A small key-value database engine written in C++, inspired by RocksDB. It stores data using the idea of a Log-Structured Merge-Tree (LSM-Tree): fast writes in memory, sorted files on disk, and a quick check before reading from disk.


## Problem

Normal data structures like hash maps and binary trees are fast only while all the data fits in RAM. RAM is expensive, and writing directly to disk is slow. Modern applications need to write a huge number of records quickly and still store more data than memory can hold.

## Our Approach

1. A key-value pair arrives (for example a user ID and a message).
2. It is written instantly into the **MemTable** in RAM, which is a Skip List.
3. Keys are added to a **Bloom Filter**, which can tell us quickly if a key is definitely not present.
4. When the MemTable is full, its data is saved to disk as a sorted file called an **SSTable**. A Min-Heap manages the order of flushing.
5. On a read, we check the MemTable first, and the disk is read only if the Bloom Filter says the key might exist.

## Modules

| Module | Files | What it does |
|---|---|---|
| Shared type | `common.h` | `Entry` struct (key, value, deleted flag) |
| MemTable | `memtable.h`, `memtable.cpp` | Skip List in RAM with put, get, remove |
| Bloom Filter | `bloom_filter.h`, `bloom_filter.cpp` | Checks whether a key might exist |
| SSTable | `sstable.h`, `sstable.cpp` | Writes and reads sorted data files |
| Core Engine | `database_engine.h`, `main.cpp` | Connects the modules |

## Team

| Member | Role | Module |
|---|---|---|
| Tanvi Kangar | Team Lead, Docs | Core Engine, design and documentation |
| Kripa Bargali | Developer | Bloom Filter |
| Yashika Rajwar | Developer | SSTable and file handling |
| Kushi Malik | DB-Testing | MemTable and testing |

## Technology

- Language: C++
- Storage: custom file-based SSTables
- Tools: C++ Standard Library, VS Code, Git/GitHub

## Build and Run

```
g++ main.cpp memtable.cpp sstable.cpp -o demo
./demo
```

(On Windows use `.\demo`.)

## Project Status

- **Phase 1:** Proposal and design (done)
- **Phase 2:** Development of the individual modules: MemTable, Bloom Filter, SSTable, basic engine (in progress)
- **Phase 3:** Final implementation: Min-Heap flush manager, Bloom Filter connected to reads, multiple SSTables, testing and speed measurement

## Future Work

Write-Ahead Logging (WAL) for crash recovery, automatic compaction of old files, and multi-threading.

## References

- Bonomi et al. (2006), An Improved Construction for Counting Bloom Filters
- M. Kleppmann, Designing Data-Intensive Applications
- RocksDB documentation and cppreference.com


Project-Based Learning (PBL) project, Department of Computer Science & Engineering, Graphic Era (Deemed to be University), Dehradun. Session 2026-27.

**Team ID:** DSCPP-III-2026-T146
**Mentor:** Dr. Siddhant Thapliyal
