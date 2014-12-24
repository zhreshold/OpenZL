OpenZL - Open EZ Library
====================================

OpenZL is a light weighted portable library packed with fundamental functions for Computer Vision projects.

Wrapped in C++ as itself lacks image IO libraries, meanwhile provide more convenience over C.

## Usage
* include "core.hpp" for basic class

* include "imgcodecs.h" for image read/write capabilities

* using namespace zl



> An example for reading an image into a zl::Mat

```
using namespace zl;
Mat image;
bool retval = imread(image, "c:/test.png", IMREAD_COLOR);
 ```
 > and the example code for writing a zl::Mat to disk
 
 ```
 bool retval = imwrite("c:/test_out.jpg", image, IMWRITE_COLOR, 90);
 ```
 > where 90 is the quality option only work for JPEG format.
 




## Purpose

+ Get rid of bulky OPENCV library and messy configuration steps!!!

+ Classes like Point, Rect, Mat are implemented either copied from OPENCV or rewritten to make it simple and clean.

+ Support cross-platform time() function

+ Support cross-platform waitkey() function without pressing enter

+ Support read image from BMP,JPG,PNG, TGA, PSD, GIF, HDR, PIC (thanks to Sean T. Barrett's stb_image library)

+ Support save image to BMP, JPG, PNG, TGA (thanks to Sean T. Barrett's stb_image_write library and Jon Olick's jpeg writer)

## What's under Construction...
+ More color space convertion

+ More TBD