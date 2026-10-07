#include <iostream>
#include <string>
#include "bloom_filter.h"
int main() 
{
    BloomFilter bf;
    for (int i = 0; i < 1000; i++) 
    {
        bf.add("key" + std::to_string(i));
    }

    int missing = 0;
    for (int i = 0; i < 1000; i++)
    {
        if (!bf.mightcontain("key" + std::to_string(i))) 
        {
            missing++;
        }
    }
    std::cout << "False negatives (must be 0): " << missing << "\n";

    int wrong = 0;
    for (int i = 0; i < 1000; i++)
    {
        if (bf.mightcontain("other" + std::to_string(i))) 
        {
            wrong++;
        }
    }
    std::cout << "False positives: " << wrong << " out of 1000\n";
    return 0;
}