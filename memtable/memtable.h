#ifndef MEMTABLE_H
#define MEMTABLE_H

#include <string>
#include <vector>
#include "common.h"

const int MAX_LEVEL = 6;

struct Node
{
    std::string key;
    std::string value;
    bool deleted;

    Node* next[MAX_LEVEL];
};

class MemTable
{
public:
    MemTable();
    ~MemTable();

    void put(std::string key, std::string value);
    void remove(std::string key);

    bool get(std::string key, Entry& result);

    int size();

    std::vector<Entry> getAll();

    void clear();

private:
    Node* head;
    int level;
    int count;

    int randomLevel();

    void insert(
        std::string key,
        std::string value,
        bool deleted
    );

    Node* newNode(
        std::string key,
        std::string value,
        bool deleted
    );
};

#endif