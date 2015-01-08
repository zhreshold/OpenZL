/***********************************************************************/
/*
/#   Script File: core.cpp
/#
/#   Description:
/#
/#   Definition of core global modules
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

#include "core.hpp"
#include <cfloat>

#if defined(_WIN32)
#include <Windows.h>

#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
#include <unistd.h>	/* POSIX flags */
#include <time.h>	/* clock_gettime(), time() */
#include <sys/time.h>	/* gethrtime(), gettimeofday() */

#if defined(__MACH__) && defined(__APPLE__)
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif

#else
#error "Unable to define get_real_time( ) for an unknown OS."
#endif

#if defined(_WIN32)
#include <conio.h>

#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
#include <unistd.h>	/* POSIX flags */
#include <termios.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>

struct termios orig_termios;
int termios_status = 0;

void reset_terminal_mode()
{
	tcsetattr(0, TCSANOW, &orig_termios);
}

void set_conio_terminal_mode()
{
	struct termios new_termios;

	if (termios_status == 0)
	{
		/* take two copies - one for now, one for later */
		tcgetattr(0, &orig_termios);
		termios_status = 1;
	}
	memcpy(&new_termios, &orig_termios, sizeof(new_termios));

	/* register cleanup handler, and set the new terminal mode */
	atexit(reset_terminal_mode);
	cfmakeraw(&new_termios);
	tcsetattr(0, TCSANOW, &new_termios);
}

int get_last_key()
{
	int r;
	unsigned char c;
	if ((r = read(0, &c, sizeof(c))) < 0) {
		return r;
	}
	else {
		reset_terminal_mode();
		return c;
	}
}



#else
#error "Unable to define get_key( ) for an unknown OS."
#endif

namespace zl
{
	/// <summary>
	/// Prints the specified error and exit.
	/// </summary>
	/// <param name="err">The error to output.</param>
	void error(String err)
	{
		std::cerr << "Fatal error occured: " + err << std::endl;
		std::cout << "Press any key to exit..." << std::endl;
		waitkey();
		exit(-1);
	}

	/// <summary>
	/// Prints the specified warning.
	/// </summary>
	/// <param name="warn">The warning string to output.</param>
	void warning(String warn)
	{
		std::cout << "Warning: " + warn << std::endl;
	}

	

	/// <summary>
	/// wait for the specficed ms until keypressed
	/// </summary>
	/// <param name="ms">The ms to wait.</param>
	/// <returns>The key pressed(ASC-II not guaranteed).</returns>
	int waitkey(double ms)
	{
		int key = -1;
		double start = get_real_time();
		double end = start + ms / 1000;

		if (ms <= 0)
		{
			// disable timer if ms <= 0
			end = DBL_MAX;
		}

		while (key == -1)
		{
			key = kb_hit();
			//printf("%d", (int)get_elapsed_time_ms(start));
			if (get_real_time() >= end)
			{
				break;
			}
		}
		//std::cin.get();
		return key;
	}

	/// <summary>
	/// Hold screen for key press
	/// </summary>
	void hold_screen()
	{
		std::cout << "Press any key to continue..." << std::endl;
		waitkey();
	}

	/// <summary>
	/// Convert the color space of given image.
	/// </summary>
	/// <param name="src">The src image.</param>
	/// <param name="dst">The dst image.</param>
	/// <param name="code">The convertion code.</param>
	/// <returns>True if success, false otherwise</returns>
	bool cvt_color(Mat& src, Mat& dst, int code)
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
						Mat::value_type r = src(row, col, 0);
						Mat::value_type g = src(row, col, 1);
						Mat::value_type b = src(row, col, 2);

						Mat::value_type v = static_cast<Mat::value_type>(0.299 * r + 0.587 * g + 0.114 * b);
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
						Mat::value_type r = src(row, col, 0);
						Mat::value_type g = src(row, col, 1);
						Mat::value_type b = src(row, col, 2);

						Mat::value_type v = static_cast<Mat::value_type>(0.299 * r + 0.587 * g + 0.114 * b);
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
						Mat::value_type v = src(row, col);
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

