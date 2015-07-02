/************************************************************************//*
 *
 *   Script File: file.hpp
 *
 *   Description:
 *
 *   File handler class
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

#ifndef _OPENZL_FILE_HPP_
#define _OPENZL_FILE_HPP_

#include "common.hpp"
#include "os.hpp"
#include <fstream>
#include <string>
#include <utility>


namespace zl
{
	namespace consts
	{
		static const int kDefaultFileOpenRetryTimes = 5;
		static const int kDefaultFileOpenRetryInterval = 10;
	}

	namespace fs
	{
		class File: private UnCopyable
		{
		public:
			/*!
			 * \fn	File::File(std::string filename, std::ios::openmode openmode = std::ios::in, int retryTimes = consts::kDefaultFileOpenRetryTimes, int retryInterval = consts::kDefaultFileOpenRetryInterval)
			 *
			 * \brief	Constructor.
			 *
			 * \param	filename	 	Filename of the file.
			 * \param	openmode	 	The openmode.
			 * \param	retryTimes   	List of times of the retries.
			 * \param	retryInterval	The retry interval.
			 */
			File(std::string filename, std::ios::openmode openmode = std::ios::in, 
				int retryTimes = consts::kDefaultFileOpenRetryTimes, 
				int retryInterval = consts::kDefaultFileOpenRetryInterval)
				: openmode_(openmode) 
			{
				// use absolute path
				filename_ = os::get_absolute_path(filename);
				// always use binary mode
				openmode_ |= std::ios::binary;	
				this->check_valid();
				this->try_open(retryTimes, retryInterval);
			};

			/*!
			* \fn	File::File(File&& from)
			*
			* \brief	Move constructor.
			*
			* \param [in,out]	other	Source for the instance.
			*/
			File(File&& other) : filename_(std::move(other.filename_)), stream_(std::move(other.stream_)), openmode_(other.openmode_)
			{ 
				other.filename_ = std::string(); 
				other.openmode_ = std::ios::in;
			};

			virtual ~File() = default;

			// No move operator
			File& operator=(File&&) = delete;

			// overload << operator
			template <typename T>
			File& operator<<(T what) { stream_ << what; return *this; }

			static std::string endl();

			std::string filename() { return filename_; }

			
			bool is_valid() { return !filename_.empty(); }
			
			bool is_open() { return stream_.is_open(); }

			void flush() { stream_.flush(); }
		private:
			File() {}

			bool open();
			void check_valid() { if (!this->is_valid()) throw RuntimeException("Invalid File handler!"); }
			void try_open(int retryTime, int retryInterval);

			std::string		filename_;
			std::fstream	stream_;
			std::ios::openmode openmode_;
		};

	} //namespace fs
} // namespace zl

#endif //END _OPENZL_FILE_HPP_

