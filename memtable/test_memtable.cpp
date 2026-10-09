#include <iostream>
#include <vector>
#include "memtable.h"

using namespace std;

int main()
{
    MemTable m;

    m.put("c", "3");
    m.put("a", "1");
    m.put("b", "2");
    m.put("a", "one");        
    m.remove("b");

    Entry e;

    if (m.get("a", e))
        cout << "a = " << e.value << "\n";

    if (m.get("b", e))
        cout << "b deleted? " << e.deleted << "\n";

    cout << "found z? " << m.get("z", e) << "\n";

    cout << "size = " << m.size() << "\n";

    vector<Entry> all = m.getAll();

    for (size_t i = 0; i < all.size(); i++)
        cout << all[i].key << " ";

    cout << "\n";

    return 0;
}