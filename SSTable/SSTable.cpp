#include "SSTable.h"
#include <iostream>

using namespace std;

void SSTable::add(const string& key, const string& value)
{
    data.push_back({key, value});
}

string SSTable::get(const string& key)
{
    for (auto& pair : data)
    {
        if (pair.first == key)
            return pair.second;
    }

    return "";
}

void SSTable::display()
{
    for (auto& pair : data)
    {
        cout << pair.first << " : " << pair.second << endl;
    }
}