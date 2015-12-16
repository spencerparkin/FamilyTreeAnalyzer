// FtaLuaPanel.h

#pragma once

#include "FtaPanel.h"
#include <wx/stc/stc.h>
#include <wx/button.h>
#include <wx/arrstr.h>

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

	wxStyledTextCtrl* textCtrl;

	wxArrayString codeHistory;
	int historyLocation;
};

// FtaLuaPanel.h
