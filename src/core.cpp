/***********************************************************************/
/*
/*   Script File: core.cpp
/*
/*   Description:
/*
/*   Core modules implementation
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

#include "core.hpp"

namespace zl
{
	bool cvtColor(Mat& src, Mat& dst, int code)
	{
		if (src.empty())
		{
			warning("src image is empty!");
			return 0;
		}

		Mat buffer;

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
						uchar r = src(row, col, 0);
						uchar g = src(row, col, 1);
						uchar b = src(row, col, 2);

						uchar v = static_cast<uchar>(0.299 * r + 0.587 * g + 0.114 * b);
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
						uchar r = src(row, col, 0);
						uchar g = src(row, col, 1);
						uchar b = src(row, col, 2);

						uchar v = static_cast<uchar>(0.299 * r + 0.587 * g + 0.114 * b);
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
						uchar v = src(row, col);
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
}