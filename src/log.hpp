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
#include "filesystem.hpp"
#include <string>
#include <sstream>
#include <utility>
#include <atomic>
#include <mutex>
#include <memory>
#include <vector>
#include <type_traits>



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
			static const char	*kLevel_names[] { "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL", "OFF"};
			static const char	*kShort_level_names[] { "T", "D", "I", "W", "E", "F", "O"};
			static const int	kAsyncQueueSize = 4096;	//!< asynchrous queue size, must be power of 2

			static const char	*kSinkDatetimeSpecifier = "%datetime";
			static const char	*kSinkLoggerNameSpecifier = "%logger";
			static const char	*kSinkThreadSpecifier = "%thread";
			static const char	*kSinkLevelSpecifier = "%level";
			static const char	*kSinkMessageSpecifier = "%msg";
			static const char	*kSinkDefaultFormat = "[%datetime][T:%thread][%logger][%level] %msg";
		}

		namespace detail
		{
			struct LogMessage;
			class LineLogger;
			class SinkInterface;
		}
		
		typedef std::shared_ptr<detail::SinkInterface> SinkPtr;
		class Logger
		{
		public:
			Logger(std::string name) : name_(name)
			{
				level_ = LogLevels::info;
			}

			~Logger()
			{
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

			
		private:

			friend detail::LineLogger;

			detail::LineLogger log_if_enabled(LogLevels lvl);

			template <typename... Args>
			detail::LineLogger log_if_enabled(LogLevels lvl, const char* fmt, const Args&... args);

			template<typename T>
			detail::LineLogger log_if_enabled(LogLevels lvl, const T& msg);

			void log_msg(detail::LogMessage msg);

			std::string				name_;
			std::atomic_int			level_;
		public:
			std::vector<SinkPtr>	sinkPtrs_;
			std::vector<bool>		sinkValves_;
		};



		namespace detail
		{
			template<typename T>
			class mpmc_bounded_queue
			{
			public:

				using item_type = T;
				mpmc_bounded_queue(size_t buffer_size)
					: buffer_(new cell_t[buffer_size]),
					buffer_mask_(buffer_size - 1)
				{
					//queue size must be power of two
					if (!((buffer_size >= 2) && ((buffer_size & (buffer_size - 1)) == 0)))
						throw ArgException("async logger queue size must be power of two");

					for (size_t i = 0; i != buffer_size; i += 1)
						buffer_[i].sequence_.store(i, std::memory_order_relaxed);
					enqueue_pos_.store(0, std::memory_order_relaxed);
					dequeue_pos_.store(0, std::memory_order_relaxed);
				}

				~mpmc_bounded_queue()
				{
					delete[] buffer_;
				}


				bool enqueue(T&& data)
				{
					cell_t* cell;
					size_t pos = enqueue_pos_.load(std::memory_order_relaxed);
					for (;;)
					{
						cell = &buffer_[pos & buffer_mask_];
						size_t seq = cell->sequence_.load(std::memory_order_acquire);
						intptr_t dif = (intptr_t)seq - (intptr_t)pos;
						if (dif == 0)
						{
							if (enqueue_pos_.compare_exchange_weak(pos, pos + 1, std::memory_order_relaxed))
								break;
						}
						else if (dif < 0)
						{
							return false;
						}
						else
						{
							pos = enqueue_pos_.load(std::memory_order_relaxed);
						}
					}
					cell->data_ = std::move(data);
					cell->sequence_.store(pos + 1, std::memory_order_release);
					return true;
				}

				bool dequeue(T& data)
				{
					cell_t* cell;
					size_t pos = dequeue_pos_.load(std::memory_order_relaxed);
					for (;;)
					{
						cell = &buffer_[pos & buffer_mask_];
						size_t seq =
							cell->sequence_.load(std::memory_order_acquire);
						intptr_t dif = (intptr_t)seq - (intptr_t)(pos + 1);
						if (dif == 0)
						{
							if (dequeue_pos_.compare_exchange_weak(pos, pos + 1, std::memory_order_relaxed))
								break;
						}
						else if (dif < 0)
							return false;
						else
							pos = dequeue_pos_.load(std::memory_order_relaxed);
					}
					data = std::move(cell->data_);
					cell->sequence_.store(pos + buffer_mask_ + 1, std::memory_order_release);
					return true;
				}

			private:
				struct cell_t
				{
					std::atomic<size_t>   sequence_;
					T                     data_;
				};

				static size_t const     cacheline_size = 64;
				typedef char            cacheline_pad_t[cacheline_size];

				cacheline_pad_t         pad0_;
				cell_t* const           buffer_;
				size_t const            buffer_mask_;
				cacheline_pad_t         pad1_;
				std::atomic<size_t>     enqueue_pos_;
				cacheline_pad_t         pad2_;
				std::atomic<size_t>     dequeue_pos_;
				cacheline_pad_t         pad3_;

				mpmc_bounded_queue(mpmc_bounded_queue const&);
				void operator = (mpmc_bounded_queue const&);
			};

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
					if (enabled_) msg_.buffer_ += what;
					return *this;
				}

				LineLogger& operator<<(const std::string& what)
				{
					if (enabled_) msg_.buffer_ += what;
					return *this;
				}

				template<typename T>
				LineLogger& operator<<(const T& what)
				{
					if (enabled_)
					{
						msg_.buffer_ += dynamic_cast<std::ostringstream &>(std::ostringstream() << std::dec << what).str();
					}
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

			class SinkInterface : private UnCopyable
			{
			public:
				virtual ~SinkInterface() {};
				virtual void log(const LogMessage& msg) = 0;
				virtual void flush() = 0;
			};

			class Sink : public SinkInterface
			{
			public:
				Sink() : queue_(consts::kAsyncQueueSize), 
					format_(consts::kSinkDefaultFormat),
					mutex_()
				{};

				virtual ~Sink() {};

				void log(const LogMessage& msg) override
				{
					fill_sink(msg);
				}

				void fill_sink(const LogMessage& msg)
				{
					// stop filling sink if ready to destory it
					if (!enabled_) return;
					std::string finalMessage = format_message(msg);
					while (!queue_.enqueue(std::move(finalMessage)))
					{
						time::sleep(10);
					}
				}

				void set_format(const std::string &format)
				{
					std::lock_guard<std::mutex> lock(mutex_);
					format_ = format;
				}

				virtual void sink_it(const std::string &finalMsg) = 0;

			protected:
				void init_sink()
				{
					enabled_ = true;
					workThread_ = std::move(std::thread(&Sink::work_loop, this));
				}

				void start_sink()
				{
					stop_sink();
					enabled_ = true;
					workThread_ = std::move(std::thread(&Sink::work_loop, this));
				}

				void stop_sink()
				{
					enabled_ = false;
					if (workThread_.joinable())
					{
						workThread_.join();
					}
					else
					{
						throw RuntimeException("Corrupted work thread.");
					}
				}

				std::string format_message(const LogMessage &msg)
				{
					std::string ret(format_);
					// datetime
					auto dt = msg.dateTime_;
					fmt::replace_all_with_escape(ret, consts::kSinkDatetimeSpecifier, dt.to_string());
					fmt::replace_all_with_escape(ret, consts::kSinkLoggerNameSpecifier, msg.loggerName_);
					fmt::replace_all_with_escape(ret, consts::kSinkThreadSpecifier, std::to_string(msg.threadId_));
					fmt::replace_all_with_escape(ret, consts::kSinkLevelSpecifier, consts::kLevel_names[msg.level_]);
					fmt::replace_all_with_escape(ret, consts::kSinkMessageSpecifier, msg.buffer_);
					if (!fmt::ends_with(ret, os::endl()))
					{
						ret += os::endl();
					}
					return ret;
				}

				void work_loop()
				{
					std::string nextMsg;
					while (1)
					{
						if (queue_.dequeue(nextMsg))
						{
							sink_it(nextMsg);
						}
						else
						{
							// empty queue
							if (enabled_)
							{
								time::sleep(10);
							}
							else
							{
								// should return and join
								return;
							}
						}
					}
				}

			private:	

				std::atomic_bool				enabled_;
				std::string						format_;
				std::mutex						mutex_;
				mpmc_bounded_queue<std::string>	queue_;
				std::thread						workThread_;

			};

		} // namespace detail

		class SimpleFileSink : public detail::Sink
		{
		public:
			SimpleFileSink(const std::string filename) :fileEditor_(filename, false)
			{
				init_sink();
			}

			~SimpleFileSink()
			{
				stop_sink();
			}

			void flush() override
			{
				fileEditor_.flush();
			}

			void sink_it(const std::string &finalMsg) override
			{
				fileEditor_ << finalMsg;
			}

		private:
			fs::FileEditor fileEditor_;
		};


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
			for (auto sink : sinkPtrs_)
			{
				sink->log(msg);
			}
		}

	} // namespace log
} // namespace zl

#endif //END _OPENZL_LOG_HPP_