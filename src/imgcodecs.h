/***********************************************************************/
/*
/*   Script File: imgcodecs.h
/*
/*   Description:
/*
/*   Definition of imgcodecs module, basicly for image read/write
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

#ifndef _OPENZL_IMGCODECS_H_
#define _OPENZL_IMGCODECS_H_

#include "core.hpp"
#include <vector>

//////////////////////////////// image codec ////////////////////////////////
namespace zl
{
	// enums
	enum {
		IMREAD_UNCHANGED = -1, // 8bit, color or not
		IMREAD_GRAYSCALE = 0,  // 8bit, gray
		IMREAD_COLOR = 1,  // ?, color
		IMREAD_ANYDEPTH = 2,  // any depth, ?
		IMREAD_ANYCOLOR = 4,  // ?, any color
		IMREAD_LOAD_GDAL = 8   // Use gdal driver
	};

	enum {
		IMWRITE_JPEG_QUALITY = 1,
		IMWRITE_JPEG_PROGRESSIVE = 2,
		IMWRITE_JPEG_OPTIMIZE = 3,
		IMWRITE_JPEG_RST_INTERVAL = 4,
		IMWRITE_JPEG_LUMA_QUALITY = 5,
		IMWRITE_JPEG_CHROMA_QUALITY = 6,
		IMWRITE_PNG_COMPRESSION = 16,
		IMWRITE_PNG_STRATEGY = 17,
		IMWRITE_PNG_BILEVEL = 18,
		IMWRITE_PXM_BINARY = 32,
		IMWRITE_WEBP_QUALITY = 64
	};

	enum {
		IMWRITE_PNG_STRATEGY_DEFAULT = 0,
		IMWRITE_PNG_STRATEGY_FILTERED = 1,
		IMWRITE_PNG_STRATEGY_HUFFMAN_ONLY = 2,
		IMWRITE_PNG_STRATEGY_RLE = 3,
		IMWRITE_PNG_STRATEGY_FIXED = 4
	};

	// load image into Mat, currently support 8-bit bmp, jpg 
	Mat imread(const String& filename, int flags = IMREAD_COLOR);
	// write Mat image to file, currently support 8-bit bmp, jpg
	bool imwrite(const String& filename, Mat& img);

	///////////////////////////////// base class for decoders ////////////////////////
	class BaseImageDecoder
	{
	public:
		BaseImageDecoder();
		virtual ~BaseImageDecoder() {}

		int width() const { return m_width; }
		int height() const { return m_height; }
		virtual int type() const { return m_type; }

		virtual bool setSource(const String& filename);
		virtual bool setSource(const Mat& buf);
		virtual bool readHeader() = 0;
		virtual bool readData(Mat& img) = 0;

		virtual size_t signatureLength() const;
		virtual bool checkSignature(const String& signature) const;
		//virtual ImageDecoder newDecoder() const;

	protected:
		int  m_width;  // width  of the image ( filled by readHeader )
		int  m_height; // height of the image ( filled by readHeader )
		int  m_type;
		String m_filename;
		String m_signature;
		Mat m_buf;
		bool m_buf_supported;
	};


	///////////////////////////// base class for encoders ////////////////////////////
	class BaseImageEncoder
	{
	public:
		BaseImageEncoder();
		virtual ~BaseImageEncoder() {}
		virtual bool isFormatSupported(int depth) const;

		virtual bool setDestination(const String& filename);
		virtual bool setDestination(std::vector<uchar>& buf);
		virtual bool write(const Mat& img, const std::vector<int>& params) = 0;

		virtual String getDescription() const;
		//virtual ImageEncoder newEncoder() const;

		virtual void throwOnEror() const;

	protected:
		String m_description;

		String m_filename;
		std::vector<uchar>* m_buf;
		bool m_buf_supported;

		String m_last_error;
	};
}

#endif