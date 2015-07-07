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
		
		
		class Logger : UnMovable
		{
		public:
			Logger(std::string name) : name_(name)
			{
				level_ = LogLevels::info;
				sinks_.store(new SinkMap());
			}

			~Logger()
			{
				delete sinks_.exchange(nullptr);
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

			SinkPtr get_sink(std::string &name);

			void attach_sink(SinkPtr sink);

			void detach_sink(SinkPtr sink);

			
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
			std::mutex				mutex_;

			typedef std::unordered_map<std::string, SinkPtr> SinkMap;
			std::atomic<SinkMap*> sinks_;
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

			template <typename Mutex> class Sink : public SinkInterface,  private UnCopyable
			{
			public:
				Sink() : queue_(consts::kAsyncQueueSize), 
					format_(consts::kSinkDefaultFormat)
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

				//void set_format(const std::string &format)
				//{
				//	std::lock_guard<Mutex> lock(mutex_);
				//	format_ = format;
				//}

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
					auto dt = msg.dateTime_;
					fmt::replace_all_with_escape(ret, consts::kSinkDatetimeSpecifier, dt.to_string());
					fmt::replace_all_with_escape(ret, consts::kSinkLoggerNameSpecifier, msg.loggerName_);
					fmt::replace_all_with_escape(ret, consts::kSinkThreadSpecifier, std::to_string(msg.threadId_));
					fmt::replace_all_with_escape(ret, consts::kSinkLevelSpecifier, consts::kLevel_names[msg.level_]);
					fmt::replace_all_with_escape(ret, consts::kSinkMessageSpecifier, msg.buffer_);
					// make sure new line
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
				mpmc_bounded_queue<std::string>	queue_;
				std::thread						workThread_;
			};

			template <typename Mutex> class SimpleFileSink_ : public Sink<Mutex>
			{
			public:
				SimpleFileSink_(const std::string filename, bool truncate) :fileEditor_(filename, truncate)
				{
					init_sink();
				}

				~SimpleFileSink_()
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

				std::string name() const override
				{
					return fileEditor_.filename();
				}

			private:
				fs::FileEditor fileEditor_;
			};

			template<class Mutex> class OStreamSink_ : public Sink<Mutex>
			{
			public:
				explicit OStreamSink_(std::ostream& os, const char *name, bool forceFlush = false)
					:ostream_(os), name_(name),	forceFlush_(forceFlush)
				{
					init_sink();
				}

				~OStreamSink_()
				{
					stop_sink();
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

			template <class Mutex> class StdoutSink_ : public OStreamSink_<Mutex>
			{
				using MyType = StdoutSink_<Mutex>;
			public:
				StdoutSink_() : OStreamSink_<Mutex>(std::cout, consts::kStdoutSinkName, true) {}
				static std::shared_ptr<MyType> instance()
				{
					static std::shared_ptr<MyType> instance = std::make_shared<MyType>();
					return instance;
				}
			};

			template <class Mutex> class StderrSink_ : public OStreamSink_<Mutex>
			{
				using MyType = StderrSink_<Mutex>;
			public:
				StderrSink_() : OStreamSink_<Mutex>(std::cerr, consts::kStderrSinkName, true) {}
				static std::shared_ptr<MyType> instance()
				{
					static std::shared_ptr<MyType> instance = std::make_shared<MyType>();
					return instance;
				}
			};

			template <typename Mutex> class LoggerRegistry_ : UnMovable
			{
			public:
				static LoggerRegistry_<Mutex>& instance()
				{
					static LoggerRegistry_<Mutex> sInstance;
					return sInstance;
				}

				LoggerPtr register_logger(Logger &logger)
				{
					std::lock_guard<Mutex> lock(mutex_);
					if (contains(logger.name()))
					{
						throw RuntimeException("Logger with name: " + name + " already existed.");
					}
					LoggerPtr ptr = std::make_shared<Logger>(logger);
					do_registry(logger.name(), ptr);
					return ptr;
				}

				LoggerPtr register_logger(LoggerPtr pLogger)
				{
					std::lock_guard<Mutex> lock(mutex_);
					if (contains(pLogger->name()))
					{
						throw RuntimeException("Logger with name: " + name + " already existed.");
					}
					do_registry(pLogger->name(), pLogger);
					return pLogger;
				}

				LoggerPtr create(std::string &name)
				{
					std::lock_guard<Mutex> lock(mutex_);

					if (contains(name))
					{
						throw RuntimeException("Logger with name: " + name + " already existed.");
					}
					return new_registry(name);
				}

				LoggerPtr ensure_get(std::string &name)
				{
					std::lock_guard<Mutex> lock(mutex_);
					if (contains(name))
					{
						return loggers_.find(name)->second;
					}
					return new_registry(name);
				}

				LoggerPtr get(std::string &name)
				{
					std::lock_guard<Mutex> lock(mutex_);
					if (contains(name))
					{
						return loggers_.find(name)->second;
					}
					return nullptr;
				}

				std::vector<LoggerPtr> list()
				{
					std::lock_guard<Mutex> lock(mutex_);
					std::vector<LoggerPtr> list;
					for (logger : loggers_)
					{
						list.push_back(logger);
					}
					return list;
				}

				void drop(std::string &name)
				{
					std::lock_guard<Mutex> lock(mutex_);
					loggers_.erase(name);
				}

				void drop_all()
				{
					std::lock_guard<Mutex> lock(mutex_);
					loggers_.clear();
				}

			private:
				LoggerRegistry_<Mutex>() : mutex_(){}

				LoggerPtr do_registry(std::string &name, LoggerPtr pLogger)
				{
					loggers_.insert({ { name, pLogger } });
				}

				LoggerPtr new_registry(std::string &name)
				{
					LoggerPtr newLogger = std::make_shared<Logger>(name);
					loggers_.insert({name, newLogger});
					return newLogger;
				}

				bool contains(std::string &name)
				{
					return loggers_.count(name) > 0;
				}

				Mutex		mutex_;
				std::unordered_map<std::string, std::shared_ptr<Logger>> loggers_;
			};

		} // namespace detail



#ifdef ZL_SINGLE_THREAD_ONLY
		typedef detail::SimpleFileSink_<thread::NullMutex> SimpleFileSink;
		typedef detail::StdoutSink_<thread::NullMutex> StdoutSink;
		typedef detail::StderrSink_<thread::NullMutex> StderrSink;
		typedef detail::LoggerRegistry_<thread::NullMutex> LoggerRegistry;
#else
		typedef detail::SimpleFileSink_<std::mutex> SimpleFileSink;
		typedef detail::StdoutSink_<std::mutex> StdoutSink;
		typedef detail::StderrSink_<std::mutex> StderrSink;
		typedef detail::LoggerRegistry_<std::mutex> LoggerRegistry;
#endif

		

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

		inline SinkPtr Logger::get_sink(std::string &name)
		{
			auto sinkmap = sinks_.load();
			auto f = sinkmap->find(name);
			return (f == sinkmap->end() ? nullptr : f->second);
		}

		inline void Logger::attach_sink(SinkPtr sink)
		{
			auto name = sink->name();
			auto sinkmap = new SinkMap(*sinks_.load());
			if (sinkmap->find(name) != sinkmap->end())
			{
				throw RuntimeException("Sink with name: " + sink->name() + " already attached to logger: " + name_);
			}
			sinkmap->insert({ name, sink });
			delete sinks_.exchange(sinkmap);
			
		}

		inline void Logger::detach_sink(SinkPtr sink)
		{
			auto name = sink->name();
			auto sinkmap = new SinkMap(*sinks_.load());
			sinkmap->erase(name);
			delete sinks_.exchange(sinkmap);
		}

		inline void Logger::log_msg(detail::LogMessage msg)
		{
			auto sinkmap = *sinks_.load();
			for (auto sink = sinkmap.begin(); sink != sinkmap.end(); ++sink)
			{
				sink->second->log(msg);
			}
		}

		inline LoggerPtr get_logger(std::string name, bool createIfNotExists = true)
		{
			if (createIfNotExists)
			{
				return LoggerRegistry::instance().ensure_get(name);
			}
			else
			{
				return LoggerRegistry::instance().get(name);
			}
		}

		inline SinkPtr new_stdout_sink()
		{
			return StdoutSink::instance();
		}

		inline SinkPtr new_stderr_sink()
		{
			return StderrSink::instance();
		}

		inline SinkPtr new_simple_file_sink(std::string filename, bool truncate = false)
		{
			return std::make_shared<SimpleFileSink>(filename, truncate);
		}

	} // namespace log
} // namespace zl

#endif //END _OPENZL_LOG_HPP_