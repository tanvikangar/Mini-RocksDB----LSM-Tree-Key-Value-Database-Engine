
#include <iostream>
#include <string>
#include <cstdio>
#include "database_engine.h"

void cleanUp(const std::string& name)
{
    int id = 1;

    while (std::remove(FlushManager::pathFor(name, id).c_str()) == 0)
        id++;
}

int main()
{
    cleanUp("testdb");

    std::string v;
    int errors = 0;

    {
        DatabaseEngine db("testdb", 100);

        for (int i = 0; i < 1000; i++)
            db.put("k" + std::to_string(i),
                   "v" + std::to_string(i));

        std::cout << "files created: " << db.tableCount() << "\n";

        for (int i = 0; i < 1000; i++)
        {
            if (!db.get("k" + std::to_string(i), v) ||
                v != "v" + std::to_string(i))
                errors++;
        }

        db.put("k5", "NEW");
        db.flush();

        if (!db.get("k5", v) || v != "NEW")
            errors++;

        db.remove("k7");
        db.flush();

        if (db.get("k7", v))
            errors++;

        for (int i = 0; i < 200; i++)
        {
            if (db.get("missing" + std::to_string(i), v))
                errors++;
        }

        std::cout << "Bloom skips: " << db.bloomSkips << "\n";
    }

    {
        DatabaseEngine db("testdb", 100);

        if (!db.get("k100", v) || v != "v100")
            errors++;

        if (!db.get("k5", v) || v != "NEW")
            errors++;

        if (db.get("k7", v))
            errors++;

        std::cout << "files loaded again: "
                  << db.tableCount() << "\n";
    }

    std::cout << (errors == 0 ? "ALL TESTS PASSED" : "SOME TESTS FAILED")
              << " (errors: " << errors << ")\n";

    cleanUp("testdb");

    return 0;
}
