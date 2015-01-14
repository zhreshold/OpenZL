/***********************************************************************/
/*
/#   Script File: core.hpp
/#
/#   Description:
/#
/#   Definition of core global modules
/#
*/
/*!	 \mainpage OpenZL Main Page
#	 OpenZL - Open EZ Library

### OpenZL is a light weighted portable library packed with fundamental functions for Computer Vision projects.

### Wrapped in C++ as itself lacks image IO libraries, meanwhile provide more convenience over C.


     Author: Joshua Zhang (zzbhf@mail.missouri.edu)
     Date: Dec-2014
  
     Copyright(C) <2014>  Joshua Zhang	 - All Rights Reserved.
  
     This software is available for non-commercial use only.
  	 Under MIT license.
  	 No warrenty implied, use at your own risk.
*/
/***********************************************************************/




#ifndef _OPENZL_CORE_HPP_
#define _OPENZL_CORE_HPP_

#ifndef __cplusplus
#  error core.hpp header must be compiled as C++
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1400
#define _CRT_SECURE_NO_WARNINGS // suppress warnings about fopen() and similar "unsafe" functions defined by MS
#endif

#include <climits>
#include <cstdint>
#include <string>
#include <sstream>
#include <string.h>
#include <iostream>
#include <ctime>
#include <algorithm>


//#include "realtime.h"
//#include "waitkey.h"

//////////////////////////////// Typedef ////////////////////////////////
/// fixed bits integer, guaranteed!
typedef uint8_t			uchar;		//!< unsigned 8-bit integer
typedef int8_t			schar;		//!< signed 8-bit integer
typedef uint16_t		ushort;		//!< unsigned 16-bit integer
typedef int16_t			sshort;		//!< signed 16-bit integer
typedef uint32_t        uint;		//!< unsigned 32-bit integer
typedef int32_t			sint;		//!< signed 32-bit integer
typedef uint64_t		ulong;		//!< unsigned 64-bit integer
typedef int64_t			slong;		//!< signed 64-bit integer
typedef std::string		String;		//!< STL string

/// <summary>
/// namespace zl
/// </summary>
/// \namespace zl where all the functionality resides 
namespace zl
{
	//! \enum channel depth (currently not implemented)
	enum
	{
		ZL_8U = 8,
		ZL_8UC1,
		ZL_8UC3,
		ZL_8UC4,
		ZL_16U = 16,
		ZL_16UC1,
		ZL_16UC3,
		ZL_16UC4,
		ZL_32U = 32,
		ZL_32UC1,
		ZL_32UC3,
		ZL_32UC4,
	};

	//! \enum color convertion code
	enum
	{
		ZL_RGB2GRAY = 0,
		ZL_RGBA2GRAY,
		ZL_GRAY2RGB,
	};

	//////////////////////////////// Utility functions ////////////////////////////////
	void error(String err);
	void warning(String warn);
	int waitkey(double ms = 0);
	void hold_screen();

	/// <summary>
	/// Return the abosolute value
	/// </summary>
	/// <param name="v">The input value.</param>
	/// <returns>Abosolute value</returns>
	template<class T> inline const T abs(const T& v) { return (v < 0) ? -v : v; }

	/// <summary>
	/// Return the smaller value
	/// </summary>
	/// <param name="a">a.</param>
	/// <param name="b">b.</param>
	/// <returns>Smaller value</returns>
	template<class T> inline const T min(const T& a, const T& b) { return (b < a) ? b : a; }

	/// <summary>
	/// Return the larger value
	/// </summary>
	/// <param name="a">a.</param>
	/// <param name="b">b.</param>
	/// <returns>Larger value</returns>
	template<class T> inline const T max(const T& a, const T& b) { return (b > a) ? b : a; }

	/// <summary>
	/// Lock the specified value inside range [low, high]
	/// </summary>
	/// <param name="value">The value.</param>
	/// <param name="low">The low threshold.</param>
	/// <param name="high">The high threshold.</param>
	/// <returns>Original or low threshold or high threshold</returns>
	template<class T> inline const T lock_in(const T& value, const T& low, const T& high)
	{ 
		T h = max(low, high);
		T l = min(low, high);
		return max(min(value, h), l);
	}

	/// <summary>
	/// Print the message, start a new line
	/// </summary>
	/// <param name="logMsg">The log message to print.</param>
	template<typename T> inline
		void println(T t)
	{
			std::ostringstream oss; // create a stream
			oss << t; // insert value to stream
			std::cout << oss.str() << std::endl;
		}

	/// <summary>
	/// Print the message with various arguments, start a new line
	/// </summary>
	/// <param name="logMsg">The log message to print.</param>
	template <typename T, typename ...U>
	void println(T t, U ...u)
	{
		std::cout << t;
		println(u...);
	}


	//////////////////////////////// Timer ////////////////////////////////
	/*! \class
	Cross-platform Timer class

	The class is used to capture the elapsed time between two timestamps.
	Mainly for complexity evaluation purposes.

	*/
	class Timer
	{
	public:
		// default constructor, will call update() to record current timestamp.
		Timer();
		~Timer();

		// update current timestamp
		void update();

		// get elapsed time
		double get_elapsed_time_s();
		double get_elapsed_time_ms();
		double get_elapsed_time_us();

	private:

		double timestamp;
	};

	//////////////////////////////// Scalar_ ////////////////////////////////
	/*! \class
	\brief Simple template Scalar_ class

	Mainly used for convenient pixel access
	*/
	template<typename _Tp> class Scalar_
	{
	public:
		//! default constructor that will set all channels to 0
		Scalar_(){ v[0] = 0; v[1] = 0; v[2] = 0; v[3] = 0; };
		//! frequently used constructor that set first 3 channels seperately
		Scalar_(_Tp r, _Tp g = 0, _Tp b = 0)
		{
			v[0] = r; v[1] = g; v[2] = b; v[3] = 0;
		};
		//! one additional channel for transparency.
		Scalar_(_Tp r, _Tp g, _Tp b, _Tp alpha)
		{
			v[0] = r; v[1] = g; v[2] = b; v[3] = alpha;
		};

		//! operator (n) retrieve the nth element in Scalar_
		_Tp& operator () (int n) { return v[n]; };

	private:
		_Tp v[4];
	};

	typedef Scalar_<double> Scalar;		///< default use double precision

