/************************************************************************//*
 *
 *   Script File: log.cpp
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

#include "log.hpp"

namespace zl
{
	namespace log
	{
		namespace detail
		{
			//std::unique_ptr<LoggerRegistry> LoggerRegistry::instance_ = nullptr;
			//std::once_flag LoggerRegistry::onceFlag_;

			//LoggerRegistry& LoggerRegistry::instance()
			//{
			//	std::call_once(onceFlag_, []{instance_.reset(new LoggerRegistry); });
			//	return *instance_.get();
			//}

			//LoggerRegistry::LoggerRegistry()
			//{
			//	std::cout << "logger registry ctor" << std::endl;
			//}
		} // namespace detail
		
	} // namespace log
}// namespace zl