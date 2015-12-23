// FtaQueryUserForString.h

#pragma once

#include "FtaLuaFunction.h"

class FtaQueryUserForString : public FtaLuaFunction
{
public:

	wxDECLARE_DYNAMIC_CLASS( FtaQueryUserForString );

	FtaQueryUserForString( void );
	virtual ~FtaQueryUserForString( void );

	virtual int Call( lua_State* L ) override;
	virtual int Help( lua_State* L ) override;
	virtual wxString LuaFunctionName( void ) override { return "queryUserForString"; }
};

// FtaQueryUserForString.h
