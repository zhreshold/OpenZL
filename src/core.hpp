/***********************************************************************/
/*
/*   Script File: core.hpp
/*
/*   Description:
/*
/*   Definition of core global modules
/*
/*
/*   Author: Joshua Zhang (zzbhf@mail.missouri.edu)
/*   Date: Dec-2014
/*
/*   Copyright(C) 2014  Joshua Zhang	 - All Rights Reserved.
/*
/*   This software is available for non-commercial use only. It must
/*   not be modified and distributed without prior permission of the
/*   author. The author is not responsible for implications from the
/*   use of this software.
/*
/***********************************************************************/



#ifndef _OPENZL_CORE_HPP_
#define _OPENZL_CORE_HPP_

#ifndef __cplusplus
#  error core.hpp header must be compiled as C++
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1400
#define _CRT_SECURE_NO_WARNINGS // suppress warnings about fopen() and similar "unsafe" functions defined by MS
#endif


#include <cstdint>
#include <string>
#include <iostream>
#include <ctime>


//#include "realtime.h"
//#include "waitkey.h"

//////////////////////////////// Typedef ////////////////////////////////
/// fixed bits integer, guaranteed!
typedef uint8_t			uchar;		//!< unsigned 8-bit integer
typedef int8_t			Char;		//!< signed 8-bit integer
typedef uint16_t		ushort;		//!< unsigned 16-bit integer
typedef int16_t			Short;		//!< signed 16-bit integer
typedef uint32_t        uint;		//!< unsigned 32-bit integer
typedef int32_t			Int;		//!< signed 32-bit integer
typedef uint64_t		ulong;		//!< unsigned 64-bit integer
typedef int64_t			Long;		//!< signed 64-bit integer
typedef std::string		String;		//!< STL string

/// <summary>
/// namespace zl
/// </summary>
/// *! \namespace zl where all the functionality resides */
namespace zl
{
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

	// color convertion code
	enum
	{
		ZL_RGB2GRAY = 0,
		ZL_RGBA2GRAY,
		ZL_GRAY2RGB,
	};

	void error(String err);
	void warning(String warn);
	int waitkey(double ms = 0);
	void hold_screen();

	//////////////////////////////// Point_ ////////////////////////////////
	/*!
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

	/*!
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

	/*!
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

	/*!
	template 2D Matrix class.

	The class defines a matrix in 2D space. Data type is specified as a template parameter. 
	There are a few shorter aliases available for user convenience.
	See zl::Mat, zl::Matf, zl::Matd and zl::Mati.
	Normally, zl::Mat ~ zl::Mat_<uchar> is used as the common 8-bit image container.
	*/
	template<typename _Tp> class Mat_
	{
	public:
		typedef _Tp value_type;

		// various constructors
		// ! default constructor, create an empty matrix
		Mat_();
		// ! create a matrix according to the input size
		Mat_(int nrows, int ncols, int nchannels);
		// ! create a matrix from user allocated memory
		// this function need to be called using caucious
		// make sure dataBuf contains at least (nrows * ncols * nchannels) elements
		Mat_(int nrows, int ncols, int nchannels, _Tp* dataBuf);

		// destructor
		~Mat_();

		// overriden methods for element access
		_Tp& operator () (int row, int col, int channel = 0);

		// overload = operator
		Mat_& operator = (const Mat_& r);

		// real construction inside
		bool create(int nrow, int ncol, int nchannel);
		// real destruction inside
		void release();
		// check if matrix is empty
		bool empty();
		// return buffer ptr
		_Tp* ptr(int i = 0);
		_Tp* ptr(int row, int col);
		// get n_rows
		int rows();
		// get n_cols
		int cols();
		// get n_channels
		int channels();
		// get step
		int step();
		// dump elements to std::cout
		void dump();

	private:
		// flags
		int flags;
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
	typedef Mat_<Int>		Mati;
	typedef Mat_<float>		Matf;
	typedef Mat_<double>	Matd;
	typedef Matu8			Mat;


	/////////////////////////////////////////////////////////////////////////
	bool cvtColor(Mat& src, Mat& dst, int code);


	/////////////////////////////////////////////////////////////////////////
	///////////////////////////// Implementation ////////////////////////////
	/////////////////////////////////////////////////////////////////////////


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
		: flags(0), m_rows(0), m_cols(0), m_step(0), m_channels(0), data(nullptr){}

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
		Mat_<_Tp>::~Mat_()
	{
			release();
		}


	template<typename _Tp> inline
		Mat_<_Tp>& Mat_<_Tp>::operator = (const Mat_<_Tp>& r)
	{
			release();
			flags = r.flags;
			m_rows = r.m_rows;
			m_cols = r.m_cols;
			m_channels = r.m_channels;
			m_step = r.m_step;
			data = new _Tp[m_rows * m_step];
			memcpy(data, r.data, sizeof(_Tp)* m_rows * m_step);
			return *this;
		}

	/// <summary>
	/// Creates the specified nrow.
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
				return 0;
			}

