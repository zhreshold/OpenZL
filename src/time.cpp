/************************************************************************//*
 *
 *   Script File: time.cpp
 *
 *   Description:
 *
 *   High resolution timer and date class
 *
 *
 *   Author: Joshua Zhang
 *   Date since: June-2015
 *
 *   Copyright (c) <2015> <JOSHUA Z. ZHANG>
 * 
 *	 Open source according to MIT License.
 *	 
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
 *   IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY 
 *   CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
 *   TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
 *   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
***************************************************************************/

#include "common.hpp"
#include "time.hpp"
#include "os.hpp"
#include "format.hpp"
#include <sstream>
#include <iomanip>
#include <cstdio>


namespace zl
{
	namespace consts
	{
		static const char			*kDateFractionSpecifier = "%frac";
		static const int			kDateFractionWidth = 3;
		static const char			*kTimerPrecisionSecSpecifier = "%sec";
		static const char			*kTimerPrecisionMsSpecifier = "%ms";
		static const char			*kTimerPrecisionUsSpecifier = "%us";
		static const char			*kTimerPrecisionNsSpecifier = "%ns";
		static const char			*kDateTimeSpecifier = "%datetime";
	}

	namespace time
	{
		Date::Date()
		{
			auto now = std::chrono::system_clock::now();
			timeStamp_ = std::chrono::system_clock::to_time_t(now);
			fraction_ = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000;
			calendar_ = os::localtime(timeStamp_);
			sprintf(fractionStr_, "%03d", fraction_);
		}

		void Date::to_local_time()
		{
			calendar_ = os::localtime(timeStamp_);
		}

		void Date::to_utc_time()
		{
			calendar_ = os::gmtime(timeStamp_);
		}

		std::string Date::to_string(const char *format)
		{
			std::string fmt(format);
			fmt::replace_all_with_escape(fmt, consts::kDateFractionSpecifier, std::string(fractionStr_));
			std::ostringstream buf;
			buf << std::put_time(&calendar_, fmt.c_str());
			return buf.str();
		}

		Date Date::local_time()
		{
			Date date;
			return date;
		}

		Date Date::utc_time()
		{
			Date date;
			date.to_utc_time();
			return date;
		}

		Timer::Timer()
		{
			this->reset();
		};

		void Timer::reset()
		{
			timeStamp_ = std::chrono::steady_clock::now();
		}

		std::size_t	Timer::elapsed_ns()
		{
			return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - timeStamp_).count();
		}

		std::string Timer::elapsed_ns_str()
		{
			char buf[25];
			sprintf(buf, "%llu", elapsed_ns());
			return std::string(buf);
		}

		std::size_t Timer::elapsed_us()
		{
			return elapsed_ns() / 1000;
		}

		std::string Timer::elapsed_us_str()
		{
			char buf[22];
			sprintf(buf, "%llu", elapsed_us());
			return std::string(buf);
		}

		std::size_t Timer::elapsed_ms()
		{
			return elapsed_ns() / 1000000;
		}

		std::string Timer::elapsed_ms_str()
		{
			char buf[19];
			sprintf(buf, "%llu", elapsed_ms());
			return std::string(buf);
		}

		std::size_t Timer::elapsed_sec()
		{
			return elapsed_ns() / 1000000000;
		}

		std::string Timer::elapsed_sec_str()
		{
			char buf[16];
			sprintf(buf, "%llu", elapsed_sec());
			return std::string(buf);
		}

		double Timer::elapsed_sec_double()
		{
			return static_cast<double>(elapsed_ns()) / 1000000000;
		}

		std::string Timer::to_string(const char *format)
		{
			std::string str(format);
			fmt::replace_all_with_escape(str, consts::kTimerPrecisionSecSpecifier, elapsed_sec_str());
			fmt::replace_all_with_escape(str, consts::kTimerPrecisionMsSpecifier, elapsed_ms_str());
			fmt::replace_all_with_escape(str, consts::kTimerPrecisionUsSpecifier, elapsed_us_str());
			fmt::replace_all_with_escape(str, consts::kTimerPrecisionNsSpecifier, elapsed_ns_str());
			return str;
		}

		
	}

	
}