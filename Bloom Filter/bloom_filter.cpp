#include "bloom_filter.h"
BloomFilter::BloomFilter() 
{
    for (int i = 0; i < bloom_size; i++) 
    {
        bits[i] = false;
    }
}
int BloomFilter::hash1(const std::string& s) 
{
    unsigned long h = 0;
    for(size_t i=0; i < s.size(); i++) 
    {
        h=h*31 + s[i];
    }
    return h % bloom_size;
}
int BloomFilter::hash2(const std::string& s) 
{
    unsigned long h = 0;
    for(size_t i=0; i < s.size(); i++) 
    {
        h=h*37 + s[i];
    }
    return h % bloom_size;
}
int BloomFilter::hash3(const std::string& s) 
{
    unsigned long h = 0;
    for(size_t i=0; i < s.size(); i++) 
    {
        h=h*41 + s[i];
    }
    return h % bloom_size;
}
void BloomFilter::add(const std::string& key)
{
    bits[hash1(key)] = true;
    bits[hash2(key)] = true;
    bits[hash3(key)] = true;
}
bool BloomFilter::mightcontain(const std::string& key) 
{
    if(!bits[hash1(key)]) 
    return false;
    if(!bits[hash2(key)]) 
    return false; 
    if(!bits[hash3(key)]) 
    return false;

    return true;
}