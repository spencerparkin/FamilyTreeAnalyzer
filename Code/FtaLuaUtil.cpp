// FtaLuaUtil.cpp

#include "FtaLuaUtil.h"
#include "FtaApp.h"
#include <wx/string.h>
#include <wx/msgdlg.h>

FtaLuaStackPopper::FtaLuaStackPopper( lua_State* L )
{
	this->L = L;
	stackTop = lua_gettop(L);
}

FtaLuaStackPopper::~FtaLuaStackPopper( void )
{
	while( stackTop < lua_gettop(L) )
		lua_pop( L, 1 );
}

bool FtaLuaReportError( lua_State* L, int result )
{
	if( result == 0 )
		return false;

	wxString errorType;
	switch( result )
	{
		case LUA_ERRSYNTAX:
		{
			errorType = "Lua Syntax Error";
			break;
		}
		case LUA_ERRRUN:
		{
			errorType = "Lua Run-Time Error";
			break;
		}
		default:
		{
			errorType = "Lua Error";
			break;
		}
	}

	wxString errorDesc = lua_tostring( L, -1 );
	if( errorDesc.IsEmpty() )
		errorDesc = "Unknown Lua error.";

	wxMessageDialog messageDialog( nullptr, errorDesc, errorType, wxOK | wxCENTRE | wxICON_ERROR );
	messageDialog.ShowModal();

	return true;
}

// FtaLuaUtil.cpp