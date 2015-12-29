// FtaFontSystem.cpp

#include "FtaFontSystem.h"
#include "FtaApp.h"
#include FT_MODULE_H
#include FT_TRUETYPE_IDS_H

// TODO: Find and plug mem-leak.

FtaFontSystem::FtaFontSystem( void )
{
	initialized = false;
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

bool FtaFontSystem::DrawText( GLfloat x, GLfloat y,
								const wxString& text,
								const wxString& font,
								GLfloat wrapLength,
								Justification justification )
{
	bool success = false;

	do
	{
		if( !initialized )
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

		if( !cachedFont->DrawText( x, y, text, wrapLength, justification ) )
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

		error = FT_Set_Char_Size( face, 0, 16*64, 300, 300 );
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

/*virtual*/ bool FtaFont::DrawText( GLfloat x, GLfloat y,
									const wxString& text,
									GLfloat wrapLength,
									FtaFontSystem::Justification justification )
{
	bool success = false;

	do
	{
		// TODO: We actually need to push/pop our own projection/modelview matrices here.
		//       Even if we track pixels per unit, the camera may not by viewing the XY-plane.

		glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

		const wchar_t* charCode = text.wc_str();

		LineOfText lineOfText;
		GLfloat baseLine = 0.f;
		
		do
		{
			if( !EatLineOfText( lineOfText, charCode, baseLine, wrapLength, justification ) )
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

	return success;
}

bool FtaFont::EatLineOfText( LineOfText& lineOfText, const wchar_t*& charCode, GLfloat& baseLine, GLfloat wrapLength, FtaFontSystem::Justification justification )
{
	lineOfText.clear();

	GLfloat x = 0.f;
	GLfloat y = baseLine;

	while( *charCode != '\0' )
	{
		FtaGlyphMap::iterator iter = glyphMap.find( FT_ULong( *charCode ) );
		if( iter == glyphMap.end() )
			return false;

		FtaGlyph* glyph = iter->second;

		GlyphRender glyphRender;
		glyphRender.glyph = glyph;
		glyphRender.x = x;
		glyphRender.y = y;

		lineOfText.push_back( glyphRender );

		x += 2.f;		// Total hack for now.

		// TODO: Know when we need to stop eating due to wrap length.

		charCode++;
	}

	// TODO: Make another pass, this type formatting by given justification.

	return true;
}

bool FtaFont::RenderLineOfText( const LineOfText& lineOfText )
{
	LineOfText::const_iterator iter = lineOfText.begin();
	while( iter != lineOfText.end() )
	{
		const GlyphRender& glyphRender = *iter;
		glRasterPos2f( glyphRender.x, glyphRender.y );
		glCallList( glyphRender.glyph->GetDisplayList() );
		iter++;
	}

	return true;
}

FtaGlyph::FtaGlyph( void )
{
	displayList = 0;
}

/*virtual*/ FtaGlyph::~FtaGlyph( void )
{
	Finalize();
}

bool FtaGlyph::Initialize( FT_GlyphSlot& glyphSlot )
{
	bool success = false;
	GLubyte* imageBuffer = nullptr;

	do
	{
		displayList = glGenLists(1);
		if( displayList == 0 )
			break;

		FT_Bitmap& bitmap = glyphSlot->bitmap;
		if( bitmap.pixel_mode != FT_PIXEL_MODE_GRAY )
			break;

		if( bitmap.pitch != bitmap.width )
			break;

		GLubyte* imageBuffer = new GLubyte[ bitmap.width * bitmap.rows ];
		if( !imageBuffer )
			break;

		GLubyte* bitmapBuffer = ( GLubyte* )bitmap.buffer;

		// Flip the image for OpenGL.
		for( int i = 0; i < bitmap.rows; i++ )
			for( int j = 0; j < bitmap.width; j++ )
				imageBuffer[ i * bitmap.width + j ] = bitmapBuffer[ ( bitmap.rows - 1 - i ) * bitmap.width + j ];

		glNewList( displayList, GL_COMPILE );
		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
		glDrawPixels( bitmap.width, bitmap.rows, GL_ALPHA, GL_UNSIGNED_BYTE, imageBuffer );
		glEndList();

		// TODO: Save off advancement information, etc...

		success = true;
	}
	while( false );

	delete[] imageBuffer;

	return success;
}

bool FtaGlyph::Finalize( void )
{
	if( displayList != 0 )
		glDeleteLists( displayList, 1 );

	return true;
}

// FtaFontSystem.cpp