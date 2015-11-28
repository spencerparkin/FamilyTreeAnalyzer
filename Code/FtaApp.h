// FtaApp.h

#pragma once

#include <wx/setup.h>
#include <wx/app.h>

class FtaClient;

class FtaApp : public wxApp
{
public:

	FtaApp( void );
	virtual ~FtaApp( void );

	virtual bool OnInit( void ) override;
	virtual int OnExit( void ) override;

	FtaClient* GetClient( void ) { return client; }

private:

	FtaClient* client;
};

wxDECLARE_APP( FtaApp );

// FtaApp.h