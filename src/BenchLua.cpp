#include "BenchLua.h"
#include <map>
#include <lua/lua.hpp>

//-------------------------------------------------------------------------------------------------
BenchLua::BenchLua()
	: Benchmark()
{
	m_sName = "Lua";
}

template< class T>
void swapVars(lua_State *L, const char *a, const char *b)
{
	double temp;
	// save a to temp
	lua_getglobal(L, a);
	temp = lua_tonumber(L, -1);
	// a = b
	lua_getglobal(L, b);
	lua_setglobal(L, a);
	// b = a
	lua_pushnumber(L, temp);
	lua_setglobal(L, b);
}

//-------------------------------------------------------------------------------------------------
double BenchLua::DoBenchmark(const std::string& sExpr, long iCount)
{	
	std::map<std::string, double> var_list;
	var_list["a"] = 1.1;
	var_list["b"] = 2.2;
	var_list["c"] = 3.3;
	var_list["x"] = 2.123456;
	var_list["y"] = 3.123456;
	var_list["z"] = 4.123456;
	var_list["w"] = 5.123456;

	var_list["e"] = 2.718281828459045235360;
	var_list["pi"] = 3.141592653589793238462;

	lua_State *L;
	try
	{		
		L = luaL_newstate();
		
		for (auto it = var_list.begin(); it != var_list.end(); ++it)
		{
			lua_pushnumber(L, it->second);
			lua_setglobal(L, it->first.c_str());
		}

		luaL_dostring(L, ("function fnkt() return " + sExpr + "; end").c_str());
		luaL_dostring(L, ("function fnktWithSwap() local result = " + sExpr + "; a,b = b,a; x,y = y,x; return result; end").c_str());
		//luaL_dostring(L, ("function fnktWithSwap2() result = " + sExpr + "; a,b = b,a; x,y = y,x; end").c_str());
		luaL_dostring(L, "function swapall() a,b = b,a; x,y = y,x; end");
	}
	catch (std::exception& e)
	{
		StopTimerAndReport(e.what());
		return std::numeric_limits<double>::quiet_NaN();
	}

	// Calculate/bench and show result finally
	double fRes = 0;
	double fSum = 0;

	lua_getglobal(L, "fnkt");
	lua_pcall(L, 0, 1, 0);
	fRes = lua_tonumber(L, -1);
	lua_pop(L, 1);

	StartTimer();

	for (int j = 0; j < iCount; ++j)
	{
		lua_getglobal(L, "fnktWithSwap");
		lua_pcall(L, 0, 1, 0);
		double result = lua_tonumber(L, -1);
		fSum += result;
		lua_pop(L, 1);

		// swap variables
		//swapVars<int>(L, "a", "b");
		//swapVars<int>(L, "x", "y");
		
		// swap by lua function call (slower)
		// lua_getglobal(L, "swapall");
		// lua_pcall(L, 0, 0, 0);
	}

	StopTimer(fRes, fSum, iCount);
	lua_close(L);
	return m_fTime1;
}
