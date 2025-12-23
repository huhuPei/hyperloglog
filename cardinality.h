#ifndef Card_IF_H_
#define Card_IF_H_

#include <stddef.h>

class Cardinality {
public:
  Cardinality() {};
  virtual ~Cardinality() {};
  virtual bool Add(const char* data, size_t n) = 0;
  // The output is an estimate of the cardinality
  virtual long long Count() = 0;
  virtual size_t UsageOfMemory() = 0;
  virtual double StandardError() = 0;
};

#endif