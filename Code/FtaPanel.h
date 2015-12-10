// FtaPanel.h

#pragma once

#include <wx/panel.h>
#include <wx/aui/aui.h>

class FtaPanel : public wxPanel
{
public:

	wxDECLARE_DYNAMIC_CLASS( FtaPanel );

	FtaPanel( void );
	virtual ~FtaPanel( void );

	virtual bool GetPaneInfo( wxAuiPaneInfo& paneInfo );
	virtual bool MakeControls( void );
	virtual bool TimerUpdate( void );
};

// FtaPanel.h
