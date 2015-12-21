// FtaWhoAmIFunction.h

#pragma once

#include "FtaLuaFunction.h"

class FtaWhoAmIFunction : public FtaLuaFunction
{
public:

	wxDECLARE_DYNAMIC_CLASS( FtaWhoAmIFunction );

	FtaWhoAmIFunction( void );
	virtual ~FtaWhoAmIFunction( void );

	virtual int Call( lua_State* L ) override;
	virtual int Help( lua_State* L ) override;
	virtual wxString LuaFunctionName( void ) override { return "whoAmI"; }
};

// FtaWhoAmIFunction.h