	//////////////////////////////// Point_ ////////////////////////////////
	/*! \class
	template 2D point class.

	The class defines a point in 2D space. Data type of the point coordinates is specified
	as a template parameter. There are a few shorter aliases available for user convenience.
	See zl::Point, zl::Point2i, zl::Point2f and zl::Point2d.
	*/
	template<typename _Tp> class Point_
	{
	public:
		typedef _Tp value_type;

		// various constructors
		Point_();
		Point_(_Tp _x, _Tp _y);
		Point_(const Point_& pt);

		Point_& operator = (const Point_& pt);

		//! conversion to another data type
		template<typename _Tp2> operator Point_<_Tp2>() const;

		//! dot product
		_Tp dot(const Point_& pt) const;
		//! dot product computed in double-precision arithmetics
		double ddot(const Point_& pt) const;
		//! cross-product
		double cross(const Point_& pt) const;
		////! checks whether the point is inside the specified rectangle
		//bool inside(const Rect_<_Tp>& r) const;

		_Tp x, y; //< the point coordinates
	};

	typedef Point_<int> Point2i;
	typedef Point_<float> Point2f;
	typedef Point_<double> Point2d;
	typedef Point2i Point;

	//////////////////////////////// Size_ ////////////////////////////////

	/*! \class
	The 2D size class

	The class represents the size of a 2D rectangle, image size, matrix size etc.
	Normally, zl::Size ~ zl::Size_<int> is used.
	*/
	template<typename _Tp> class Size_
	{
	public:
		typedef _Tp value_type;

		//! various constructors
		Size_();
		Size_(_Tp _width, _Tp _height);
		Size_(const Size_& sz);

		Size_& operator = (const Size_& sz);
		//! the area (width*height)
		_Tp area() const;

		//! conversion of another data type.
		template<typename _Tp2> operator Size_<_Tp2>() const;

		_Tp width, height; // the width and the height
	};

	typedef Size_<int> Size2i;
	typedef Size_<float> Size2f;
	typedef Size_<double> Size2d;
	typedef Size2i Size;

	//////////////////////////////// Rect_ ////////////////////////////////

	/*! \class
	The 2D up-right rectangle class

	The class represents a 2D rectangle with coordinates of the specified data type.
	Normally, zl::Rect ~ zl::Rect_<int> is used.
	*/
	template<typename _Tp> class Rect_
	{
	public:
		typedef _Tp value_type;

		//! various constructors
		Rect_();
		Rect_(_Tp _x, _Tp _y, _Tp _width, _Tp _height);
		Rect_(const Rect_& r);
		Rect_(const Point_<_Tp>& org, const Size_<_Tp>& sz);
		Rect_(const Point_<_Tp>& pt1, const Point_<_Tp>& pt2);

		Rect_& operator = (const Rect_& r);
		//! the top-left corner
		Point_<_Tp> tl() const;
		//! the bottom-right corner
		Point_<_Tp> br() const;

		//! size (width, height) of the rectangle
		Size_<_Tp> size() const;
		//! area (width*height) of the rectangle
		_Tp area() const;

		//! conversion to another data type
		template<typename _Tp2> operator Rect_<_Tp2>() const;

		//! checks whether the rectangle contains the point
		bool contains(const Point_<_Tp>& pt) const;

		_Tp x, y, width, height; //< the top-left corner, as well as width and height of the rectangle
	};

	typedef Rect_<int> Rect2i;
	typedef Rect_<float> Rect2f;
	typedef Rect_<double> Rect2d;
	typedef Rect2i Rect;


	//////////////////////////////// Mat_ ////////////////////////////////

	/*! \class
	template 2D Matrix class.

	The class defines a matrix in 2D space. Data type is specified as a template parameter. 
	There are a few shorter aliases available for user convenience.
	See zl::Mat, zl::Matf, zl::Matd and zl::Mati.
	Normally, zl::Mat ~ zl::Mat_<uchar> is used as the common 8-bit image container.

	About stored pixel values, the pixel values are stored interleaved (R1G1B1R2G2B2...)
	*/
	template<typename _Tp> class Mat_
	{
	public:
		typedef _Tp value_type;

		// various constructors
		//! default constructor, create an empty matrix
		Mat_();
		//! create a matrix according to the input size
		Mat_(int nrows, int ncols, int nchannels);
		//! create a matrix from user allocated memory
		// this function need to be called using caucious
		// make sure dataBuf contains at least (nrows * ncols * nchannels) elements
		Mat_(int nrows, int ncols, int nchannels, _Tp* dataBuf);
		//! copy constructor, deep copy inside
		Mat_(const Mat_& other);

		//! destructor
		virtual ~Mat_();

		//! overriden methods for element access
		virtual _Tp& operator () (int row, int col, int channel = 0);

		//! overload = operator to be deep copy, reasonable since reference couting was not implemented
		virtual Mat_& operator = (Mat_& r);

		// real construction inside
		virtual bool create(int nrow, int ncol, int nchannel = 1);

		// real destruction inside
		void release();

		// check if matrix is empty
		bool empty() const;

		// return buffer ptr
		_Tp* ptr(int i = 0) const;
		_Tp* ptr(int row, int col) const;

		// deep copy function
		//void copy_from(const Mat_& other) ;

		// swap
		void swap(Mat_& other);

		// get flags
		int flags() const;

		// get n_rows
		int rows() const;

		// get n_cols
		int cols() const;

		// get n_channels
		int channels() const;

		// get step
		int step() const;

		// set all elements to value 
		void set_all(_Tp value);

		// put pixel from Scalar
		void set(int row, int col, Scalar value, bool ignore = false);

		// export pixel value to de-interleaved buffer, make sure the buffer is large enough!
		// void export_deinterleave(_Tp* outBuf);

		// dump elements to std::cout
		void dump() const;

	protected:
		// flags
		int m_flags;
		// rows, cols
		int m_rows;
		int m_cols;
		// channels support up to 4? RGBa
		int m_channels;
		// steps
		int m_step;

		// data container, be caucious when handling it directly
		_Tp* data;
	};

	typedef Mat_<uchar>		Matu8;
	typedef Mat_<ushort>	Matu16;
	typedef Mat_<sint>		Mati;
	typedef Mat_<float>		Matf;
	typedef Mat_<double>	Matd;
	typedef Matu8			Mat;


