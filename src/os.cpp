/************************************************************************//*
 *
 *   Script File: os.cpp
 *
 *   Description:
 *
 *   OS specific implementations
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



// Unless you are very confident, don't set either OS flag
#if defined(OPENZL_OS_UNIX) && defined(OPENZL_OS_WINDOWS)
#error Both Unix and Windows flags are set, which is not allowed!
#elif defined(OPENZL_OS_UNIX)
#pragma message Using defined Unix flag
#elif defined(OPENZL_OS_WINDOWS)
#pragma message Using defined Windows flag
#else
#if defined(unix)        || defined(__unix)      || defined(__unix__) \
	|| defined(linux) || defined(__linux) || defined(__linux__) \
	|| defined(sun) || defined(__sun) \
	|| defined(BSD) || defined(__OpenBSD__) || defined(__NetBSD__) \
	|| defined(__FreeBSD__) || defined (__DragonFly__) \
	|| defined(sgi) || defined(__sgi) \
	|| (defined(__MACOSX__) || defined(__APPLE__)) \
	|| defined(__CYGWIN__) || defined(__MINGW32__)
#define OPENZL_OS_UNIX	1	//!< Unix like OS
#undef OPENZL_OS_WINDOWS
#elif defined(_MSC_VER) || defined(WIN32)  || defined(_WIN32) || defined(__WIN32__) \
	|| defined(WIN64) || defined(_WIN64) || defined(__WIN64__)
#define OPENZL_OS_WINDOWS	1	//!< Microsoft Windows
#undef OPENZL_OS_UNIX
#else
#error Unable to support this unknown OS.
#endif
#endif

#if OPENZL_OS_WINDOWS
#include <Windows.h>
#include <direct.h>
#elif OPENZL_OS_UNIX
#include <unistd.h>	/* POSIX flags */
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#endif

#undef ZL_UNUSED
#define ZL_UNUSED(x) (void)(x)

#include "os.hpp"
#include "format.hpp"
#include <string>
#include <functional>
#include <thread>

namespace zl
{
	namespace consts
	{
		static const std::string kEndLineWindows = std::string("\r\n");
		static const std::string kEndLineUniversal = std::string("\n");
	}

	namespace os
	{

		std::size_t thread_id()
		{
#if OPENZL_OS_WINDOWS
			// It exists because the std::this_thread::get_id() is much slower(espcially under VS 2013)
			return  static_cast<size_t>(::GetCurrentThreadId());
#elif __linux__
			return  static_cast<size_t>(syscall(SYS_gettid));
#else //Default to standard C++11 (OSX and other Unix)
			return static_cast<size_t>(std::hash<std::thread::id>()(std::this_thread::get_id()));
#endif

		}

		/**
		 * \fn	std::tm localtime(std::time_t t)
		 *
		 * \brief	Thread safe localtime
		 *
		 * \param	t	The std::time_t to process.
		 *
		 * \return	A std::tm.
		 */
		std::tm localtime(std::time_t t)
		{
			std::tm temp;
#if OPENZL_OS_WINDOWS
			localtime_s(&temp, &t);
			return temp;
#elif OPENZL_OS_UNIX
			// POSIX SUSv2 thread safe localtime_r
			return *localtime_r(&t, &temp);
#else
			return temp; // return default tm struct, no idea what it is
#endif
		}

		/**
		 * \fn	std::tm gmtime(std::time_t t)
		 *
		 * \brief	Thread safe gmtime
		 *
		 * \param	t	The std::time_t to process.
		 *
		 * \return	A std::tm.
		 */
		std::tm gmtime(std::time_t t)
		{
			std::tm temp;
#if OPENZL_OS_WINDOWS
			gmtime_s(&temp, &t);
			return temp;
#elif OPENZL_OS_UNIX
			// POSIX SUSv2 thread safe gmtime_r
			return *gmtime_r(&t, &temp);
#else
			return temp; // return default tm struct, no idea what it is
#endif 
		}

