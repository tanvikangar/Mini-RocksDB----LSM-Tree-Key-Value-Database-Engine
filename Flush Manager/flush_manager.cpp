#include "flush_manager.h"
FlushManager::FlushManager(const std::string& dbName)
{
    name = dbName;
    nextId = 1;
}

void FlushManager::setNextId(int id) 
{
    nextId = id;
}

std::string FlushManager::pathFor(const std::string& name, int id) 
{
    return name + "_" + std::to_string(id) + ".sst";     
}

void FlushManager::schedule(const std::vector<Entry>& entries) 
{
    FlushTask task;
    task.id = nextId;
    task.entries = entries;
    nextId++;
    heap.push(task);
}

int FlushManager::pending() 
{
    return (int)heap.size();
}

std::vector<TableInfo> FlushManager::processAll() 
{
    std::vector<TableInfo> done;
    while (!heap.empty()) 
    {
        FlushTask task = heap.top();    
        heap.pop();

        TableInfo info;
        info.path = pathFor(name, task.id);

        SSTable sstable;
        sstable.write(info.path, task.entries);

        for (size_t i = 0; i < task.entries.size(); i++)
        {
            info.bloom.add(task.entries[i].key);
        }
        done.push_back(info);
    }
    return done;
}