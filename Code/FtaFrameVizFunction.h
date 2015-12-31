// FtaFrameVizFunction.h

#pragma once

#include "FtaLuaFunction.h"

class FtaFrameVizFunction : public FtaLuaFunction
{
public:

	wxDECLARE_DYNAMIC_CLASS( FtaFrameVizFunction );

	FtaFrameVizFunction( void );
	virtual ~FtaFrameVizFunction( void );

	virtual int Call( lua_State* L ) override;
	virtual int Help( lua_State* L ) override;
	virtual wxString LuaFunctionName( void ) override { return "frameViz"; }
};

// FtaFrameVizFunction.h