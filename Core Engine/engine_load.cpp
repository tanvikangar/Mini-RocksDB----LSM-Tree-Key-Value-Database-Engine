#include "database_engine.h"
void DatabaseEngine::loadExistingFiles() 
{
    int id = 1;
    while (true) 
    {
        std::string path = FlushManager::pathFor(name, id);
        SSTable sstable;
        std::vector<Entry> entries = sstable.readAll(path);
        
        if (entries.empty())
            break;  
        TableInfo info;
        info.path = path;

        for (size_t i = 0; i < entries.size(); i++)
        info.bloom.add(entries[i].key);  
        tables.push_back(info);
        id++;
    }
    flusher.setNextId(id);  
}