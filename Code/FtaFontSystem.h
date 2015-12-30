// FtaFontSystem.h

#pragma once

#include <wx/string.h>
#include <wx/glcanvas.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "c3ga/c3ga.h"

class FtaFont;
class FtaGlyph;

WX_DECLARE_STRING_HASH_MAP( FtaFont*, FtaFontMap );
WX_DECLARE_HASH_MAP( FT_ULong, FtaGlyph*, wxIntegerHash, wxIntegerEqual, FtaGlyphMap );
WX_DECLARE_HASH_MAP( FT_ULong, FT_Vector, wxIntegerHash, wxIntegerEqual, FtaKerningMap );

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

	void SetJustification( Justification justification ) { this->justification = justification; }
	Justification GetJustification( void ) { return justification; }

	void SetLineWidth( GLfloat lineWidth ) { this->lineWidth = lineWidth; }
	GLfloat GetLineWidth( void ) { return lineWidth; }

	void SetLineHeight( GLfloat lineheight ) { this->lineHeight = lineHeight; }
	GLfloat GetLineHeight( void ) { return lineHeight; }

	void SetFont( const wxString& font ) { this->font = font; }
	const wxString& GetFont( void ) { return font; }

	// When called, we assume that an OpenGL context is already bound.  Only one font
	// system should be used per context since the system caches texture objects and display lists.
	// To position and orient text, the caller must setup the appropriate modelview matrix.
	// The object-space of the text is the 4th quadrant of the XY-plane.
	bool DrawText( const wxString& text );

	FT_Library& GetLibrary( void ) { return library; }

private:

	wxString MakeFontKey( const wxString& font );

	wxString font;
	GLfloat lineWidth, lineHeight;
	Justification justification;
	bool initialized;
	FT_Library library;
	FtaFontMap fontMap;
};

// An instance of this class maintains a means of rendering a cached font using OpenGL.
class FtaFont
{
public:

	FtaFont( FtaFontSystem* fontSystem );
	virtual ~FtaFont( void );

	virtual bool Initialize( const wxString& font );
	virtual bool Finalize( void );

	virtual bool DrawText( const wxString& text );

private:

	struct GlyphLink
	{
		GLfloat dx, dy;		// Adding this to the previous glyph origin gives us our origin.
		GLfloat x, y;		// This is the lower-left cornder position of the glyph.
		GLfloat w, h;		// This is the width and height of the glyph.
		FtaGlyph* glyph;
		//bool canStretch;
		GlyphLink* nextGlyphLink;

		void GetMetrics( FT_Glyph_Metrics& metrics ) const;
	};

	GlyphLink* GenerateGlyphChain( const wchar_t* charCodeString, GLfloat conversionFactor );
	void KernGlyphChain( GlyphLink* glyphLink, GLfloat conversionFactor );
	void RenderGlyphChain( GlyphLink* glyphLink, GLfloat ox, GLfloat oy );
	void DeleteGlyphChain( GlyphLink* glyphLink );

	FT_ULong MakeKerningKey( FT_UInt leftGlyphIndex, FT_UInt rightGlyphIndex );

	bool initialized;
	FtaFontSystem* fontSystem;
	FtaGlyphMap glyphMap;
	FtaKerningMap kerningMap;
	GLuint lineHeightMetric;
};

class FtaGlyph
{
public:

	FtaGlyph( void );
	virtual ~FtaGlyph( void );

	bool Initialize( FT_GlyphSlot& glyphSlot, FT_UInt glyphIndex );
	bool Finalize( void );

	GLuint GetTexture( void ) { return texture; }
	const FT_Glyph_Metrics& GetMetrics( void ) { return metrics; }
	FT_UInt GetIndex( void ) { return glyphIndex; }

private:

	GLuint texture;
	FT_UInt glyphIndex;
	FT_Glyph_Metrics metrics;
};

// FtaFontSystem.h
