#include <iostream>
#include <cstdio>
#include "flush_manager.h"

using namespace std;

int main()
{
    FlushManager fm("testflush");

    vector<Entry> a, b, c;
    Entry e;

    e.key = "a";
    e.value = "1";
    a.push_back(e);

    e.key = "b";
    e.value = "2";
    b.push_back(e);

    e.key = "c";
    e.value = "3";
    c.push_back(e);

    fm.schedule(a);
    fm.schedule(b);
    fm.schedule(c);

    cout << "pending jobs: " << fm.pending()
         << " (expected 3)" << endl;

    vector<TableInfo> done = fm.processAll();

    cout << "files written: " << done.size()
         << " (expected 3)" << endl;

    for (size_t i = 0; i < done.size(); i++)
    {
        cout << done[i].path << endl;
    }

    cout << "pending after: " << fm.pending()
         << " (expected 0)" << endl;

    cout << "bloom knows 'a' in file 1: "
         << done[0].bloom.mightcontain("a") << endl;

    for (size_t i = 0; i < done.size(); i++)
    {
        remove(done[i].path.c_str());
    }

    return 0;
}