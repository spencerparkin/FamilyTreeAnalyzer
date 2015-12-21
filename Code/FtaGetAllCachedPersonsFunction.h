// FtaGetAllCachedPersonsFunction.h

#pragma once

#include "FtaLuaFunction.h"

class FtaGetAllCachedPersonsFunction : public FtaLuaFunction
{
public:

	wxDECLARE_DYNAMIC_CLASS( FtaGetAllCachedPersonsFunction );

	FtaGetAllCachedPersonsFunction( void );
	virtual ~FtaGetAllCachedPersonsFunction( void );

	virtual int Call( lua_State* L ) override;
	virtual int Help( lua_State* L ) override;
	virtual wxString LuaFunctionName( void ) override { return "getAllCachedPersons"; }
};

// FtaGetAllCachedPersonsFunction.h
