// FtaGraphPanel.h

#pragma once

#include "FtaPanel.h"
#include <wx/glcanvas.h>

// TODO: Own a wxCanvas control.  Draw family tree using a spring-lattice method.
// TODO: Support picking?
class FtaGraphPanel : public FtaPanel
{
public:

	wxDECLARE_DYNAMIC_CLASS( FtaGraphPanel );

	FtaGraphPanel( void );
	virtual ~FtaGraphPanel( void );

	virtual bool GetPaneInfo( wxAuiPaneInfo& paneInfo );
	virtual bool MakeControls( void );
	virtual bool TimerUpdate( void );

	class Canvas : public wxGLCanvas
	{
	public:

		Canvas( wxWindow* parent );
		virtual ~Canvas( void );

		void OnPaint( wxPaintEvent& event );
		void OnSize( wxSizeEvent& event );

		GLuint GenerateTexture( const wxImage& image );
		void FreeTexture( GLuint texture );

	private:

		void BindContext( void );

		wxGLContext* context;
		static int attributeList[];
	};

	Canvas* GetCanvas( void ) { return canvas; }

private:

	Canvas* canvas;
};

// FtaGraphPanel.h
