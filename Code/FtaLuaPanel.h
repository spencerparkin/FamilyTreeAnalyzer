// FtaLuaPanel.h

#pragma once

#include "FtaPanel.h"
#include <wx/stc/stc.h>
#include <wx/button.h>
#include <wx/arrstr.h>
#include <wx/dnd.h>

class FtaLuaPanel : public FtaPanel
{
public:

	wxDECLARE_DYNAMIC_CLASS( FtaLuaPanel );

	FtaLuaPanel( void );
	virtual ~FtaLuaPanel( void );

	virtual bool GetPaneInfo( wxAuiPaneInfo& paneInfo );
	virtual bool MakeControls( void );
	virtual bool TimerUpdate( void );

	void OnExecuteButtonPressed( wxCommandEvent& event );
	void OnCharHook( wxKeyEvent& event );

private:

	bool Execute( void );

	class FileDropTarget : public wxFileDropTarget
	{
	public:

		FileDropTarget( void );
		virtual ~FileDropTarget( void );

		virtual bool OnDropFiles( wxCoord x, wxCoord y, const wxArrayString& fileNames ) override;

		wxStyledTextCtrl* textCtrl;
	};

	wxStyledTextCtrl* textCtrl;

	wxArrayString codeHistory;
	int historyLocation;
};

// FtaLuaPanel.h
