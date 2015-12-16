// FtaSetGraphFunction.cpp

#include "FtaSetGraphFunction.h"
#include "FtaGraph.h"
#include "FtaFrame.h"
#include "FtaApp.h"
#include "FtaGraphPanel.h"

wxIMPLEMENT_DYNAMIC_CLASS( FtaSetGraphFunction, FtaLuaFunction );

FtaSetGraphFunction::FtaSetGraphFunction( void )
{
}

/*virtual*/ FtaSetGraphFunction::~FtaSetGraphFunction( void )
{
}

/*virtual*/ int FtaSetGraphFunction::Call( lua_State* L )
{
	bool success = false;
	wxString errorMsg;
	FtaGraph* graph = nullptr;

	do
	{
		if( lua_gettop( L ) < 2 )
		{
			errorMsg = "Expected two arguments.";
			break;
		}

		if( !lua_isstring( L, -2 ) )
		{
			errorMsg = "Expected first arg as string.";
			break;
		}

		wxString graphType = lua_tostring( L, -2 );
		if( graphType == "default" )
			graph = new FtaGraph();
		//else if( graphType == ...
		else
		{
			errorMsg.Format( "Unknown graph type \"" + graphType + "\"." );
			break;
		}

		if( !lua_istable( L, -1 ) )
		{
			errorMsg = "Expected second arg as table.";
			break;
		}
		
		lua_len( L, -1 );
		int len = lua_tointeger( L, -1 );
		lua_pop( L, 1 );

		int i;
		for( i = 1; i <= len; i++ )
		{
			lua_pushinteger( L, i );
			lua_gettable( L, -2 );

			if( !lua_isstring( L, -1 ) )
			{
				errorMsg = "Expected table entries to be strings.";
				break;
			}

			wxString personId = lua_tostring( L, -1 );
			lua_pop( L, 1 );

			graph->AddPerson( personId );
		}

		if( i < len )
			break;

		FtaGraphPanel* graphPanel = wxGetApp().GetFrame()->GetPanel< FtaGraphPanel >();
		if( !graphPanel )
			break;

		graphPanel->GetCanvas()->SetGraph( graph );

		success = true;
	}
	while( false );

	if( !success )
	{
		delete graph;

		if( errorMsg.IsEmpty() )
			errorMsg = "Unknown error.";

		return luaL_error( L, errorMsg );
	}

	return 0;
}

// FtaSetGraphFunction.cpp