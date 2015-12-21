// FtaSetGraphFunction.h

#pragma once

#include "FtaLuaFunction.h"

class FtaSetGraphFunction : public FtaLuaFunction
{
public:

	wxDECLARE_DYNAMIC_CLASS( FtaSetGraphFunction );

	FtaSetGraphFunction( void );
	virtual ~FtaSetGraphFunction( void );

	virtual int Call( lua_State* L ) override;
	virtual int Help( lua_State* L ) override;
	virtual wxString LuaFunctionName( void ) override { return "setGraph"; }
};

// FtaSetGraphFunction.h