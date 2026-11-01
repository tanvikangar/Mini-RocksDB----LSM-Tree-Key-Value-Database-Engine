#include "database_engine.h"
void Database::loadExistingFiles() 
{
    int id = 1;

    while (true) 
    {
        std::string path = FlushManager::pathFor(name, id);
        std::vector<Entry> entries = SSTable::readAll(path);
        
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