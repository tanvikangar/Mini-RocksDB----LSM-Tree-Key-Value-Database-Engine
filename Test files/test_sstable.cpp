#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include "sstable.h"

int main()
{
    SSTable table;
    std::vector<Entry> entries;

    for (int i = 0; i < 1000; i++) 
    {
        Entry e;
        e.key = "key" + std::to_string(i);
        e.value = "value" + std::to_string(i);
        entries.push_back(e);
    }

    std::sort(entries.begin(), entries.end(),[](const Entry& a, const Entry& b)
    {
            return a.key < b.key;
    }
);

    table.write("test.sst", entries);

    std::ifstream file("test.sst");
    if (!file) 
    {
        std::cout << "Error creating SSTable file.\n";
        return 1;
    }
    file.close();

    int missing = 0, wrong = 0;
    Entry result;

    for (int i = 0; i < 1000; i++) 
    {
        if (!table.get("test.sst", "key" + std::to_string(i), result))
            missing++;

        if (table.get("test.sst", "other" + std::to_string(i), result))
            wrong++;
    }

    std::cout << "Missing keys: " << missing << "\n";
    std::cout << "Incorrect matches: " << wrong << "\n";
    std::cout << "Entries read: " << table.readAll("test.sst").size() << "\n";

    return 0;
}