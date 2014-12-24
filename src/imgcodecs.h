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
	};

	enum {
		IMWRITE_UNCHANGED = -1, // 8bit, color or not
		IMWRITE_GRAYSCALE = 0,  // 8bit, gray
		IMWRITE_COLOR = 1,  // ?, color
		IMWRITE_ANYDEPTH = 2,  // any depth, ?
		IMWRITE_ANYCOLOR = 4,  // ?, any color
	};

	//! load image into Mat, currently support various formats
	bool imread(Mat& image, const String& filename, int flags = IMREAD_COLOR);
	//! write Mat image to file, currently support bmp, jpg, png and tga
	// quality option only work for JPEG format, from (0-100)
	bool imwrite(const String& filename, Mat& img, int flags = IMWRITE_COLOR, int quality = 90);


}

#endif