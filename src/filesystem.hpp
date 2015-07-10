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
#include "thread.hpp"
#include <fstream>
#include <string>
#include <utility>
#include <iostream>


namespace zl
{
	namespace consts
	{
		static const int kDefaultFileOpenRetryTimes = 5;
		static const int kDefaultFileOpenRetryInterval = 10;
	}

	namespace fs
	{
		class FileEditor: private UnCopyable
		{
		public:
			FileEditor() = delete;
			FileEditor(std::string filename, bool truncateOrNot = false,
				int retryTimes = consts::kDefaultFileOpenRetryTimes, 
				int retryInterval = consts::kDefaultFileOpenRetryInterval)
			{
				// use absolute path
				filename_ = os::get_absolute_path(filename);
				// try open
				this->try_open(retryTimes, retryInterval, truncateOrNot);
			};

			/*!
			* \fn	File::File(File&& from)
			*
			* \brief	Move constructor.
			*
			* \param [in,out]	other	Source for the instance.
			*/
			FileEditor(FileEditor&& other) : filename_(std::move(other.filename_)),
				stream_(std::move(other.stream_)), 
				readPos_(std::move(other.readPos_)),
				writePos_(std::move(other.writePos_))
			{ 
				other.filename_ = std::string();
			};

			virtual ~FileEditor() { this->close(); }

			// No move operator
			FileEditor& operator=(FileEditor&&) = delete;

			// overload << operator
			template <typename T>
			FileEditor& operator<<(T what) { stream_ << what; return *this; }

			std::string filename() const { return filename_; }

			bool open(bool truncateOrNot = false);
			bool try_open(int retryTime, int retryInterval, bool truncateOrNot = false);
			void close();
			bool is_valid() const { return !filename_.empty(); }
			bool is_open() const { return stream_.is_open(); }
			void flush() { stream_.flush(); }
		private:
			
			void check_valid() { if (!this->is_valid()) throw RuntimeException("Invalid File Editor!"); }

			std::string		filename_;
			std::fstream	stream_;
			std::streampos	readPos_;
			std::streampos	writePos_;
		};

		class FileReader : private UnCopyable
		{
		public:
			FileReader() = delete;
			FileReader(std::string filename, int retryTimes = consts::kDefaultFileOpenRetryTimes,
				int retryInterval = consts::kDefaultFileOpenRetryInterval)
			{
				// use absolute path
				filename_ = os::get_absolute_path(filename);
				// try open
				this->try_open(retryTimes, retryInterval);
			}

			FileReader(FileReader&& other) : filename_(std::move(other.filename_)), istream_(std::move(other.istream_))
			{
				other.filename_ = std::string();
			};

			std::string filename() const { return filename_; }

			bool is_open() const { return istream_.is_open(); }
			bool is_valid() const { return !filename_.empty(); }
			bool open();
			bool try_open(int retryTime, int retryInterval);
			void close() { istream_.close(); };

		private:
			
			void check_valid(){ if (!this->is_valid()) throw RuntimeException("Invalid File Reader!"); }
			std::string		filename_;
			std::ifstream	istream_;
		};

		bool is_occupied(std::string &filename);

		namespace detail
		{
			template <typename Mutex> class FileEditorRegistry_ : private UnMovable
			{
			public:
				static FileEditorRegistry_<Mutex>& instance()
				{
					static FileEditorRegistry_<Mutex> sInstance;
					return sInstance;
				}

				//~FileEditorRegistry_<Mutex>()
				//{
				//	std::cout << "FileRegistry dtor" << std::endl;
				//}

				bool contains(std::string &entry)
				{
					std::lock_guard<Mutex> lock(mutex_);
					return set_.count(entry) > 0;
				}

				bool try_insert(std::string &entry)
				{
					std::lock_guard<Mutex> lock(mutex_);
					if (set_.count(entry) > 0) return false;
					set_.insert(entry);
					return true;
				}

				void erase(std::string &entry)
				{
					std::lock_guard<Mutex> lock(mutex_);
					if (set_.count(entry) <= 0) return;
					set_.erase(entry);
				}

			private:
				void clear()
				{
					std::lock_guard<Mutex> lock(mutex_);
					set_.clear();
				}

				FileEditorRegistry_<Mutex>(){};

				std::unordered_set<std::string>	set_;
				Mutex		mutex_;
			};


#ifdef ZL_SINGLE_THREAD_ONLY
			typedef FileEditorRegistry_<thread::NullMutex> FileEditorRegistry;
#else
			typedef FileEditorRegistry_<std::mutex> FileEditorRegistry;
#endif
		} // namespace detail

	} //namespace fs
} // namespace zl

#endif //END _OPENZL_FILE_HPP_

