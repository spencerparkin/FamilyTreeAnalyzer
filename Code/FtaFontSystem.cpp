// FtaFontSystem.cpp

#include "FtaFontSystem.h"
#include "FtaApp.h"
#include FT_MODULE_H
#include FT_TRUETYPE_IDS_H
#include <gl/GLU.h>

// TODO: Find and plug mem-leak.

FtaFontSystem::FtaFontSystem( void )
{
	initialized = false;
	font = "ChanticleerRomanNF.ttf"; //"OpenSans-Regular.ttf"; //"Anonymous_Pro.ttf";
	lineWidth = 0.f;
	lineHeight = 5.f;
	justification = JUSTIFY_LEFT;
}

/*virtual*/ FtaFontSystem::~FtaFontSystem( void )
{
	Finalize();
}

bool FtaFontSystem::Initialize( void )
{
	bool success = false;

	do
	{
		if( initialized )
			break;

		FT_Error error = FT_Init_FreeType( &library );
		if( error != FT_Err_Ok )
			break;

		initialized = true;

		success = true;
	}
	while( false );

	return success;
}

bool FtaFontSystem::Finalize( void )
{
	bool success = false;
	
	do
	{
		while( fontMap.size() > 0 )
		{
			FtaFontMap::iterator iter = fontMap.begin();
			FtaFont* font = iter->second;
			font->Finalize();
			delete font;
			fontMap.erase( iter );
		}

		FT_Error error = FT_Done_Library( library );
		if( error != FT_Err_Ok )
			break;

		initialized = false;

		success = true;
	}
	while( false );

	return success;
}

bool FtaFontSystem::DrawText( const wxString& text )
{
	bool success = false;

	do
	{
		if( !initialized )
			break;

		if( font.IsEmpty() )
			break;

		wxString key = MakeFontKey( font );

		FtaFont* cachedFont = nullptr;

		FtaFontMap::iterator iter = fontMap.find( key );
		if( iter != fontMap.end() )
			cachedFont = iter->second;
		else
		{
			cachedFont = new FtaFont( this );

			if( !cachedFont->Initialize( font ) )
			{
				delete cachedFont;
				break;
			}

			fontMap[ key ] = cachedFont;
		}

		if( !cachedFont )
			break;

		if( !cachedFont->DrawText( text ) )
			break;

		success = true;
	}
	while( false );

	return success;
}

wxString FtaFontSystem::MakeFontKey( const wxString& font )
{
	wxString key = font;
	key.MakeLower();
	return key;
}

FtaFont::FtaFont( FtaFontSystem* fontSystem )
{
	initialized = false;
	this->fontSystem = fontSystem;
	lineHeightMetric = 0;
}

/*virtual*/ FtaFont::~FtaFont( void )
{
	if( initialized )
		Finalize();
}

// TODO: Dare I tackle kerning?  It would make the text look a bit better.
/*virtual*/ bool FtaFont::Initialize( const wxString& font )
{
	bool success = false;
	FT_Face face = nullptr;

	do
	{
		if( initialized )
			break;

		wxString fontFile = wxGetApp().GetResourceFolder() + "\\Fonts\\" + font;

		FT_Error error = FT_New_Face( fontSystem->GetLibrary(), fontFile, 0, &face );
		if( error != FT_Err_Ok )
			break;

		int i, j = -1;
		for( i = 0; i < face->num_charmaps && j == -1; i++ )
		{
			FT_CharMapRec* charmapRec = face->charmaps[i];
			if( charmapRec->encoding == FT_ENCODING_UNICODE )
				j = i;
		}

		if( j == -1 )
			break;

		error = FT_Set_Charmap( face, face->charmaps[j] );
		if( error != FT_Err_Ok )
			break;

		error = FT_Set_Char_Size( face, 0, 128*64, 0, 0 );
		if( error != FT_Err_Ok )
			break;

		const wchar_t* charCodeString = L"abcdefghijklmnopqrstuvwxyz"
										"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
										"`~!@#$%^&*()_+-={}[],.<>/?'\";: ";

		lineHeightMetric = 0;

		for( i = 0; charCodeString[i] != '\0'; i++ )
		{
			FT_ULong charCode = charCodeString[i];
			FT_UInt glyph_index = FT_Get_Char_Index( face, charCode );
			if( glyph_index == 0 )
				continue;

			error = FT_Load_Glyph( face, glyph_index, FT_LOAD_DEFAULT );
			if( error != FT_Err_Ok )
				break;

			FT_GlyphSlot& glyph = face->glyph;

			if( glyph->format != FT_GLYPH_FORMAT_BITMAP )
			{
				error = FT_Render_Glyph( glyph, FT_RENDER_MODE_NORMAL );
				if( error != FT_Err_Ok )
					break;
			}

			FtaGlyph* cachedGlyph = new FtaGlyph();
			glyphMap[ charCode ] = cachedGlyph;

			if( !cachedGlyph->Initialize( glyph ) )
				break;

			if( glyph->metrics.height == glyph->metrics.horiBearingY )
				if( ( unsigned )glyph->metrics.height > lineHeightMetric )
					lineHeightMetric = glyph->metrics.height;
		}

		if( charCodeString[i] != '\0' )
			break;

		if( lineHeightMetric == 0 )
			break;

		initialized = true;

		success = true;
	}
	while( false );

	if( face )
		FT_Done_Face( face );

	return success;
}

