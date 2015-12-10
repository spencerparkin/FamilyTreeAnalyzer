// FtaFrame.h

#pragma once

#include "FtaLogPanel.h"
#include <wx/frame.h>
#include <wx/aui/aui.h>
#include <wx/timer.h>

class FtaFrame : public wxFrame
{
public:

	FtaFrame( wxWindow* parent, const wxPoint& pos, const wxSize& size );
	virtual ~FtaFrame( void );

	void AddLogMessage( const wxString& message );
	void ClearLog( void );

	template< typename FtaPanelType >
	FtaPanelType* GetPanel( void )
	{
		return( ( FtaPanelType* )FindPanel( &FtaPanelType::ms_classInfo ) );
	}

private:

	enum
	{
		ID_AcquireAccessToken = wxID_HIGHEST,
		ID_DeleteAccessToken,
		ID_FillCache,
		ID_WipeCache,
		ID_DumpCache,
		ID_ClearLog,
		ID_Exit,
		ID_About,
		ID_Timer,
	};

	void OnAcquireAccessToken( wxCommandEvent& event );
	void OnDeleteAccessToken( wxCommandEvent& event );
	void OnFillCache( wxCommandEvent& event );
	void OnWipeCache( wxCommandEvent& event );
	void OnDumpCache( wxCommandEvent& event );
	void OnClearLog( wxCommandEvent& event );
	void OnExit( wxCommandEvent& event );
	void OnAbout( wxCommandEvent& event );
	void OnUpdateMenuItemUI( wxUpdateUIEvent& event );
	void OnTimer( wxTimerEvent& event );

	bool MakePanels( void );

	wxWindow* FindPanel( wxClassInfo* classInfo );

	wxAuiManager* auiManager;

	wxTimer timer;
};

// FtaFrame.h
