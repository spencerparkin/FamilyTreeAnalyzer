// FtaFrame.cpp

#include "FtaFrame.h"
#include "FtaApp.h"
#include "FtaClient.h"
#include "FtaTreeCache.h"
#include <wx/aboutdlg.h>
#include <wx/menu.h>
#include <wx/statusbr.h>
#include <wx/msgdlg.h>
#include <wx/textdlg.h>
#include <wx/numdlg.h>
#include <wx/textdlg.h>

FtaFrame::FtaFrame( wxWindow* parent, const wxPoint& pos, const wxSize& size ) : wxFrame( parent, wxID_ANY, "Family Tree Analyzer", pos, size ), timer( this, ID_Timer )
{
	auiManager = new wxAuiManager( this, wxAUI_MGR_LIVE_RESIZE | wxAUI_MGR_DEFAULT );

	wxMenu* programMenu = new wxMenu();
	wxMenuItem* acquireAccessTokenMenuItem = new wxMenuItem( programMenu, ID_AcquireAccessToken, "Acquire Access Token", "Authenticate with FamilySearch.org." );
	wxMenuItem* deleteAccessTokenMenuItem = new wxMenuItem( programMenu, ID_DeleteAccessToken, "Delete Access Token", "Logout of FamilySearch.org." );
	wxMenuItem* clearLogMenuItem = new wxMenuItem( programMenu, ID_ClearLog, "Clear Log", "Clear the log." );
	wxMenuItem* exitMenuItem = new wxMenuItem( programMenu, ID_Exit, "Exit", "Exit this program." );
	programMenu->Append( acquireAccessTokenMenuItem );
	programMenu->Append( deleteAccessTokenMenuItem );
	programMenu->AppendSeparator();
	programMenu->Append( clearLogMenuItem );
	programMenu->AppendSeparator();
	programMenu->Append( exitMenuItem );

	wxMenu* cacheMenu = new wxMenu();
	wxMenuItem* fillCacheMenuItem = new wxMenuItem( programMenu, ID_FillCache, "Fill Cache", "Fill the cache with information obtained from www.familysearch.org." );
	wxMenuItem* dumpCacheMenuItem = new wxMenuItem( programMenu, ID_DumpCache, "Dump Cache", "Print to the log the current contents of the cache." );
	wxMenuItem* wipeAllCacheMenuItem = new wxMenuItem( programMenu, ID_WipeCache, "Wipe Cache", "Wipe the cache of all information obtained from www.familysearch.org." );
	cacheMenu->Append( fillCacheMenuItem );
	cacheMenu->Append( dumpCacheMenuItem );
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
	Bind( wxEVT_MENU, &FtaFrame::OnDeleteAccessToken, this, ID_DeleteAccessToken );
	Bind( wxEVT_MENU, &FtaFrame::OnClearLog, this, ID_ClearLog );
	Bind( wxEVT_MENU, &FtaFrame::OnFillCache, this, ID_FillCache );
	Bind( wxEVT_MENU, &FtaFrame::OnWipeCache, this, ID_WipeCache );
	Bind( wxEVT_MENU, &FtaFrame::OnDumpCache, this, ID_DumpCache );
	Bind( wxEVT_MENU, &FtaFrame::OnExit, this, ID_Exit );
	Bind( wxEVT_MENU, &FtaFrame::OnAbout, this, ID_About );
	Bind( wxEVT_UPDATE_UI, &FtaFrame::OnUpdateMenuItemUI, this, ID_AcquireAccessToken );
	Bind( wxEVT_UPDATE_UI, &FtaFrame::OnUpdateMenuItemUI, this, ID_DeleteAccessToken );
	Bind( wxEVT_UPDATE_UI, &FtaFrame::OnUpdateMenuItemUI, this, ID_ClearLog );
	Bind( wxEVT_UPDATE_UI, &FtaFrame::OnUpdateMenuItemUI, this, ID_FillCache );
	Bind( wxEVT_UPDATE_UI, &FtaFrame::OnUpdateMenuItemUI, this, ID_WipeCache );
	Bind( wxEVT_UPDATE_UI, &FtaFrame::OnUpdateMenuItemUI, this, ID_DumpCache );

	MakePanels();

	auiManager->Update();

	timer.Start( 1 );
}

