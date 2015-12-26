// FtaCanvasPlugin.h

#pragma once

#include <wx/event.h>

class FtaCanvas;

class FtaCanvasPlugin : public wxEvtHandler
{
public:

	FtaCanvasPlugin( void );
	virtual ~FtaCanvasPlugin( void );

	virtual bool Bind( void ) = 0;
	virtual bool Unbind( void ) = 0;

	virtual bool Animate( void );

	FtaCanvas* canvas;
};

// FtaCanvasPlugin.h
