/***********************************************************************/
/*
/*   Script File: imgcodecs.cpp
/*
/*   Description:
/*
/*   Imgcodecs module, basicly for image read/write
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


#include "imgcodecs.h"
#include <iostream>
#include <fstream>
#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "jo_jpeg.h"


namespace zl
{
	enum
	{
		IMREAD_UNSUPPORTED = -1,
		IMREAD_BMP,
		IMREAD_JPEG,
		IMREAD_PNG,
		IMREAD_TGA,
		IMREAD_PSD,
		IMREAD_GIF,
		IMREAD_HDR,
		IMREAD_PIC,
	};

	enum
	{
		IMWRITE_UNSUPPORTED = -1,
		IMWRITE_BMP,
		IMWRITE_JPEG,
		IMWRITE_PNG,
		IMWRITE_TGA,
	};

	int parse_ext_read(String filename)
	{
		if (filename.size() <= 1)
		{
			return IMREAD_UNSUPPORTED;
		}

		const char* _ext = strrchr(filename.c_str(), '.');
		if (!_ext)
		{
			return IMREAD_UNSUPPORTED;
		}

		_ext++;
		String ext(_ext);
		std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

		if (ext == "bmp")
		{
			return IMREAD_BMP;
		}
		else if (ext == "jpg" || ext == "jpeg" || ext == "jpe")
		{
			return IMREAD_JPEG;
		}
		else if (ext == "png")
		{
			return IMREAD_PNG;
		}
		else if (ext == "tga")
		{
			return IMREAD_TGA;
		}
		else if (ext == "psd")
		{
			return IMREAD_PSD;
		}
		else if (ext == "gif")
		{
			return IMREAD_GIF;
		}
		else if (ext == "hdr")
		{
			return IMREAD_HDR;
		}
		else if (ext == "pic")
		{
			return IMREAD_PIC;
		}
		else
		{
			return IMREAD_UNSUPPORTED;
		}

		return IMREAD_UNSUPPORTED;
	}

	int parse_ext_write(String filename)
	{
		if (filename.size() <= 1)
		{
			return IMREAD_UNSUPPORTED;
		}

		const char* _ext = strrchr(filename.c_str(), '.');
		if (!_ext)
		{
			return IMREAD_UNSUPPORTED;
		}

		_ext++;
		String ext(_ext);
		std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

		if (ext == "bmp")
		{
			return IMWRITE_BMP;
		}
		else if (ext == "jpeg" || ext == "jpg" || ext == "jpe")
		{
			return IMWRITE_JPEG;
		}
		else if (ext == "png")
		{
			return IMWRITE_PNG;
		}
		else if (ext == "tga")
		{
			return IMWRITE_TGA;
		}
		else
		{
			return IMREAD_UNSUPPORTED;
		}

		return IMREAD_UNSUPPORTED;
	}


	bool imread_general(Mat& image, const String& filename, int flags)
	{
		int width = 0;
		int height = 0;
		int channels = 0;
		int code = 0;

		uchar* data = stbi_load(filename.c_str(), &width, &height, &channels, code);
		if (data == nullptr || width < 1 || height < 1 || channels < 1)
		{
			warning("load image: " + filename + " failed!");
			return 0;
		}

		image.create(height, width, channels);
		memcpy(image.ptr(), data, sizeof(uchar)* width * height * channels);
		stbi_image_free(data);

		if (flags == IMREAD_GRAYSCALE && image.channels() == 3)
		{
			cvtColor(image, image, ZL_RGB2GRAY);
		}

		return 1;
	}


	/// <summary>
	/// Read image from file
	/// </summary>
	/// <param name="image">The image container.</param>
	/// <param name="filename">The filename.</param>
	/// <param name="flags">The flags.</param>
	/// <returns>True if successful, false otherwise</returns>
	bool imread(Mat& image, const String& filename, int flags)
	{
		if (parse_ext_read(filename) < 0)
		{
			warning("Invalid filename or Unsupported format!");
			return 0;
		}

		return imread_general(image, filename, flags);
		
	}

	/// <summary>
	/// Write image to file
	/// </summary>
	/// <param name="filename">The filename.</param>
	/// <param name="img">The image container.</param>
	/// <param name="flags">The flags.</param>
	/// <param name="quality">The quality(only for JPEG).</param>
	/// <returns>True if successful, false otherwise</returns>
	bool imwrite(const String& filename, Mat& img, int flags, int quality)
	{
		if (img.empty())
		{
			warning("image is empty!");
			return 0;
		}

		if (flags == IMWRITE_GRAYSCALE)
		{
			if (img.channels() == 3)
				cvtColor(img, img, ZL_RGB2GRAY);
			else if (img.channels() == 4)
				cvtColor(img, img, ZL_RGBA2GRAY);
		}

		int retval = 0;

		switch (parse_ext_write(filename))
		{
			case IMWRITE_BMP:
				if (img.channels() == 1)
				{
					// fake 3 channel gray image
					cvtColor(img, img, ZL_GRAY2RGB);
				}
				retval = stbi_write_bmp(filename.c_str(), img.cols(), img.rows(), img.channels(), img.ptr());
				break;
			case IMWRITE_TGA:
				retval = stbi_write_tga(filename.c_str(), img.cols(), img.rows(), img.channels(), img.ptr());
				break;
			case IMWRITE_PNG:
				retval = stbi_write_png(filename.c_str(), img.cols(), img.rows(), img.channels(), img.ptr(), 0);
				break;
			case IMWRITE_JPEG:
				if (quality <= 0 || quality > 100)
				{
					warning("invalid quality option, use default 90 instead!");
					quality = 90;
				}
				retval = jo_write_jpg(filename.c_str(), img.ptr(), img.cols(), img.rows(), img.channels(), quality);
				break;
			default:
				warning("Invalid filename or Unsupported format!");
				retval = 0;
				break;
		}

		if (retval == 0)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}