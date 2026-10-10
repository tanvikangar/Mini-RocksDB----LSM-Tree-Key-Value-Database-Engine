#include "sstable.h"
#include <fstream>
#include <iostream>

void SSTable::write(const std::string& path,
                    const std::vector<Entry>& sorted) {
    std::ofstream file(path.c_str());

    if (!file) {
        std::cerr << "Error: cannot create " << path << "\n";
        return;
    }

    for (size_t i = 0; i < sorted.size(); i++) {
        file << sorted[i].key << "|"
             << sorted[i].value << "|"
             << (sorted[i].deleted ? 1 : 0) << "\n";
    }
}

static bool parseLine(const std::string& line, Entry& e) {
    size_t first = line.find('|');
    size_t last = line.rfind('|');

    if (first == std::string::npos || first == last)
        return false;

    e.key = line.substr(0, first);
    e.value = line.substr(first + 1, last - first - 1);
    e.deleted = (line.substr(last + 1) == "1");

    return true;
}

std::vector<Entry> SSTable::readAll(const std::string& path) {
    std::vector<Entry> all;
    std::ifstream file(path.c_str());
    std::string line;

    while (std::getline(file, line)) {
        Entry e;
        if (parseLine(line, e))
            all.push_back(e);
    }

    return all;
}

bool SSTable::get(const std::string& path,
                  const std::string& key,
                  Entry& result) {
    std::ifstream file(path.c_str());
    std::string line;

    while (std::getline(file, line)) {
        Entry e;

        if (!parseLine(line, e))
            continue;

        if (e.key == key) {
            result = e;
            return true;
        }

        if (e.key > key)
            break;
    }

    return false;
}