// FtaGetPersonFunction.h

#pragma once

#include "FtaLuaFunction.h"

class FtaGetPersonFunction : public FtaLuaFunction
{
public:

	wxDECLARE_DYNAMIC_CLASS( FtaGetPersonFunction );

	FtaGetPersonFunction( void );
	virtual ~FtaGetPersonFunction( void );

	virtual int Call( lua_State* L ) override;
	virtual int Help( lua_State* L ) override;
	virtual wxString LuaFunctionName( void ) override { return "getPerson"; }
};

// FtaGetPersonFunction.h