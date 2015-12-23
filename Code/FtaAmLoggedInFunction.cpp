// FtaAmLoggedInFunction.cpp

#include "FtaAmLoggedInFunction.h"
#include "FtaClient.h"
#include "FtaFrame.h"
#include "FtaApp.h"

wxIMPLEMENT_DYNAMIC_CLASS( FtaAmLoggedInFunction, FtaLuaFunction );

FtaAmLoggedInFunction::FtaAmLoggedInFunction( void )
{
}

/*virtual*/ FtaAmLoggedInFunction::~FtaAmLoggedInFunction( void )
{
}

/*virtual*/ int FtaAmLoggedInFunction::Help( lua_State* L )
{
	wxGetApp().GetFrame()->AddLogMessage(
		"Use the amLoggedIn() function to determine whether or not you have an access token with FamilySearch.org.  "
		"If you do not have one, you need to login using the login() function." );
	return 0;
}

/*virtual*/ int FtaAmLoggedInFunction::Call( lua_State* L )
{
	if( wxGetApp().GetClient()->HasAccessToken() )
		lua_pushboolean( L, true );
	else
		lua_pushboolean( L, false );
	return 1;
}

// FtaAmLoggedInFunction.cpp