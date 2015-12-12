// FtaLuaUtil.h

#pragma once

#include <lua.hpp>

class FtaLuaStackPopper
{
public:

	FtaLuaStackPopper( lua_State* L );
	~FtaLuaStackPopper( void );

private:

	lua_State* L;
	int stackTop;
};

bool FtaLuaReportError( lua_State* L, int result );

// FtaLuaUtil.h