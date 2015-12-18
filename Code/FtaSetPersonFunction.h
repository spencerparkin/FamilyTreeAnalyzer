// FtaSetPersonFunction.h

#pragma once

#include "FtaLuaFunction.h"

class FtaSetPersonFunction : public FtaLuaFunction
{
public:

	wxDECLARE_DYNAMIC_CLASS( FtaSetPersonFunction );

	FtaSetPersonFunction( void );
	virtual ~FtaSetPersonFunction( void );

	virtual int Call( lua_State* L ) override;
	virtual wxString LuaFunctionName( void ) override { return "setPerson"; }
};

// FtaSetPersonFunction.h
