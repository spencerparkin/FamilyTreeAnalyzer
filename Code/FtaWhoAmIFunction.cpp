// FtaWhoAmIFunction.cpp

#pragma once

#include "FtaWhoAmIFunction.h"
#include "FtaClient.h"
#include "FtaFrame.h"
#include "FtaApp.h"

wxIMPLEMENT_DYNAMIC_CLASS( FtaWhoAmIFunction, FtaLuaFunction );

FtaWhoAmIFunction::FtaWhoAmIFunction( void )
{
}

/*virtual*/ FtaWhoAmIFunction::~FtaWhoAmIFunction( void )
{
}

/*virtual*/ int FtaWhoAmIFunction::Help( lua_State* L )
{
	wxGetApp().GetFrame()->AddLogMessage(
		"Use the whoAmI() function to get back a string that is your person-ID.  "
		"That is, the person-ID of the person who is currently logged into FamilySearch.org.  "
		"If no one is logged in, an empty string is returned."  );
	return 0;
}

/*virtual*/ int FtaWhoAmIFunction::Call( lua_State* L )
{
	FtaClient* client = wxGetApp().GetClient();
	if( !client->HasAccessToken() )
		return 0;

	wxString personId = wxGetApp().GetClient()->GetCurrentUserPersonId();
	lua_pushstring( L, personId );
	return 1;
}

// FtaWhoAmIFunction.cpp
