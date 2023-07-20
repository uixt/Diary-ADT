// hash_str(str, hash_length) calculates the hash value of string str with a
//   length of hash_length bits.
// requires: hash_length must be between 2 and 16 inclusive
// time: O(n), where n is the length of the string


//This hash function uses the djb2 algorithm to hash the input string. 


#include "hash-functions.h"
#include <assert.h>
#include <stdio.h>


int hash_str(const char *str, int hash_length) {
    assert(2 <= hash_length && hash_length <= 16);
    assert(str);

    unsigned int hash_val = 0;
    int i = 0;
    while (*str) {
        hash_val = (hash_val << 5) + hash_val + *str;
        str++;
        i++;
    }
    return hash_val % (1 << hash_length);
}

int hash_char(char c, int hash_length) {
    assert(2 <= hash_length && hash_length <= 16);

    unsigned int hash_val = 0;
    hash_val = (hash_val << 5) + hash_val + c;
    return hash_val % (1 << hash_length);
}






