#ifndef DATABASE_ENGINE_H
#define DATABASE_ENGINE_H
#include <string>
#include "common.h"
#include "memtable.h"
#include "SSTable.h"

class DatabaseEngine
{
    public:
        void put(const std::string& key, const std::string& value) 
        {
            memtable.put(key, value);
        }

        void remove(const std::string& key) 
        {
            memtable.remove(key);
        }

        bool get(const std::string& key, std::string& value) 
        {
            Entry e;
            if (!memtable.get(key, e)) 
                return false;
            if (e.deleted) 
                return false;
            value = e.value;
            return true;
        }

        void flushToDisk(const std::string& path) 
        {  
            SSTable::write(path, memtable.getAll());
            memtable.clear();
        }

    private:
        MemTable memtable;
};

#endif