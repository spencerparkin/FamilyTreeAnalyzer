// FtaFrame.cpp

#include "FtaFrame.h"
#include "FtaApp.h"
#include "FtaClient.h"
#include "FtaTreeCache.h"
#include "FtaMiscCache.h"
#include <wx/aboutdlg.h>
#include <wx/menu.h>
#include <wx/statusbr.h>
#include <wx/msgdlg.h>
#include <wx/textdlg.h>

FtaFrame::FtaFrame( wxWindow* parent, const wxPoint& pos, const wxSize& size ) : wxFrame( parent, wxID_ANY, "Family Tree Analyzer", pos, size )
{
	wxMenu* programMenu = new wxMenu();
	wxMenuItem* acquireAccessTokenMenuItem = new wxMenuItem( programMenu, ID_AcquireAccessToken, "Acquire Access Token", "Authenticate with FamilySearch.org." );
	wxMenuItem* exitMenuItem = new wxMenuItem( programMenu, ID_Exit, "Exit", "Exit this program." );
	programMenu->Append( acquireAccessTokenMenuItem );
	programMenu->AppendSeparator();
	programMenu->Append( exitMenuItem );

	wxMenu* cacheMenu = new wxMenu();
	wxMenuItem* populateTreeCacheAtPersonMenuItem = new wxMenuItem( programMenu, ID_PopulateTreeCacheAtPerson, "Populate Tree Cache At Person", "Update the tree cache at the given person." );
	wxMenuItem* wipeAllCacheMenuItem = new wxMenuItem( programMenu, ID_WipeAllCache, "Wipe All Cache", "Effectively invalidate all information obtained from FamilySearch.org." );
	cacheMenu->Append( populateTreeCacheAtPersonMenuItem );
	cacheMenu->AppendSeparator();
	cacheMenu->Append( wipeAllCacheMenuItem );

	wxMenu* analysisMenu = new wxMenu();
	//...

	wxMenu* helpMenu = new wxMenu();
	wxMenuItem* aboutMenuItem = new wxMenuItem( helpMenu, ID_About, "About", "Show the about-box." );
	helpMenu->Append( aboutMenuItem );

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append( programMenu, "Program" );
	menuBar->Append( cacheMenu, "Cache" );
	menuBar->Append( analysisMenu, "Analysis" );
	menuBar->Append( helpMenu, "Help" );
	SetMenuBar( menuBar );

	wxStatusBar* statusBar = new wxStatusBar( this );
	SetStatusBar( statusBar );

	Bind( wxEVT_MENU, &FtaFrame::OnAcquireAccessToken, this, ID_AcquireAccessToken );
	Bind( wxEVT_MENU, &FtaFrame::OnPopulateTreeCacheAtPerson, this, ID_PopulateTreeCacheAtPerson );
	Bind( wxEVT_MENU, &FtaFrame::OnWipeAllCache, this, ID_WipeAllCache );
	Bind( wxEVT_MENU, &FtaFrame::OnExit, this, ID_Exit );
	Bind( wxEVT_MENU, &FtaFrame::OnAbout, this, ID_About );
	Bind( wxEVT_UPDATE_UI, &FtaFrame::OnUpdateMenuItemUI, this, ID_AcquireAccessToken );
	Bind( wxEVT_UPDATE_UI, &FtaFrame::OnUpdateMenuItemUI, this, ID_PopulateTreeCacheAtPerson );
	Bind( wxEVT_UPDATE_UI, &FtaFrame::OnUpdateMenuItemUI, this, ID_WipeAllCache );
}

/*virtual*/ FtaFrame::~FtaFrame( void )
{
}

void FtaFrame::OnAcquireAccessToken( wxCommandEvent& event )
{
	if( wxGetApp().GetClient()->Authenticate() )
		wxMessageBox( "Authentication succeeded!" );
}

void FtaFrame::OnWipeAllCache( wxCommandEvent& event )
{
	wxGetApp().GetTreeCache()->Wipe();
	wxGetApp().GetMiscCache()->Wipe();
}

void FtaFrame::OnPopulateTreeCacheAtPerson( wxCommandEvent& event )
{
	wxString personId = wxGetTextFromUser( "Enter person-ID.", "Person-ID", wxEmptyString, nullptr );
	if( personId.IsEmpty() )
		return;

	wxGetApp().GetClient()->PopulateTreeCacheAt( personId );
}

void FtaFrame::OnExit( wxCommandEvent& event )
{
	Close( true );
}

void FtaFrame::OnAbout( wxCommandEvent& event )
{
	wxAboutDialogInfo aboutDialogInfo;

	aboutDialogInfo.SetName( "Family Tree Analyzer" );
	aboutDialogInfo.SetVersion( "0.1" );
	aboutDialogInfo.SetDescription( "This program is free software and distributed under the MIT license." );
	aboutDialogInfo.SetCopyright( "Copyright (C) 2015 -- Spencer T. Parkin <SpencerTParkin@gmail.com>" );

	wxAboutBox( aboutDialogInfo );
}

void FtaFrame::OnUpdateMenuItemUI( wxUpdateUIEvent& event )
{
	switch( event.GetId() )
	{
		case ID_AcquireAccessToken:
		{
			event.Enable( !wxGetApp().GetClient()->HasAccessToken() );
			break;
		}
		case ID_PopulateTreeCacheAtPerson:
		{
			event.Enable( wxGetApp().GetClient()->HasAccessToken() );
			break;
		}
		case ID_WipeAllCache:
		{
			event.Enable( !wxGetApp().GetTreeCache()->IsEmpty() || !wxGetApp().GetMiscCache()->IsEmpty() );
			break;
		}
	}
}

// FtaFrame.cpp