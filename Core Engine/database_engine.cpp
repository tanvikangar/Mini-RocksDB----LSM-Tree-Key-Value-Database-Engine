#include "database_engine.h"
DatabaseEngine::DatabaseEngine(const std::string& dbName, int memtableLimit)
    : flusher(dbName) 
{
    name = dbName;
    limit = memtableLimit;
    bloomSkips = 0;
    diskReads = 0;
    loadExistingFiles();     
}

DatabaseEngine::~DatabaseEngine() 
{
    flush();      
}

void DatabaseEngine::put(const std::string& key, const std::string& value) 
{
    memtable.put(key, value);
    if (memtable.size() >= limit) 
        flush();   
}

void DatabaseEngine::remove(const std::string& key) 
{
    memtable.remove(key);
    if (memtable.size() >= limit) 
        flush();
}

void DatabaseEngine::flush() {

    if (memtable.size() == 0) 
        return;

    flusher.schedule(memtable.getAll());      
    memtable.clear();       
    std::vector<TableInfo> done = flusher.processAll();
    for (size_t i = 0; i < done.size(); i++)
    {
        tables.push_back(done[i]);
    }

}

int DatabaseEngine::tableCount() 
{
    return (int)tables.size();
}