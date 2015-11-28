// FtaApp.cpp

#include "FtaApp.h"
#include "FtaClient.h"

wxIMPLEMENT_APP( FtaApp );

FtaApp::FtaApp( void )
{
	client = nullptr;
}

/*virtual*/ FtaApp::~FtaApp( void )
{
	delete client;
}

/*virtual*/ bool FtaApp::OnInit( void )
{
	if( !wxApp::OnInit() )
		return false;

	client = new FtaClient();
	if( !client->Initialize() )
		return false;

	client->Authenticate();

	return true;
}

/*virtual*/ int FtaApp::OnExit( void )
{
	if( client )
		( void )client->Shutdown();

	return 0;
}

// FtaApp.cpp