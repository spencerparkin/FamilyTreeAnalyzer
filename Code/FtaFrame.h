// FtaFrame.h

#pragma once

#include <wx/frame.h>
#include <wx/stc/stc.h>

class FtaFrame : public wxFrame
{
public:

	FtaFrame( wxWindow* parent, const wxPoint& pos, const wxSize& size );
	virtual ~FtaFrame( void );

	void AddLogMessage( const wxString& message );
	void ClearLog( void );

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

	wxStyledTextCtrl* textCtrl;
};

// FtaFrame.h
