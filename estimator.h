#ifndef Card_IF_H_
#define Card_IF_H_

#include <stddef.h>
#include <initializer_list>

class Estimator {
public:
  Estimator() {};
  virtual ~Estimator() {};
  virtual bool Add(const char* data, size_t n) = 0;
  // The output is an estimate of the cardinality
  virtual long long Count() = 0;
  virtual size_t UsageOfMemory() = 0;
  virtual double StandardError() = 0;
  virtual size_t NumOfBuckets() = 0;
  virtual Estimator* Merge(std::initializer_list<Estimator*> list) = 0;
};

extern Estimator* NewLogLog(int);
extern Estimator* NewHyperLogLog(int);

#endif