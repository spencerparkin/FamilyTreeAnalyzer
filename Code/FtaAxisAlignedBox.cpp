// FtaAxisAlignedBox.cpp

#include "FtaAxisAlignedBox.h"
#include <wx/glcanvas.h>

FtaAxisAlignedBox::FtaAxisAlignedBox( void )
{
}

FtaAxisAlignedBox::FtaAxisAlignedBox( const c3ga::vectorE3GA& min, const c3ga::vectorE3GA max )
{
	SetMin( min );
	SetMax( max );
}

FtaAxisAlignedBox::~FtaAxisAlignedBox( void )
{
}

bool FtaAxisAlignedBox::IsValid( void ) const
{
	if( min.get_e1() > max.get_e1() )
		return true;

	if( min.get_e2() > max.get_e2() )
		return true;

	if( min.get_e3() > max.get_e3() )
		return true;

	return false;
}

bool FtaAxisAlignedBox::IsDegenerate( void ) const
{
	return( GetVolume() == 0.f ? true : false );
}

double FtaAxisAlignedBox::GetVolume( void ) const
{
	return( GetWidth() * GetHeight() * GetDepth() );
}

c3ga::vectorE3GA FtaAxisAlignedBox::GetCenter( void ) const
{
	c3ga::vectorE3GA center;

	center.m_e1 = ( min.m_e1 + max.m_e1 ) / 2.f;
	center.m_e2 = ( min.m_e2 + max.m_e2 ) / 2.f;
	center.m_e3 = ( min.m_e3 + max.m_e3 ) / 2.f;

	return center;
}

void FtaAxisAlignedBox::Combine( const FtaAxisAlignedBox& aab0, const FtaAxisAlignedBox& aab1 )
{
	min.m_e1 = MIN( aab0.min.m_e1, aab1.min.m_e1 );
	min.m_e2 = MIN( aab0.min.m_e2, aab1.min.m_e2 );
	min.m_e3 = MIN( aab0.min.m_e3, aab1.min.m_e3 );

	max.m_e1 = MAX( aab0.max.m_e1, aab1.max.m_e1 );
	max.m_e2 = MAX( aab0.max.m_e2, aab1.max.m_e2 );
	max.m_e3 = MAX( aab0.max.m_e3, aab1.max.m_e3 );
}

void FtaAxisAlignedBox::Intersect( const FtaAxisAlignedBox& aab0, const FtaAxisAlignedBox& aab1 )
{
	min.m_e1 = MAX( aab0.min.m_e1, aab1.min.m_e1 );
	min.m_e2 = MAX( aab0.min.m_e2, aab1.min.m_e2 );
	min.m_e3 = MAX( aab0.min.m_e3, aab1.min.m_e3 );

	max.m_e1 = MIN( aab0.max.m_e1, aab1.max.m_e1 );
	max.m_e2 = MIN( aab0.max.m_e2, aab1.max.m_e2 );
	max.m_e3 = MIN( aab0.max.m_e3, aab1.max.m_e3 );
}

void FtaAxisAlignedBox::Scale( double scale )
{
	c3ga::vectorE3GA center = GetCenter();

	double halfWidth = GetWidth() * scale / 2.f;
	double halfHeight = GetHeight() * scale / 2.f;
	double halfDepth = GetDepth() * scale / 2.f;

	min.m_e1 = center.m_e1 - halfWidth;
	max.m_e1 = center.m_e1 + halfWidth;

	min.m_e2 = center.m_e2 - halfHeight;
	max.m_e2 = center.m_e2 + halfHeight;

	min.m_e3 = center.m_e3 - halfDepth;
	min.m_e3 = center.m_e3 + halfDepth;
}

bool FtaAxisAlignedBox::ContainsPoint( const c3ga::vectorE3GA& point, bool compact /*= true*/, double eps /*= 0.f*/ ) const
{
	if( compact )
	{
		if( !( min.m_e1 - eps <= point.m_e1 && point.m_e1 <= max.m_e1 + eps ) )
			return false;

		if( !( min.m_e2 - eps <= point.m_e2 && point.m_e2 <= max.m_e2 + eps ) )
			return false;

		if( !( min.m_e3 - eps <= point.m_e3 && point.m_e3 <= max.m_e3 + eps ) )
			return false;

		return true;
	}

	if( !( min.m_e1 - eps < point.m_e1 && point.m_e1 < max.m_e1 + eps ) )
		return false;

	if( !( min.m_e2 - eps < point.m_e2 && point.m_e2 < max.m_e2 + eps ) )
		return false;

	if( !( min.m_e3 - eps < point.m_e3 && point.m_e3 < max.m_e3 + eps ) )
		return false;

	return true;
}

