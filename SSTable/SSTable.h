#ifndef SSTABLE_H
#define SSTABLE_H

#include <string>
#include <vector>
#include <utility>

using namespace std;

class SSTable
{
private:
    vector<pair<string, string>> data;

public:
    void add(const string& key, const string& value);
    string get(const string& key);
    void display();
};

#endif