// FtaSetGraphFunction.cpp

#include "FtaSetGraphFunction.h"
#include "FtaGraph.h"
#include "FtaGraphViz.h"
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

/*virtual*/ int FtaSetGraphFunction::Help( lua_State* L )
{
	wxGetApp().GetFrame()->AddLogMessage(
		"Use the setGraph() function to tell FamilyTreeAnalyzer what set of people "
		"you would like to visualize and in what way.  The first argument is the graph type.  "
		"As of this writing, \"default\" is the only acceptable value for this argument.  "
		"The second argument is an array consisting of person-IDs in the form of string.  "
		"These are the people you would like to see graphed." );
	return 0;
}

/*virtual*/ int FtaSetGraphFunction::Call( lua_State* L )
{
	bool success = false;
	wxString errorMsg;
	FtaGraph* graph = nullptr;

	// TODO: We might provide another arg that is a table interpreted by the chosen graph type.
	//       We can call a virtual method on the graph to configure itself based on that table.

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
		else if( graphType == "graphviz" )
			graph = new FtaGraphViz();
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