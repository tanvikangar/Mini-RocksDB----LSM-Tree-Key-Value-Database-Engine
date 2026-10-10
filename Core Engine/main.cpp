#include <iostream>
#include "database_engine.h"
int main() 
{
    DatabaseEngine db;
    db.put("grapes", "green");
    db.put("cherry", "dark red");
    db.put("watermelon", "dark green");
    db.remove("grapes");

    std::string v;
    if (db.get("cherry", v)) 
        std::cout << "cherry -> " << v << "\n";
    if (!db.get("grapes", v)) 
        std::cout << "grapes -> not found (deleted)\n";

    db.flushToDisk("phase2.sst");
    std::cout << "\nContents of phase2.sst:\n";

    SSTable sstable;
    std::vector<Entry> all = sstable.readAll("phase2.sst");
    for (size_t i = 0; i < all.size(); i++) 
    {
        std::cout << all[i].key << " = " << all[i].value;
        if (all[i].deleted) std::cout << "  [deleted]";
        std::cout << "\n";
    }
    return 0;
}
