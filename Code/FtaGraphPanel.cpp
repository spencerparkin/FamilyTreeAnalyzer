// FtaGraphPanel.cpp

#include "FtaGraphPanel.h"
#include <wx/sizer.h>

wxIMPLEMENT_DYNAMIC_CLASS( FtaGraphPanel, FtaPanel );

// TODO: Implement kinetic camera model for 2D and 3D space examination.

FtaGraphPanel::FtaGraphPanel( void )
{
	canvas = nullptr;
}

/*virtual*/ FtaGraphPanel::~FtaGraphPanel( void )
{
}

/*virtual*/ bool FtaGraphPanel::GetPaneInfo( wxAuiPaneInfo& paneInfo )
{
	paneInfo.Caption( "Graph" );
	paneInfo.CenterPane();
	paneInfo.Name( "GraphPanel" );
	return true;
}

/*virtual*/ bool FtaGraphPanel::MakeControls( void )
{
	canvas = new Canvas( this );

	wxBoxSizer* boxSizer = new wxBoxSizer( wxVERTICAL );
	boxSizer->Add( canvas, 1, wxALL | wxGROW, 0 );
	SetSizer( boxSizer );

	return true;
}

/*virtual*/ bool FtaGraphPanel::TimerUpdate( void )
{
	return true;
}

int FtaGraphPanel::Canvas::attributeList[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, 0 };

FtaGraphPanel::Canvas::Canvas( wxWindow* parent ) : wxGLCanvas( parent, wxID_ANY, attributeList )
{
	context = nullptr;
	graph = nullptr;

	Bind( wxEVT_PAINT, &Canvas::OnPaint, this );
	Bind( wxEVT_SIZE, &Canvas::OnSize, this );
}

/*virtual*/ FtaGraphPanel::Canvas::~Canvas( void )
{
	delete context;

	SetGraph( nullptr );
}

void FtaGraphPanel::Canvas::SetGraph( FtaGraph* graph )
{
	if( this->graph )
		delete this->graph;

	this->graph = graph;

	Refresh();
}

void FtaGraphPanel::Canvas::OnPaint( wxPaintEvent& event )
{
	BindContext();

	glClearColor( 1.f, 1.f, 1.f, 1.f );
	glClear( GL_COLOR_BUFFER_BIT );

	if( graph )
		graph->Draw( GL_RENDER );

	glFlush();

	SwapBuffers();
}

void FtaGraphPanel::Canvas::OnSize( wxSizeEvent& event )
{
	BindContext();

	wxSize size = event.GetSize();
	glViewport( 0, 0, size.GetWidth(), size.GetHeight() );

	Refresh();
}

void FtaGraphPanel::Canvas::BindContext( void )
{
	if( !context )
		context = new wxGLContext( this );

	SetCurrent( *context );
}

GLuint FtaGraphPanel::Canvas::GenerateTexture( const wxImage& image )
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

void FtaGraphPanel::Canvas::FreeTexture( GLuint texture )
{
	BindContext();

	glDeleteTextures( 1, &texture );
}

// FtaGraphPanel.cpp