int FtaAxisAlignedBox::IntersectWithLineSegment( const c3ga::vectorE3GA& point0, const c3ga::vectorE3GA& point1, c3ga::vectorE3GA* intersectionPoints /*= nullptr*/ ) const
{
	// TODO: Use conformal model outer product to compute intersection points.
	return 0;
}

bool FtaAxisAlignedBox::DivertLineSegment( const c3ga::vectorE3GA& point0, const c3ga::vectorE3GA& point1, c3ga::vectorE3GA& point ) const
{
	// TODO: Should be able to use conformal model to get orthogonal distances to line.
	return false;
}

void FtaAxisAlignedBox::Draw( bool wireFrame ) const
{
	if( wireFrame )
	{
		glBegin( GL_LINES );

		// X edges

		glVertex3f( min.m_e1, min.m_e2, min.m_e3 );
		glVertex3f( max.m_e1, min.m_e2, min.m_e3 );

		glVertex3f( min.m_e1, max.m_e2, min.m_e3 );
		glVertex3f( max.m_e1, max.m_e2, min.m_e3 );

		glVertex3f( min.m_e1, max.m_e2, max.m_e3 );
		glVertex3f( max.m_e1, max.m_e2, max.m_e3 );

		glVertex3f( min.m_e1, min.m_e2, max.m_e3 );
		glVertex3f( max.m_e1, min.m_e2, max.m_e3 );

		// Y edges

		glVertex3f( min.m_e1, min.m_e2, min.m_e3 );
		glVertex3f( min.m_e1, max.m_e2, min.m_e3 );

		glVertex3f( max.m_e1, min.m_e2, min.m_e3 );
		glVertex3f( max.m_e1, max.m_e2, min.m_e3 );

		glVertex3f( max.m_e1, min.m_e2, max.m_e3 );
		glVertex3f( max.m_e1, max.m_e2, max.m_e3 );

		glVertex3f( min.m_e1, min.m_e2, max.m_e3 );
		glVertex3f( min.m_e1, max.m_e2, max.m_e3 );

		// Z edges

		glVertex3f( min.m_e1, min.m_e2, min.m_e3 );
		glVertex3f( min.m_e1, min.m_e2, max.m_e3 );

		glVertex3f( max.m_e1, min.m_e2, min.m_e3 );
		glVertex3f( max.m_e1, min.m_e2, max.m_e3 );

		glVertex3f( max.m_e1, max.m_e2, min.m_e3 );
		glVertex3f( max.m_e1, max.m_e2, max.m_e3 );

		glVertex3f( min.m_e1, max.m_e2, min.m_e3 );
		glVertex3f( min.m_e1, max.m_e2, max.m_e3 );

		glEnd();
	}
	else
	{
		// TODO: Supply lighting normals and texture coordinates.

		glBegin( GL_QUADS );

		// -X face
		glVertex3f( min.m_e1, min.m_e2, min.m_e3 );
		glVertex3f( min.m_e1, min.m_e2, max.m_e3 );
		glVertex3f( min.m_e1, max.m_e2, max.m_e3 );
		glVertex3f( min.m_e1, max.m_e2, min.m_e3 );

		// +X face
		glVertex3f( max.m_e1, max.m_e2, min.m_e3 );
		glVertex3f( max.m_e1, max.m_e2, max.m_e3 );
		glVertex3f( max.m_e1, min.m_e2, max.m_e3 );
		glVertex3f( max.m_e1, min.m_e2, min.m_e3 );

		// -Y face
		glVertex3f( min.m_e1, min.m_e2, min.m_e3 );
		glVertex3f( max.m_e1, min.m_e2, min.m_e3 );
		glVertex3f( max.m_e1, min.m_e2, max.m_e3 );
		glVertex3f( min.m_e1, min.m_e2, max.m_e3 );

		// +Y face
		glVertex3f( min.m_e1, max.m_e2, max.m_e3 );
		glVertex3f( max.m_e1, max.m_e2, max.m_e3 );
		glVertex3f( max.m_e1, max.m_e2, min.m_e3 );
		glVertex3f( min.m_e1, max.m_e2, min.m_e3 );

		// -Z face
		glVertex3f( min.m_e1, max.m_e2, min.m_e3 );
		glVertex3f( max.m_e1, max.m_e2, min.m_e3 );
		glVertex3f( max.m_e1, min.m_e2, min.m_e3 );
		glVertex3f( min.m_e1, min.m_e2, min.m_e3 );

		// +Z face
		glVertex3f( min.m_e1, min.m_e2, max.m_e3 );
		glVertex3f( max.m_e1, min.m_e2, max.m_e3 );
		glVertex3f( max.m_e1, max.m_e2, max.m_e3 );
		glVertex3f( min.m_e1, max.m_e2, max.m_e3 );

		glEnd();
	}
}

// FtaAxisAlignedBox.cpp