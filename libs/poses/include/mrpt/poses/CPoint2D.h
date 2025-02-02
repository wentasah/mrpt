/* +------------------------------------------------------------------------+
   |                     Mobile Robot Programming Toolkit (MRPT)            |
   |                          https://www.mrpt.org/                         |
   |                                                                        |
   | Copyright (c) 2005-2023, Individual contributors, see AUTHORS file     |
   | See: https://www.mrpt.org/Authors - All rights reserved.               |
   | Released under BSD License. See: https://www.mrpt.org/License          |
   +------------------------------------------------------------------------+ */
#pragma once

#include <mrpt/math/CVectorFixed.h>
#include <mrpt/poses/CPoint.h>

namespace mrpt::poses
{
class CPose2D;

/** A class used to store a 2D point.
 *
 *  For a complete description of Points/Poses, see mrpt::poses::CPoseOrPoint,
 * or refer
 *    to the <a href="http://www.mrpt.org/2D_3D_Geometry" >2D/3D Geometry
 * tutorial</a> in the wiki.
 *
 *  <div align=center>
 *   <img src="CPoint2D.gif">
 *  </div>
 *
 * \sa CPoseOrPoint,CPose, CPoint
 * \ingroup poses_grp
 */
class CPoint2D : public CPoint<CPoint2D, 2>,
				 public mrpt::serialization::CSerializable
{
	DEFINE_SERIALIZABLE(CPoint2D, mrpt::poses)
	DEFINE_SCHEMA_SERIALIZABLE()
   public:
	/** [x,y] */
	mrpt::math::CVectorFixedDouble<2> m_coords;

   public:
	/** Constructor for initializing point coordinates. */
	inline CPoint2D(double x = 0, double y = 0)
	{
		m_coords[0] = x;
		m_coords[1] = y;
	}

	/** Constructor from x/y coordinates given from other pose. */
	template <class OTHERCLASS, std::size_t DIM2>
	inline explicit CPoint2D(const CPoseOrPoint<OTHERCLASS, DIM2>& b)
	{
		m_coords[0] = b.x();
		m_coords[1] = b.y();
	}

	/** Implicit constructor from lightweight type. */
	explicit CPoint2D(const mrpt::math::TPoint2D& o);

	/** Explicit constructor from lightweight type (loses the z coord). */
	explicit CPoint2D(const mrpt::math::TPoint3D& o);

	mrpt::math::TPoint2D asTPoint() const;

	/** Return the pose or point as a 2x1 vector [x, y]' */
	void asVector(vector_t& v) const { v = m_coords; }

	/** The operator D="this"-b is the pose inverse compounding operator,
	 *   the resulting points "D" fulfils: "this" = b + D, so that: b == a +
	 * (b-a)
	 */
	CPoint2D operator-(const CPose2D& b) const;

	enum
	{
		is_3D_val = 0
	};
	static constexpr bool is_3D() { return is_3D_val != 0; }
	enum
	{
		is_PDF_val = 0
	};
	static constexpr bool is_PDF() { return is_PDF_val != 0; }
	/** @name STL-like methods and typedefs
	  @{   */
	/** The type of the elements */
	using value_type = double;
	using reference = double&;
	using const_reference = double;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;

	// size is constant
	enum
	{
		static_size = 2
	};
	static constexpr size_type size() { return static_size; }
	static constexpr bool empty() { return false; }
	static constexpr size_type max_size() { return static_size; }
	static inline void resize(size_t n)
	{
		if (n != static_size)
			throw std::logic_error(format(
				"Try to change the size of CPoint2D to %u.",
				static_cast<unsigned>(n)));
	}
	/** @} */

	void setToNaN() override;

};	// End of class def.

/** Dumps a point as a string (x,y) */
std::ostream& operator<<(std::ostream& o, const CPoint2D& p);

}  // namespace mrpt::poses
