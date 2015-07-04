/************************************************************************//*
 *
 *   Script File: thread.hpp
 *
 *   Description:
 *
 *   Thread safe data structures controlled by mutex behaviour
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

#ifndef _OPENZL_THREAD_HPP_
#define _OPENZL_THREAD_HPP_

#include "common.hpp"
#include <thread>
#include <mutex>
#include <set>
#include <string>

namespace zl
{
	namespace thread
	{
		/*!
		* \struct	NullMutex
		*
		* \brief	A null mutex, no cost.
		*/
		class NullMutex
		{
			void lock() {}
			void unlock() {}
			bool try_lock()
			{
				return true;
			}
		};

		template <typename Mutex> class HashMap_
		{

		};

		template <typename Mutex> class FileEditorRegistry_: private UnMovable
		{
		public:
			static FileEditorRegistry_<Mutex>& instance()
			{
				static FileEditorRegistry_<Mutex> instance_;
				return instance_;
			}

			bool contains(std::string &entry) const
			{
				return set_.count(entry) > 0;
			}

			bool try_insert(std::string &entry)
			{
				// return false if already exist
				if (contains(entry)) return false;
				std::lock_guard<Mutex> lock(mutex_);
				set_.insert(entry);
				return true;
			}

			void erase(std::string &entry)
			{
				if (!contains(entry)) return;
				std::lock_guard<Mutex> lock(mutex_);
				set_.erase(entry);
			}

		private:
			void clear()
			{
				std::lock_guard<Mutex> lock(mutex_);
				set_.clear();
			}

			FileEditorRegistry_<Mutex>() {};

			std::set<std::string>	set_;
			Mutex		mutex_;
		};

		typedef FileEditorRegistry_<std::mutex> FileEditorRegistry;

	} // namespace thread
} // namespace zl

#endif //END _OPENZL_THREAD_HPP_

