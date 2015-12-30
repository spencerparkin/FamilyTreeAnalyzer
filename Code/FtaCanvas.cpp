// FtaCanvas.cpp

#include "FtaCanvas.h"
#include "FtaCamera.h"
#include "FtaKinematicCamera.h"
#include "FtaCanvasPlugin.h"
#include <wx/image.h>

int FtaCanvas::attributeList[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, 0 };

FtaCanvas::FtaCanvas( wxWindow* parent ) : wxGLCanvas( parent, wxID_ANY, attributeList )
{
	lastFrameTimeSeconds = 0.0;
	framesPerSecond = 0.0;

	fontSystem = new FtaFontSystem();
	bool initialized = fontSystem->Initialize();
	wxASSERT( initialized );

	context = nullptr;
	viz = nullptr;
	camera = nullptr;

	SetCamera( new FtaKinematicCamera() );

	Bind( wxEVT_PAINT, &FtaCanvas::OnPaint, this );
	Bind( wxEVT_SIZE, &FtaCanvas::OnSize, this );
}

/*virtual*/ FtaCanvas::~FtaCanvas( void )
{
	fontSystem->Finalize();

	// Must delete this before our context.
	delete fontSystem;
	delete context;

	SetVisualization( nullptr );
	SetCamera( nullptr );
}

void FtaCanvas::SetVisualization( FtaVisualization* viz )
{
	SwapPlugin( ( FtaCanvasPlugin*& )this->viz, viz );
	Refresh();
}

void FtaCanvas::SetCamera( FtaCamera* camera )
{
	SwapPlugin( ( FtaCanvasPlugin*& )this->camera, camera );
	Refresh();
}

void FtaCanvas::SwapPlugin( FtaCanvasPlugin*& curPlugin, FtaCanvasPlugin* newPlugin )
{
	if( curPlugin )
	{
		curPlugin->Unbind();
		delete curPlugin;
	}

	curPlugin = newPlugin;

	if( curPlugin )
	{
		curPlugin->canvas = this;
		curPlugin->Bind();
	}
}

void FtaCanvas::OnPaint( wxPaintEvent& event )
{
	if( !camera )
		return;

	double curFrameTimeSeconds = double( clock() ) / double( CLOCKS_PER_SEC );
	double deltaFrameTimeSeconds = 0.0;
	if( lastFrameTimeSeconds != 0.0 )
		deltaFrameTimeSeconds = curFrameTimeSeconds - lastFrameTimeSeconds;
	if( deltaFrameTimeSeconds != 0.0 )
		framesPerSecond = 1.0 / deltaFrameTimeSeconds;
	lastFrameTimeSeconds = curFrameTimeSeconds;

	BindContext();

	camera->PreRender( GL_RENDER );

	if( viz )
		viz->Draw( GL_RENDER );

	glColor3f( 0.f, 0.f, 0.f );
	fontSystem->DrawText( "The quick brown fox jumped over the lazy dogs." );

	glBegin( GL_LINES );

	glColor3f( 1.f, 0.f, 0.f );
	glVertex3f( 0.f, 0.f, 0.f );
	glVertex3f( 5.f, 0.f, 0.f );

	glColor3f( 0.f, 1.f, 0.f );
	glVertex3f( 0.f, 0.f, 0.f );
	glVertex3f( 0.f, 5.f, 0.f );

	glColor3f( 0.f, 0.f, 1.f );
	glVertex3f( 0.f, 0.f, 0.f );
	glVertex3f( 0.f, 0.f, 5.f );

	glEnd();

	// TODO: Render FPS in upper-right hande corner?

	camera->PostRender( GL_RENDER );
}

void FtaCanvas::OnSize( wxSizeEvent& event )
{
	BindContext();

	wxSize size = event.GetSize();
	glViewport( 0, 0, size.GetWidth(), size.GetHeight() );

	Refresh();
}

void FtaCanvas::PerformPick( void )
{
	if( !( camera && viz ) )
		return;

	BindContext();

	camera->PreRender( GL_SELECT );
	viz->Draw( GL_SELECT );
	camera->PostRender( GL_SELECT );
}

bool FtaCanvas::TimerUpdate( void )
{
	bool animating = false;
	if( viz && viz->Animate() )
		animating = true;

	if( camera && camera->Animate() )
		animating = true;

	// TODO: Only request refresh if we're animating?
	Refresh();

	return true;
}

void FtaCanvas::BindContext( void )
{
	if( !context )
		context = new wxGLContext( this );

	SetCurrent( *context );
}

GLuint FtaCanvas::GenerateTexture( const wxImage& image )
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

void FtaCanvas::FreeTexture( GLuint texture )
{
	BindContext();

	glDeleteTextures( 1, &texture );
}

// FtaCanvas.cpp