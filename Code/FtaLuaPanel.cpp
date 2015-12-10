// FtaLuaPanel.cpp

#include "FtaLuaPanel.h"

wxIMPLEMENT_DYNAMIC_CLASS( FtaLuaPanel, FtaPanel );

FtaLuaPanel::FtaLuaPanel( void )
{
}

/*virtual*/ FtaLuaPanel::~FtaLuaPanel( void )
{
}

/*virtual*/ bool FtaLuaPanel::GetPaneInfo( wxAuiPaneInfo& paneInfo )
{
	paneInfo.Caption( "Lua" );
	paneInfo.CenterPane();
	return true;
}

/*virtual*/ bool FtaLuaPanel::MakeControls( void )
{
	return true;
}

/*virtual*/ bool FtaLuaPanel::TimerUpdate( void )
{
	return true;
}

// FtaLuaPanel.cpp