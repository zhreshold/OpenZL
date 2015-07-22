/************************************************************************//*
 *
 *   Script File: concurrent.hpp
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
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <functional>

namespace zl
{
	namespace cds
	{
		/*!
		* \struct	NullMutex
		*
		* \brief	A null mutex, no cost.
		*/
		class NullMutex
		{
		public:
			void lock() {}
			void unlock() {}
			bool try_lock()
			{
				return true;
			}
		};


		template <typename Key, typename Value> class AtomicUnorderedMap
		{
			using MapType = std::unordered_map<Key, Value>;
			using MapPtr = std::shared_ptr<MapType>;
		public:
			AtomicUnorderedMap()
			{
				mapPtr_ = std::make_shared<MapType>();
			}

			MapPtr get()
			{
				return std::atomic_load(&mapPtr_);
			}

			bool insert(const Key& key, const Value& value)
			{
				MapPtr p = std::atomic_load(&mapPtr_);
				MapPtr copy;
				do
				{
					if ((*p).count(key)> 0) return false;
					copy = std::make_shared<MapType>(*p);
					(*copy).insert({ key, value });
				} while (!std::atomic_compare_exchange_weak(&mapPtr_, &p, std::move(copy)));
				return true;
			}

			bool erase(const Key& key)
			{
				MapPtr p = std::atomic_load(&mapPtr_);
				MapPtr copy;
				do
				{
					if ((*p).count(key) <= 0) return false;
					copy = std::make_shared<MapType>(*p);
					(*copy).erase(key);
				} while (!std::atomic_compare_exchange_weak(&mapPtr_, &p, std::move(copy)));
				return true;
			}

			void clear()
			{
				MapPtr p = atomic_load(&mapPtr_);
				auto copy = std::make_shared<MapType>();
				do
				{
					;	// do clear when possible does not require old status
				} while (!std::atomic_compare_exchange_weak(&mapPtr_, &p, std::move(copy)));
			}

		private:
			std::shared_ptr<MapType>	mapPtr_;
		};

		template <typename T> class AtomicNonTrivial
		{
		public:
			AtomicNonTrivial()
			{
				ptr_ = std::make_shared<T>();
			}

			std::shared_ptr<T> get()
			{
				return std::atomic_load(&ptr_);
			}

			void set(const T& val)
			{
				std::shared_ptr<T> copy = std::make_shared<T>(val);
				std::atomic_store(&ptr_, copy);
			}

		private:
			std::shared_ptr<T>	ptr_;
		};

	} // namespace cds
} // namespace zl

#endif //END _OPENZL_THREAD_HPP_