	//////////////////////////////// Vec2_ ////////////////////////////////
	/*! \class
	template 2D Vector class.

	The class defines a 2-d vector derived from Mat_. Data type is specified as a template parameter.
	More specifically, Vec2_<_Tp> is the special case of Mat_<_Tp> that m_channels = 1;
	There are a few shorter aliases available for user convenience.
	See zl::Vec2, zl::Vec2f, zl::Vec2d and zl::Vec2i.
	Normally, zl::Vec2 ~ zl::Vec2_<double> is used as the general floating point vector container.

	*/
	template<typename _Tp> class Vec2_: public Mat_<_Tp>
	{
	public:
		//! default constructor, create an empty Vec2_
		Vec2_();
		//! create a Vec2_ according to the input size
		Vec2_(int nrows, int ncols);
		//! create a Vec2_ from user allocated memory
		// this function need to be called using caucious
		// make sure dataBuf contains at least (nrows * ncols) elements
		Vec2_(int nrows, int ncols, _Tp* dataBuf);

		

		//! overriden methods for element access
		_Tp& operator () (int row, int col);

		//! overload = operator to be deep copy, reasonable since reference couting was not implemented
		Vec2_<_Tp>& operator = (Mat_<_Tp>& r);

		// real construction inside
		bool create(int nrow, int ncol);

	private:


	};

	typedef Vec2_<uchar>	Vec2u8;
	typedef Vec2_<ushort>	Vec2u16;
	typedef Vec2_<sint>		Vec2i;
	typedef Vec2_<float>	Vec2f;
	typedef Vec2_<double>	Vec2d;
	typedef Vec2d			Vec2;

	/////////////////////////////////////////////////////////////////////////
	///////////////////////////// Implementation ////////////////////////////
	/////////////////////////////////////////////////////////////////////////

	/// <summary>
	/// inline fast round function
	/// </summary>
	/// <param name="value">The floating value.</param>
	/// <returns>The nearest interger value.</returns>
	inline int fast_round(double value)
	{
#if ((defined _MSC_VER && defined _M_X64) || (defined __GNUC__ && defined __x86_64__ && defined __SSE2__ && !defined __APPLE__)) && !defined(__CUDACC__)
		__m128d t = _mm_set_sd(value);
		return _mm_cvtsd_si32(t);
#elif defined _MSC_VER && defined _M_IX86
		int t;
		__asm
		{
			fld value;
			fistp t;
		}
		return t;
#elif defined _MSC_VER && defined _M_ARM && defined HAVE_TEGRA_OPTIMIZATION
		TEGRA_ROUND(value);
#elif defined CV_ICC || defined __GNUC__
#  ifdef HAVE_TEGRA_OPTIMIZATION
		TEGRA_ROUND(value);
#  else
		return (int)lrint(value);
#  endif
#else
		double intpart, fractpart;
		fractpart = modf(value, &intpart);
		if ((fabs(fractpart) != 0.5) || ((((int)intpart) % 2) != 0))
			return (int)(value + (value >= 0 ? 0.5 : -0.5));
		else
			return (int)intpart;
#endif
	}

	//! \cond
	/////////////// saturate_cast (used in image & signal processing) ///////////////////

	template<typename _Tp> static inline _Tp saturate_cast(uchar v)    { return _Tp(v); }
	template<typename _Tp> static inline _Tp saturate_cast(schar v)    { return _Tp(v); }
	template<typename _Tp> static inline _Tp saturate_cast(ushort v)   { return _Tp(v); }
	template<typename _Tp> static inline _Tp saturate_cast(short v)    { return _Tp(v); }
	template<typename _Tp> static inline _Tp saturate_cast(unsigned v) { return _Tp(v); }
	template<typename _Tp> static inline _Tp saturate_cast(int v)      { return _Tp(v); }
	template<typename _Tp> static inline _Tp saturate_cast(float v)    { return _Tp(v); }
	template<typename _Tp> static inline _Tp saturate_cast(double v)   { return _Tp(v); }

	template<> inline uchar saturate_cast<uchar>(schar v)        { return (uchar)std::max((int)v, 0); }
	template<> inline uchar saturate_cast<uchar>(ushort v)       { return (uchar)std::min((unsigned)v, (unsigned)UCHAR_MAX); }
	template<> inline uchar saturate_cast<uchar>(int v)          { return (uchar)((unsigned)v <= UCHAR_MAX ? v : v > 0 ? UCHAR_MAX : 0); }
	template<> inline uchar saturate_cast<uchar>(short v)        { return saturate_cast<uchar>((int)v); }
	template<> inline uchar saturate_cast<uchar>(unsigned v)     { return (uchar)std::min(v, (unsigned)UCHAR_MAX); }
	template<> inline uchar saturate_cast<uchar>(float v)        { int iv = fast_round(v); return saturate_cast<uchar>(iv); }
	template<> inline uchar saturate_cast<uchar>(double v)       { int iv = fast_round(v); return saturate_cast<uchar>(iv); }

	template<> inline schar saturate_cast<schar>(uchar v)        { return (schar)std::min((int)v, SCHAR_MAX); }
	template<> inline schar saturate_cast<schar>(ushort v)       { return (schar)std::min((unsigned)v, (unsigned)SCHAR_MAX); }
	template<> inline schar saturate_cast<schar>(int v)          { return (schar)((unsigned)(v - SCHAR_MIN) <= (unsigned)UCHAR_MAX ? v : v > 0 ? SCHAR_MAX : SCHAR_MIN); }
	template<> inline schar saturate_cast<schar>(short v)        { return saturate_cast<schar>((int)v); }
	template<> inline schar saturate_cast<schar>(unsigned v)     { return (schar)std::min(v, (unsigned)SCHAR_MAX); }
	template<> inline schar saturate_cast<schar>(float v)        { int iv = fast_round(v); return saturate_cast<schar>(iv); }
	template<> inline schar saturate_cast<schar>(double v)       { int iv = fast_round(v); return saturate_cast<schar>(iv); }

	template<> inline ushort saturate_cast<ushort>(schar v)      { return (ushort)std::max((int)v, 0); }
	template<> inline ushort saturate_cast<ushort>(short v)      { return (ushort)std::max((int)v, 0); }
	template<> inline ushort saturate_cast<ushort>(int v)        { return (ushort)((unsigned)v <= (unsigned)USHRT_MAX ? v : v > 0 ? USHRT_MAX : 0); }
	template<> inline ushort saturate_cast<ushort>(unsigned v)   { return (ushort)std::min(v, (unsigned)USHRT_MAX); }
	template<> inline ushort saturate_cast<ushort>(float v)      { int iv = fast_round(v); return saturate_cast<ushort>(iv); }
	template<> inline ushort saturate_cast<ushort>(double v)     { int iv = fast_round(v); return saturate_cast<ushort>(iv); }

