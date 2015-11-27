// FtaApp.cpp

#include "FtaApp.h"

wxIMPLEMENT_APP( FtaApp );

FtaApp::FtaApp( void )
{
}

/*virtual*/ FtaApp::~FtaApp( void )
{
}

/*virtual*/ bool FtaApp::OnInit( void )
{
	if( !wxApp::OnInit() )
		return false;

	return true;
}

/*virtual*/ int FtaApp::OnExit( void )
{
	return 0;
}

// FtaApp.cpp