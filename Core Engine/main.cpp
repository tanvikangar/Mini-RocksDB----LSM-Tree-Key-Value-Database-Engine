#include <iostream>
#include "database_engine.h"
int main() 
{
    DatabaseEngine db;
    db.put("banana", "yellow");
    db.put("apple", "red");
    db.put("cherry", "dark red");
    db.remove("banana");
    std::string v;
    if (db.get("apple", v)) 
        std::cout << "apple -> " << v << "\n";
    if (!db.get("banana", v)) 
        std::cout << "banana -> not found (deleted)\n";
    db.flushToDisk("phase2.sst");
    std::cout << "\nContents of phase2.sst:\n";
    std::vector<Entry> all = SSTable::readAll("phase2.sst");
    for (size_t i = 0; i < all.size(); i++) 
    {
        std::cout << all[i].key << " = " << all[i].value;
        if (all[i].deleted) std::cout << "  [deleted]";
        std::cout << "\n";
    }
    return 0;
}
