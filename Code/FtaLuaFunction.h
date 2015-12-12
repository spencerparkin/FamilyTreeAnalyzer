// FtaLuaFunction.h

#pragma once

#include <lua.hpp>
#include <lauxlib.h>
#include <lualib.h>
#include <wx/object.h>
#include <wx/rtti.h>

class FtaLuaFunction : public wxObject
{
public:

	wxDECLARE_DYNAMIC_CLASS( FtaLuaFunction );

	static void RegisterAllFunctions( void );

	static bool Register( const wxClassInfo* classInfo );
	static bool Register( FtaLuaFunction* function );
	static bool Unregister( const wxString& functionName );

	FtaLuaFunction( void );
	virtual ~FtaLuaFunction( void );

	virtual int Call( lua_State* L ) { return 0; }
	virtual wxString LuaFunctionName( void ) { return "?"; }

private:

	static int EntryPoint( lua_State* L );
	static int GarbageCollect( lua_State* L );
};

// FtaLuaFunction.h