	/*
	* Author:  David Robert Nadeau
	* Site:    http://NadeauSoftware.com/
	* License: Creative Commons Attribution 3.0 Unported License
	*          http://creativecommons.org/licenses/by/3.0/deed.en_US
	*/


	
	/// <summary>
	/// Time is measured since an arbitrary and OS-dependent start time.
	/// The returned real time is only useful for computing an elapsed time
	/// between two calls to this function.
	/// </summary>
	/// <returns>Returns the real time, in seconds, or -1.0 if an error occurred.</returns>
	double get_real_time()
	{
#if defined(_WIN32)
		FILETIME tm;
		ULONGLONG t;
#if defined(NTDDI_WIN8) && NTDDI_VERSION >= NTDDI_WIN8 && 0
		/* Windows 8, Windows Server 2012 and later. ---------------- */
		GetSystemTimePreciseAsFileTime(&tm);
#else
		/* Windows 2000 and later. ---------------------------------- */
		GetSystemTimeAsFileTime(&tm);
#endif
		t = ((ULONGLONG)tm.dwHighDateTime << 32) | (ULONGLONG)tm.dwLowDateTime;
		return (double)t / 10000000.0;

#elif (defined(__hpux) || defined(hpux)) || ((defined(__sun__) || defined(__sun) || defined(sun)) && (defined(__SVR4) || defined(__svr4__)))
		/* HP-UX, Solaris. ------------------------------------------ */
		return (double)gethrtime() / 1000000000.0;

#elif defined(__MACH__) && defined(__APPLE__)
		/* OSX. ----------------------------------------------------- */
		static double timeConvert = 0.0;
		if (timeConvert == 0.0)
		{
			mach_timebase_info_data_t timeBase;
			(void)mach_timebase_info(&timeBase);
			timeConvert = (double)timeBase.numer /
				(double)timeBase.denom /
				1000000000.0;
		}
		return (double)mach_absolute_time() * timeConvert;

#elif defined(_POSIX_VERSION)
		/* POSIX. --------------------------------------------------- */
#if defined(_POSIX_TIMERS) && (_POSIX_TIMERS > 0)
		{
			struct timespec ts;
#if defined(CLOCK_MONOTONIC_PRECISE)
			/* BSD. --------------------------------------------- */
			const clockid_t id = CLOCK_MONOTONIC_PRECISE;
#elif defined(CLOCK_MONOTONIC_RAW)
			/* Linux. ------------------------------------------- */
			const clockid_t id = CLOCK_MONOTONIC_RAW;
#elif defined(CLOCK_HIGHRES)
			/* Solaris. ----------------------------------------- */
			const clockid_t id = CLOCK_HIGHRES;
#elif defined(CLOCK_MONOTONIC)
			/* AIX, BSD, Linux, POSIX, Solaris. ----------------- */
			const clockid_t id = CLOCK_MONOTONIC;
#elif defined(CLOCK_REALTIME)
			/* AIX, BSD, HP-UX, Linux, POSIX. ------------------- */
			const clockid_t id = CLOCK_REALTIME;
#else
			const clockid_t id = (clockid_t)-1;	/* Unknown. */
#endif /* CLOCK_* */
			if (id != (clockid_t)-1 && clock_gettime(id, &ts) != -1)
				return (double)ts.tv_sec +
				(double)ts.tv_nsec / 1000000000.0;
			/* Fall thru. */
		}
#endif /* _POSIX_TIMERS */

		/* AIX, BSD, Cygwin, HP-UX, Linux, OSX, POSIX, Solaris. ----- */
		struct timeval tm;
		gettimeofday(&tm, NULL);
		return (double)tm.tv_sec + (double)tm.tv_usec / 1000000.0;
#else
		return -1.0;		/* Failed. */
#endif
	}


	//////////////////////////////// Timer class ////////////////////////////////////
	/// <summary>
	/// Updates the current timestamp.
	/// </summary>
	void Timer::update()
	{
		timestamp = get_real_time();
	}

	/// <summary>
	/// Default constructor, will call update() to record current timestamp.
	/// </summary>
	Timer::Timer()
	{
		update();
	}

	Timer::~Timer()
	{

	}



	/// <summary>
	/// Get the time elapsed in ms since last update.
	/// </summary>
	/// <param name="timeStamp">The last timestamp.</param>
	/// <returns>The time elapsed in ms</returns>
	double Timer::get_elapsed_time_ms()
	{
		return (get_real_time() - timestamp) * 1000.0;
	}

	/// <summary>
	/// Get the time elapsed in second since last update.
	/// </summary>
	/// <returns>The time elapsed in second</returns>
	double Timer::get_elapsed_time_s()
	{
		return get_real_time() - timestamp;
	}

	/// <summary>
	/// Get the time elapsed in us since last update.
	/// </summary>
	/// <returns>The time elapsed in us</returns>
	double Timer::get_elapsed_time_us()
	{
		return (get_real_time() - timestamp) * 1000000.0;
	}

	/***********************************************************************/
	/*
	/#   Script File: waitkey.cpp
	/#
	/#   Description:
	/#
	/#   Cross-platform get keystroke function
	/#
	/#
	/#   Author: Joshua Zhang (zzbhf@mail.missouri.edu)
	/#   Date: Dec-2014
	/#
	/#   Copyright(C) 2014  Joshua Zhang	 - All Rights Reserved.
	/#
	/#   This software is available for non-commercial use only. It must
	/#   not be modified and distributed without prior permission of the
	/#   author. The author is not responsible for implications from the
	/#   use of this software.
	*/
	/***********************************************************************/



	/// <summary>
	/// Get asynchronized keyboard press.
	/// </summary>
	/// <returns>
	/// The pressed key value.
	/// </returns>
	int get_key()
	{
#if defined(_WIN32)
		return _getch();

#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
		int buf = -1;
		struct termios old = { 0 };
		if (tcgetattr(0, &old) < 0)
			perror("tcsetattr()");
		old.c_lflag &= ~ICANON;
		old.c_lflag &= ~ECHO;
		old.c_cc[VMIN] = 1;
		old.c_cc[VTIME] = 0;
		if (tcsetattr(0, TCSANOW, &old) < 0)
			perror("tcsetattr ICANON");
		if (read(0, &buf, 1) < 0)
			perror("read()");
		old.c_lflag |= ICANON;
		old.c_lflag |= ECHO;
		if (tcsetattr(0, TCSADRAIN, &old) < 0)
			perror("tcsetattr ~ICANON");
		return (buf);

#else
		return -1;		// failed
#endif
	}


	/// <summary>
	/// Detect keyboard hit, no wait
	/// </summary>
	/// <returns>The key pressed or -1 if not(ASC-II not guaranteed, platform specific).</returns>
	int kb_hit()
	{
#if defined(_WIN32)
		if (_kbhit())
			return _getch();
		else
			return -1;

#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))

		
		set_conio_terminal_mode();

		struct timeval tv = { 0L, 0L };
		fd_set fds;
		FD_ZERO(&fds);
		FD_SET(0, &fds);
		if (select(1, &fds, NULL, NULL, &tv))
		{
			return get_last_key();
		}
		else
		{
			return -1;
		}

#else
		return -1;		// unsupported OS
#endif
	}
}



