// FtaQueryUserForInteger.h

#pragma once

#include "FtaLuaFunction.h"

class FtaQueryUserForInteger : public FtaLuaFunction
{
public:

	wxDECLARE_DYNAMIC_CLASS( FtaQueryUserForInteger );

	FtaQueryUserForInteger( void );
	virtual ~FtaQueryUserForInteger( void );

	virtual int Call( lua_State* L ) override;
	virtual int Help( lua_State* L ) override;
	virtual wxString LuaFunctionName( void ) override { return "queryUserForInteger"; }
};

// FtaQueryUserForInteger.h
