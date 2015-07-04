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
#include "time.hpp"
#include "os.hpp"
#include "format.hpp"
#include <string>
#include <sstream>
#include <utility>
#include <atomic>



#include <iostream>

namespace zl
{
	namespace log
	{
		

		typedef enum 
		{
			trace = 0,
			debug = 1,
			info = 2,
			warn = 3,
			error = 4,
			fatal = 5,
			off = 6
		}LogLevels;

		namespace consts
		{
			static const char* level_names[] { "TRACE", "DEBUG", "INFO ", "WARN ", "ERROR", "FATAL", "'OFF'"};
			static const char* short_level_names[] { "T", "D", "I", "W", "E", "F", "O"};
		}

		namespace detail
		{
			struct LogMessage;
			class LineLogger;
		}
		

		class Logger
		{
		public:
			Logger(std::string name) : name_(name)
			{
				level_ = LogLevels::info;
			}

			// logger.info(format string, arg1, arg2, arg3, ...) call style
			template <typename... Args> detail::LineLogger trace(const char* fmt, const Args&... args);
			template <typename... Args> detail::LineLogger debug(const char* fmt, const Args&... args);
			template <typename... Args> detail::LineLogger info(const char* fmt, const Args&... args);
			template <typename... Args> detail::LineLogger warn(const char* fmt, const Args&... args);
			template <typename... Args> detail::LineLogger error(const char* fmt, const Args&... args);
			template <typename... Args> detail::LineLogger fatal(const char* fmt, const Args&... args);


			// logger.info(msg) << ".." call style
			template <typename T> detail::LineLogger trace(const T& msg);
			template <typename T> detail::LineLogger debug(const T& msg);
			template <typename T> detail::LineLogger info(const T& msg);
			template <typename T> detail::LineLogger warn(const T& msg);
			template <typename T> detail::LineLogger error(const T& msg);
			template <typename T> detail::LineLogger fatal(const T& msg);


			// logger.info() << ".." call  style
			detail::LineLogger trace();
			detail::LineLogger debug();
			detail::LineLogger info();
			detail::LineLogger warn();
			detail::LineLogger error();
			detail::LineLogger fatal();

			const LogLevels level() const
			{
				return static_cast<LogLevels>(level_.load(std::memory_order_relaxed));
			}
			
			bool should_log(LogLevels msgLevel) const
			{
				return msgLevel >= this->level();
			}
			const std::string& name() const { return name_; };

			void log_msg(detail::LogMessage msg);

		private:

			friend detail::LineLogger;

			detail::LineLogger log_if_enabled(LogLevels lvl);

			template <typename... Args>
			detail::LineLogger log_if_enabled(LogLevels lvl, const char* fmt, const Args&... args);

			template<typename T>
			detail::LineLogger log_if_enabled(LogLevels lvl, const T& msg);

			

			std::string		name_;
			std::atomic_int	level_;
		};



		namespace detail
		{

			struct LogMessage
			{
				std::string			loggerName_;
				LogLevels			level_;
				time::Date			dateTime_;
				size_t				threadId_;
				std::string			buffer_;
			};

			class LineLogger : private UnCopyable
			{
			public:
				LineLogger(Logger* callbacker, LogLevels lvl, bool enable) :callbackLogger_(callbacker), enabled_(enable)
				{
					msg_.level_ = lvl;
				}

				LineLogger(LineLogger&& other) :
					callbackLogger_(other.callbackLogger_),
					msg_(std::move(other.msg_)),
					enabled_(other.enabled_)
				{
					other.disable();
				}

				~LineLogger()
				{
					if (enabled_)
					{
						msg_.loggerName_ = callbackLogger_->name();
						msg_.dateTime_ = time::Date::local_time();
						msg_.threadId_ = os::thread_id();
						callbackLogger_->log_msg(msg_);
					}
				}

				LineLogger& operator=(LineLogger&&) = delete;

				void write(const char* what)
				{
					if (enabled_)
						msg_.buffer_ += what;
				}

				template <typename... Args>
				void write(const char* fmt, const Args&... args)
				{
					if (!enabled_)
						return;
					std::string buf(fmt);
					fmt::format_string(buf, args...);
					msg_.buffer_ += buf;
				}


				LineLogger& operator<<(const char* what)
				{
					if (enabled_)
						msg_.buffer_ += what;
					return *this;
				}

				LineLogger& operator<<(const std::string& what)
				{
					if (enabled_)
						msg_.buffer_ += what;
					return *this;
				}