/*virtual*/ bool FtaFont::Finalize( void )
{
	bool success = false;

	do
	{
		while( glyphMap.size() > 0 )
		{
			FtaGlyphMap::iterator iter = glyphMap.begin();
			FtaGlyph* glyph = iter->second;
			glyph->Finalize();
			delete glyph;
			glyphMap.erase( iter );
		}

		initialized = false;

		success = true;
	}
	while( false );
	
	return success;
}

// TODO: Add cached display-list optimization for static text.
/*virtual*/ bool FtaFont::DrawText( const wxString& text )
{
	bool success = false;
	GlyphLink* glyphLink = nullptr;

	do
	{
		if( text.IsEmpty() )
			break;

		glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

		glEnable( GL_TEXTURE_2D );
		glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND );

		GLfloat color[4];
		glGetFloatv( GL_CURRENT_COLOR, color );
		glTexEnvfv( GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, color );

		GLfloat conversionFactor = fontSystem->GetLineHeight() / GLfloat( lineHeightMetric );

		const wchar_t* charCodeString = text.wc_str();

		glyphLink = GenerateGlyphChain( charCodeString, conversionFactor );
		if( !glyphLink )
			break;

		// TODO: Manipulate and split glyph-chain according to desired justification here.

		RenderGlyphChain( glyphLink, 0.f, 0.f );

		success = true;
	}
	while( false );

	glDisable( GL_BLEND );
	glDisable( GL_TEXTURE_2D );

	DeleteGlyphChain( glyphLink );

	return success;
}

void FtaFont::GlyphLink::GetMetrics( FT_Glyph_Metrics& metrics ) const
{
	if( glyph )
		metrics = glyph->GetMetrics();
	else
	{
		memset( &metrics, 0, sizeof( FT_Glyph_Metrics ) );
		metrics.width = 500;
		metrics.height = 500;
	}
}

FtaFont::GlyphLink* FtaFont::GenerateGlyphChain( const wchar_t* charCodeString, GLfloat conversionFactor )
{
	GlyphLink* firstGlyphLink = nullptr;
	GlyphLink* prevGlyphLink = nullptr;

	for( int i = 0; charCodeString[i] != '\0'; i++ )
	{
		wchar_t charCode = charCodeString[i];

		FtaGlyph* glyph = nullptr;
		FtaGlyphMap::iterator iter = glyphMap.find( charCode );
		if( iter != glyphMap.end() )
			glyph = iter->second;

		GlyphLink* glyphLink = new GlyphLink();
		glyphLink->glyph = glyph;

		FT_Glyph_Metrics metrics;
		glyphLink->GetMetrics( metrics );

		glyphLink->w = GLfloat( metrics.width ) * conversionFactor;
		glyphLink->h = GLfloat( metrics.height ) * conversionFactor;

		if( !prevGlyphLink )
		{
			glyphLink->dx = 0.f;
			glyphLink->dy = 0.f;

			glyphLink->x = 0.f;
			glyphLink->y = GLfloat( metrics.horiBearingY - metrics.height ) * conversionFactor;

			firstGlyphLink = glyphLink;
		}
		else
		{
			FT_Glyph_Metrics prevMetrics;
			prevGlyphLink->GetMetrics( prevMetrics );

			glyphLink->dx = GLfloat( prevMetrics.horiAdvance ) * conversionFactor;
			glyphLink->dy = 0.f;

			glyphLink->x = GLfloat( metrics.horiBearingX ) * conversionFactor;
			glyphLink->y = GLfloat( metrics.horiBearingY - metrics.height ) * conversionFactor;

			prevGlyphLink->nextGlyphLink = glyphLink;
		}

		prevGlyphLink = glyphLink;
	}

	return firstGlyphLink;
}

