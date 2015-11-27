// FtaApp.h

#pragma once

#include <wx/setup.h>
#include <wx/app.h>

class FtaApp : public wxApp
{
public:

	FtaApp( void );
	virtual ~FtaApp( void );

	virtual bool OnInit( void ) override;
	virtual int OnExit( void ) override;
};

wxDECLARE_APP( FtaApp );

// FtaApp.h