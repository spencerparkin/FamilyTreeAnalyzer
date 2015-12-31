// FtaFrameVizFunction.cpp

#include "FtaFrameVizFunction.h"
#include "FtaFrame.h"
#include "FtaVizPanel.h"
#include "FtaCanvas.h"
#include "FtaApp.h"

wxIMPLEMENT_DYNAMIC_CLASS( FtaFrameVizFunction, FtaLuaFunction );

FtaFrameVizFunction::FtaFrameVizFunction( void )
{
}

/*virtual*/ FtaFrameVizFunction::~FtaFrameVizFunction( void )
{
}

/*virtual*/ int FtaFrameVizFunction::Help( lua_State* L )
{
	wxGetApp().GetFrame()->AddLogMessage( "Use the frameViz() function to position the camera in such as a way as to make the entire visualization visible." );
	return 0;
}

/*virtual*/ int FtaFrameVizFunction::Call( lua_State* L )
{
	bool framed = wxGetApp().GetFrame()->GetPanel< FtaVizPanel >()->GetCanvas()->FrameVisualization();
	lua_pushboolean( L, framed );
	return 1;
}

// FtaFrameVizFunction.cpp