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