	template<> inline short saturate_cast<short>(ushort v)       { return (short)std::min((int)v, SHRT_MAX); }
	template<> inline short saturate_cast<short>(int v)          { return (short)((unsigned)(v - SHRT_MIN) <= (unsigned)USHRT_MAX ? v : v > 0 ? SHRT_MAX : SHRT_MIN); }
	template<> inline short saturate_cast<short>(unsigned v)     { return (short)std::min(v, (unsigned)SHRT_MAX); }
	template<> inline short saturate_cast<short>(float v)        { int iv = fast_round(v); return saturate_cast<short>(iv); }
	template<> inline short saturate_cast<short>(double v)       { int iv = fast_round(v); return saturate_cast<short>(iv); }

	template<> inline int saturate_cast<int>(float v)            { return fast_round(v); }
	template<> inline int saturate_cast<int>(double v)           { return fast_round(v); }

	// we intentionally do not clip negative numbers, to make -1 become 0xffffffff etc.
	template<> inline unsigned saturate_cast<unsigned>(float v)  { return fast_round(v); }
	template<> inline unsigned saturate_cast<unsigned>(double v) { return fast_round(v); }


	//////////////////////////////// 2D Point ///////////////////////////////
	template<typename _Tp> inline
		Point_<_Tp>::Point_()
		: x(0), y(0) {}

	template<typename _Tp> inline
		Point_<_Tp>::Point_(_Tp _x, _Tp _y)
		: x(_x), y(_y) {}

	template<typename _Tp> inline
		Point_<_Tp>::Point_(const Point_& pt)
		: x(pt.x), y(pt.y) {}

	/// operator override
	template<typename _Tp> inline
		Point_<_Tp>& Point_<_Tp>::operator = (const Point_& pt)
	{
			x = pt.x; y = pt.y;
			return *this;
		}

	template<typename _Tp> template<typename _Tp2> inline
		Point_<_Tp>::operator Point_<_Tp2>() const
	{
			return Point_<_Tp2>(saturate_cast<_Tp2>(x), saturate_cast<_Tp2>(y));
		}

	template<typename _Tp> static inline
		Point_<_Tp>& operator += (Point_<_Tp>& a, const Point_<_Tp>& b)
	{
			a.x += b.x;
			a.y += b.y;
			return a;
		}

	template<typename _Tp> static inline
		Point_<_Tp>& operator -= (Point_<_Tp>& a, const Point_<_Tp>& b)
	{
			a.x -= b.x;
			a.y -= b.y;
			return a;
		}

	template<typename _Tp> static inline
		Point_<_Tp>& operator *= (Point_<_Tp>& a, int b)
	{
			a.x = saturate_cast<_Tp>(a.x * b);
			a.y = saturate_cast<_Tp>(a.y * b);
			return a;
		}

	template<typename _Tp> static inline
		Point_<_Tp>& operator *= (Point_<_Tp>& a, float b)
	{
			a.x = saturate_cast<_Tp>(a.x * b);
			a.y = saturate_cast<_Tp>(a.y * b);
			return a;
		}

	template<typename _Tp> static inline
		Point_<_Tp>& operator *= (Point_<_Tp>& a, double b)
	{
			a.x = saturate_cast<_Tp>(a.x * b);
			a.y = saturate_cast<_Tp>(a.y * b);
			return a;
		}

	template<typename _Tp> static inline
		Point_<_Tp>& operator /= (Point_<_Tp>& a, int b)
	{
			a.x = saturate_cast<_Tp>(a.x / b);
			a.y = saturate_cast<_Tp>(a.y / b);
			return a;
		}

	template<typename _Tp> static inline
		Point_<_Tp>& operator /= (Point_<_Tp>& a, float b)
	{
			a.x = saturate_cast<_Tp>(a.x / b);
			a.y = saturate_cast<_Tp>(a.y / b);
			return a;
		}

	template<typename _Tp> static inline
		Point_<_Tp>& operator /= (Point_<_Tp>& a, double b)
	{
			a.x = saturate_cast<_Tp>(a.x / b);
			a.y = saturate_cast<_Tp>(a.y / b);
			return a;
		}

	template<typename _Tp> static inline
		bool operator == (const Point_<_Tp>& a, const Point_<_Tp>& b)
	{
			return a.x == b.x && a.y == b.y;
		}

	template<typename _Tp> static inline
		bool operator != (const Point_<_Tp>& a, const Point_<_Tp>& b)
	{
			return a.x != b.x || a.y != b.y;
		}

	template<typename _Tp> static inline
		Point_<_Tp> operator + (const Point_<_Tp>& a, const Point_<_Tp>& b)
	{
			return Point_<_Tp>(saturate_cast<_Tp>(a.x + b.x), saturate_cast<_Tp>(a.y + b.y));
		}

	template<typename _Tp> static inline
		Point_<_Tp> operator - (const Point_<_Tp>& a, const Point_<_Tp>& b)
	{
			return Point_<_Tp>(saturate_cast<_Tp>(a.x - b.x), saturate_cast<_Tp>(a.y - b.y));
		}

	template<typename _Tp> static inline
		Point_<_Tp> operator - (const Point_<_Tp>& a)
	{
			return Point_<_Tp>(saturate_cast<_Tp>(-a.x), saturate_cast<_Tp>(-a.y));
		}

	template<typename _Tp> static inline
		Point_<_Tp> operator * (const Point_<_Tp>& a, int b)
	{
			return Point_<_Tp>(saturate_cast<_Tp>(a.x*b), saturate_cast<_Tp>(a.y*b));
		}

	template<typename _Tp> static inline
		Point_<_Tp> operator * (int a, const Point_<_Tp>& b)
	{
			return Point_<_Tp>(saturate_cast<_Tp>(b.x*a), saturate_cast<_Tp>(b.y*a));
		}

	template<typename _Tp> static inline
		Point_<_Tp> operator * (const Point_<_Tp>& a, float b)
	{
			return Point_<_Tp>(saturate_cast<_Tp>(a.x*b), saturate_cast<_Tp>(a.y*b));
		}

	template<typename _Tp> static inline
		Point_<_Tp> operator * (float a, const Point_<_Tp>& b)
	{
			return Point_<_Tp>(saturate_cast<_Tp>(b.x*a), saturate_cast<_Tp>(b.y*a));
		}

