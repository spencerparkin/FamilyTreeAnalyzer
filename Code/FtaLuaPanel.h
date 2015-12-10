// FtaLuaPanel.h

#pragma once

#include "FtaPanel.h"

// TODO: Own a Lua state.  Write frame-work for registering Lua functions.
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
