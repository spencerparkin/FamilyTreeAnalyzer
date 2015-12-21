// FtaClearFunction.h

#pragma once

#include "FtaLuaFunction.h"

class FtaClearFunction : public FtaLuaFunction
{
public:

	wxDECLARE_DYNAMIC_CLASS( FtaClearFunction );

	FtaClearFunction( void );
	virtual ~FtaClearFunction( void );

	virtual int Call( lua_State* L ) override;
	virtual int Help( lua_State* L ) override;
	virtual wxString LuaFunctionName( void ) override { return "clear"; }
};

// FtaClearFunction.h