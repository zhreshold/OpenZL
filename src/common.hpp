/************************************************************************//*
 *
 *   Script File: common.hpp
 *
 *   Description:
 *
 *   Common modules
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




#ifndef _OPENZL_COMMON_HPP_
#define _OPENZL_COMMON_HPP_

///////////////////////////////////////////////////////////////
// Require C++ 11 features
///////////////////////////////////////////////////////////////
#if ((defined(_MSC_VER) && _MSC_VER >= 1800) || __cplusplus >= 201103L)
// VC++ defined an older version of __cplusplus, but it should work later than vc12
#else
#error C++11 required, add -std=c++11 to CFLAG.
#endif

///////////////////////////////////////////////////////////////
// Various hacks apply to MS VC++ Compliers
///////////////////////////////////////////////////////////////
#ifdef _MSC_VER

// Disable silly security warnings
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

// Define __func__ macro
#ifndef __func__
#define __func__ __FUNCTION__
#endif 

// Define NDEBUG in Release Mode, ensure assert() disabled.
#if (!defined(_DEBUG) && !defined(NDEBUG))
#define NDEBUG
#endif

#define ZL_NOEXCEPT throw()

#else // NON MSVC
#define ZL_NOEXCEPT noexcept

#endif // MSVC Hacks

///////////////////////////////////////////////////////////////
// Whether or not use header only structure, not recommanded
///////////////////////////////////////////////////////////////
#ifdef ZL_HEADER_ONLY
#define ZL_EXPORT inline
#else
#define ZL_EXPORT
#endif

///////////////////////////////////////////////////////////////
// Define helper macros if ZL_NO_MACRO not defined
///////////////////////////////////////////////////////////////
#ifndef ZL_NO_MACRO

// using stringstream hack to convert chain input to std::string
#define TO_STRING( x ) (dynamic_cast< std::ostringstream & >( std::ostringstream() << std::dec << x).str())

#endif

#include <string>
#include <exception>

namespace zl
{

	/*!
	 * \namespace	params
	 *
	 * \brief	namespace for parameters.
	 */
	namespace consts
	{
		static const char* kExceptionPrefixGeneral = "[OpenZL Exception] ";
		static const char* kExceptionPrefixLogic = "[OpenZL Exception->Logic] ";
		static const char* kExceptionPrefixArgument = "[OpenZL Exception->Logic->Argument] ";
		static const char* kExceptionPrefixRuntime = "[OpenZL Exception->Runtime] ";
		static const char* kExceptionPrefixIO = "[OpenZL Exception->Runtime->IO] ";
		static const char* kExceptionPrefixMemory = "[OpenZL Exception->Runtime->Memory] ";
		static const char* kExceptionPrefixStrictWarn = "[OpenZL Exception->StrictWarn] ";
	}

	/*!
	* \class	UnCopyable
	*
	* \brief	A not copyable base class, should be inheritated privately.
	*/
	class UnCopyable
	{
	public:
		UnCopyable() {};
		// no copy constructor
		UnCopyable(const UnCopyable&) = delete;
		// no copy operator
		UnCopyable& operator=(const UnCopyable&) = delete;
	};

	/*!
	 * \class	UnMovable
	 *
	 * \brief	A not movable/copyable base class, should be inheritated privately.
	 */
	class UnMovable
	{
	public:
		UnMovable() {};
		// no copy constructor
		UnMovable(const UnMovable&) = delete;
		// no copy operator
		UnMovable& operator=(const UnMovable&) = delete;
		// no move constructor
		UnMovable(UnMovable&&) = delete;
		// no move operator
		UnMovable& operator=(UnMovable&&) = delete;
	};

	/*!
	 * \class	Exception
	 *
	 * \brief	An exception with customized prefix information.
	 */
	class Exception : public std::exception
	{
	public:
		explicit Exception(const char* message, const char* prefix = consts::kExceptionPrefixGeneral)
		{
			message_ = std::string(prefix) + message;
		};
		explicit Exception(const std::string message, const char* prefix = consts::kExceptionPrefixGeneral)
		{
			message_ = std::string(prefix) + message;
		};
		virtual ~Exception() ZL_NOEXCEPT {};

		const char* what() const ZL_NOEXCEPT { return message_.c_str(); };
	private:
		std::string message_;
	};

	/*!
	 * \class	LogicException
	 *
	 * \brief	Exception for signalling logic errors.
	 */
	class LogicException : public Exception
	{
	public:
		explicit LogicException(const char *message) : Exception(message, consts::kExceptionPrefixLogic){};
		explicit LogicException(const std::string &message) : Exception(message, consts::kExceptionPrefixLogic){};
	};

	/*!
	 * \class	ArgException
	 *
	 * \brief	Exception for signalling argument errors.
	 */
	class ArgException : public Exception
	{
	public:
		explicit ArgException(const char *message) : Exception(message, consts::kExceptionPrefixArgument){};
		explicit ArgException(const std::string &message) : Exception(message, consts::kExceptionPrefixArgument){};
	};

	/*!
	 * \class	RuntimeException
	 *
	 * \brief	Exception for signalling unexpected runtime errors.
	 */
	class RuntimeException : public Exception
	{
	public:
		explicit RuntimeException(const char *message) : Exception(message, consts::kExceptionPrefixRuntime){};
		explicit RuntimeException(const std::string &message) : Exception(message, consts::kExceptionPrefixRuntime){};
	};

	class IOException : public Exception
	{
	public:
		explicit IOException(const char *message) : Exception(message, consts::kExceptionPrefixIO){};
		explicit IOException(const std::string &message) : Exception(message, consts::kExceptionPrefixIO){};
	};

	/*!
	 * \class	MemException
	 *
	 * \brief	Exception for signalling memory errors.
	 */
	class MemException : public Exception
	{
	public:
		explicit MemException(const char *message) : Exception(message, consts::kExceptionPrefixMemory){};
		explicit MemException(const std::string &message) : Exception(message, consts::kExceptionPrefixMemory){};
	};

	/*!
	 * \class	WarnException
	 *
	 * \brief	Exception for signalling warning errors when strict warning is enabled.
	 */
	class WarnException : public Exception
	{
	public:
		explicit WarnException(const char *message) : Exception(message, consts::kExceptionPrefixStrictWarn){};
		explicit WarnException(const std::string &message) : Exception(message, consts::kExceptionPrefixStrictWarn){};
	};


} // namespace zl

#endif //END _OPENZL_COMMON_HPP_


