#ifndef LOG_LOG_H_
#define LOG_LOG_H_

#include "estimator.h"

#include <stdint.h>
#include <stddef.h>

class LogLog : public Estimator {
 public:
  LogLog(int k);
  LogLog(LogLog& other);
  virtual ~LogLog();
  virtual bool Add(const char* data, size_t n);
  virtual long long Count();
  virtual size_t UsageOfMemory();
  virtual double StandardError();
  virtual size_t NumOfBuckets();
  virtual Estimator* Merge(std::initializer_list<Estimator*> list);
  LogLog& operator=(LogLog& other);

 private:
  static const double alpha_table_[];
  const int k_;
  const size_t m_;
  double alpham_;
  // store R of each bucket 
  uint8_t* M_;
};

#endif