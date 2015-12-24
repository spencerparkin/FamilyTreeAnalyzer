// FtaVizPanel.cpp

#include "FtaVizPanel.h"
#include <wx/sizer.h>

wxIMPLEMENT_DYNAMIC_CLASS( FtaVizPanel, FtaPanel );

// TODO: Implement kinetic camera model for 2D and 3D space examination.

FtaVizPanel::FtaVizPanel( void )
{
	canvas = nullptr;
}

/*virtual*/ FtaVizPanel::~FtaVizPanel( void )
{
}

/*virtual*/ bool FtaVizPanel::GetPaneInfo( wxAuiPaneInfo& paneInfo )
{
	paneInfo.Caption( "Graph" );
	paneInfo.CenterPane();
	paneInfo.Name( "GraphPanel" );
	return true;
}

/*virtual*/ bool FtaVizPanel::MakeControls( void )
{
	canvas = new Canvas( this );

	wxBoxSizer* boxSizer = new wxBoxSizer( wxVERTICAL );
	boxSizer->Add( canvas, 1, wxALL | wxGROW, 0 );
	SetSizer( boxSizer );

	return true;
}

/*virtual*/ bool FtaVizPanel::TimerUpdate( void )
{
	return true;
}

int FtaVizPanel::Canvas::attributeList[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, 0 };

FtaVizPanel::Canvas::Canvas( wxWindow* parent ) : wxGLCanvas( parent, wxID_ANY, attributeList )
{
	context = nullptr;
	viz = nullptr;
	camera = nullptr;

	Bind( wxEVT_PAINT, &Canvas::OnPaint, this );
	Bind( wxEVT_SIZE, &Canvas::OnSize, this );
}

/*virtual*/ FtaVizPanel::Canvas::~Canvas( void )
{
	delete context;

	SetVisualization( nullptr );
	SetCamera( nullptr );
}

void FtaVizPanel::Canvas::SetVisualization( FtaVisualization* viz )
{
	if( this->viz )
		delete this->viz;

	this->viz = viz;

	Refresh();
}

void FtaVizPanel::Canvas::SetCamera( FtaCamera* camera )
{
	if( this->camera )
		delete this->camera;

	this->camera = camera;

	Refresh();
}

void FtaVizPanel::Canvas::OnPaint( wxPaintEvent& event )
{
	BindContext();

	glClearColor( 1.f, 1.f, 1.f, 1.f );
	glClear( GL_COLOR_BUFFER_BIT );

	// TODO: Call upon camera to setup our projection matrix, etc.

	if( viz )
		viz->Draw( GL_RENDER );

	glFlush();

	SwapBuffers();
}

void FtaVizPanel::Canvas::OnSize( wxSizeEvent& event )
{
	BindContext();

	wxSize size = event.GetSize();
	glViewport( 0, 0, size.GetWidth(), size.GetHeight() );

	Refresh();
}

void FtaVizPanel::Canvas::BindContext( void )
{
	if( !context )
		context = new wxGLContext( this );

	SetCurrent( *context );
}

GLuint FtaVizPanel::Canvas::GenerateTexture( const wxImage& image )
{
	BindContext();

	GLuint texture;
	glGenTextures( 1, &texture );
	if( texture != GL_INVALID_VALUE )
	{
		glBindTexture( GL_TEXTURE_2D, texture );
		
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, image.GetWidth(), image.GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, image.GetData() );
	}

	return texture;
}

void FtaVizPanel::Canvas::FreeTexture( GLuint texture )
{
	BindContext();

	glDeleteTextures( 1, &texture );
}

// FtaVizPanel.cpp