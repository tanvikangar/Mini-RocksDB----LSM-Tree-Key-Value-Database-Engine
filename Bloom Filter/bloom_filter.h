#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H
#include <string>

const int bloom_size = 1000;

class BloomFilter 
{
    public:
        BloomFilter();
        void add(const std::string& key);
        bool mightcontain(const std::string& key) ;
    private:
        bool bits[bloom_size];
        int hash1(const std::string& s);
        int hash2(const std::string& s);
        int hash3(const std::string& s);
};

#endif 