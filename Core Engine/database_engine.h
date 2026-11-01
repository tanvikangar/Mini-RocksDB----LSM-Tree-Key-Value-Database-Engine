#ifndef DATABASE_ENGINE_H
#define DATABASE_ENGINE_H
#include <string>
#include <vector>
#include "common.h"
#include "memtable.h"
#include "sstable.h"
#include "bloom_filter.h"
#include "flush_manager.h"

class DatabaseEngine 
{
public:
    DatabaseEngine(const std::string& name = "mydb", int memtableLimit = 1000);
    ~DatabaseEngine();

    void put(const std::string& key, const std::string& value);
    void remove(const std::string& key);

    bool get(const std::string& key, std::string& value);
    void flush();                     

    int tableCount();
    int bloomSkips;                  
    int diskReads;                 

private:

    std::string name;
    int limit;
    MemTable memtable;
    FlushManager flusher;
    
    std::vector<TableInfo> tables;  
    void loadExistingFiles();
};

#endif