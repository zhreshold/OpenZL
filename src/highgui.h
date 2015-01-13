/***********************************************************************/
/*
/#   Script File: highgui.h
/#
/#   Description:
/#
/#   Highgui modules, for displaying / ...
/#
/#
/#   Author: Joshua Zhang (zzbhf@mail.missouri.edu)
/#   Date: Dec-2014
/#
/#   Copyright(C) <2014>  Joshua Zhang	 - All Rights Reserved.
/#
/#   This software is available for non-commercial use only.
/#	 According to MIT license.
/#	 No warrenty implied, use at your own risk.
*/
/***********************************************************************/

#ifndef _OPENZL_HIGHGUI_H_
#define _OPENZL_HIGHGUI_H_

#include "core.hpp"

#define ZL_MAX_NUM_WINDOWS	50			// maximum number of windows allowed in each run, overwrite from the start if overflow

//////////////////////////////// highgui ////////////////////////////////
namespace zl
{
	void imshow(const char* windowname, Mat& image);
	void destroy_window(const char* windowname);
	void destroy_all_windows();
	void draw_line(Mat& image, int x1, int y1, int x2, int y2, Scalar color, int lineWidth = 1);
	void draw_line(Mat& image, Point pt1, Point pt2, Scalar color, int lineWidth = 1); 
}



#endif  //end of _OPENZL_HIGHGUI_H_




