#ifndef BENCH_LUA_H
#define BENCH_LUA_H

#include <vector>
#include <string>

#include "Benchmark.h"

//-------------------------------------------------------------------------------------------------
class BenchLua : public Benchmark
{
public:

   BenchLua();

   double DoBenchmark(const std::string &sExpr, long iCount);

};

#endif