/*virtual*/ FtaFrame::~FtaFrame( void )
{
	auiManager->UnInit();
	delete auiManager;
}

bool FtaFrame::MakePanels( void )
{
	wxClassInfo* basePanelClassInfo = wxClassInfo::FindClass( "FtaPanel" );
	if( !basePanelClassInfo )
		return false;

	const wxClassInfo* classInfo = wxClassInfo::GetFirst();
	while( classInfo )
	{
		if( classInfo != basePanelClassInfo && classInfo->IsKindOf( basePanelClassInfo ) )
		{
			FtaPanel* panel = ( FtaPanel* )classInfo->CreateObject();
			panel->Create( this );
			if( !panel->MakeControls() )
				delete panel;
			else
			{
				wxAuiPaneInfo paneInfo;
				panel->GetPaneInfo( paneInfo );
				paneInfo.CloseButton( false );
				auiManager->AddPane( panel, paneInfo );
			}
		}

		classInfo = classInfo->GetNext();
	}

	return true;
}

void FtaFrame::OnClearLog( wxCommandEvent& event )
{
	ClearLog();
}

void FtaFrame::AddLogMessage( const wxString& message )
{
	GetPanel< FtaLogPanel >()->AddLogMessage( message );
}

void FtaFrame::ClearLog( void )
{
	GetPanel< FtaLogPanel >()->ClearLog();
}

void FtaFrame::OnAcquireAccessToken( wxCommandEvent& event )
{
	if( wxGetApp().GetClient()->Authenticate() )
		wxMessageBox( "Authentication succeeded!" );
}

void FtaFrame::OnDeleteAccessToken( wxCommandEvent& event )
{
	if( wxGetApp().GetClient()->DeleteAccessToken() )
		wxMessageBox( "Logged out!" );
}

void FtaFrame::OnWipeCache( wxCommandEvent& event )
{
	( void )wxGetApp().GetTreeCache()->Wipe();
}

void FtaFrame::OnDumpCache( wxCommandEvent& event )
{
	wxGetApp().GetTreeCache()->Dump();
}

void FtaFrame::OnFillCache( wxCommandEvent& event )
{
	int personCount = wxGetApp().GetTreeCache()->GetPersonCount();

	int personCountThreshold = ( int )wxGetNumberFromUser( "", "Try to fill cache with as many as how many persons?", "Person Count Threshold", personCount );
	if( personCountThreshold == -1 )
		return;

	wxString rootPersonId = wxGetTextFromUser( "Enter root person-ID", "Root Person-ID", wxEmptyString, nullptr );
	if( rootPersonId.IsEmpty() )
		return;

	( void )wxGetApp().GetTreeCache()->Fill( rootPersonId, personCountThreshold );
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

void FtaFrame::OnTimer( wxTimerEvent& event )
{
	static bool inHere = false;

	if( inHere )
		return;

	inHere = true;

	wxAuiPaneInfoArray& paneInfoArray = auiManager->GetAllPanes();
	for( int i = 0; i < ( signed )paneInfoArray.GetCount(); i++ )
	{
		FtaPanel* panel = wxDynamicCast( paneInfoArray[i].window, FtaPanel );
		if( panel )
			panel->TimerUpdate();
	}

	inHere = false;
}

wxWindow* FtaFrame::FindPanel( wxClassInfo* classInfo )
{
	wxAuiPaneInfoArray& paneInfoArray = auiManager->GetAllPanes();
	for( int i = 0; i < ( signed )paneInfoArray.GetCount(); i++ )
	{
		wxWindow* window = paneInfoArray[i].window;
		if( window->IsKindOf( classInfo ) )
			return window;
	}

	return nullptr;
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
		case ID_DeleteAccessToken:
		case ID_FillCache:
		{
			event.Enable( wxGetApp().GetClient()->HasAccessToken() );
			break;
		}
		case ID_DumpCache:
		case ID_WipeCache:
		{
			event.Enable( !wxGetApp().GetTreeCache()->IsEmpty() );
			break;
		}
		case ID_ClearLog:
		{
			event.Enable( !GetPanel< FtaLogPanel >()->IsEmpty() );
			break;
		}
	}
}

// FtaFrame.cpp