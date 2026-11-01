#ifndef FLUSH_MANAGER_H
#define FLUSH_MANAGER_H
#include <string>
#include <vector>
#include <queue>
#include "common.h"
#include "bloom_filter.h"
#include "sstable.h"
struct TableInfo 
{
    std::string path;
    BloomFilter bloom;
};

struct FlushTask 
{
    int id;                   
    std::vector<Entry> entries;
};

struct OlderFirst 
{
    bool operator()(const FlushTask& a, const FlushTask& b) const 
    {
        return a.id > b.id;
    }
};

class FlushManager 
{
public:
    FlushManager(const std::string& name);
    void setNextId(int id);
    void schedule(const std::vector<Entry>& entries);    
    std::vector<TableInfo> processAll();                
    int pending();
    static std::string pathFor(const std::string& name, int id);

private:
    std::string name;
    int nextId;
    std::priority_queue<FlushTask, std::vector<FlushTask>, OlderFirst> heap;
};

#endif