// FtaLoginFunction.cpp

#include "FtaLoginFunction.h"
#include "FtaFrame.h"
#include "FtaClient.h"
#include "FtaApp.h"

wxIMPLEMENT_DYNAMIC_CLASS( FtaLoginFunction, FtaLuaFunction );

FtaLoginFunction::FtaLoginFunction( void )
{
}

/*virtual*/ FtaLoginFunction::~FtaLoginFunction( void )
{
}

/*virtual*/ int FtaLoginFunction::Help( lua_State* L )
{
	wxGetApp().GetFrame()->AddLogMessage(
				"Use the login() function to acquire an access token with FamilySearch.org.  "
				"No information can be obtained from FamilySearch.org until you log in.  "
				"This function can take a username, but never requires a password.  You will always be "
				"prompted to enter a password.  If someone is already logged in, this function "
				"will fail.  Use the logout() function in that case." );
	return 0;
}

/*virtual*/ int FtaLoginFunction::Call( lua_State* L )
{
	bool success = false;

	do
	{
		FtaClient* client = wxGetApp().GetClient();
		if( client->HasAccessToken() )
			break;
		
		wxString userName;
		if( lua_gettop(L) > 0 && lua_isstring( L, -1 ) )
			userName = lua_tostring( L, -1 );

		if( !client->Authenticate( userName ) )
			break;

		success = true;
	}
	while( false );

	lua_pushboolean( L, success );
	return 1;
}

// FtaLoginFunction.cpp