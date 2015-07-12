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
#include "thread.hpp"
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
		typedef enum LogLevelEnum
		{
			trace = 0,
			debug = 1,
			info = 2,
			warn = 3,
			error = 4,
			fatal = 5,
			off = 6,
			sentinel = 63
		}LogLevels;

		namespace consts
		{
			static const char	*kLevelNames[] { "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL", "OFF"};
			static const char	*kShortLevelNames[] { "T", "D", "I", "W", "E", "F", "O"};
			static const int	kAsyncQueueSize = 4096;	//!< asynchrous queue size, must be power of 2

			static const char	*kSinkDatetimeSpecifier = "%datetime";
			static const char	*kSinkLoggerNameSpecifier = "%logger";
			static const char	*kSinkThreadSpecifier = "%thread";
			static const char	*kSinkLevelSpecifier = "%level";
			static const char	*kSinkLevelShortSpecifier = "%lvl";
			static const char	*kSinkMessageSpecifier = "%msg";
			static const char	*kSinkDefaultFormat = "[%datetime][T%thread][%logger][%level] %msg";
			static const char	*kStdoutSinkName = "stdout";
			static const char	*kStderrSinkName = "stderr";
		}

		// forward declaration
		namespace detail
		{
			struct LogMessage;
			class LineLogger;
			class SinkInterface;
		} // namespace detail
		typedef std::shared_ptr<detail::SinkInterface> SinkPtr;
		
		inline bool level_should_log(int levelMask, LogLevels lvl)
		{
			return (LogLevels::sentinel & levelMask & (1 << lvl)) > 0;
		}
		
		class Logger : UnMovable
		{
		public:
			Logger(std::string name) : name_(name)
			{
#ifdef NDEBUG
				levelMask_ = 0x3C;	//!< 0x3C->b111100: no debug, no trace
#else
				levelMask_ = 0x3E;	//!< 0x3E->b111110: debug, no trace
#endif
			}

			Logger(std::string name, int levelMask) : name_(name)
			{
				levelMask_ = levelMask;
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

			//const LogLevels level() const
			//{
			//	return static_cast<LogLevels>(level_.load(std::memory_order_relaxed));
			//}
			
			void set_level_mask(int levelMask)
			{
				levelMask_ = levelMask & LogLevels::sentinel;
			}
			
			bool should_log(LogLevels msgLevel) const
			{
				return level_should_log(levelMask_, msgLevel);
			}

			std::string name() const { return name_; };

			SinkPtr get_sink(std::string name);

			void attach_sink(SinkPtr sink);

			void detach_sink(SinkPtr sink);

			void attach_console();

			void detach_console();
			
		private:

			friend detail::LineLogger;

			detail::LineLogger log_if_enabled(LogLevels lvl);

			template <typename... Args>
			detail::LineLogger log_if_enabled(LogLevels lvl, const char* fmt, const Args&... args);

			template<typename T>
			detail::LineLogger log_if_enabled(LogLevels lvl, const T& msg);

			void log_msg(detail::LogMessage msg);

			std::string				name_;
			std::atomic_int			levelMask_;
			thread::AtomicUnorderedMap<std::string, SinkPtr> sinks_;
		};
		typedef std::shared_ptr<Logger> LoggerPtr;

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

			class SinkInterface 
			{
			public:
				virtual ~SinkInterface() {};
				virtual void log(const LogMessage& msg) = 0;
				virtual void flush() = 0;
				virtual std::string name() const = 0;
			};

			class Sink : public SinkInterface,  private UnCopyable
			{
			public:
				Sink()
				{
					levelMask_ = 0x3F;
					set_format(std::string(consts::kSinkDefaultFormat));
				};

				virtual ~Sink()  {};

				void log(const LogMessage& msg) override
				{
					if (!level_should_log(levelMask_, msg.level_))
					{
						return;
					}

					std::string finalMessage = format_message(msg);
					sink_it(finalMessage);
				}

				void set_level_mask(int levelMask)
				{
					levelMask_ = levelMask & LogLevels::sentinel;
				}

				void set_format(const std::string &format)
				{
					format_.set(format);
				}

				virtual void sink_it(const std::string &finalMsg) = 0;

			private:
				std::string format_message(const LogMessage &msg)
				{
					std::string ret(*(format_.get()));
					auto dt = msg.dateTime_;
					fmt::replace_all_with_escape(ret, consts::kSinkDatetimeSpecifier, dt.to_string());
					fmt::replace_all_with_escape(ret, consts::kSinkLoggerNameSpecifier, msg.loggerName_);
					fmt::replace_all_with_escape(ret, consts::kSinkThreadSpecifier, std::to_string(msg.threadId_));
					fmt::replace_all_with_escape(ret, consts::kSinkLevelSpecifier, consts::kLevelNames[msg.level_]);
					fmt::replace_all_with_escape(ret, consts::kSinkLevelShortSpecifier, consts::kShortLevelNames[msg.level_]);
					fmt::replace_all_with_escape(ret, consts::kSinkMessageSpecifier, msg.buffer_);
					// make sure new line
					if (!fmt::ends_with(ret, os::endl()))
					{
						ret += os::endl();
					}
					return ret;
				}

				std::atomic_int								levelMask_;
				thread::AtomicNonTrivial<std::string>		format_;
			};

			class SimpleFileSink : public Sink
			{
			public:
				SimpleFileSink(const std::string filename, bool truncate) :fileEditor_(filename, truncate)
				{
				}

				void flush() override
				{
					fileEditor_.flush();
				}

				void sink_it(const std::string &finalMsg) override
				{
					fileEditor_ << finalMsg;
				}

				std::string name() const override
				{
					return fileEditor_.filename();
				}

			private:
				fs::FileEditor fileEditor_;
			};

			class RotateFileSink : public Sink
			{
			public:
				RotateFileSink(const std::string filename, int maxSizeInByte, int checkEveryNLogs)
					: fileReader_(filename), maxSizeInByte_(maxSizeInByte),
					checkFrequency_(checkEveryNLogs), checkCount_(0)
				{
					if (reach_size_limit())
					{
						fileEditor_.open(filename, true);
					}
					else
					{
						fileEditor_.open(filename, false);
					}
				}

				void flush() override
				{
					fileEditor_.flush();
				}

				void sink_it(const std::string &finalMsg) override
				{
					rotate_if_necessary();
					fileEditor_ << finalMsg;
				}

				std::string name() const override
				{
					return fileEditor_.filename();
				}

			private:
				bool reach_size_limit()
				{
					if (fileReader_.file_size() >= maxSizeInByte_)
					{
						return true;
					}
					return false;
				}

				void rotate_if_necessary()
				{
					++checkCount_;
					if (checkCount_ >= checkFrequency_)
					{
						if (reach_size_limit())
						{
							fileEditor_.open(fileEditor_.filename() + time::Date::local_time().to_string("_%S%frac.log"), true);
							//fileEditor_.close();
							//fileEditor_.try_open(5, 10, true);
						}
						checkCount_ = 0;
					}
				}

				fs::FileEditor	fileEditor_;
				fs::FileReader	fileReader_;
				int				maxSizeInByte_;
				int				checkFrequency_;
				int				checkCount_;
			};

			class OStreamSink : public Sink
			{
			public:
				explicit OStreamSink(std::ostream& os, const char *name, bool forceFlush = false)
					:ostream_(os), name_(name),	forceFlush_(forceFlush)
				{
				}

				std::string name() const override
				{
					return name_;
				}

			private:
				void sink_it(const std::string &finalMsg) override
				{
					ostream_ << finalMsg;
					if (forceFlush_) ostream_.flush();
				}

				void flush() override
				{
					ostream_.flush();
				}

				std::ostream&	ostream_;
				std::string		name_;
				bool			forceFlush_;
			};

			class StdoutSink : public OStreamSink
			{
			public:
				StdoutSink() : OStreamSink(std::cout, consts::kStdoutSinkName, true) 
				{
					// stdout by design should not receive warning and error message
					// so add cout and cerr together and filter by level is a better idea
					set_level_mask(0x07);
 				}
				static std::shared_ptr<StdoutSink> instance()
				{
					static std::shared_ptr<StdoutSink> instance = std::make_shared<StdoutSink>();
					return instance;
				}
			};

			class StderrSink : public OStreamSink
			{
			public:
				StderrSink() : OStreamSink(std::cerr, consts::kStderrSinkName, true)
				{
					// stderr by design should only log error/warning msg
					set_level_mask(0x38);
				}
				static std::shared_ptr<StderrSink> instance()
				{
					static std::shared_ptr<StderrSink> instance = std::make_shared<StderrSink>();
					return instance;
				}
			};

			class LoggerRegistry : UnMovable
			{
			public:
				static LoggerRegistry& instance()
				{
					static LoggerRegistry sInstance;
					return sInstance;
				}

				//LoggerPtr register_logger(Logger &logger)
				//{
				//	std::lock_guard<std::mutex> lock(mutex_);
				//	if (contains(logger.name()))
				//	{
				//		throw RuntimeException("Logger with name: " + logger.name() + " already existed.");
				//	}
				//	LoggerPtr ptr = std::make_shared<Logger>(logger);
				//	do_registry(logger.name(), ptr);
				//	return ptr;
				//}

				//LoggerPtr register_logger(LoggerPtr pLogger)
				//{
				//	std::lock_guard<std::mutex> lock(mutex_);
				//	if (contains(pLogger->name()))
				//	{
				//		throw RuntimeException("Logger with name: " + pLogger->name() + " already existed.");
				//	}
				//	do_registry(pLogger->name(), pLogger);
				//	return pLogger;
				//}

				LoggerPtr create(const std::string &name)
				{
					auto ptr = new_registry(name);
					if (!ptr)
					{
						throw RuntimeException("Logger with name: " + name + " already existed.");
					}
					return ptr;
				}

				LoggerPtr ensure_get(std::string &name)
				{
					auto map = loggers_.get();
					LoggerPtr	ptr;
					while (map->find(name) == map->end())
					{
						ptr = new_registry(name);
						map = loggers_.get();
					}
					return map->find(name)->second;
				}

				LoggerPtr get(std::string &name)
				{
					auto ptr = loggers_.get();
					auto pos = ptr->find(name);
					if (pos != ptr->end())
					{
						return pos->second;
					}
					return nullptr;
				}

				//std::vector<LoggerPtr> list()
				//{
				//	std::lock_guard<std::mutex> lock(mutex_);
				//	std::vector<LoggerPtr> list;
				//	for (logger : loggers_)
				//	{
				//		list.push_back(logger);
				//	}
				//	return list;
				//}

				void drop(const std::string &name)
				{
					loggers_.erase(name);
				}

				void drop_all()
				{
					loggers_.clear();
				}

			private:
				LoggerRegistry(){}

				LoggerPtr do_registry(const std::string &name, LoggerPtr pLogger)
				{
					if (loggers_.insert(name, pLogger))
					{
						return pLogger;
					}
					return nullptr;
				}

				LoggerPtr new_registry(const std::string &name)
				{
					LoggerPtr newLogger = std::make_shared<Logger>(name);
					if (loggers_.insert(name, newLogger))
					{
						return newLogger;
					}
					return nullptr;
				}

				thread::AtomicUnorderedMap<std::string, LoggerPtr> loggers_;
			};

		} // namespace detail

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

		inline SinkPtr Logger::get_sink(std::string name)
		{
			auto sinkmap = sinks_.get();
			auto f = sinkmap->find(name);
			return (f == sinkmap->end() ? nullptr : f->second);
		}

		inline void Logger::attach_sink(SinkPtr sink)
		{
			if (!sinks_.insert(sink->name(), sink))
			{
				throw RuntimeException("Sink with name: " + sink->name() + " already attached to logger: " + name_);
			}
		}

		inline void Logger::detach_sink(SinkPtr sink)
		{
			sinks_.erase(sink->name());
		}

		inline void Logger::log_msg(detail::LogMessage msg)
		{
			auto sinkmap = sinks_.get();
			for (auto sink : *sinkmap)
			{
				sink.second->log(msg);
			}
		}

		inline LoggerPtr get_logger(std::string name, bool createIfNotExists = true)
		{
			if (createIfNotExists)
			{
				return detail::LoggerRegistry::instance().ensure_get(name);
			}
			else
			{
				return detail::LoggerRegistry::instance().get(name);
			}
		}

		//inline void destroy_registry()
		//{
		//	detail::LoggerRegistry::instance().~LoggerRegistry();
		//}

		inline SinkPtr new_stdout_sink()
		{
			return detail::StdoutSink::instance();
		}

		inline SinkPtr new_stderr_sink()
		{
			return detail::StderrSink::instance();
		}

		inline SinkPtr new_simple_file_sink(std::string filename, bool truncate = false)
		{
			return std::make_shared<detail::SimpleFileSink>(filename, truncate);
		}

		inline SinkPtr new_rotate_file_sink(std::string filename, int maxSizeInByte = 4194304, int checkEveryNLogs = 100)
		{
			return std::make_shared<detail::RotateFileSink>(filename, maxSizeInByte, checkEveryNLogs);
		}

		inline void Logger::attach_console()
		{
			sinks_.insert(std::string(consts::kStdoutSinkName), new_stdout_sink());
			sinks_.insert(std::string(consts::kStderrSinkName), new_stderr_sink());
		}

		inline void Logger::detach_console()
		{
			sinks_.erase(std::string(consts::kStdoutSinkName));
			sinks_.erase(std::string(consts::kStderrSinkName));
		}

	} // namespace log
} // namespace zl

#endif //END _OPENZL_LOG_HPP_