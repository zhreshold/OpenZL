OpenZL - Open EZ Library
====================================

OpenZL is a light weighted portable X-platform library packed with fundamental functions for simple Computer Vision projects.

Wrapped in C++ as itself lacks image IO libraries, meanwhile provide more convenience over C.

Full documentation in doc/html/

## License
MIT License.

## Usage
* Make sure you have c++11 support(vc11+, -std=c++11 in gcc 4.6+/clang3.3+)

* include "core.hpp" for basic class

* include "imgcodecs.h" for image read/write capabilities

* include "highgui.h" for GUI related functions, e.g. imshow().

* using namespace zl


## Examples
An example for reading an image into a zl::Mat

```
using namespace zl;
Mat image;
bool retval = imread(image, "c:/test.png", IMREAD_COLOR);
 ```
and the example code for writing a zl::Mat to disk
 
 ```
 bool retval = imwrite("c:/test_out.jpg", image, IMWRITE_COLOR, 90);
 ```
where 90 is the quality option only work for JPEG format.
 
An example for display an image in zl::Mat container
 
 ```
 imshow("debug", image);  // show image in "debug" window
 destroy_window("debug"); // destroy window named "debug"
 ```
 
or
 
 ```
 destroy_all_windows(); // close all opened windows
 ```
 
 Convert RGB image to Gray
 ```
cvt_color(image, gray, ZL_RGB2GRAY);
```
 
Draw a red line with thickness
 
 ```
 draw_line(image, Point(100, 100), Point(200, 200), Scalar(255, 0, 0), thickness);
 imshow("Drawn", image);
 ```
 
Similarly, draw rectangles, circles, and even polygons with fill/no_fill option
```
draw_rectangle(image, Pt1, Pt2, Scalar(255, 255, 0), thickness, fill);
draw_circle(image, Point(300, 300), radius, Scalar(255, 255, 0), thickness, fill);
// fill in polygon vertices
Vecpt pts;
pts.push_back(Point(30, 40));
pts.push_back(Point(50, 5));
pts.push_back(Point(100, 60));
pts.push_back(Point(200, 400));
pts.push_back(Point(50, 70));
draw_polygon(image, pts, Scalar(0, 255, 0));
```

Asynchronus waitkey function(won't block keyboard input) 
```
// wait for 10 sec or until any keypress
waitkey(10000);
// or wait forever
waitkey(0);
// which equals to 
hold_screen();
```


## Capability

+ Get rid of bulky OpenCV library and messy configuration steps when you want start a very simple app.

+ Classes like Point, Rect, Mat are implemented either copied from OpenCV or rewritten to make it simple and clean.

+ Support cross-platform time() function.

+ Support cross-platform waitkey() function without pressing enter.

+ Support read image from BMP,JPG,PNG, TGA, PSD, GIF, HDR, PIC (thanks to Sean T. Barrett's stb_image library).

+ Support save image to BMP, JPG, PNG, TGA (thanks to Sean T. Barrett's stb_image_write library and Jon Olick's jpeg writer).

+ Support image display (thanks to CImg library).

+ Support various drawing functions(line, rectangle, circle, polygon...).

## What's under Construction...
+ More color space convertion

+ Image resize functions

+ More TBD