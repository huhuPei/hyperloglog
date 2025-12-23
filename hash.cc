#include "hash.h"

// murmur hash2 implementation copied from here: https://github.com/aappleby/smhasher
// then just modify a few non-core code

// Platform-specific functions and macros

// Microsoft Visual Studio
#if defined(_MSC_VER)

#define BIG_CONSTANT(x) (x)
// Other compilers
#else	// defined(_MSC_VER)
#define BIG_CONSTANT(x) (x##ULL)

#endif // !defined(_MSC_VER)

// 32bit murmurhash2
uint32_t MurmurHash(const char* data, size_t n, uint32_t seed) {
  const uint32_t m = 0x5bd1e995;
  const int r = 24;
  const char* limit = data + n;

  uint32_t h = n ^ seed;

  while (data + 4 <= limit) {
    uint32_t k = *(uint32_t*)data;
    k *= m;
    k ^= k >> r;
    k *= m;

    h *= m;
    h ^= k;

    data += 4;
  }

  switch (limit - data) {
    case 3:
      h ^= static_cast<unsigned char>(data[2]) << 16;
    case 2:
      h ^= static_cast<unsigned char>(data[1]) << 8;
    case 1:
      h ^= static_cast<unsigned char>(data[0]);
      h *= m;
      break;
  }

  h ^= h >> 13;
  h *= m;
  h ^= h >> 15;

  return h;
}

// 64bit murmurhash2
uint64_t MurmurHash64(const char* data, size_t n, uint64_t seed) {
  const uint64_t m = BIG_CONSTANT(0xc6a4a7935bd1e995);
  const int r = 47;
  const char* limit = data + n;

  uint64_t h = seed ^ (n * m);

  while (data + 8 <= limit) {
    uint64_t k = *(uint64_t*)data;
    k *= m; 
    k ^= k >> r; 
    k *= m; 
    
    h ^= k;
    h *= m;

    data += 8;
  }

  const unsigned char* remain = reinterpret_cast<const unsigned char*>(data);
  switch (limit - data) {
    case 7:
      h ^= static_cast<uint64_t>(remain[6]) << 48;
    case 6:
      h ^= static_cast<uint64_t>(remain[5]) << 40;
    case 5:
      h ^= static_cast<uint64_t>(remain[4]) << 32;
    case 4:
      h ^= static_cast<uint64_t>(remain[3]) << 24;
    case 3:
      h ^= static_cast<uint64_t>(remain[2]) << 16;
    case 2:
      h ^= static_cast<uint64_t>(remain[1]) << 8;
    case 1:
      h ^= static_cast<uint64_t>(remain[0]);
      h *= m;
      break;
  }

  h ^= h >> r;
  h *= m;
  h ^= h >> r;
  
  return h;
}