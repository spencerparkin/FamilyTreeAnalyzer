// FtaLuaFunction.cpp

#include "FtaLuaFunction.h"
#include "FtaFrame.h"
#include "FtaApp.h"

wxIMPLEMENT_DYNAMIC_CLASS( FtaLuaFunction, wxObject );

#define FTA_LUA_FUNCTION		"FtaLuaFunction"

bool FtaLuaFunction::functionHelp = false;

/*static*/ void FtaLuaFunction::RegisterAllFunctions( void )
{
	wxClassInfo* functionClassInfo = wxClassInfo::FindClass( "FtaLuaFunction" );
	if( !functionClassInfo )
		return;

	const wxClassInfo* classInfo = wxClassInfo::GetFirst();
	while( classInfo )
	{
		if( classInfo != functionClassInfo && classInfo->IsKindOf( functionClassInfo ) )
			Register( classInfo );

		classInfo = classInfo->GetNext();
	}
}

/*static*/ bool FtaLuaFunction::Register( const wxClassInfo* classInfo )
{
	wxObjectConstructorFn constructorFunc = classInfo->GetConstructor();
	wxObject* object = constructorFunc();
	FtaLuaFunction* function = wxDynamicCast( object, FtaLuaFunction );
	if( !function )
	{
		delete object;
		return false;
	}

	return Register( function );
}

/*static*/ bool FtaLuaFunction::Register( FtaLuaFunction* function )
{
	lua_State* L = wxGetApp().GetLuaState();

	while( true )
	{
		lua_getglobal( L, "fta" );
		if( !lua_isnil( L, -1 ) )
			break;
	
		lua_pop( L, 1 );
		lua_newtable( L );
		lua_setglobal( L, "fta" );
	}

	FtaLuaFunction** userData = ( FtaLuaFunction** )lua_newuserdata( L, sizeof( FtaLuaFunction* ) );
	*userData = function;

	if( 1 == luaL_newmetatable( L, FTA_LUA_FUNCTION ) )
	{
		lua_pushstring( L, "__gc" );
		lua_pushcfunction( L, &FtaLuaFunction::GarbageCollect );
		lua_settable( L, -3 );
	}

	lua_setmetatable( L, -2 );
	lua_pushcclosure( L, &FtaLuaFunction::EntryPoint, 1 );

	wxString functionName = function->LuaFunctionName();
	lua_setfield( L, -2, functionName );
	
	lua_pop( L, 1 );

	return true;
}

/*static*/ bool FtaLuaFunction::Unregister( const wxString& functionName )
{
	lua_State* L =  wxGetApp().GetLuaState();

	lua_getglobal( L, functionName );
	FtaLuaFunction** userData = ( FtaLuaFunction** )luaL_testudata( L, -1, FTA_LUA_FUNCTION );
	if( !userData )
	{
		lua_pop( L, 1 );
		return false;
	}

	lua_pushnil( L );
	lua_setglobal( L, functionName );
	return true;
}

FtaLuaFunction::FtaLuaFunction( void )
{
}

/*virtual*/ FtaLuaFunction::~FtaLuaFunction( void )
{
}

/*virtual*/ int FtaLuaFunction::Help( lua_State* L )
{
	FtaFrame* frame = wxGetApp().GetFrame();
	frame->AddLogMessage( "No help provided for this function." );
	return 0;
}

/*static*/ bool FtaLuaFunction::IsEntryPoint( lua_CFunction function )
{
	if( function == &FtaLuaFunction::EntryPoint )
		return true;

	return false;
}

/*static*/ int FtaLuaFunction::EntryPoint( lua_State* L )
{
	FtaLuaFunction** userData = ( FtaLuaFunction** )luaL_testudata( L, lua_upvalueindex(1), FTA_LUA_FUNCTION );
	if( !userData )
		return 0;

	FtaLuaFunction* function = *userData;
	if( functionHelp )
		return function->Help( L );

	return function->Call( L );
}

/*static*/ int FtaLuaFunction::GarbageCollect( lua_State* L )
{
	FtaLuaFunction** userData = ( FtaLuaFunction** )luaL_checkudata( L, 1, FTA_LUA_FUNCTION );
	if( !userData )
		return 0;

	FtaLuaFunction* function = *userData;
	delete function;
	return 0;
}

// FtaLuaFunction.cpp