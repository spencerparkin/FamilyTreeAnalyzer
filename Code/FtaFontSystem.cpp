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
	font = "ChanticleerRomanNF.ttf";
	lineWidth = 0.f;
	lineHeight = 1.f;
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
	faceHeight = 0;
}

/*virtual*/ FtaFont::~FtaFont( void )
{
	if( initialized )
		Finalize();
}

/*virtual*/ bool FtaFont::Initialize( const wxString& font )
{
	bool success = false;
	FT_Face face;

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

		error = FT_Set_Char_Size( face, 0, 32*64, 300, 300 );
		if( error != FT_Err_Ok )
			break;

		const wchar_t* charCodeString = L"abcdefghijklmnopqrstuvwxyz"
										"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
										"`~!@#$%^&*()_+-={}[],.<>/?'\";: ";

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
		}

		if( charCodeString[i] != '\0' )
			break;

		faceHeight = face->height;

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

/*virtual*/ bool FtaFont::DrawText( const wxString& text )
{
	bool success = false;

	do
	{
		glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

		glEnable( GL_TEXTURE_2D );
		glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND );

		GLfloat color[4];
		glGetFloatv( GL_CURRENT_COLOR, color );
		glTexEnvfv( GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, color );

		const wchar_t* charCode = text.wc_str();

		LineOfText lineOfText;
		GLfloat baseLine = -fontSystem->GetLineHeight();
		
		do
		{
			if( !FormatLineOfText( lineOfText, charCode, baseLine ) )
				break;

			if( !RenderLineOfText( lineOfText ) )
				break;
		}
		while( *charCode != '\0' );

		if( *charCode != '\0' )
			break;

		success = true;
	}
	while( false );

	glDisable( GL_BLEND );
	glDisable( GL_TEXTURE_2D );

	return success;
}

bool FtaFont::FormatLineOfText( LineOfText& lineOfText, const wchar_t*& charCode, GLfloat& baseLine )
{
	lineOfText.clear();

	GLfloat conversionFactor = fontSystem->GetLineHeight() / GLfloat( faceHeight );

	GLuint spaceCount = 0;
	GLfloat totalSpaceAdvance = 0.f;
	GLfloat totalAdvance = 0.f;

	while( *charCode != '\0' )
	{
		FtaGlyphMap::iterator iter = glyphMap.find( FT_ULong( *charCode ) );
		if( iter == glyphMap.end() )
			return false;

		FtaGlyph* glyph = iter->second;

		GlyphRender glyphRender;
		glyphRender.glyph = glyph;

		GLfloat advance = GLfloat( glyph->GetMetrics().horiAdvance ) * conversionFactor;
		//if( totalAdvance + advance > fontSystem->GetLineWidth() )
		//	break;

		totalAdvance += advance;

		if( *charCode == ' ' )
		{
			spaceCount++;
			totalSpaceAdvance += advance;
		}

		lineOfText.push_back( glyphRender );

		charCode++;
	}

	// This offset is used for centering and right-justification by
	// simply adding to it to the left-justification calculations.
	GLfloat justifyOffset = 0.f;

	switch( fontSystem->GetJustification() )
	{
		// If this case we left-justify, but alter the advance of all spaces.
		// Note that if the altered advance is too big, we'll probably just want to left-justify.
		case FtaFontSystem::JUSTIFY_LEFT_AND_RIGHT:
		{
			break;
		}
		case FtaFontSystem::JUSTIFY_RIGHT:
		{
			break;
		}
		case FtaFontSystem::JUSTIFY_CENTER:
		{
			break;
		}
	}

	GLfloat xPen = 0.f;
	GLfloat yPen = baseLine;

	LineOfText::iterator iter = lineOfText.begin();
	while( iter != lineOfText.end() )
	{
		GlyphRender& glyphRender = *iter;

		glyphRender.x = xPen;
		glyphRender.y = yPen;
		glyphRender.w = 1.f;
		glyphRender.h = 1.f;

		xPen += 2.f;

		iter++;
	}

	baseLine -= fontSystem->GetLineHeight();
	return true;
}

bool FtaFont::RenderLineOfText( const LineOfText& lineOfText )
{
	LineOfText::const_iterator iter = lineOfText.begin();
	while( iter != lineOfText.end() )
	{
		const GlyphRender& glyphRender = *iter;
		
		GLuint texture = glyphRender.glyph->GetTexture();
		if( texture != 0 )
		{
			glBindTexture( GL_TEXTURE_2D, texture );

			glBegin( GL_QUADS );

			glTexCoord2f( 0.f, 0.f );	glVertex2f( glyphRender.x, glyphRender.y );
			glTexCoord2f( 1.f, 0.f );	glVertex2f( glyphRender.x + glyphRender.w, glyphRender.y );
			glTexCoord2f( 1.f, 1.f );	glVertex2f( glyphRender.x + glyphRender.w, glyphRender.y + glyphRender.h );
			glTexCoord2f( 0.f, 1.f );	glVertex2f( glyphRender.x, glyphRender.y + glyphRender.h );

			glEnd();
		}

		iter++;
	}

	return true;
}

FtaGlyph::FtaGlyph( void )
{
	texture = 0;
	alteredAdvance = 0.f;
}

/*virtual*/ FtaGlyph::~FtaGlyph( void )
{
	Finalize();
}

bool FtaGlyph::Initialize( FT_GlyphSlot& glyphSlot )
{
	bool success = false;
	GLubyte* textureBuffer = nullptr;

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
			// TODO: We could probably save on memory by using smaller resolution here and maybe smaller size.
			GLuint textureWidth = 128;
			GLuint textureHeight = 128;
			GLuint bytesPerTexel = 4;
			textureBuffer = new GLubyte[ textureWidth * textureHeight * bytesPerTexel ];

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

			glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

			glGenTextures( 1, &texture );
			if( texture == 0 )
				break;

			glBindTexture( GL_TEXTURE_2D, texture );

			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureBuffer );

			GLenum error = glGetError();
			if( error != GL_NO_ERROR )
			{
				const GLubyte* errorStr = gluErrorString( error );
				errorStr = nullptr;
				break;
			}
		}

		success = true;
	}
	while( false );

	delete[] textureBuffer;

	return success;
}

bool FtaGlyph::Finalize( void )
{
	if( texture != 0 )
		glDeleteTextures( 1, &texture );

	return true;
}

// FtaFontSystem.cpp