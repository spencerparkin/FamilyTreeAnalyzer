// FtaLuaPanel.h

#pragma once

#include "FtaPanel.h"

class FtaLuaPanel : public FtaPanel
{
public:

	wxDECLARE_DYNAMIC_CLASS( FtaLuaPanel );

	FtaLuaPanel( void );
	virtual ~FtaLuaPanel( void );

	virtual bool GetPaneInfo( wxAuiPaneInfo& paneInfo );
	virtual bool MakeControls( void );
	virtual bool TimerUpdate( void );
};

// FtaLuaPanel.h
