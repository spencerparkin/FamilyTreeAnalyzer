// FtaLoginFunction.h

#pragma once

#include "FtaLuaFunction.h"

class FtaLoginFunction : public FtaLuaFunction
{
public:

	wxDECLARE_DYNAMIC_CLASS( FtaLoginFunction );

	FtaLoginFunction( void );
	virtual ~FtaLoginFunction( void );

	virtual int Call( lua_State* L ) override;
	virtual int Help( lua_State* L ) override;
	virtual wxString LuaFunctionName( void ) override { return "login"; }
};

// FtaLoginFunction.h