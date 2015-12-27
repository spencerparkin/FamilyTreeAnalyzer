// FtaFillCacheFunction.h

#pragma once

#include "FtaLuaFunction.h"

class FtaFillCacheFunction : public FtaLuaFunction
{
public:

	wxDECLARE_DYNAMIC_CLASS( FtaFillCacheFunction );

	FtaFillCacheFunction( void );
	virtual ~FtaFillCacheFunction( void );

	virtual int Call( lua_State* L ) override;
	virtual int Help( lua_State* L ) override;
	virtual wxString LuaFunctionName( void ) override { return "fillCache"; }
};

// FtaFillCacheFunction.h
