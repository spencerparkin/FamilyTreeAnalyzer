// FtaWhoAmIFunction.cpp

#pragma once

#include "FtaWhoAmIFunction.h"
#include "FtaClient.h"
#include "FtaApp.h"

wxIMPLEMENT_DYNAMIC_CLASS( FtaWhoAmIFunction, FtaLuaFunction );

FtaWhoAmIFunction::FtaWhoAmIFunction( void )
{
}

/*virtual*/ FtaWhoAmIFunction::~FtaWhoAmIFunction( void )
{
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
