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
/#   Date since: Dec-2014
/#
/#   Copyright (c) <2014> <JOSHUA Z. ZHANG>	 - All Rights Reserved.
/#
/#	 Open source according to MIT License.
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
	void draw_line(Mat& image, Point pt1, Point pt2, Scalar color, int thickness = 1); 
	void draw_circle(Mat& image, int xm, int ym, int r, Scalar color, int thickness = 1, bool fill = false);
	void draw_circle(Mat& image, Point center, int radius, Scalar color, int thickness, bool fill);
	void draw_rectangle(Mat& image, int x0, int y0, int x1, int y1, Scalar color, int thickness, bool fill = false);
	void draw_rectangle(Mat& image, Point pt0, Point pt1, Scalar color, int thickness, bool fill);
	void draw_rectangle(Mat& image, Rect rect, Scalar color, int thickness, bool fill);
	void draw_polygon(Mat& image, Vecpt& vertices, Scalar color, bool fill = false);
}



#endif  //end of _OPENZL_HIGHGUI_H_




