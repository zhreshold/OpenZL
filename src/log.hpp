/************************************************************************//*
 *
 *   Script File: log.hpp
 *
 *   Description:
 *
 *   Fast asynchronous logger
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


#ifndef _OPENZL_LOG_HPP_
#define _OPENZL_LOG_HPP_

#include "common.hpp"
#include <string>
#include <utility>

namespace zl
{
	namespace log
	{
		template<typename Callback> class LineLogger : private UnCopyable
		{
		public:
			LineLogger(Callback *callback, bool enable) : callback_(callback), enabled_(enable) {}
			LineLogger(LineLogger&& other) :callback_(other.callback_), msg_(std::move(other.msg_)), enabled_(other.enabled_)
			{
				other.msg_ = std::string();
				other.enabled_ = false;
			}

			
			~LineLogger()
			{
				if (enabled_)
				{
					callback_->write(msg_);
				}
			}

			LineLogger& operator=(LineLogger&&) = delete;


			void disable()
			{
				enabled_ = false;
			}

			bool is_enabled() const
			{
				return enabled_;
			}

		private:
			Callback	*callback_;
			std::string msg_;
			bool		enabled_;
		};
	} // namespace log
} // namespace zl

#endif //END _OPENZL_LOG_HPP_