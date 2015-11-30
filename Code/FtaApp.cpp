// FtaApp.cpp

#include "FtaApp.h"
#include "FtaClient.h"
#include "FtaFrame.h"
#include "FtaTreeCache.h"
#include "FtaMiscCache.h"

wxIMPLEMENT_APP( FtaApp );

FtaApp::FtaApp( void )
{
	client = nullptr;
	frame = nullptr;
	treeCache = nullptr;
	miscCache = nullptr;
}

/*virtual*/ FtaApp::~FtaApp( void )
{
	delete client;
	delete treeCache;
	delete miscCache;
}

/*virtual*/ bool FtaApp::OnInit( void )
{
	if( !wxApp::OnInit() )
		return false;

	treeCache = new FtaTreeCache();
	miscCache = new FtaMiscCache();

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