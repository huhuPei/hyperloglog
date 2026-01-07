#include "loglog.h"
#include "hash.h"

#include <stdint.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <initializer_list>

// if m > 64, then alpha = 0.3970
const double LogLog::alpha_table_[8] = {
  0,      0.2639, 0.3118, 0.3460, 
  0.3685, 0.3827, 0.3903, 0.3970
};

LogLog::LogLog(int k): k_(k), m_(1 << k) {
  assert(k < 32 && k >= 1);
  alpham_ = k >= 7? alpha_table_[7] : alpha_table_[k]; 
  M_ = new uint8_t[m_];
  memset(M_, 0, m_);
}

LogLog::~LogLog() {
  delete[] M_;
}

// N = alpha * m * 2^(Rsum/m)
long long LogLog::Count() {
  long long Rsum = 0;
  for (int j = 0; j < m_; j++) {
    Rsum += M_[j];
  }
  double Ravg = Rsum / (double)m_;
  return static_cast<long long>(alpham_* m_ * pow(2, Ravg));
}

// each bucket need 1B
size_t LogLog::UsageOfMemory() {
  return m_;
}

double LogLog::StandardError() {
  return 1.30 / sqrt(m_);
}

size_t LogLog::NumOfBuckets() {
  return m_;
}

bool LogLog::Add(const char* data, size_t n) {
  uint32_t j, hash;
  hash = MurmurHash(data, n, 0xcd6f1b43);
  j = hash >> (32 - k_);
  hash = (hash << k_) | (1 << (k_ - 1));
  // the position of its first 1-bit
  int r = __builtin_clz(hash) + 1;
  if (M_[j] < r) {
    M_[j] = r;
    return true;
  }
  return false;
}

Estimator* LogLog::Merge(std::initializer_list<Estimator*> list) {
  if (list.size() <= 0) {
    return this;
  }

  LogLog* merged = new LogLog(this->k_);
  memcpy(merged->M_, this->M_, m_);
  
  for (auto est : list) {
    auto log = dynamic_cast<LogLog*>(est);
    // all of counter must be the same type, and have equal buckets
    if (log == nullptr || merged->NumOfBuckets() != log->NumOfBuckets()) {
      delete merged;
      return nullptr;
    }
    for (int j = 0; j < m_; j++) {
      if (merged->M_[j] < log->M_[j]) {
        merged->M_[j] = log->M_[j];
      }
    }
  }
  return merged;
}

Estimator* NewLogLog(int k) {
  return new LogLog(k);
}