			// if already existed, release first
			if ((flags & 0x01) == 1)
			{
				release();
			}

			flags = 1;
			m_rows = nrow;
			m_cols = ncol;
			m_channels = nchannel;
			m_step = m_cols * m_channels;
			data = new _Tp[m_rows * m_step];

			return 1;
		}

	/// <summary>
	/// Releases this instance.
	/// </summary>
	template<typename _Tp> inline
		void Mat_<_Tp>::release()
	{
			if (data != nullptr)
			{
				delete [] data;
				data = nullptr;
			}

			flags = 0;
			m_rows = 0;
			m_cols = 0;
			m_channels = 0;
			m_step = 0;
			
		}

	/// <summary>
	/// Operator()s access the specified element in Mat
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
		_Tp* Mat_<_Tp>::ptr(int i)
	{
			return data + i;
		}

	/// <summary>
	/// Return the pointer to the specified element (row, col) in 2-D order.
	/// </summary>
	/// <param name="row">The row.</param>
	/// <param name="col">The col.</param>
	/// <returns>Pointer to this element</returns>
	template<typename _Tp> inline
		_Tp* Mat_<_Tp>::ptr(int row, int col)
	{
			return data + row * m_step + col * m_channels;
		}

	/// <summary>
	/// Check if Mat is empty.
	/// </summary>
	/// <returns>True if empty, otherwise false</returns>
	template<typename _Tp> inline
		bool Mat_<_Tp>::empty()
	{
			if ((flags & 0x01) == 0 || m_rows < 1 || m_cols < 1 || m_channels < 1 || data == nullptr)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}

	template<typename _Tp> inline
		int Mat_<_Tp>::rows()
	{
			return m_rows;
		}

	template<typename _Tp> inline
		int Mat_<_Tp>::cols()
	{
			return m_cols;
		}

	template<typename _Tp> inline
		int Mat_<_Tp>::channels()
	{
			return m_channels;
		}

	template<typename _Tp> inline
		int Mat_<_Tp>::step()
	{
			return m_step;
		}

	/// <summary>
	/// Dumps all elements to std::cout.
	/// </summary>
	template<typename _Tp> inline
		void Mat_<_Tp>::dump()
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

	///////////////////////////// Utility functions ////////////////////////////////////
	/*
	* Author:  David Robert Nadeau
	* Site:    http://NadeauSoftware.com/
	* License: Creative Commons Attribution 3.0 Unported License
	*          http://creativecommons.org/licenses/by/3.0/deed.en_US
	*/

#ifndef _OPENZL_REALTIME_H_
#define _OPENZL_REALTIME_H_


	/**
	* Returns the real time, in seconds, or -1.0 if an error occurred.
	*
	* Time is measured since an arbitrary and OS-dependent start time.
	* The returned real time is only useful for computing an elapsed time
	* between two calls to this function.
	*/
	double get_real_time();

	/**
	* Wrapped elapsed time function for convenience
	* Added by Zhi
	*/
	// get elapsed time in second
	double get_elapsed_time_s(double timeStamp);
	// get elapsed time in ms
	double get_elapsed_time_ms(double timeStamp);
	// get elapsed time in us
	double get_elapsed_time_us(double timeStamp);




#endif

#ifndef _OPENZL_WAITKEY_H
#define _OPENZL_WAITKEY_H


	// return the first key stroke from keyboard
	int get_key();
	int kb_hit();



#endif
}

#endif
