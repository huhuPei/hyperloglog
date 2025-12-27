#ifndef HYPER_LOG_LOG_H_
#define HYPER_LOG_LOG_H_

#include "cardinality.h"
#include <stdint.h>

class HyperLogLog: public Cardinality {
 public:
  HyperLogLog(int b);
  virtual ~HyperLogLog();
  virtual bool Add(const char* data, size_t n);
  virtual long long Count();
  virtual size_t UsageOfMemory();
  virtual double StandardError();
  virtual size_t NumOfBuckets();
 private:
  void InitRegisters(size_t m);
  // equal to M[reg_num]
  uint8_t GetRegister(long long reg_num);
  void SetRegister(long long reg_num, uint8_t val);
  double GetAlpha(size_t m);

 private:
  // b_>=4, m_>=16
  const int b_;
  const size_t m_;
  double alpham_;
  // registers store R of each bucket, 6 bits per register
  uint8_t* registers_;
  int regs_bytes_;
};

#endif