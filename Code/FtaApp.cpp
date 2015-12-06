// FtaApp.cpp

#include "FtaApp.h"
#include "FtaClient.h"
#include "FtaFrame.h"
#include "FtaTreeCache.h"

// TODO: What is throttling, and how might it affect the data we're getting back from familysearch.org?

wxIMPLEMENT_APP( FtaApp );

FtaApp::FtaApp( void )
{
	client = nullptr;
	frame = nullptr;
	treeCache = nullptr;
}

/*virtual*/ FtaApp::~FtaApp( void )
{
	delete client;
	delete treeCache;
}

/*virtual*/ bool FtaApp::OnInit( void )
{
	if( !wxApp::OnInit() )
		return false;

	treeCache = new FtaTreeCache();

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