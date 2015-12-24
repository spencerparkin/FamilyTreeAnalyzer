// FtaCanvas.cpp

#include "FtaCanvas.h"
#include "FtaCamera.h"
#include "FtaKinematicCamera.h"
#include "FtaCanvasPlugin.h"
#include <wx/image.h>

int FtaCanvas::attributeList[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, 0 };

FtaCanvas::FtaCanvas( wxWindow* parent ) : wxGLCanvas( parent, wxID_ANY, attributeList )
{
	context = nullptr;
	viz = nullptr;
	camera = new FtaKinematicCamera();

	Bind( wxEVT_PAINT, &FtaCanvas::OnPaint, this );
	Bind( wxEVT_SIZE, &FtaCanvas::OnSize, this );
}

/*virtual*/ FtaCanvas::~FtaCanvas( void )
{
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
		RemoveEventHandler( curPlugin );
		delete curPlugin;
	}

	curPlugin = newPlugin;

	if( curPlugin )
	{
		curPlugin->Bind();
		SetNextHandler( curPlugin );
		curPlugin->SetPreviousHandler( this );
	}
}

void FtaCanvas::OnPaint( wxPaintEvent& event )
{
	if( !camera )
		return;

	BindContext();

	camera->PreRender( GL_RENDER, this );

	if( viz )
		viz->Draw( GL_RENDER );

	camera->PostRender( GL_RENDER, this );
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

	camera->PreRender( GL_SELECT, this );
	viz->Draw( GL_SELECT );
	camera->PostRender( GL_SELECT, this );
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