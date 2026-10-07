#include "memtable.h"
#include <cstdlib>

using namespace std;

MemTable::MemTable()
{
    head = newNode("", "", false);
    level = 1;
    count = 0;
}

MemTable::~MemTable()
{
    clear();
    delete head;
}

Node* MemTable::newNode(string key, string value, bool deleted)
{
    Node* n = new Node;

    n->key = key;
    n->value = value;
    n->deleted = deleted;

    for (int i = 0; i < MAX_LEVEL; i++)
    {
        n->next[i] = NULL;
    }

    return n;
}

int MemTable::randomLevel()
{
    int lvl = 1;

    while (lvl < MAX_LEVEL && rand() % 2 == 0)
    {
        lvl++;
    }

    return lvl;
}

void MemTable::insert(string key, string value, bool deleted)
{
    Node* update[MAX_LEVEL];
    Node* cur = head;

    for (int i = level - 1; i >= 0; i--)
    {
        while (cur->next[i] != NULL &&
               cur->next[i]->key < key)
        {
            cur = cur->next[i];
        }

        update[i] = cur;
    }

    Node* found = cur->next[0];

    if (found != NULL && found->key == key)
    {
        found->value = value;
        found->deleted = deleted;
        return;
    }

    int lvl = randomLevel();

    if (lvl > level)
    {
        for (int i = level; i < lvl; i++)
        {
            update[i] = head;
        }

        level = lvl;
    }

    Node* n = newNode(key, value, deleted);

    for (int i = 0; i < lvl; i++)
    {
        n->next[i] = update[i]->next[i];
        update[i]->next[i] = n;
    }

    count++;
}

void MemTable::put(string key, string value)
{
    insert(key, value, false);
}

void MemTable::remove(string key)
{
    insert(key, "", true);
}

bool MemTable::get(string key, Entry& result)
{
    Node* cur = head;

    for (int i = level - 1; i >= 0; i--)
    {
        while (cur->next[i] != NULL &&
               cur->next[i]->key < key)
        {
            cur = cur->next[i];
        }
    }

    Node* found = cur->next[0];

    if (found != NULL && found->key == key)
    {
        result.key = found->key;
        result.value = found->value;
        result.deleted = found->deleted;

        return true;
    }

    return false;
}

int MemTable::size()
{
    return count;
}

vector<Entry> MemTable::getAll()
{
    vector<Entry> all;
    Node* n = head->next[0];

    while (n != NULL)
    {
        Entry e;

        e.key = n->key;
        e.value = n->value;
        e.deleted = n->deleted;

        all.push_back(e);

        n = n->next[0];
    }

    return all;
}

void MemTable::clear()
{
    Node* n = head->next[0];

    while (n != NULL)
    {
        Node* nextNode = n->next[0];

        delete n;

        n = nextNode;
    }

    for (int i = 0; i < MAX_LEVEL; i++)
    {
        head->next[i] = NULL;
    }

    level = 1;
    count = 0;
}