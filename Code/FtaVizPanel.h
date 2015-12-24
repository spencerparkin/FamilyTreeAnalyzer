// FtaVizPanel.h

#pragma once

#include "FtaPanel.h"
#include "FtaVisualization.h"
#include "FtaCamera.h"
#include <wx/glcanvas.h>

class FtaVizPanel : public FtaPanel
{
public:

	wxDECLARE_DYNAMIC_CLASS( FtaVizPanel );

	FtaVizPanel( void );
	virtual ~FtaVizPanel( void );

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

		FtaVisualization* GetVisualization( void ) { return viz; }
		void SetVisualization( FtaVisualization* viz );

		FtaCamera* GetCamera( void ) { return camera; }
		void SetCamera( FtaCamera* camera );

	private:

		void BindContext( void );

		wxGLContext* context;
		static int attributeList[];
		FtaVisualization* viz;
		FtaCamera* camera;
	};

	Canvas* GetCanvas( void ) { return canvas; }

private:

	Canvas* canvas;
};

// FtaVizPanel.h
