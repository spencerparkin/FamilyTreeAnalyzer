// FtaApp.cpp

#include "FtaApp.h"
#include "FtaClient.h"
#include "FtaFrame.h"
#include "FtaTreeCache.h"
#include "FtaLuaFunction.h"

wxIMPLEMENT_APP( FtaApp );

FtaApp::FtaApp( void )
{
	client = nullptr;
	frame = nullptr;
	treeCache = nullptr;
	L = nullptr;
}

/*virtual*/ FtaApp::~FtaApp( void )
{
	delete client;
	delete treeCache;

	lua_close(L);
}

/*virtual*/ bool FtaApp::OnInit( void )
{
	if( !wxApp::OnInit() )
		return false;

	//wxInitAllImageHandlers();

	wxImage::AddHandler( new wxJPEGHandler() );

	treeCache = new FtaTreeCache();

	L = luaL_newstate();
	if( !L )
		return false;

	luaL_openlibs(L);

	FtaLuaFunction::RegisterAllFunctions();

	client = new FtaClient();
	if( !client->Initialize() )
		return false;

	frame = new FtaFrame( nullptr, wxDefaultPosition, wxSize( 500, 500 ) );
	frame->Show();

	return true;
}

/*virtual*/ int FtaApp::OnExit( void )
{
	frame = nullptr;

	if( client )
		( void )client->Shutdown();

	return 0;
}

wxString FtaApp::GetResourceFolder( void )
{
	// TODO: Clearly we need a way of finding our resource folder; hard-code it for now.
	wxString resourceFolder = "C:\\FamilyTreeAnalyzer\\Resources\\";
	return resourceFolder;
}

// FtaApp.cpp