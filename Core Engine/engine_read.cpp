#include "database_engine.h"

bool DatabaseEngine::get(const std::string& key, std::string& value) {
    Entry e;

    if (memtable.get(key, e)) {
        if (e.deleted) return false;
        value = e.value;
        return true;
    }

    for (int i = (int)tables.size() - 1; i >= 0; i--) {
        if (!tables[i].bloom.mightcontain(key)) {
            bloomSkips++;
            continue;
        }
        diskReads++;
        SSTable sstable;
        if (sstable.get(tables[i].path, key, e)) {
            if (e.deleted) return false;
            value = e.value;
            return true;
        }
    }
    return false;
}