	template<typename _Tp> static inline
		Point_<_Tp> operator * (const Point_<_Tp>& a, double b)
	{
			return Point_<_Tp>(saturate_cast<_Tp>(a.x*b), saturate_cast<_Tp>(a.y*b));
		}

	template<typename _Tp> static inline
		Point_<_Tp> operator * (double a, const Point_<_Tp>& b)
	{
			return Point_<_Tp>(saturate_cast<_Tp>(b.x*a), saturate_cast<_Tp>(b.y*a));
		}


	template<typename _Tp> static inline
		Point_<_Tp> operator / (const Point_<_Tp>& a, int b)
	{
			Point_<_Tp> tmp(a);
			tmp /= b;
			return tmp;
		}

	template<typename _Tp> static inline
		Point_<_Tp> operator / (const Point_<_Tp>& a, float b)
	{
			Point_<_Tp> tmp(a);
			tmp /= b;
			return tmp;
		}

	template<typename _Tp> static inline
		Point_<_Tp> operator / (const Point_<_Tp>& a, double b)
	{
			Point_<_Tp> tmp(a);
			tmp /= b;
			return tmp;
		}

	template<typename _Tp> inline
		_Tp Point_<_Tp>::dot(const Point_& pt) const
	{
			return saturate_cast<_Tp>(x*pt.x + y*pt.y);
		}

	template<typename _Tp> inline
		double Point_<_Tp>::ddot(const Point_& pt) const
	{
			return (double)x*pt.x + (double)y*pt.y;
		}

	template<typename _Tp> inline
		double Point_<_Tp>::cross(const Point_& pt) const
	{
			return (double)x*pt.y - (double)y*pt.x;
		}

	//template<typename _Tp> inline bool
	//	Point_<_Tp>::inside(const Rect_<_Tp>& r) const
	//{
	//		return r.contains(*this);
	//	}

	////////////////////////////////// Size /////////////////////////////////

	template<typename _Tp> inline
		Size_<_Tp>::Size_()
		: width(0), height(0) {}

	template<typename _Tp> inline
		Size_<_Tp>::Size_(_Tp _width, _Tp _height)
		: width(_width), height(_height) {}

	template<typename _Tp> inline
		Size_<_Tp>::Size_(const Size_& sz)
		: width(sz.width), height(sz.height) {}

	template<typename _Tp> template<typename _Tp2> inline
		Size_<_Tp>::operator Size_<_Tp2>() const
	{
			return Size_<_Tp2>(saturate_cast<_Tp2>(width), saturate_cast<_Tp2>(height));
		}

	template<typename _Tp> inline
		Size_<_Tp>& Size_<_Tp>::operator = (const Size_<_Tp>& sz)
	{
			width = sz.width; height = sz.height;
			return *this;
		}

	template<typename _Tp> inline
		_Tp Size_<_Tp>::area() const
	{
			return width * height;
		}

	template<typename _Tp> static inline
		Size_<_Tp>& operator *= (Size_<_Tp>& a, _Tp b)
	{
			a.width *= b;
			a.height *= b;
			return a;
		}

	template<typename _Tp> static inline
		Size_<_Tp> operator * (const Size_<_Tp>& a, _Tp b)
	{
			Size_<_Tp> tmp(a);
			tmp *= b;
			return tmp;
		}

	template<typename _Tp> static inline
		Size_<_Tp>& operator /= (Size_<_Tp>& a, _Tp b)
	{
			a.width /= b;
			a.height /= b;
			return a;
		}

	template<typename _Tp> static inline
		Size_<_Tp> operator / (const Size_<_Tp>& a, _Tp b)
	{
			Size_<_Tp> tmp(a);
			tmp /= b;
			return tmp;
		}

	template<typename _Tp> static inline
		Size_<_Tp>& operator += (Size_<_Tp>& a, const Size_<_Tp>& b)
	{
			a.width += b.width;
			a.height += b.height;
			return a;
		}

	template<typename _Tp> static inline
		Size_<_Tp> operator + (const Size_<_Tp>& a, const Size_<_Tp>& b)
	{
			Size_<_Tp> tmp(a);
			tmp += b;
			return tmp;
		}

	template<typename _Tp> static inline
		Size_<_Tp>& operator -= (Size_<_Tp>& a, const Size_<_Tp>& b)
	{
			a.width -= b.width;
			a.height -= b.height;
			return a;
		}

	template<typename _Tp> static inline
		Size_<_Tp> operator - (const Size_<_Tp>& a, const Size_<_Tp>& b)
	{
			Size_<_Tp> tmp(a);
			tmp -= b;
			return tmp;
		}

	template<typename _Tp> static inline
		bool operator == (const Size_<_Tp>& a, const Size_<_Tp>& b)
	{
			return a.width == b.width && a.height == b.height;
		}

	template<typename _Tp> static inline
		bool operator != (const Size_<_Tp>& a, const Size_<_Tp>& b)
	{
			return !(a == b);
		}



	////////////////////////////////// Rect /////////////////////////////////

	template<typename _Tp> inline
		Rect_<_Tp>::Rect_()
		: x(0), y(0), width(0), height(0) {}

	template<typename _Tp> inline
		Rect_<_Tp>::Rect_(_Tp _x, _Tp _y, _Tp _width, _Tp _height)
		: x(_x), y(_y), width(_width), height(_height) {}

	template<typename _Tp> inline
		Rect_<_Tp>::Rect_(const Rect_<_Tp>& r)
		: x(r.x), y(r.y), width(r.width), height(r.height) {}

	template<typename _Tp> inline
		Rect_<_Tp>::Rect_(const Point_<_Tp>& org, const Size_<_Tp>& sz)
		: x(org.x), y(org.y), width(sz.width), height(sz.height) {}

	template<typename _Tp> inline
		Rect_<_Tp>::Rect_(const Point_<_Tp>& pt1, const Point_<_Tp>& pt2)
	{
			x = std::min(pt1.x, pt2.x);
			y = std::min(pt1.y, pt2.y);
			width = std::max(pt1.x, pt2.x) - x;
			height = std::max(pt1.y, pt2.y) - y;
		}

	template<typename _Tp> inline
		Rect_<_Tp>& Rect_<_Tp>::operator = (const Rect_<_Tp>& r)
	{
			x = r.x;
			y = r.y;
			width = r.width;
			height = r.height;
			return *this;
		}

