// FtaAxisAlignedBox.h

#pragma once

#include "c3ga/c3ga.h"

#ifndef MIN
#	define MIN(a,b)		((a) < (b) ? (a) : (b))
#endif

#ifndef MAX
#	define MAX(a,b)		((a) > (b) ? (a) : (b))
#endif

class FtaAxisAlignedBox
{
public:

	FtaAxisAlignedBox( void );
	FtaAxisAlignedBox( const c3ga::vectorE3GA& min, const c3ga::vectorE3GA max );
	~FtaAxisAlignedBox( void );

	void SetMin( const c3ga::vectorE3GA& min ) { this->min = min; }
	void SetMax( const c3ga::vectorE3GA& max ) { this->max = max; }

	const c3ga::vectorE3GA& GetMin( void ) const { return min; }
	const c3ga::vectorE3GA& GetMax( void ) const { return max; }

	// Tell us if min <= max.
	bool IsValid( void ) const;

	// Tell us if the box has zero volume.
	bool IsDegenerate( void ) const;

	// Compute and return the volume of the box.  This may be negative in some cases.
	double GetVolume( void ) const;

	// Compute the dimensions of the box.
	double GetWidth( void ) const { return max.get_e1() - min.get_e1(); }
	double GetHeight( void ) const { return max.get_e2() - min.get_e2(); }
	double GetDepth( void ) const { return max.get_e3() - min.get_e3(); }

	// Compute and return the point most inside the box.
	c3ga::vectorE3GA GetCenter( void ) const;

	// Make this box the smallest possible box containing the two given boxes.
	// The result is undefined if either of the two given boxes is invalid.
	// Either of the two given boxes may be this box.
	void Combine( const FtaAxisAlignedBox& aab0, const FtaAxisAlignedBox& aab1 );

	// Make this box the intersection of the two given boxes.  If either of the two
	// given boxes is invalid, the result is undefind.  If the two given valid boxes
	// have no intersection, then an invalid box is computed.  Either of the two given
	// boxes may be this box.
	void Intersect( const FtaAxisAlignedBox& aab0, const FtaAxisAlignedBox& aab1 );

	// Scale this box about its center by the given scalar.  If the scalar is 2, then
	// the edge-lengths of the box are doubled.  If 1/2, they're halved, and so on.
	void Scale( double scale );

	// Tell us if the given point is inside the box.
	bool ContainsPoint( const c3ga::vectorE3GA& point, bool compact = true, double eps = 0.f ) const;

	// Return the number of intersection points between the given line segment and this box.
	// If an array is given, then it must be large enough to contain at least two points.
	int IntersectWithLineSegment( const c3ga::vectorE3GA& point0, const c3ga::vectorE3GA& point1, c3ga::vectorE3GA* intersectionPoints = nullptr ) const;

	// Given a line-segment whose end-points are assumed to be outside this box, here
	// we compute and return a point closest to the line of the line-segment that is
	// on the boundary of this box.  If such a point would be on the line, false is returned.
	bool DivertLineSegment( const c3ga::vectorE3GA& point0, const c3ga::vectorE3GA& point1, c3ga::vectorE3GA& point ) const;

	// Draw this box as 6 quads.  Winding order is such that back-face culling can be used.
	// If the box is invalid, the result is undefined.
	void Draw( bool wireFrame ) const;

public:

	c3ga::vectorE3GA min, max;
};

// FtaAxisAlignedBox.h