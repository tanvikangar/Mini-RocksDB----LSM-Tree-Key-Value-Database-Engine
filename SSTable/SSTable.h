#ifndef SSTABLE_H
#define SSTABLE_H

#include <string>
#include <vector>
#include "common.h"

class SSTable
{
    public:
        void write(const std::string& path, const std::vector<Entry>& sorted);
        std::vector<Entry> readAll(const std::string& path);
        bool get(const std::string& path, const std::string& key, Entry& result);

};

#endif