		std::wstring utf8_to_wstring(std::string &u8str)
		{
#if OPENZL_OS_WINDOWS
			// windows use 16 bit wstring 
			std::u16string u16str = fmt::utf8_to_utf16(u8str);
			return std::wstring(u16str.begin(), u16str.end());
#else
			// otherwise use 32 bit wstring
			std::u32string u32str = fmt::utf8_to_utf32(u8str);
			return std::wstring(u32str.begin(), u32str.end());
#endif
		}

		std::string wstring_to_utf8(std::wstring &wstr)
		{
#if OPENZL_OS_WINDOWS
			// windows use 16 bit wstring 
			std::u16string u16str(wstr.begin(), wstr.end());
			return fmt::utf16_to_utf8(u16str);
#else
			// otherwise use 32 bit wstring
			std::u32string u32str(wstr.begin(), wstr.end());
			return fmt::utf32_to_utf8(u32str);
#endif
		}

		bool path_exists(std::string &path, bool considerFile) 
		{
#if OPENZL_OS_WINDOWS
			DWORD fileType = GetFileAttributesW(utf8_to_wstring(path).c_str());
			if (fileType == INVALID_FILE_ATTRIBUTES) {
				return false;
			}
			return considerFile ? true : ((fileType & FILE_ATTRIBUTE_DIRECTORY) == 0 ? false : true);
#elif OPENZL_OS_UNIX
			ZL_UNUSED(considerFile);
			struct stat st;
			return (stat(path.c_str(), &st) == 0);
#endif  
		}

		
		void fstream_open(std::fstream &stream, std::string &filename, std::ios::openmode openmode)
		{
#if OPENZL_OS_WINDOWS
			stream.open(utf8_to_wstring(filename), openmode);
#elif OPENZL_OS_UNIX
			stream.open(filename, openmode);
#endif
		}

		void ifstream_open(std::ifstream &stream, std::string &filename, std::ios::openmode openmode)
		{
#if OPENZL_OS_WINDOWS
			stream.open(utf8_to_wstring(filename), openmode);
#elif OPENZL_OS_UNIX
			stream.open(filename, openmode);
#endif
		}


		std::string endl()
		{
#if OPENZL_OS_WINDOWS
			return consts::kEndLineWindows;
#else
			return consts::kEndLineUniversal;
#endif
		}

		std::string get_current_working_directory()
		{
#if OPENZL_OS_WINDOWS
			wchar_t *buffer = nullptr;
			if ((buffer = _wgetcwd(nullptr, 0)) == nullptr)
			{
				// failed
				return std::string();
			}
			else
			{
				std::wstring ret(buffer);
				free(buffer);
				return wstring_to_utf8(ret);
			}
#elif __GNUC__
			char *buffer = get_current_dir_name();
			if (buffer == nullptr)
			{
				// failed
				return std::string();
			}
			else
			{
				// success
				std::string ret(buffer);
				free(buffer);
				return ret;
			}
#else
			char *buffer = getcwd(nullptr, 0);
			if (buffer == nullptr)
			{
				// failed
				return std::string();
			}
			else
			{
				// success
				std::string ret(buffer);
				free(buffer);
				return ret;
			}
#endif
		}

		std::string get_absolute_path(std::string reletivePath)
		{
#if OPENZL_OS_WINDOWS
			wchar_t *buffer = nullptr;
			std::wstring widePath = utf8_to_wstring(reletivePath);
			buffer = _wfullpath(buffer, widePath.c_str(), _MAX_PATH);
			if (buffer == nullptr)
			{
				// failed
				return std::string();
			}
			else
			{
				std::wstring ret(buffer);
				free(buffer);
				return wstring_to_utf8(ret);
			}
#elif OPENZL_OS_UNIX
			char *buffer = realpath(reletivePath.c_str(), nullptr);
			if (buffer == nullptr)
			{
				// failed
				return std::string();
			}
			else
			{
				std::string ret(buffer);
				free(buffer);
				return ret;
			}
#endif
		}

//		bool create_directory(std::string &path)
//		{
//#if OPENZL_OS_WINDOWS
//			
//#elif OPENZL_OS_UNIX
//#endif
//		}

	}// namespace os

} // namespace zl