	template<typename _Tp> inline
		Point_<_Tp> Rect_<_Tp>::tl() const
	{
			return Point_<_Tp>(x, y);
		}

	template<typename _Tp> inline
		Point_<_Tp> Rect_<_Tp>::br() const
	{
			return Point_<_Tp>(x + width, y + height);
		}

	template<typename _Tp> inline
		Size_<_Tp> Rect_<_Tp>::size() const
	{
			return Size_<_Tp>(width, height);
		}

	template<typename _Tp> inline
		_Tp Rect_<_Tp>::area() const
	{
			return width * height;
		}

	template<typename _Tp> template<typename _Tp2> inline
		Rect_<_Tp>::operator Rect_<_Tp2>() const
	{
			return Rect_<_Tp2>(saturate_cast<_Tp2>(x), saturate_cast<_Tp2>(y), saturate_cast<_Tp2>(width), saturate_cast<_Tp2>(height));
		}

	template<typename _Tp> inline
		bool Rect_<_Tp>::contains(const Point_<_Tp>& pt) const
	{
			return x <= pt.x && pt.x < x + width && y <= pt.y && pt.y < y + height;
		}


	template<typename _Tp> static inline
		Rect_<_Tp>& operator += (Rect_<_Tp>& a, const Point_<_Tp>& b)
	{
			a.x += b.x;
			a.y += b.y;
			return a;
		}

	template<typename _Tp> static inline
		Rect_<_Tp>& operator -= (Rect_<_Tp>& a, const Point_<_Tp>& b)
	{
			a.x -= b.x;
			a.y -= b.y;
			return a;
		}

	template<typename _Tp> static inline
		Rect_<_Tp>& operator += (Rect_<_Tp>& a, const Size_<_Tp>& b)
	{
			a.width += b.width;
			a.height += b.height;
			return a;
		}

	template<typename _Tp> static inline
		Rect_<_Tp>& operator -= (Rect_<_Tp>& a, const Size_<_Tp>& b)
	{
			a.width -= b.width;
			a.height -= b.height;
			return a;
		}

	template<typename _Tp> static inline
		Rect_<_Tp>& operator &= (Rect_<_Tp>& a, const Rect_<_Tp>& b)
	{
			_Tp x1 = std::max(a.x, b.x);
			_Tp y1 = std::max(a.y, b.y);
			a.width = std::min(a.x + a.width, b.x + b.width) - x1;
			a.height = std::min(a.y + a.height, b.y + b.height) - y1;
			a.x = x1;
			a.y = y1;
			if (a.width <= 0 || a.height <= 0)
				a = Rect();
			return a;
		}

	template<typename _Tp> static inline
		Rect_<_Tp>& operator |= (Rect_<_Tp>& a, const Rect_<_Tp>& b)
	{
			_Tp x1 = std::min(a.x, b.x);
			_Tp y1 = std::min(a.y, b.y);
			a.width = std::max(a.x + a.width, b.x + b.width) - x1;
			a.height = std::max(a.y + a.height, b.y + b.height) - y1;
			a.x = x1;
			a.y = y1;
			return a;
		}

	template<typename _Tp> static inline
		bool operator == (const Rect_<_Tp>& a, const Rect_<_Tp>& b)
	{
			return a.x == b.x && a.y == b.y && a.width == b.width && a.height == b.height;
		}

	template<typename _Tp> static inline
		bool operator != (const Rect_<_Tp>& a, const Rect_<_Tp>& b)
	{
			return a.x != b.x || a.y != b.y || a.width != b.width || a.height != b.height;
		}

	template<typename _Tp> static inline
		Rect_<_Tp> operator + (const Rect_<_Tp>& a, const Point_<_Tp>& b)
	{
			return Rect_<_Tp>(a.x + b.x, a.y + b.y, a.width, a.height);
		}

	template<typename _Tp> static inline
		Rect_<_Tp> operator - (const Rect_<_Tp>& a, const Point_<_Tp>& b)
	{
			return Rect_<_Tp>(a.x - b.x, a.y - b.y, a.width, a.height);
		}

	template<typename _Tp> static inline
		Rect_<_Tp> operator + (const Rect_<_Tp>& a, const Size_<_Tp>& b)
	{
			return Rect_<_Tp>(a.x, a.y, a.width + b.width, a.height + b.height);
		}

	template<typename _Tp> static inline
		Rect_<_Tp> operator & (const Rect_<_Tp>& a, const Rect_<_Tp>& b)
	{
			Rect_<_Tp> c = a;
			return c &= b;
		}

	template<typename _Tp> static inline
		Rect_<_Tp> operator | (const Rect_<_Tp>& a, const Rect_<_Tp>& b)
	{
			Rect_<_Tp> c = a;
			return c |= b;
		}

	////////////////////////////////// Mat /////////////////////////////////

	template<typename _Tp> inline
		Mat_<_Tp>::Mat_()
		: m_flags(0), m_rows(0), m_cols(0), m_channels(0), m_step(0), data(NULL){}

	template<typename _Tp> inline
		Mat_<_Tp>::Mat_(int nrow, int ncol, int nchannel)
	{
			create(nrow, ncol, nchannel);
		}

	template<typename _Tp> inline
		Mat_<_Tp>::Mat_(int nrow, int ncol, int nchannel, _Tp* dataBuf)
	{
			if (create(nrow, ncol, nchannel))
			{
				// doing deep copy rather than copy pointer
				memcpy(data, dataBuf, m_rows * m_step * sizeof(_Tp));
			}
		}

	template<typename _Tp> inline
		Mat_<_Tp>::Mat_(const Mat_& other)
	{
			if (!other.empty())
			{
				create(other.rows(), other.cols(), other.channels());
				memcpy(data, other.data, sizeof(_Tp)* m_rows * m_step);
				m_flags = other.flags();
			}
			else
			{
				warning("Copy constructor from an empty Mat!");
				m_flags = 0;
				m_rows = 0;
				m_cols = 0;
				m_channels = 0;
				m_step = 0;
				data = NULL;
			}
		}

	template<typename _Tp> inline
		Mat_<_Tp>::~Mat_()
	{
			release();
		}


	template<typename _Tp> inline
		Mat_<_Tp>& Mat_<_Tp>::operator = (Mat_<_Tp>& r)
	{
			Mat_<_Tp>(r).swap(*this);
			return *this;
		}

	//@ \endcond

