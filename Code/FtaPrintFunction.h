// FtaPrintFunction.h

#pragma once

#include "FtaLuaFunction.h"

class FtaPrintFunction : public FtaLuaFunction
{
public:

	wxDECLARE_DYNAMIC_CLASS( FtaPrintFunction );

	FtaPrintFunction( void );
	virtual ~FtaPrintFunction( void );

	virtual int Call( lua_State* L ) override;
	virtual wxString LuaFunctionName( void ) override { return "Print"; }
};

// FtaPrintFunction.h
