#ifndef MUR_MUR_HASH_H_
#define MUR_MUR_HASH_H_

#include <stddef.h>
#include <stdint.h>

// 32bit murmurhash2
uint32_t MurmurHash(const char* data, size_t n, uint32_t seed);

// 64bit murmurhash2
uint64_t MurmurHash64(const char* data, size_t n, uint64_t seed);

#endif