	/// <summary>
	/// Create an instance given specific size
	/// </summary>
	/// <param name="nrow">The num of rows.</param>
	/// <param name="ncol">The num of cols.</param>
	/// <param name="nchannel">The num of channels.</param>
	/// <returns>True if success, false otherwise</returns>
	template<typename _Tp> inline
		bool Mat_<_Tp>::create(int nrow, int ncol, int nchannel)
	{
			// check arguments
			if (nrow < 1 || ncol < 1 || nchannel < 1)
			{
				warning("Mat dimension not valid! Empty Mat created.");
				m_flags = 0;
				m_rows = 0;
				m_cols = 0;
				m_channels = 0;
				m_step = 0;
				data = NULL;
				return 0;
			}

			// if already existed, release first
			if ((m_flags & 0x01) == 1)
			{
				release();
			}

			m_flags = 1;
			m_rows = nrow;
			m_cols = ncol;
			m_channels = nchannel;
			m_step = m_cols * m_channels;
			data = new _Tp[m_rows * m_step];

			if (data == NULL)
			{
				error("Bad allocation when creating Mat!");
			}

			return 1;
		}

	/// <summary>
	/// Release this instance.
	/// </summary>
	template<typename _Tp> inline
		void Mat_<_Tp>::release()
	{
			if (data != NULL)
			{
				delete [] data;
				data = NULL;
			}

			m_flags = 0;
			m_rows = 0;
			m_cols = 0;
			m_channels = 0;
			m_step = 0;
			
		}

	//! \cond
	/*
	/// <summary>
	/// Deep copy from the specified Mat_.
	/// </summary>
	/// <param name="r">The Mat_ to copy from.</param>
	template<typename _Tp> inline
		void Mat_<_Tp>::copy_from(const Mat_<_Tp>& other)
	{
			release();
			create(other.rows(), other.cols(), other.channels());
			memcpy(data, other.data, sizeof(_Tp)* m_rows * m_step);
			m_flags = other.flags();
		}
		*/

	template<typename _Tp> inline
		void Mat_<_Tp>::swap(Mat_<_Tp>& other)
	{
			std::swap(other.m_flags, m_flags);
			std::swap(other.m_rows, m_rows);
			std::swap(other.m_cols, m_cols);
			std::swap(other.m_channels, m_channels);
			std::swap(other.m_step, m_step);
			std::swap(other.data, data);
		}

	//! \endcond

	/// <summary>
	/// Operator() to access the specified element in Mat
	/// </summary>
	/// <param name="row">The row.</param>
	/// <param name="col">The col.</param>
	/// <param name="channel">The channel.</param>
	/// <returns>The reference to the specified element</returns>
	template<typename _Tp> inline
		_Tp& Mat_<_Tp>::operator () (const int row, const int col, const int channel)
	{
			if (row < 0 || row >= m_rows || col < 0 || col >= m_cols || channel < 0 || channel >= m_channels)
			{
				error("Indices out of range!");
			}

			return data[row * m_step + col * m_channels + channel];
		}

	/// <summary>
	/// Return the pointer to the specified element i in 1-D order.
	/// </summary>
	/// <param name="i">The index of element.</param>
	/// <returns>Pointer to this element</returns>
	template<typename _Tp> inline
		_Tp* Mat_<_Tp>::ptr(int i) const
	{
			if (i < 0 || i > m_rows * m_step)
			{
				error("Indices out of range!");
			}
			return data + i;
		}

	/// <summary>
	/// Return the pointer to the specified element (row, col) in 2-D order.
	/// </summary>
	/// <param name="row">The row.</param>
	/// <param name="col">The col.</param>
	/// <returns>Pointer to this element</returns>
	template<typename _Tp> inline
		_Tp* Mat_<_Tp>::ptr(int row, int col) const
	{
			if (row < 0 || row > m_rows || col < 0 || col > m_cols)
			{
				error("Indices out of range!");
			}
			return data + row * m_step + col * m_channels;
		}

	/// <summary>
	/// Check if Mat is empty.
	/// </summary>
	/// <returns>True if empty, otherwise false</returns>
	template<typename _Tp> inline
		bool Mat_<_Tp>::empty() const
	{
			if ((m_flags & 0x01) == 0 || m_rows < 1 || m_cols < 1 || m_channels < 1 || data == NULL)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}

	/// <summary>
	/// Return flags
	/// </summary>
	/// <returns>Flags</returns>
	template<typename _Tp> inline
		int Mat_<_Tp>::flags() const
	{
			return m_flags;
		}

	/// <summary>
	/// Return number of rows
	/// </summary>
	/// <returns>Number of rows</returns>
	template<typename _Tp> inline
		int Mat_<_Tp>::rows() const
	{
			return m_rows;
		}

	/// <summary>
	/// Return number of columns
	/// </summary>
	/// <returns>Number of columns</returns>
	template<typename _Tp> inline
		int Mat_<_Tp>::cols() const
	{
			return m_cols;
		}

	/// <summary>
	/// Return number of channels
	/// </summary>
	/// <returns>Number of channels</returns>
	template<typename _Tp> inline
		int Mat_<_Tp>::channels() const
	{
			return m_channels;
		}

	/// <summary>
	/// Return step size
	/// </summary>
	/// <returns>Step size which is m_cols * m_channels</returns>
	template<typename _Tp> inline
		int Mat_<_Tp>::step() const
	{
			return m_step;
		}

	/// <summary>
	/// Set all elements to the specific value
	/// </summary>
	template<typename _Tp> inline
		void Mat_<_Tp>::set_all(_Tp value)
	{
			if (empty())
			{
				return;
			}

			for (int i = 0; i < m_rows * m_step; i++)
			{
				data[i] = value;
			}
		}

	/// <summary>
	/// Set specified element by value
	/// </summary>
	/// <param name="row">The row.</param>
	/// <param name="col">The column.</param>
	/// <param name="value">The value.</param>
	/// <param name="ignore">if set to <c>true</c> ignore any error, be careful with this option!.</param>
	template<typename _Tp> inline
		void Mat_<_Tp>::set(int row, int col, Scalar value, bool ignore)
	{
			if (empty())
			{
				if (ignore)
				{
					return;
				}
				error("Mat is empty!");
			}

			if (row < 0 || row > m_rows || col < 0 || col > m_cols)
			{
				if (ignore)
				{
					return;
				}
				error("Indices out of range!");
			}

			_Tp* p = this->ptr(row, col);
			
			for (int i = 0; (i < m_channels) && (i < 4); i++)
			{
				p[i] = saturate_cast<_Tp>(value(i));
			}
			
		}

