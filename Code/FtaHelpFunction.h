// FtaHelpFunction.h

#pragma once

#include "FtaLuaFunction.h"

class FtaHelpFunction : public FtaLuaFunction
{
public:

	wxDECLARE_DYNAMIC_CLASS( FtaHelpFunction );

	FtaHelpFunction( void );
	virtual ~FtaHelpFunction( void );

	virtual int Call( lua_State* L ) override;
	virtual wxString LuaFunctionName( void ) override { return "help"; }
};

// FtaHelpFunction.h