void FtaFont::RenderGlyphChain( GlyphLink* glyphLink, GLfloat ox, GLfloat oy )
{
	while( glyphLink )
	{
		ox += glyphLink->dx;
		oy += glyphLink->dy;

		GLuint texture = 0;
		if( glyphLink->glyph )
			texture = glyphLink->glyph->GetTexture();

		// Note that if the 0-texture is bound, we should just draw a solid quad.
		glBindTexture( GL_TEXTURE_2D, texture );
		glBegin( GL_QUADS );

		glTexCoord2f( 0.f, 0.f );	glVertex2f( ox + glyphLink->x, oy + glyphLink->y );
		glTexCoord2f( 1.f, 0.f );	glVertex2f( ox + glyphLink->x + glyphLink->w, oy + glyphLink->y );
		glTexCoord2f( 1.f, 1.f );	glVertex2f( ox + glyphLink->x + glyphLink->w, oy + glyphLink->y + glyphLink->h );
		glTexCoord2f( 0.f, 1.f );	glVertex2f( ox + glyphLink->x, oy + glyphLink->y + glyphLink->h );

		glEnd();

		glyphLink = glyphLink->nextGlyphLink;
	}
}

void FtaFont::DeleteGlyphChain( GlyphLink* glyphLink )
{
	while( glyphLink )
	{
		GlyphLink* nextGlyphLink = glyphLink->nextGlyphLink;
		delete glyphLink;
		glyphLink = nextGlyphLink;
	}
}

FtaGlyph::FtaGlyph( void )
{
	texture = 0;
}

/*virtual*/ FtaGlyph::~FtaGlyph( void )
{
	Finalize();
}

bool FtaGlyph::Initialize( FT_GlyphSlot& glyphSlot )
{
	bool success = false;
	
	do
	{
		FT_Bitmap& bitmap = glyphSlot->bitmap;
		if( bitmap.pixel_mode != FT_PIXEL_MODE_GRAY )
			break;

		if( bitmap.pitch != bitmap.width )
			break;

		metrics = glyphSlot->metrics;

		GLuint width = bitmap.width;
		GLuint height = bitmap.rows;

		// Note that the formatting code will depend on the texture map fitting the glyph as tightly as possible.
		// Some glyphs are just spaces, in which cases, there will be no buffer.
		GLubyte* bitmapBuffer = ( GLubyte* )bitmap.buffer;
		if( bitmapBuffer != nullptr )
		{
			glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

			glGenTextures( 1, &texture );
			if( texture == 0 )
				break;

			glBindTexture( GL_TEXTURE_2D, texture );

			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

			GLuint bytesPerTexel = 4;
			GLuint textureWidth = 128;
			GLuint textureHeight = 128;
			GLuint mipLevel = 0;
			while( textureWidth > 0 && textureHeight > 0 )
			{
				GLubyte* textureBuffer = new GLubyte[ textureWidth * textureHeight * bytesPerTexel ];

				// Flip the image and make it of appropriate dimensions for OpenGL.
				for( GLuint i = 0; i < textureHeight; i++ )
				{
					GLfloat u = 1.f - GLfloat(i) / GLfloat( textureHeight - 1 );
					GLuint bitmap_i = GLuint( u * GLfloat( height ) );
					if( bitmap_i >= height )
						bitmap_i = height - 1;

					for( GLuint j = 0; j < textureWidth; j++ )
					{
						GLfloat v = GLfloat(j) / GLfloat( textureWidth - 1 );
						GLuint bitmap_j = GLuint( v * GLfloat( width ) );
						if( bitmap_j >= width )
							bitmap_j = width - 1;

						GLubyte grey = bitmapBuffer[ bitmap_i * width + bitmap_j ];
						GLubyte* texel = &textureBuffer[ ( i * textureWidth + j ) * bytesPerTexel ];
						for( GLuint k = 0; k < bytesPerTexel; k++ )
							texel[k] = grey;
					}
				}

				glTexImage2D( GL_TEXTURE_2D, mipLevel, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureBuffer );

				delete[] textureBuffer;

				GLenum error = glGetError();
				if( error != GL_NO_ERROR )
				{
					const GLubyte* errorStr = gluErrorString( error );
					errorStr = nullptr;
					break;
				}

				mipLevel++;

				textureWidth >>= 1;
				textureHeight >>= 1;
			}

			if( textureWidth > 0 || textureHeight > 0 )
				break;
		}

		success = true;
	}
	while( false );

	return success;
}

bool FtaGlyph::Finalize( void )
{
	if( texture != 0 )
		glDeleteTextures( 1, &texture );

	return true;
}

// FtaFontSystem.cpp