	/// <summary>
	/// Dumps all elements to std::cout.
	/// </summary>
	template<typename _Tp> inline
		void Mat_<_Tp>::dump() const
	{
			if (empty())
			{
				return;
			}
			else
			{
				std::cout << "[ ";
				for (int i = 0; i < m_rows; i++)
				{
					for (int j = 0; j < m_cols; j++)
					{
						for (int k = 0; k < m_channels; k++)
						{
							std::cout << data[i * m_step + j * m_channels + k] << " ";
						}
						std::cout << "; ";
					}
					std::cout << std::endl;
				}
				std::cout << "]" << std::endl;
			}
		}


	////////////////////////////////// Vec2 /////////////////////////////////

	template<typename _Tp> inline
		Vec2_<_Tp>::Vec2_(){}

	template<typename _Tp> inline
		Vec2_<_Tp>::Vec2_(int nrow, int ncol)
	{
			create(nrow, ncol);
		}

	template<typename _Tp> inline
		Vec2_<_Tp>::Vec2_(int nrow, int ncol, _Tp* dataBuf)
	{
			if (create(nrow, ncol))
			{
				// doing deep copy rather than copy pointer
				memcpy(data, dataBuf, m_rows * m_step * sizeof(_Tp));
			}
		}


	/// <summary>
	/// Create an instance given specific size
	/// </summary>
	/// <param name="nrow">The num of rows.</param>
	/// <param name="ncol">The num of cols.</param>
	/// <returns>True if success, false otherwise</returns>
	template<typename _Tp> inline
		bool Vec2_<_Tp>::create(int nrow, int ncol)
	{
			return Mat_<_Tp>::create(nrow, ncol, 1);
		}

	/// <summary>
	/// Operator() to access the specified element in Mat
	/// </summary>
	/// <param name="row">The row.</param>
	/// <param name="col">The col.</param>
	/// <returns>The reference to the specified element</returns>
	template<typename _Tp> inline
		_Tp& Vec2_<_Tp>::operator () (const int row, const int col)
	{
			return Mat_<_Tp>(row, col, 0);
		}

	template<typename _Tp> inline
		Vec2_<_Tp>& Vec2_<_Tp>::operator = (Mat_<_Tp>& r)
	{
			if (r.channels() != 1)
			{
				warning("Invalid channels for converting from Mat to Vec2, only 1 channel allowed!");
				return *this;
			}
			
			Mat_<_Tp>(r).swap(*this);
			return *this;
		}

	///////////////////////////// Utility functions ////////////////////////////////////
	/// <summary>
	/// Convert the color space of given image.
	/// </summary>
	/// <param name="src">The src image.</param>
	/// <param name="dst">The dst image.</param>
	/// <param name="code">The convertion code.</param>
	/// <returns>True if success, false otherwise</returns>
	template<typename _Tp> inline
		bool cvt_color(Mat_<_Tp>& src, Mat_<_Tp>& dst, int code)
	{
		if (src.empty())
		{
			warning("src image is empty!");
			return 0;
		}

		Mat_<_Tp> buffer;

		switch (code)
		{
		case ZL_RGB2GRAY:
			if (src.channels() != 3)
			{
				warning("invalid src image channels corrspond to code: RGB2GRAY!");
				return 0;
			}
			buffer.create(src.rows(), src.cols(), 1);
			for (int row = 0; row < src.rows(); row++)
			{
				for (int col = 0; col < src.cols(); col++)
				{
					Mat_<_Tp>::value_type r = src(row, col, 0);
					Mat_<_Tp>::value_type g = src(row, col, 1);
					Mat_<_Tp>::value_type b = src(row, col, 2);

					Mat_<_Tp>::value_type v = static_cast<Mat_<_Tp>::value_type>(0.299 * r + 0.587 * g + 0.114 * b);
					buffer(row, col) = v;
				}
			}
			break;
		case ZL_RGBA2GRAY:
			if (src.channels() != 4)
			{
				warning("invalid src image channels corrspond to code: RGBA2GRAY!");
				return 0;
			}
			buffer.create(src.rows(), src.cols(), 1);
			for (int row = 0; row < src.rows(); row++)
			{
				for (int col = 0; col < src.cols(); col++)
				{
					Mat_<_Tp>::value_type r = src(row, col, 0);
					Mat_<_Tp>::value_type g = src(row, col, 1);
					Mat_<_Tp>::value_type b = src(row, col, 2);

					Mat_<_Tp>::value_type v = static_cast<Mat::value_type>(0.299 * r + 0.587 * g + 0.114 * b);
					buffer(row, col) = v;
				}
			}
			break;
		case ZL_GRAY2RGB:
			if (src.channels() != 1)
			{
				warning("invalid src image channels corrspond to code: RGB2GRAY!");
				return 0;
			}
			buffer.create(src.rows(), src.cols(), 3);
			for (int row = 0; row < src.rows(); row++)
			{
				for (int col = 0; col < src.cols(); col++)
				{
					Mat_<_Tp>::value_type v = src(row, col);
					buffer(row, col, 0) = v;
					buffer(row, col, 1) = v;
					buffer(row, col, 2) = v;
				}
			}
			break;
		default:
			warning("Unrecognized code!");
			return 0;
		}

		dst = buffer;
		return 1;
	}






	/*
	* Author:  David Robert Nadeau
	* Site:    http://NadeauSoftware.com/
	* License: Creative Commons Attribution 3.0 Unported License
	*          http://creativecommons.org/licenses/by/3.0/deed.en_US
	*/

#ifndef _OPENZL_REALTIME_H_
#define _OPENZL_REALTIME_H_

	/*
	* Returns the real time, in seconds, or -1.0 if an error occurred.
	*
	* Time is measured since an arbitrary and OS-dependent start time.
	* The returned real time is only useful for computing an elapsed time
	* between two calls to this function.
	*/
	//double get_real_time();

	/*
	* Wrapped elapsed time function for convenience
	*/
	// get elapsed time in second
	//double get_elapsed_time_s(double timeStamp);
	// get elapsed time in ms
	//double get_elapsed_time_ms(double timeStamp);
	// get elapsed time in us
	//double get_elapsed_time_us(double timeStamp);




#endif

#ifndef _OPENZL_WAITKEY_H
#define _OPENZL_WAITKEY_H


	// return the first key stroke from keyboard
	int get_key();
	int kb_hit();



#endif
}

#endif


