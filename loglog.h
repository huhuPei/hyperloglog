#ifndef LOG_LOG_H_
#define LOG_LOG_H_

#include "cardinality.h"

#include <stdint.h>
#include <stddef.h>

class LogLog : public Cardinality {
 public:
  LogLog(int k);
  virtual ~LogLog();
  virtual bool Add(const char* data, size_t n);
  virtual long long Count();
  virtual size_t UsageOfMemory();
  virtual double StandardError();

 private:
  static const double alpha_table_[];
  const int k_;
  const size_t m_;
  double alpham_;
  // store R of each bucket 
  uint8_t* M_;
  long Rsum_;
};

#endif