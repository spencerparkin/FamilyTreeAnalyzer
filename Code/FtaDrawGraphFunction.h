// FtaDrawGraphFunction.h

#pragma once

#include "FtaLuaFunction.h"

class FtaDrawGraphFunction : public FtaLuaFunction
{
public:

	wxDECLARE_DYNAMIC_CLASS( FtaDrawGraphFunction );

	FtaDrawGraphFunction( void );
	virtual ~FtaDrawGraphFunction( void );

	virtual int Call( lua_State* L ) override;
	virtual int Help( lua_State* L ) override;
	virtual wxString LuaFunctionName( void ) override { return "drawGraph"; }
};

// FtaDrawGraphFunction.h