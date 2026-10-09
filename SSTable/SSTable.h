#ifndef SSTABLE_H
#define SSTABLE_H
#include <string>
#include <vector>
#include "common.h"

class SSTable
{
    public:
        static void write(const std::string& path, const std::vector<Entry>& sorted);
        static std::vector<Entry> readdAll(const std::string& path);
        static bool get(const std::string& path, const std::string& key, Entry& result);
};

#endif