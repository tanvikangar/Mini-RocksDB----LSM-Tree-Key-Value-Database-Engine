#include <iostream>
#include <string>
#include <chrono>
#include <cstdio>
#include "database_engine.h"

using namespace std;

// Delete old files from a previous run
void cleanUp(const string& name)
{
    int id = 1;

    while (remove(FlushManager::pathFor(name, id).c_str()) == 0)
    {
        id++;
    }
}

int main()
{
    cleanUp("benchdb");

    DatabaseEngine db("benchdb", 1000);

    const int N = 20000;

    auto start = chrono::steady_clock::now();

    for (int i = 0; i < N; i++)
    {
        db.put("key_" + to_string(i),
               "value_" + to_string(i));
    }

    auto end = chrono::steady_clock::now();

    double ms = chrono::duration<double, milli>(end - start).count();

    cout << N << " writes took " << ms << " ms\n";

    cout << "SSTable files created: "
         << db.tableCount() << "\n";

    // Read some keys that exist and some that do not

    string v;
    int found = 0, notFound = 0;

    for (int i = 0; i < 1000; i++)
    {
        if (db.get("key_" + to_string(i * 7), v))
            found++;

        if (!db.get("missing_" + to_string(i), v))
            notFound++;
    }

    cout << "found " << found
         << " of 1000 existing keys\n";

    cout << "correctly missing " << notFound
         << " of 1000 absent keys\n";

    int total = db.bloomSkips + db.diskReads;

    cout << "File lookups avoided by Bloom Filter: "
         << db.bloomSkips << "\n";

    cout << "File lookups really done: "
         << db.diskReads << "\n";

    if (total > 0)
    {
        cout << "Disk reads saved: "
             << (100 * db.bloomSkips / total) << "%\n";
    }

    return 0;
}