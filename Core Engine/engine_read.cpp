#include "database_engine.h"

bool DatabaseEngine::get(const std::string& key, std::string& value)
{
    Entry e;

    if (memtable.get(key, e)) 
    {
        if (e.deleted) 
        {
            return false;
        }
        value = e.value;
        return true;
    }

    return false;
}