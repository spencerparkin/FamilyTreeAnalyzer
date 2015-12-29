// FtaFontSystem.h

#pragma once

#include <wx/string.h>
#include <wx/glcanvas.h>
#include <ft2build.h>
#include FT_FREETYPE_H

class FtaFont;
class FtaGlyph;

WX_DECLARE_STRING_HASH_MAP( FtaFont*, FtaFontMap );
WX_DECLARE_HASH_MAP( FT_ULong, FtaGlyph*, wxIntegerHash, wxIntegerEqual, FtaGlyphMap );

// An instance of this class is a layer of software that sits between
// our application and the free-type library.
class FtaFontSystem
{
public:

	FtaFontSystem( void );
	virtual ~FtaFontSystem( void );

	bool Initialize( void );
	bool Finalize( void );

	enum Justification
	{
		JUSTIFY_LEFT,
		JUSTIFY_RIGHT,
		JUSTIFY_CENTER,
		JUSTIFY_LEFT_AND_RIGHT,
	};

	// Text is always drawn in the 4th-quadrant of the XY-plane.
	// It is up to the caller to setup the appropriate transformation matrices to
	// get the text drawn where they would like it, and in what orientation.
	// When called, we assume that an OpenGL context is already bound.  Only one font
	// system should be used per context since the system caches display lists.
	bool DrawText( const wxString& text, const wxString& font, GLfloat wrapLength, Justification justification );

	FT_Library& GetLibrary( void ) { return library; }

private:

	wxString MakeFontKey( const wxString& font );

	bool initialized;
	FT_Library library;
	FtaFontMap fontMap;
};

// An instance of this class maintains a means of rendering a cached
// font as quickly as possible using OpenGL.
class FtaFont
{
public:

	FtaFont( FtaFontSystem* fontSystem );
	virtual ~FtaFont( void );

	virtual bool Initialize( const wxString& font );
	virtual bool Finalize( void );

	virtual bool DrawText( const wxString& text, GLfloat wrapLength, FtaFontSystem::Justification justification );

private:

	struct GlyphRender
	{
		GLfloat x, y;
		FtaGlyph* glyph;
	};

	typedef wxVector< GlyphRender > LineOfText;

	bool EatLineOfText( LineOfText& lineOfText, const wchar_t*& charCode, GLfloat& baseLine, GLfloat wrapLength, FtaFontSystem::Justification justification );
	bool RenderLineOfText( const LineOfText& lineOfText );

	bool initialized;
	FtaFontSystem* fontSystem;
	FtaGlyphMap glyphMap;
};

class FtaGlyph
{
public:

	FtaGlyph( void );
	virtual ~FtaGlyph( void );

	bool Initialize( FT_GlyphSlot& glyphSlot );
	bool Finalize( void );

	GLuint GetDisplayList( void ) { return displayList; }

private:

	GLuint displayList;
	//...also own info about character width and moving to the next character, etc...
};

// FtaFontSystem.h
