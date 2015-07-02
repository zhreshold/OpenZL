/************************************************************************//*
 *
 *   Script File: time.hpp
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


#ifndef _OPENZL_TIME_HPP_
#define _OPENZL_TIME_HPP_

#include <string>
#include <ctime>
#include <chrono>
#include <thread>

namespace zl
{
	namespace consts
	{

	}

	namespace time
	{
		/*!
		* \class	Date
		*
		* \brief	A date class.
		*/
		class Date
		{
		public:
			Date();
			virtual ~Date() = default;

			void to_local_time();

			void to_utc_time();

			std::string to_string(const char *format = "[%c.%frac]");

			static Date local_time();

			static Date utc_time();

		private:
			std::time_t		timeStamp_;
			int				fraction_;
			std::string		fractionStr_;
			struct std::tm	calendar_;

		};

		/*!
		 * \class	Timer
		 *
		 * \brief	A timer class.
		 */
		class Timer
		{
		public:
			Timer();

			void reset();

			std::size_t	elapsed_ns();

			std::string elapsed_ns_str();

			std::size_t elapsed_us();

			std::string elapsed_us_str();

			std::size_t elapsed_ms();

			std::string elapsed_ms_str();

			std::size_t elapsed_sec();

			std::string elapsed_sec_str();

			double elapsed_sec_double();

			std::string to_string(const char *format = "[%ms ms]");

		private:
			std::chrono::steady_clock::time_point timeStamp_;
		};

		/*!
		 * \fn	void sleep(int milliseconds);
		 *
		 * \brief	Sleep for specified milliseconds
		 *
		 * \param	milliseconds	The milliseconds.
		 */
		inline void sleep(int milliseconds)
		{
			if (milliseconds > 0)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
			}
		}

	} // namespace time

} // namespace zl

#endif //END _OPENZL_TIME_HPP_