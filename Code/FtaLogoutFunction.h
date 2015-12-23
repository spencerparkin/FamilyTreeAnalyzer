// FtaLogoutFunction.h

#pragma once

#include "FtaLuaFunction.h"

class FtaLogoutFunction : public FtaLuaFunction
{
public:

	wxDECLARE_DYNAMIC_CLASS( FtaLogoutFunction );

	FtaLogoutFunction( void );
	virtual ~FtaLogoutFunction( void );

	virtual int Call( lua_State* L ) override;
	virtual int Help( lua_State* L ) override;
	virtual wxString LuaFunctionName( void ) override { return "logout"; }
};

// FtaLogoutFunction.h