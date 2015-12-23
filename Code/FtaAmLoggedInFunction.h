// FtaAmLoggedInFunction.h

#pragma once

#include "FtaLuaFunction.h"

class FtaAmLoggedInFunction : public FtaLuaFunction
{
public:

	wxDECLARE_DYNAMIC_CLASS( FtaAmLoggedInFunction );

	FtaAmLoggedInFunction( void );
	virtual ~FtaAmLoggedInFunction( void );

	virtual int Call( lua_State* L ) override;
	virtual int Help( lua_State* L ) override;
	virtual wxString LuaFunctionName( void ) override { return "amLoggedIn"; }
};

// FtaAmLoggedInFunction.h
