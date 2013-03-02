#include "BenchExprTk.h"

#include <windows.h>
#include <cmath>

#define exprtk_enable_all_optimizations
#include "exprtk/exprtk.hpp"

using namespace std;


//-------------------------------------------------------------------------------------------------
BenchExprTk::BenchExprTk()
: Benchmark()
{
   m_sName = "ExprTk";
}

//-------------------------------------------------------------------------------------------------
double BenchExprTk::DoBenchmark(const std::string &sExpr, long iCount)
{
   double a = 1.0;
   double b = 2.0;
   double c = 3.0;

   exprtk::symbol_table<double> symbol_table;
   exprtk::expression<double> expr;

   symbol_table.add_variable("a", a);
   symbol_table.add_variable("b", b);
   symbol_table.add_variable("c", c);

   static double e = exprtk::details::numeric::constant::e;
   symbol_table.add_variable("e", e, true);

   symbol_table.add_constants();

   expr.register_symbol_table(symbol_table);

   exprtk::parser<double> parser;
   parser.compile(sExpr, expr);

   // Calculate/bench and show result finally
   double fTime = 0;
   double fRes  = 0;
   double fSum  = 0;

   fRes = expr.value();
   StartTimer();
   for (int j = 0; j < iCount; j++)
   {
      std::swap(a,b);
      fSum += expr.value();
   }
   StopTimer(fRes, fSum, iCount);
   return m_fTime1;
}