// FtaApp.h

#pragma once

#include <wx/setup.h>
#include <wx/app.h>
#include <lua.hpp>
#include <lauxlib.h>

class FtaClient;
class FtaFrame;
class FtaTreeCache;
class FtaMiscCache;

class FtaApp : public wxApp
{
public:

	FtaApp( void );
	virtual ~FtaApp( void );

	virtual bool OnInit( void ) override;
	virtual int OnExit( void ) override;

	FtaClient* GetClient( void ) { return client; }
	FtaFrame* GetFrame( void ) { return frame; }
	FtaTreeCache* GetTreeCache( void ) { return treeCache; }
	lua_State* GetLuaState( void ) { return L; }

private:

	FtaClient* client;
	FtaFrame* frame;
	FtaTreeCache* treeCache;
	lua_State* L;
};

wxDECLARE_APP( FtaApp );

// FtaApp.h