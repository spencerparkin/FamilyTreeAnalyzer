// FtaGetAllCachedPersonIdsFunction.h

#pragma once

#include "FtaLuaFunction.h"

class FtaGetAllCachedPersonIdsFunction : public FtaLuaFunction
{
public:

	wxDECLARE_DYNAMIC_CLASS( FtaGetAllCachedPersonIdsFunction );

	FtaGetAllCachedPersonIdsFunction( void );
	virtual ~FtaGetAllCachedPersonIdsFunction( void );

	virtual int Call( lua_State* L ) override;
	virtual int Help( lua_State* L ) override;
	virtual wxString LuaFunctionName( void ) override { return "getAllCachedPersonIds"; }
};

// FtaGetAllCachedPersonIdsFunction.h
