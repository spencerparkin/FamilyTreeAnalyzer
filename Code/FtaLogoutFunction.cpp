// FtaLogoutFunction.cpp

#include "FtaLogoutFunction.h"
#include "FtaFrame.h"
#include "FtaClient.h"
#include "FtaApp.h"

wxIMPLEMENT_DYNAMIC_CLASS( FtaLogoutFunction, FtaLuaFunction );

FtaLogoutFunction::FtaLogoutFunction( void )
{
}

/*virtual*/ FtaLogoutFunction::~FtaLogoutFunction( void )
{
}

/*virtual*/ int FtaLogoutFunction::Help( lua_State* L )
{
	wxGetApp().GetFrame()->AddLogMessage(
		"Use the logout() function to delete your access token.  This ends your session "
		"with FamilySearch.org, and all cache is purged.  If you are not already logged in, "
		"this function will fail." );
	return 0;
}

/*virtual*/ int FtaLogoutFunction::Call( lua_State* L )
{
	bool success = false;

	do
	{
		FtaClient* client = wxGetApp().GetClient();
		if( !client->HasAccessToken() )
			break;

		if( !client->DeleteAccessToken() )
			break;

		success = true;
	}
	while( false );

	lua_pushboolean( L, success );
	return 1;
}

// FtaLogoutFunction.cpp