				template<typename T>
				LineLogger& operator<<(const T& what)
				{
					if (enabled_)
						msg_.buffer_ += dynamic_cast<std::ostringstream &>(std::ostringstream() << std::dec << what).str();
					return *this;
				}

				void disable()
				{
					enabled_ = false;
				}

				bool is_enabled() const
				{
					return enabled_;
				}
			private:
				Logger			*callbackLogger_;
				LogMessage		msg_;
				bool			enabled_;
			};
		}

		inline detail::LineLogger Logger::log_if_enabled(LogLevels lvl)
		{
			detail::LineLogger l(this, lvl, should_log(lvl));
			return l;
		}

		template <typename... Args>
		inline detail::LineLogger Logger::log_if_enabled(LogLevels lvl, const char* fmt, const Args&... args)
		{
			detail::LineLogger l(this, lvl, should_log(lvl));
			l.write(fmt, args...);
			return l;
		}

		template<typename T>
		inline detail::LineLogger Logger::log_if_enabled(LogLevels lvl, const T& msg)
		{
			detail::LineLogger l(this, lvl, should_log(lvl));
			l.write(msg);
			return l;
		}

		// logger.info(format string, arg1, arg2, arg3, ...) call style
		template <typename... Args> 
		inline detail::LineLogger Logger::trace(const char* fmt, const Args&... args)
		{
			return log_if_enabled(LogLevels::trace, fmt, args...);
		}
		template <typename... Args> 
		inline detail::LineLogger Logger::debug(const char* fmt, const Args&... args)
		{
			return log_if_enabled(LogLevels::debug, fmt, args...);
		}
		template <typename... Args> 
		inline detail::LineLogger Logger::info(const char* fmt, const Args&... args)
		{
			return log_if_enabled(LogLevels::info, fmt, args...);
		}
		template <typename... Args>
		inline detail::LineLogger Logger::warn(const char* fmt, const Args&... args)
		{
			return log_if_enabled(LogLevels::warn, fmt, args...);
		}
		template <typename... Args> 
		inline detail::LineLogger Logger::error(const char* fmt, const Args&... args)
		{
			return log_if_enabled(LogLevels::error, fmt, args...);
		}
		template <typename... Args>
		inline detail::LineLogger Logger::fatal(const char* fmt, const Args&... args)
		{
			return log_if_enabled(LogLevels::fatal, fmt, args...);
		}


		// logger.info(msg) << ".." call style
		template <typename T> 
		inline detail::LineLogger Logger::trace(const T& msg)
		{
			return log_if_enabled(LogLevels::trace, msg);
		}
		template <typename T> 
		inline detail::LineLogger Logger::debug(const T& msg)
		{
			return log_if_enabled(LogLevels::debug, msg);
		}
		template <typename T> 
		inline detail::LineLogger Logger::info(const T& msg)
		{
			return log_if_enabled(LogLevels::info, msg);
		}
		template <typename T> 
		inline detail::LineLogger Logger::warn(const T& msg)
		{
			return log_if_enabled(LogLevels::warn, msg);
		}
		template <typename T> 
		inline detail::LineLogger Logger::error(const T& msg)
		{
			return log_if_enabled(LogLevels::error, msg);
		}
		template <typename T> 
		inline detail::LineLogger Logger::fatal(const T& msg)
		{
			return log_if_enabled(LogLevels::fatal, msg);
		}


		// logger.info() << ".." call  style
		inline detail::LineLogger Logger::trace()
		{
			return log_if_enabled(LogLevels::trace);
		}
		inline detail::LineLogger Logger::debug()
		{
			return log_if_enabled(LogLevels::debug);
		}
		inline detail::LineLogger Logger::info()
		{
			return log_if_enabled(LogLevels::info);
		}
		inline detail::LineLogger Logger::warn()
		{
			return log_if_enabled(LogLevels::warn);
		}
		inline detail::LineLogger Logger::error()
		{
			return log_if_enabled(LogLevels::error);
		}
		inline detail::LineLogger Logger::fatal()
		{
			return log_if_enabled(LogLevels::fatal);
		}

		inline void Logger::log_msg(detail::LogMessage msg)
		{
			std::cout << "called log_msg: " << std::endl;
			std::cout << msg.buffer_ << std::endl;
		}

	} // namespace log
} // namespace zl

#endif //END _OPENZL_LOG_HPP_