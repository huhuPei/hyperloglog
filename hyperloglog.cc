/* Registers encoding refer to "hyperloglog.c" of redis [https://github.com/redis/redis.git].
   Implement detail can be obtained from this repo.
*/
#include "hyperloglog.h"
#include "hash.h"

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <assert.h>
#include <string.h>

#define REG_BITS 5
#define REG_MAX ((1<<REG_BITS)-1)

HyperLogLog::HyperLogLog(int b): b_(b), m_(1<<b), alpham_(GetAlpha(m_)){
  assert(b_ >= 4 && b < 32);
  InitRegisters(m_);
}

HyperLogLog::~HyperLogLog() {
  delete[] registers_;
}

bool HyperLogLog::Add(const char* data, size_t n) {
  uint32_t j, hash;
  hash = MurmurHash(data, n, 0xcd6f1b43);
  j = hash >> (32 - b_);
  hash = (hash << b_) | (1 << (b_ - 1));
  // the position of its first 1-bit
  uint8_t new_val = __builtin_clz(hash) + 1;
  
  uint8_t old_val = GetRegister(j);
  if (new_val > old_val) {
    SetRegister(j, new_val);
    return true;
  }
  return false;
}

long long HyperLogLog::Count() {
  double Z, V = 0;
  long long count;
  for (int j = 0; j < m_; j++) {
    uint8_t val = GetRegister(j);
    Z += 1.0 / (1 << val);
    if (val == 0) {
      ++V;
    }
  }
  Z = 1.0 / Z;
  // harmonic mean: m_*Z
  double E = alpham_ * m_ * m_ * Z;
  if (E <= 2.5 * m_) {
    if (V > 0) {
      count = static_cast<long long>(m_ * log(m_ / V));
    }
  } else if (E <= (1ll << 32) / 30.0) {
    count = static_cast<long long>(E);    
  } else {
    count = static_cast<long long>(-(1ll << 32) * log(1-E/(1ll << 32)));
  }
  return count;
}

size_t HyperLogLog::UsageOfMemory() {
  return regs_bytes_;
}

double HyperLogLog::StandardError() {
  return 1.04 / sqrt(m_);
}

size_t HyperLogLog::NumOfBuckets() {
  return m_;
}

void HyperLogLog::InitRegisters(size_t m) {
  unsigned int bytes = m * REG_BITS / 8 + 1;
  if (m * REG_BITS % 8 >= 6) {
    bytes += 1;
  }
  registers_ = new uint8_t[bytes];
  regs_bytes_ = bytes;
  
  memset(registers_, 0, bytes);
}

uint8_t HyperLogLog::GetRegister(long long reg_num) {
  unsigned int byte = reg_num * REG_BITS / 8;
  unsigned int fb = reg_num * REG_BITS & 7;
  unsigned int fb8 = 8 - fb;
  unsigned int b0 = registers_[byte];
  unsigned int b1 = registers_[byte+1];
  return ((b0 >> fb) | (b1 << fb8)) & REG_MAX;
}

void HyperLogLog::SetRegister(long long reg_num, uint8_t val) {
  unsigned int byte = reg_num * REG_BITS / 8;
  unsigned int fb = reg_num * REG_BITS & 7;
  unsigned int fb8 = 8 - fb;
  unsigned int v = (val);
  registers_[byte] &= ~(REG_MAX << fb);
  registers_[byte] |= v << fb;
  registers_[byte+1] &= ~(REG_MAX >> fb8);
  registers_[byte+1] |= v >> fb8;
}

double HyperLogLog::GetAlpha(size_t m) {
  switch (m) {
    case 16:
      return 0.673;
    case 32:
      return 0.697;
    case 64:
      return 0.709;
    // m >= 128
    default:
      return (0.7213 / (1 + 1.079 / m));
  }
}