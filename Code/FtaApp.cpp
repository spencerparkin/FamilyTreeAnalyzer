// FtaApp.cpp

#include "FtaApp.h"
#include "FtaClient.h"
#include "FtaFrame.h"
#include "FtaTreeCache.h"

wxIMPLEMENT_APP( FtaApp );

FtaApp::FtaApp( void )
{
	client = nullptr;
	frame = nullptr;
	cache = nullptr;
}

/*virtual*/ FtaApp::~FtaApp( void )
{
	delete client;
	delete cache;
}

/*virtual*/ bool FtaApp::OnInit( void )
{
	cache = new FtaTreeCache();

	if( !wxApp::OnInit() )
		return false;

	client = new FtaClient();
	if( !client->Initialize() )
		return false;

	frame = new FtaFrame( nullptr, wxDefaultPosition, wxSize( 500, 500 ) );
	frame->Show();

	return true;
}

/*virtual*/ int FtaApp::OnExit( void )
{
	if( client )
		( void )client->Shutdown();

	return 0;
}

// FtaApp.cpp