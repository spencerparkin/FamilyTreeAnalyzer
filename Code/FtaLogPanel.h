// FtaLogPanel.h

#pragma once

#include "FtaPanel.h"
#include <wx/stc/stc.h>

class FtaLogPanel : public FtaPanel
{
public:

	wxDECLARE_DYNAMIC_CLASS( FtaLogPanel );

	FtaLogPanel( void );
	virtual ~FtaLogPanel( void );
	
	virtual bool GetPaneInfo( wxAuiPaneInfo& paneInfo ) override;
	virtual bool MakeControls( void ) override;

	void AddLogMessage( const wxString& message );
	void ClearLog( void );
	bool IsEmpty( void );

private:

	wxStyledTextCtrl* textCtrl;
};

// FtaLogPanel.h